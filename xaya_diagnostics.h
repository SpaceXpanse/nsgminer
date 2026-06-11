/* Xaya Neoscrypt Diagnostic Tools
 * 
 * This file provides diagnostic functions to debug share rejection issues.
 * Add these to miner.c and call them from gen_stratum_work(), begin_submission(), 
 * and submit_upstream_work_request() when debugging.
 * 
 * Usage: 
 *   gcc -DDEBUG_XAYA -o test_xaya test_xaya_diagnostics.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Print a 80-byte block header in human-readable format */
void print_block_header_diagnostic(const uint8_t *data, const char *label, int is_xaya) {
    printf("=== %s (Xaya=%d) ===\n", label, is_xaya);
    
    printf("Version:       %08x (big-endian)\n", 
           (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    
    printf("PrevHash:      ");
    for (int i = 4; i < 36; i++) printf("%02x", data[i]);
    printf("\n");
    
    printf("MerkleRoot:    ");
    for (int i = 36; i < 68; i++) printf("%02x", data[i]);
    printf("\n");
    
    printf("Time:          %08x\n", 
           (data[68] << 24) | (data[69] << 16) | (data[70] << 8) | data[71]);
    
    printf("NBits:         %08x\n", 
           (data[72] << 24) | (data[73] << 16) | (data[74] << 8) | data[75]);
    
    printf("Nonce:         %08x\n", 
           (data[76] << 24) | (data[77] << 16) | (data[78] << 8) | data[79]);
    
    /* Xaya-specific: Check if bytes 36-67 contain Neoscrypt hash of real header */
    if (is_xaya) {
        printf("[XAYA] Bytes 36-67 should be Neoscrypt(real_header_hash)\n");
        printf("[XAYA] Bytes 37-68 per spec should contain real block hash\n");
    }
    printf("\n");
}

/* Print stratum submission parameters */
void print_stratum_submit_diagnostic(const char *user, const char *job_id,
                                    const char *nonce2, const char *ntime,
                                    const uint8_t *nonce, int is_xaya) {
    printf("=== Stratum Submit Parameters (Xaya=%d) ===\n", is_xaya);
    printf("User:     %s\n", user);
    printf("JobID:    %s\n", job_id);
    printf("Nonce2:   %s\n", nonce2);
    printf("nTime:    %s\n", ntime);
    printf("Nonce:    %02x%02x%02x%02x\n", nonce[0], nonce[1], nonce[2], nonce[3]);
    
    if (is_xaya) {
        /* For Xaya, nonce should be in big-endian format for the fake header */
        uint32_t nonce_val = (nonce[0] << 24) | (nonce[1] << 16) | (nonce[2] << 8) | nonce[3];
        printf("[XAYA] Nonce value: %u (0x%08x)\n", nonce_val, nonce_val);
    }
    printf("\n");
}

/* Compare two block headers and report differences */
void compare_block_headers(const uint8_t *expected, const uint8_t *actual, 
                          const char *exp_label, const char *act_label) {
    printf("=== Header Comparison ===\n");
    
    int diff_count = 0;
    for (int i = 0; i < 80; i++) {
        if (expected[i] != actual[i]) {
            printf("DIFF at offset %3d: expected %02x, got %02x\n", 
                   i, expected[i], actual[i]);
            diff_count++;
            if (diff_count >= 10) {
                printf("(showing first 10 differences only)\n");
                break;
            }
        }
    }
    
    if (diff_count == 0) {
        printf("Headers are IDENTICAL\n");
    }
    printf("\n");
}

/* Verify endianness of multi-word fields */
void check_endianness(const uint8_t *data) {
    printf("=== Endianness Check ===\n");
    
    uint32_t version = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    uint32_t nbits = (data[72] << 24) | (data[73] << 16) | (data[74] << 8) | data[75];
    uint32_t nonce = (data[76] << 24) | (data[77] << 16) | (data[78] << 8) | data[79];
    
    printf("version = 0x%08x (BE interpretation)\n", version);
    printf("nbits   = 0x%08x (BE interpretation)\n", nbits);
    printf("nonce   = 0x%08x (BE interpretation)\n", nonce);
    
    /* Check if version looks reasonable for a crypto chain */
    if (version > 0x20000000) {
        printf("NOTE: Version field seems high, may indicate endianness issue\n");
    }
    printf("\n");
}

#ifndef STANDALONE_DIAGNOSTIC
/* Stub for when included in miner.c - replace with actual applog calls */
#define applog_diag(priority, fmt, ...) \
    do { if (opt_debug) printf("[DBG] " fmt "\n", ##__VA_ARGS__); } while(0)
#else
#include <stdarg.h>
#include <time.h>
#define applog_diag(priority, fmt, ...) \
    do { printf("[" #priority "] " fmt "\n", ##__VA_ARGS__); } while(0)
#endif

int main(int argc, char *argv[]) {
    printf("Xaya Neoscrypt Diagnostic Tools\n");
    printf("==============================\n\n");
    
    /* Example block header (random test data) */
    uint8_t test_header[80] = {0};
    
    /* Set some test values */
    test_header[0] = 0x03;  /* version */
    test_header[1] = 0x00;
    test_header[2] = 0x00;
    test_header[3] = 0x00;
    
    /* Fill with test pattern */
    for (int i = 4; i < 80; i++) {
        test_header[i] = (uint8_t)(i * 17 + 1);
    }
    
    printf("Testing with sample header:\n\n");
    print_block_header_diagnostic(test_header, "Test Header", 1);
    check_endianness(test_header);
    
    printf("\nTo use these diagnostics in miner.c:\n");
    printf("1. Add #include \"xaya_diagnostics.h\" to miner.c\n");
    printf("2. Call print_block_header_diagnostic() in gen_stratum_work() after building header\n");
    printf("3. Call print_stratum_submit_diagnostic() in begin_submission() before sending\n");
    printf("4. Rebuild with --enable-debug and run with -D flag\n");
    
    return 0;
}