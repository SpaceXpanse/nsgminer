# Tech Stack

Technology inventory for NSGminer with evidence links to source files.

## Runtime & Language

| Component | Version/Details | Evidence |
|------------|-----------------|----------|
| Primary Language | C (C99) | `configure.ac:AC_PROG_CC_C99` |
| Build System | GNU Autotools (autoconf, automake, libtool) | `autogen.sh`, `configure.ac`, `Makefile.am` |
| License | GPLv3 | `COPYING`, `configure.ac` |

## Core Dependencies

### Required Libraries
- **libcurl** (with GnuTLS or OpenSSL) - HTTP/JSON-RPC communication
  - Check: `configure.ac:AC_CHECK_LIB([curl], [...])`
  - Used in: `api.c`, `miner.c` for pool communication
- **uthash** - Hash table implementation
  - Located in: `uthash.h` (vendored)
  - Used throughout for device tracking, work queues
- **Curses/ncurses** - Terminal UI
  - Check: `configure.ac:AC_CHECK_LIB([ncursesw], [...])`
  - Optional via `--without-curses`

### Optional Libraries
- **OpenCL** (1.0+) - GPU mining
  - Headers: `CL/cl.h` (vendor-provided or `CL/` directory)
  - Check: `configure.ac:AC_CHECK_HEADERS(\[CL/cl.h\])`
  - Enabled by default, `--disable-opencl` to skip
- **ADL** (AMD Display Library) - AMD GPU monitoring/control
  - Located in: `ADL/adl_sdk.h`, `ADL/adl_structures.h`, `ADL/adl_defines.h`
  - Check: `configure.ac:AC_CHECK_HEADERS([adl_sdk.h])`
  - Enabled by default, `--disable-adl` to skip
- **NVML** (NVIDIA Management Library) - NVIDIA GPU monitoring
  - Located in: `NVML/nvml.h` (vendored)
  - Check: `configure.ac:AC_CHECK_HEADERS([nvml.h])`
  - Enabled by default, `--disable-nvml` to skip
- **libusb-1.0** - FPGA device communication
  - Used for: X6500, ZTEX boards
  - Check: `configure.ac:AC_CHECK_LIB([usb-1.0], [...])`
- **libudev** - FPGA autodetection on Linux
  - Check: `configure.ac:AC_CHECK_LIB([udev], [...])`
  - Optional via `--without-libudev`
- **yasm** (1.0.1+) - Assembly optimizations for CPU mining
  - Check: `configure.ac:AC_CHECK_PROG([YASM], [yasm])`

## Platform-Specific

### Unix/Linux
- **epoll** - I/O multiplexing for FPGA devices
  - Check: `configure.ac:AC_CHECK_HEADERS([sys/epoll.h])`
- **prctl** - Thread naming
  - Check: `configure.ac:AC_CHECK_HEADERS([sys/prctl.h])`
- **libdl** - Dynamic library loading (OpenCL)
  - Linked via `-ldl` when OpenCL enabled

### Windows
- **ws2_32** - Windows sockets
  - Linked via `-lws2_32`
- **pthreads-win32** - POSIX threads compatibility
  - Linked via `-lpthread` (mingw)
- **PDCurses** - Curses implementation for Windows

### macOS
- No special dependencies beyond standard Unix libraries
- Uses system frameworks for OpenCL

## Vendored Libraries

NSGminer includes several libraries directly in the repository:

| Library | Directory | Purpose |
|---------|-----------|---------|
| uthash | `uthash.h` | Hash tables |
| libbase58 | `libbase58/` | Base58 encoding/decoding |
| libblkmaker | `libblkmaker/` | GetBlockTemplate support |
| libjansson | `libjansson/` | JSON parsing |
| NVML headers | `NVML/` | NVIDIA GPU monitoring |
| ccan | `ccan/` | Compiler collections (various utilities) |
| gnulib replacements | `lib/` | Portability layer |

## Kernel Code

OpenCL kernels for different algorithms and architectures:

| Kernel | File | Algorithm | Target |
|--------|------|-----------|--------|
| neoscrypt | `neoscrypt.cl` | NeoScrypt | Generic |
| neoscrypt_vliw | `neoscrypt_vliw.cl` | NeoScrypt | AMD VLIW |
| neoscrypt_vliwp | `neoscrypt_vliwp.cl` | NeoScrypt | AMD VLIW (parallel) |
| diablo | `diablo.cl` | SHA-256d | Generic |
| diakgcn | `diakgcn.cl` | SHA-256d | AMD GCN |
| phatk | `phatk.cl` | SHA-256d | General purpose |
| poclbm | `poclbm.cl` | SHA-256d | Python OpenCL Bitcoin Miner variant |
| scrypt | `scrypt.cl` | Scrypt | Generic |

## CPU Mining Algorithms

Assembly/C implementations in `sha256_*.c` and `neoscrypt.c`:

- `sha256_generic.c` - Pure C implementation
- `sha256_sse2_amd64.c` - SSE2 64-bit x86
- `sha256_sse2_i386.c` - SSE2 32-bit x86
- `sha256_sse4_amd64.c` - SSE4 64-bit x86
- `sha256_4way.c` - 4-way parallel (tcatm)
- `sha256_altivec_4way.c` - Altivec 4-way (PowerPC)
- `sha256_cryptopp.c` - Crypto++ implementation
- `sha256_via.c` - VIA Padlock hardware acceleration
- `neoscrypt.c` - CPU NeoScrypt implementation

## Configuration

Build configuration via `configure.ac`:

### Feature Flags
- `--disable-neoscrypt` / `--enable-neoscrypt` (default: enabled)
- `--enable-scrypt` / `--disable-scrypt` (default: disabled)
- `--enable-sha256d` / `--disable-sha256d` (default: disabled)
- `--enable-cpumining` / `--disable-cpumining` (default: disabled)
- `--disable-opencl` / `--enable-opencl` (default: enabled)
- `--disable-adl` / `--enable-adl` (default: enabled)
- `--disable-nvml` / `--enable-nvml` (default: enabled)

### Hardware Support
- `--enable-bitforce` - BitForce FPGA support
- `--enable-icarus` - Icarus FPGA support
- `--enable-modminer` - ModMiner FPGA support
- `--enable-x6500` - X6500 FPGA support (requires libusb)
- `--enable-ztex` - ZTEX FPGA support (requires libusb)

### UI & Detection
- `--without-curses` - Disable curses TUI (default: enabled)
- `--without-libudev` - Disable libudev autodetection (default: enabled)

## Evidence Collection Notes

This inventory is derived from:
- `configure.ac` (build system and feature detection)
- `Makefile.am` (source file organization)
- `README` (user-facing documentation)
- Source file headers and comments
- Direct inspection of vendor code in `ADL/`, `NVML/`, `lib/`

For detailed configure checks, see `configure.ac` lines 120-350 (feature detection).
For library dependencies, see `configure.ac` lines 70-120 (header/library checks).