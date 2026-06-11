# Xaya Neoscrypt Share Rejection Analysis

## Executive Summary

The shares are being rejected because **Xaya has a fundamentally different block header architecture** than standard Bitcoin-style mining. The miner constructs shares using standard NeoScrypt merkle root calculation, but Xaya requires a "fake block header" where the merkle root field (bytes 36-67) contains the **Neoscrypt hash of the real Xaya block header**.

## Root Cause Analysis

### The Problem: Xaya's Fake Block Header Architecture

According to [official Xaya mining documentation](https://github.com/xaya/xaya/blob/master/doc/xaya/mining.md):

> "Instead of just receiving the data to hash, they receive the fields necessary to construct the final block header and coinbase transaction, and just submit their nonces back. This allows for efficient data transfer..."
> 
> "the Stratum client receives the **real Xaya block header** as "coinbase template", together with an empty Merkle branch. The client will then put its hash into the "Merkle root" of the fake block header and solve PoW on it."

#### Xaya Stratum Mining Flow:
1. Pool sends real Xaya block header (minus nonce) as "coinbase part 1"
2. Miner constructs what it thinks is a "coinbase" = real Xaya block header
3. **Miner hashes the real Xaya block header with Neoscrypt** → gets 32-byte hash
4. **Miner puts this Neoscrypt hash into bytes 37-68 of the fake 80-byte block header** (as "Merkle root")
5. Miner solves PoW (Neoscrypt) on the fake block header
6. Miner submits the nonce from the fake block header

### What nsgminer Currently Does (Incorrect for Xaya):

In `gen_stratum_work()`, the code does:
```c
// THIS IS WRONG FOR XAYA
gen_hash(coinbase, merkle_root, pool->swork.cb_len);  // Standard merkle tree computation
for(i = 0; i < pool->swork.merkles; i++) {
    hex2bin((uchar *) temp_bin, (char *) pool->swork.merkle[i], 32);
    memcpy(&merkle_root, &temp_bin, 32);
    gen_hash(merkle_root, merkle_root, 64);
}
```

This computes a traditional Bitcoin-style merkle root. For Xaya, bytes 36-67 should be **the Neoscrypt hash of the real Xaya block header**, NOT a merkle root.

### The Fix Required

For Xaya mode (`opt_neoscrypt && opt_xayaswab`), the merkle root calculation should be:
```c
// Construct the REAL Xaya block header from coinbase parts
// (version + prev_hash + nonce_template + time + bits)
// Compute Neoscrypt hash of this real header
// Put that 32-byte hash into bytes 36-67 of the fake header
```

## Secondary Issues Found

### 1. Type Safety Bug (Line 6727)
```c
// BROKEN - missing cast causes Windows compilation error
char *header = bin2hex(data, 80);

// FIXED
char *header = bin2hex((const unsigned char *)data, 80);
```

### 2. `--xaya-byteswap` Flag Not Always Used
The byteswap logic `!opt_neoscrypt || opt_xayaswab` means:
- When `opt_neoscrypt=true` and `opt_xayaswab=false` → NO byteswap (wrong for Xaya!)
- When `opt_xayaswab=true` → byteswap applied (correct for Xaya)

**Users MUST use `--xaya-byteswap` flag for Xaya mining.**

## Checklist of Issues

| # | Issue | Severity | Status |
|---|-------|----------|--------|
| 1 | Fake block header merkle root should be Neoscrypt(real_header_hash) | CRITICAL | Needs fix |
| 2 | `--xaya-byteswap` flag required for Xaya mode | HIGH | User must enable |
| 3 | Type safety bug at line 6727 (missing cast) | MEDIUM | Windows build error |
| 4 | Pool may not implement Xaya Stratum protocol correctly | MEDIUM | Pool compatibility |

## Verification Steps

### Step 1: Confirm Flag Usage
Run with `--xaya-byteswap` flag:
```bash
./nsgminer --neoscrypt --xaya-byteswap -o stratum+tcp://pool:port -u worker -p password
```

### Step 2: Add Debug Logging
Apply these patches to `miner.c`:

```c
// In gen_stratum_work(), after building data[80], add:
if(opt_neoscrypt && opt_xayaswab && opt_debug) {
    char *hdr = bin2hex((const unsigned char *)data, 80);
    applog(LOG_DEBUG, "XAYA_FAKE_HDR: %s", hdr);
    free(hdr);
    // Log merkle root specifically
    applog(LOG_DEBUG, "XAYA_MERKLE_ROOT: %s", 
           bin2hex((const unsigned char *)&data[36], 32));
}

// In begin_submission(), before constructing submit JSON:
if(opt_neoscrypt && opt_xayaswab && opt_debug) {
    applog(LOG_DEBUG, "XAYA_SUBMIT: job_id=%s nonce2=%s ntime=%s noncehex=%s",
           work->job_id, work->nonce2, work->ntime, noncehex);
}
```

### Step 3: Capture Comparison
Compare with a known-working miner (like sgminer with neoscrypt-xaya kernel) to see the difference in:
- Stratum notify message format
- mining.submit parameters

## Technical Details

### Block Header Offsets for Xaya Fake Header

| Offset | Size | Field | Endianness | Xaya-Specific Meaning |
|--------|------|-------|------------|----------------------|
| 0-3 | 4 | version | BE | Block version |
| 4-35 | 32 | prev_hash | BE | Previous block hash |
| 36-67 | 32 | merkle_root | N/A | **Neoscrypt hash of real Xaya header** |
| 68-71 | 4 | time | BE | Timestamp |
| 72-75 | 4 | nbits | BE | Difficulty target |
| 76-79 | 4 | nonce | BE | Solution nonce |

### Stratum Message Differences

**Standard NeoScrypt:**
- `coinbase1` + `coinbase2` → merkle tree → merkle_root
- merkle_root goes into block header offset 36-67

**Xaya Neoscrypt:**
- `coinbase1` IS the real Xaya block header (minus nonce)
- Compute: `neoscrypt_hash = Neoscrypt(coinbase1)`  
- Put `neoscrypt_hash` into block header offset 36-67
- Submit nonce from fake header

## References

1. Xaya Mining Documentation: https://github.com/xaya/xaya/blob/master/doc/xaya/mining.md
2. Reference Commit (ddf1bb0): https://github.com/xaya/nsgminer/commit/ddf1bb089df357f9842415b6a91d6818988b1415
3. Chain ID for Xaya merged mining: 1829 (but standalone uses 0x02)
4. PoW signaling values: 0x02 = standalone Neoscrypt, 0x81 = merge-mined SHA-256d