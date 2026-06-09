# Workflows

Standard procedures for building, testing, and releasing NSGminer.

## Build Workflow

### Prerequisites

#### Debian/Ubuntu
```bash
sudo apt-get update
sudo apt-get install -y build-essential autoconf automake libtool pkg-config \
  libcurl4-gnutls-dev uthash-dev libncursesw5-dev
```

For GPU mining (AMD):
```bash
# Install AMD APP SDK (2.4-2.6 recommended)
# Download from AMD, then:
sudo tar -C /opt -xzf AMD-APP-SDK-*.tgz
sudo ln -s /opt/AMD-APP-SDK-*/include/CL /usr/include
sudo ln -s /opt/AMD-APP-SDK-*/lib/x86_64/* /usr/lib/
sudo ldconfig
```

For FPGA mining (X6500, ZTEX):
```bash
sudo apt-get install -y libusb-1.0-0-dev
```

For CPU mining optimizations:
```bash
sudo apt-get install -y yasm
```

#### macOS (Homebrew)
```bash
brew install autoconf automake libtool pkg-config curl uthash ncurses
# For OpenCL: Install appropriate SDK from AMD/NVIDIA
```

#### Windows
See `windows-build.txt` (if present) or use MSYS2/MinGW environment.

### Building

```bash
# From git repository (first time)
./autogen.sh

# Configure with desired options
./configure [OPTIONS]

# Common options:
# --disable-opencl        # Build without GPU support
# --enable-scrypt         # Enable Scrypt mining
# --enable-sha256d        # Enable SHA-256d mining
# --enable-cpumining      # Enable CPU mining
# --disable-adl           # Disable AMD GPU monitoring
# --disable-nvml          # Disable NVIDIA GPU monitoring
# --enable-bitforce       # Enable BitForce FPGA
# --enable-icarus         # Enable Icarus FPGA
# --enable-modminer       # Enable ModMiner FPGA
# --enable-x6500          # Enable X6500 FPGA
# --enable-ztex           # Enable ZTEX FPGA
# --without-curses        # Build without ncurses UI

# Build
make -j$(nproc)

# Optional: Run tests if available
make check
```

### Build Outputs

- `nsgminer` - Main miner executable
- `nsgminer-rpc` - RPC command-line tool (if built)
- `bitforce-firmware-flash` - FPGA firmware utility (if built)

## Testing Workflow

### Basic Functionality Tests

```bash
# 1. Check version
./nsgminer --version

# 2. List detected devices (no mining)
./nsgminer -n

# 3. Run with test pool (if available) or benchmark mode
./nsgminer --benchmark -I 9

# 4. Test with configuration file
./nsgminer -c example.conf --dry-run  # if supported
```

### Pool Connectivity Test

```bash
# Test pool connection without mining (use test pool)
./nsgminer -o http://testpool:port -u testuser -p testpass -I d
```

### FPGA Detection

```bash
# With debug output to see autodetection
./nsgminer -D -d? -T
```

### GPU Mining Test (AMD)

```bash
# Ensure OpenCL platform is detected
./nsgminer -n

# Start with conservative intensity
./nsgminer -o http://pool:port -u user -p pass -I d --kernel neoscrypt
```

## Debugging Workflow

### Enable Debug Logging

```bash
# Verbose output
./nsgminer -v -D -T

# Log to file
./nsgminer -o ... 2>debug.log

# With API enabled for remote monitoring
./nsgminer -o ... --api-listen --api-allow 127.0.0.1
```

### Common Issues

| Issue | Check | Fix |
|-------|-------|-----|
| No GPU detected | `./nsgminer -n` output | Ensure X server running, DISPLAY set, OpenCL SDK installed |
| ADL errors | Verify AMD drivers | Ensure APP SDK installed, ADL support enabled |
| FPGA not found | Check udev rules, permissions | Add user to dialout group, reload udev rules |
| High CPU usage | Driver version | Try `export GPU_USE_SYNC_OBJECTS=1` on Linux |
| Kernel build fails | OpenCL headers | Verify CL/cl.h exists in include path |

## Release Workflow

### Versioning

NSGminer uses semantic versioning: `MAJOR.MINOR.PATCH`

- MAJOR: Incompatible API changes
- MINOR: New features (backward compatible)
- PATCH: Bug fixes (backward compatible)

Current: `0.9.4`

### Pre-Release Checklist

1. **Update version** in `configure.ac`:
   ```m4
   m4_define([v_maj], [0])
   m4_define([v_min], [9])
   m4_define([v_mic], [4])  # Increment for release
   ```

2. **Update NEWS file** with user-facing changes since last release

3. **Test build** on all supported platforms:
   - Linux (x86_64, i686)
   - Windows (mingw32)
   - macOS (if supported)

4. **Verify configure options**:
   ```bash
   ./configure && make clean && make -j$(nproc)
   ```

5. **Tag release**:
   ```bash
   git tag -a v0.9.4 -m "Release 0.9.4"
   git push origin v0.9.4
   ```

6. **Create distribution tarball**:
   ```bash
   ./autogen.sh
   ./configure
   make dist
   ```

### Post-Release

- Update website/documentation with download links
- Announce on relevant channels (BitcoinTalk, GitHub Releases)
- Monitor issues and prepare patches if needed

## Feature Development Workflow

### Adding a New Algorithm

1. Implement CPU code in `sha256_*.c` or `neoscrypt.c`
2. Add OpenCL kernel if GPU support desired
3. Update `configure.ac` with `--enable-<algo>` option
4. Add command-line argument handling in `miner.c`
5. Test thoroughly with various hardware
6. Document in README and NEWS
7. Update decision log (decisions.md)

### Adding New Hardware Support

1. Create driver file `driver-<device>.c` following patterns in existing drivers
2. Implement required functions: `detect`, `initialize`, `hash`, `shutdown`
3. Add `--enable-<device>` to `configure.ac`
4. Update `Makefile.am` to include new driver
5. Add hardware-specific documentation
6. Test on actual hardware
7. Update concept-architecture-overview.md

### Modifying Configuration

1. Add `opt_` variable in `miner.h`
2. Add command-line option in `opt.c` (from ccan)
3. Update `parse_config` and `write_config` functions
4. Add help text in usage()
5. Document in README and API-README if API-accessible
6. Update example.conf

## Documentation Update Workflow

When code changes require documentation updates:

1. **Identify affected docs**:
   - README: User-facing changes
   - NEWS: What's new for users
   - API-README: RPC API changes
   - FPGA-README: FPGA-specific changes
   - SCRYPT-README: Scrypt tuning changes
   - Wiki: Technical details for maintainers

2. **Update in same commit** if minor, separate commit if major

3. **Follow documentation standards**:
   - Be accurate and specific
   - Include examples where helpful
   - Cite evidence from source code
   - Mark uncertain info as UNVERIFIED

4. **Validate wiki links** and index references

## Validation

Before committing documentation changes:

- [ ] All technical facts verified against source code
- [ ] Configuration options documented with default values
- [ ] Hardware requirements clearly stated
- [ ] Build instructions tested on clean environment
- [ ] Wiki pages referenced from index.md
- [ ] CHANGELOG updated for user-visible changes
- [ ] No speculative or unverified information included

See also: [Agent guide](agent-guide.md) for AI-assisted documentation workflows.