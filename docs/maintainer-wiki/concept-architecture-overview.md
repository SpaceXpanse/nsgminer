# Architecture Overview

High-level system design and organization of NSGminer.

## System Type

NSGminer is a **specialized utility application** for cryptocurrency mining. It is not a full blockchain node. Its primary function is to:
- Perform proof-of-work calculations efficiently
- Manage multiple mining devices (GPU, CPU, FPGA)
- Communicate with mining pools via various protocols
- Monitor and adjust hardware parameters in real-time

## Architectural Style

**Layered architecture** with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────┐
│                      User Interface Layer                   │
│  (Curses TUI • RPC API • Logging • Configuration)          │
├─────────────────────────────────────────────────────────────┤
│                  Device Abstraction Layer                  │
│  (GPU/CPU/FPGA drivers • ADL/NVML monitoring • libusb)     │
├─────────────────────────────────────────────────────────────┤
│                    Mining Core Layer                       │
│  (Work management • Pool communication • Share submission) │
├─────────────────────────────────────────────────────────────┤
│                    Algorithm Layer                        │
│  (NeoScrypt • Scrypt • SHA-256d implementations)          │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. Main Application (`miner.c`, `miner.h`)

The central orchestrator that:
- Initializes all subsystems
- Manages configuration and command-line options
- Creates and coordinates mining threads
- Handles signal processing and shutdown
- Updates UI and logs

**Key data structures**:
- `struct cgpu_info` - Device information (GPU/CPU/FPGA specific)
- `struct pool` - Pool connection and work state
- `struct work` - Work item (block header, target, midstate)
- `struct mining_thread` - Thread-specific state

### 2. Device Drivers

Each hardware type has a dedicated driver implementing a common interface:

| Driver | File | Hardware | Interface |
|--------|------|----------|-----------|
| OpenCL GPU | `driver-opencl.c` | AMD/NVIDIA GPUs | OpenCL API |
| CPU | `driver-cpu.c` | x86/ARM CPUs | Native C/ASM |
| BitForce | `driver-bitforce.c` | BitForce FPGAs | Serial USB |
| Icarus | `driver-icarus.c` | Icarus FPGAs | Serial USB |
| ModMiner | `driver-modminer.c` | ModMiner FPGAs | USB/FTDI |
| X6500 | `driver-x6500.c` | X6500 FPGAs | USB/JTAG |
| ZTEX | `driver-ztex.c` | ZTEX FPGAs | USB/libusb |

**Driver contract** (see `miner.h`):
- `detect()` - Find available devices
- `initialize()` - Set up device and allocate resources
- `hash_queued()` - Perform mining operation (may be blocking/non-blocking)
- `shutdown()` - Clean up device resources
- Optional: reinitialization for hung devices

### 3. Pool Management

Pool communication handled by:
- `api.c` - JSON-RPC interface for control
- `util.c` - Network I/O, URL parsing, socket utilities
- `miner.c` - Pool logic (selection, failover, work distribution)

**Supported protocols**:
- **getwork** - Original Bitcoin mining protocol (HTTP JSON-RPC)
- **GBT** - GetBlockTemplate (BIP 22) with rolltime support
- **Stratum** - More efficient protocol (TCP-based)

**Multipool strategies**:
- Failover (default) - Use primary until it fails
- Round-robin - Rotate through pools
- Rotate - Change at fixed intervals
- Load balance - Distribute work proportionally
- Balance - Equal work distribution based on shares

### 4. Algorithm Implementations

#### Hash Function Implementations

**SHA-256d** (double SHA-256):
- Generic C: `sha256_generic.c`
- SSE2 32-bit: `sha256_sse2_i386.c`
- SSE2 64-bit: `sha256_sse2_amd64.c`
- SSE4 64-bit: `sha256_sse4_amd64.c`
- 4-way parallel: `sha256_4way.c`
- Altivec (PowerPC): `sha256_altivec_4way.c`
- Crypto++: `sha256_cryptopp.c`
- VIA Padlock: `sha256_via.c`

**NeoScrypt**:
- CPU: `neoscrypt.c`
- GPU kernels:
  - `neoscrypt.cl` - Generic
  - `neoscrypt_vliw.cl` - AMD VLIW architecture
  - `neoscrypt_vliwp.cl` - AMD VLIW parallel

**Scrypt** (optional):
- GPU kernel: `scrypt.cl`
- CPU support via external libraries (not included by default)

### 5. Work Management

Work flow:
```
Pool → getwork/GBT/Stratum → Work queue → Mining threads → Share submission
```

**Key concepts**:
- **Staged work**: Pool requests queued for processing
- **Cloned work**: Work items split among multiple threads
- **Rollable work**: Can modify nonce/time/extra nonce without fetching new work
- **Stale work**: Discarded if too old (configurable expiry)
- **Longpoll**: Immediate notification of new block (reduces stale rate)

**Work structure** (`struct work`):
- Block header data (80 bytes)
- Midstate (precomputed first 64 bytes hash state)
- Target difficulty
- Pool association
- Timestamp and expiry

### 6. GPU Management (OpenCL)

OpenCL abstraction in `ocl.c`/`ocl.h`:
- Platform enumeration
- Device detection and capability probing
- Kernel compilation and binary caching
- Work queue management
- Result collection

**Kernel parameters**:
- `-I` intensity (affects work size)
- `-g` GPU threads (concurrent work items)
- `-w` worksize (items per kernel invocation)
- `-v` vector size (1, 2, 4, 8 depending on hardware)
- `--kernel` choice of algorithm implementation

**GPU clock/fan control** (ADL/NVML):
- Engine clock speed
- Memory clock speed
- Fan speed
- Voltage (Vddc)
- Temperature monitoring
- Auto-tuning modes

### 7. Monitoring and Statistics

**Statistics collected**:
- Hashrate (5s, average, utility)
- Accepted/rejected/stale shares
- Hardware errors
- Pool latency and efficiency
- Device-specific: temperature, fan speed, clock speeds, voltage
- Network: bytes sent/received, getwork timing

**Display modes**:
- Curses TUI (default with terminal)
- Plain text (`--text-only`)
- RPC JSON API (`--api-listen`)
- Log file with various detail levels

### 8. Configuration System

**Sources** (in order of precedence):
1. Command-line arguments
2. Configuration file (JSON format)
3. Built-in defaults

**Configuration file** (`.nsgminer/nsgminer.conf` or specified):
```json
{
  "pools": [...],
  "intensity": "d",
  "gpu-engine": "0-1000",
  "gpu-memclock": "1500",
  "auto-fan": true,
  ...
}
```

**Option parsing** uses ccan's `opt` library for uniform handling.

## Mining Protocol

### Header Formatting and Endianness

NSGminer supports two distinct header formats depending on the mining algorithm:

- **NeoScrypt-Xaya**: Uses a standard 80-byte block header with **big-endian** byte order.
- **NeoScrypt (regular)**: Uses a 128-byte structure consisting of an 80-byte **little-endian** header followed by 48 bytes of padding. The padding must start with the 4-byte value `0x00000080` at offset 80.

These format differences are critical for share validation. The miner performs header validation at several key points:

- [`work_decode()`](miner.c) – Decodes work from the pool and constructs the appropriate header format.
- [`gen_stratum_work()`](miner.c) – Generates Stratum work templates with correct header layout.
- [`_test_nonce2()`](miner.c) – Validates header integrity before submitting shares.

Ensuring strict adherence to these formats prevents share rejections due to malformed headers.

## Data Flow

### Mining Loop (per device thread)

```
1. Get work from queue (or fetch from pool)
2. Prepare work for device (midstate, target, etc.)
3. Submit to device (OpenCL kernel, CPU function, FPGA command)
4. Wait for completion (blocking or polling)
5. Check results against target
6. Submit valid shares to pool
7. Loop back to step 1
```

### Block Detection

New block detected via:
1. **Longpoll** - Pool notifies immediately (preferred)
2. **Stratum** - Pool sends mining.notify message
3. **GBT polling** - Detect changed `previousblockhash`
4. **Getwork comparison** - Compare block headers (fallback)

When new block found:
- Flush all staged work (it becomes stale)
- Fetch new work immediately
- Log "New block detected" message
- Reset per-block statistics

## Cross-Cutting Concerns

### Threading Model

- **Main thread**: UI, configuration, signal handling
- **Watchdog thread**: Monitor device health, restart hung threads
- **Getwork threads**: One per pool, fetch work asynchronously
- **Mining threads**: One per device (or multiple per GPU with `-g`)
- **Longpoll threads**: One per pool that supports it
- **API thread**: Handle RPC requests
- **Submit threads**: Share submission pool

### Error Handling

- **Device failure**: Mark as DEAD, attempt restart after interval
- **Pool failure**: Mark as idle, failover to backup pools
- **Network errors**: Retry with exponential backoff (configurable `--retries`)
- **GPU hang**: Reinitialize OpenCL context (if possible)
- **FPGA timeouts**: Reopen USB connection, reset if needed

### Signal Handling

- `SIGINT` (Ctrl+C): Graceful shutdown, save config if requested
- `SIGTERM`: Same as SIGINT
- `SIGHUP`: (Unix) Restart (if implemented)

### Resource Management

- **Memory**: Work queues use uthash for dynamic allocation
- **File descriptors**: Limited per-process; pool sockets managed carefully
- **OpenCL resources**: Contexts, command queues, memory buffers cached per device
- **USB handles**: Kept open for devices that need frequent polling (FPGAs)

## Performance Considerations

### GPU Mining
- Use largest possible worksize that fits in GPU memory
- Balance intensity vs. desktop responsiveness (`-I d` for dynamic)
- Minimize PCIe transfers (keep work on GPU when possible)
- Overlap computation with data transfer using async operations

### CPU Mining
- Set CPU affinity to avoid migration overhead
- Use lowest priority scheduling (`idle` or `batch`)
- Choose algorithm based on CPU capabilities (SSE2, Altivec, etc.)
- Limit concurrency to avoid oversubscription

### FPGA Mining
- USB transfer optimization (bulk transfers, minimal overhead)
- Serial baud rate tuning (`--icarus-options baud:...`)
- Work size tuning to match FPGA hash rate

### Network
- Keep-alive connections where supported
- HTTP/1.1 persistent connections
- Limit curl handles to prevent exhaustion (`--queue`)
- Use proxies if needed (SOCKS4/5, HTTP)

## Security Model

**Assumptions**:
- Mining pool operators are trusted (they control code execution)
- RPC API should typically be bound to localhost only
- Configuration files may contain pool credentials (protect accordingly)

**Risks**:
- Malicious pool could send crafted work to exploit client
- RPC API without authentication allows full control
- Configuration parsing uses JSON (vulnerable to parser bugs)
- OpenCL kernels execute untrusted pool code (not applicable; kernels are fixed)

**Mitigations**:
- Use `--api-allow` to restrict RPC access
- Validate all JSON input from pools
- Run mining software with minimal privileges
- Keep dependencies updated (curl, jansson)
- Use HTTPS/TLS for pool communication where available

## Extensibility

### Adding a New Device Driver

1. Create `driver-<name>.c` and `driver-<name>.h`
2. Implement required functions following `driver-*.c` examples
3. Add to `Makefile.am` under appropriate condition
4. Add `--enable-<name>` to `configure.ac`
5. Update `miner.h` with device-specific statistics if needed
6. Document in `concept-architecture-overview.md` and user-facing docs

### Adding a New Algorithm

1. Implement CPU code in `sha256_*.c` or equivalent
2. (Optional) Create OpenCL kernel `.cl` file
3. Add `--enable-<algo>` to `configure.ac`
4. Add algorithm enumeration and detection
5. Update command-line parsing for `--algo` or `--<algo>`
6. Document algorithm support and tuning parameters

### Modifying Pool Protocol

1. Extend `struct pool` with protocol-specific fields
2. Add detection logic in `pool_active()` or similar
3. Implement work generation/submission for protocol
4. Update RPC API if needed to expose protocol info
5. Test with actual pool server implementing protocol

## Future Directions

Given the project's maintenance status (last update 2017), major architectural changes are unlikely. However, if reviving development:

- Consider modularizing drivers as plugins (`.so`/`.dll`)
- Evaluate modern mining protocols (Stratum V2, etc.)
- Update to OpenCL 2.0+ if backward compatibility allows
- Add support for ASIC miners (if any open standards exist)
- Implement better telemetry and metrics (Prometheus exporter?)
- Containerization (Docker) for easy deployment

## See Also

- [Tech stack](tech-stack.md) - Detailed dependency inventory
- [Workflows](workflows.md) - Build and development procedures
- Source code files: `miner.c`, `driver-*.c`, `sha256_*.c`, `*.cl`
- External: [BFGMiner documentation](https://github.com/luke-jr/bfgminer)