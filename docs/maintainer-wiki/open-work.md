# Open Work

Unresolved issues, known gaps, and areas needing improvement in NSGminer.

## Known Issues

### Maintenance Mode
- **Issue**: Project appears unmaintained since 2017 (v0.9.4)
- **Impact**: No bug fixes or new features for 8+ years
- **Recommendation**: Fork or migrate to actively maintained alternatives (cgminer, bfgminer)
- **Evidence**: `NEWS` last entry: "NSGminer v0.9.4 - 26th of December 2017"

### Build System
- **Gap**: No CI/CD pipeline detected
- **Impact**: Manual testing required for each platform
- **Suggestion**: Set up GitHub Actions or similar for automated builds/test

### Test Coverage
- **Issue**: Limited test infrastructure
- **Evidence**: `make check` may not exist or have minimal coverage
- **Need**: Unit tests for critical algorithms (sha256, neoscrypt, scrypt)

### Documentation Validation
- **Gap**: No automated documentation consistency check script implemented
- **Impact**: Documentation drift may go undetected without manual review
- **Evidence**: No `docs:check` script found; validation currently manual (`agent-guide.md`)
- **Need**: Implement a docs consistency validator aligned with wiki evidence references

### Documentation Gaps
- Missing detailed FPGA bitstream documentation
- No API versioning policy documented
- Incomplete contributor guidelines
- No security policy or vulnerability reporting process

### Algorithm Support
- **Status**: CPU mining algorithms lack modern optimizations
- **Gap**: No support for newer algorithms (e.g., ethash, KawPow, etc.)
- **Context**: Project focuses on legacy algorithms (SHA-256d, Scrypt, NeoScrypt)

### Hardware Support
- **Missing**: Modern GPU support may be incomplete
- **Concern**: ADL/NVML APIs may be outdated
- **Test needed**: Verify compatibility with recent drivers/OpenCL versions

### Security
- **Unknown**: Security audit status
- **Potential**: No dependency vulnerability scanning
- **Note**: Old dependencies (jansson, libblkmaker) may have known vulnerabilities

## Architecture Questions

### Memory Bank Usage
- Should we formalize `.kilocode/rules/memory-bank/` content structure?
- Need guidelines on what belongs in memory vs. wiki

### Code Organization
- Could driver code be better abstracted?
- Are there opportunities to reduce code duplication across drivers?

### Configuration
- Consider migrating from JSON to more structured config format?
- Should default values be centralized?

## Improvements Needed

1. **Modernize build system**: Consider CMake or Meson for better cross-platform support
2. **Add proper testing**: Unit tests, integration tests, fuzzing
3. **Update dependencies**: Newer jansson, libcurl, OpenCL support
4. **Improve documentation**: API docs, developer guide, hardware compatibility matrix
5. **Security hardening**: Input validation, fuzzing, dependency scanning
6. **Community**: Establish contribution guidelines, code of conduct
7. **Monitoring**: Add metrics/telemetry (configurable, privacy-respecting)

## Feature Requests (From Community)

- Stratum protocol improvements
- Better Windows support
- Linux distribution packages (deb, rpm)
- Docker container builds
- RPC API authentication/authorization
- Configurable alerting (email, webhook)
- Performance profiling tools

## Research Tasks

- Investigate active forks that may have better maintenance
- Assess viability of updating to modern OpenCL standards
- Review security of current RPC implementation
- Benchmark against current mining software

## Prioritization

### High Priority
- Address security vulnerabilities (if any)
- Update dependencies with known CVEs
- Document build process for common platforms

### Medium Priority
- Add automated testing
- Improve documentation gaps
- Modernize build system

### Low Priority
- Algorithm optimizations
- New hardware support (unlikely given project state)
- Feature enhancements

## How to Help

If you want to contribute to resolving open work:

1. Check [GitHub Issues](https://github.com/ghostlander/nsgminer/issues) for current status
2. Pick an item from this list and announce your intent
3. Follow AGENTS.md guidelines for code quality and documentation
4. Submit PR with tests and documentation updates
5. Update this page with resolution notes

## Closing Items

When an issue is resolved:

1. Move item to "Completed" section with date and resolution summary
2. Update relevant wiki pages (workflows, architecture)
3. Document any lessons learned

---

**Note**: This is a living document. New items should be added as they're identified.