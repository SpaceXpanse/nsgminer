# NSGminer Maintainer/Agent Guide

## Purpose

This document serves as the primary operating manual for maintainers, contributors, and AI agents working on the NSGminer codebase. It defines the canonical documentation structure, information sourcing policies, and workflows for this repository.

## Project Identity

**NSGminer** (version 0.9.4) is a multithreaded multipool cryptocurrency mining software supporting GPU (OpenCL), CPU, and FPGA mining. It is a fork of BFGMiner/CGMiner with extensive hardware support and NeoScrypt features.

- **License**: GPLv3 (see COPYING)
- **Build System**: GNU Autotools (autoconf, automake, libtool)
- **Primary Language**: C
- **Repository**: https://github.com/ghostlander/nsgminer
- **Maintainer**: John Doering <ghostlander@phoenixcoin.org>

## Source-of-Truth Policy

Information in this repository must be authoritative and verifiable:

1. **Code is law**: The actual implementation in source files is the ultimate source of truth.
2. **Documentation must match code**: All documentation must be kept in sync with code changes.
3. **Cite sources**: When documenting external dependencies or protocols, cite official sources with URLs and retrieval dates.
4. **No speculation**: Only document verified functionality; mark uncertain information as `UNVERIFIED` with explanation.

## Maintainer Wiki (docs/maintainer-wiki/)

The maintainer wiki contains durable, committed documentation. All agents and maintainers MUST consult these pages before making changes:

- `index.md` - Complete catalog of all wiki pages
- `tech-stack.md` - Technology inventory with evidence links
- `workflows.md` - Standard procedures (build, test, release)
- `decisions.md` - Record of architectural and technical decisions
- `open-work.md` - Unresolved issues and known gaps
- `concept-architecture-overview.md` - High-level system design
- `agent-guide.md` - This document (agent-specific guidance)
- `log.md` - Chronological record of wiki updates

**Never edit wiki pages without first running documentation validation** (if scripts exist) and ensuring changes are committed.

## Volatile Memory Bank (`.kilocode/rules/memory-bank/`)

Kilo Code may use a local-only, gitignored memory bank for session-scoped context. This memory is **NON-CANONICAL** - it may contain temporary notes, incomplete analysis, or session-specific information. Always verify critical facts against the wiki or source code.

Memory bank files:
- `brief.md` - Brief project summary (1-2 paragraphs)
- `context.md` - Current work context, recent changes, next steps
- `active.md` - Active tasks and known issues

**Rule**: Update memory bank after significant work, before task completion, or when context changes. Use Documentation Specialist mode for these updates.

## Agent Operating Principles

When working on NSGminer:

1. **Start with documentation**: Read the wiki and relevant source files before making changes.
2. **Preserve existing code**: Do not remove unrelated functionality; maintain backward compatibility where possible.
3. **Test before commit**: Build the project and run basic functionality tests.
4. **Follow coding style**: Existing code uses a specific style; match it in new code.
5. **Document changes**: Update relevant wiki pages when fixing bugs or adding features.
6. **Update NEWS**: The NEWS file tracks version changes; add entries for user-visible changes.
7. **Respect licensing**: All contributions must be GPLv3-compatible.

## Restricted Files

NEVER read or modify files containing sensitive data:
- supersecrets.txt
- credentials.json
- .env

These are explicitly gitignored and should not exist in the repository. If encountered, report immediately.

## Information Validation Priority

When determining "how things work" or "what the current state is", use this source hierarchy:

1. **Source code** (ultimate truth)
2. **Build configuration** (configure.ac, Makefile.am)
3. **Committed documentation** (docs/maintainer-wiki/)
4. **README, NEWS, AUTHORS** (project-level docs)
5. **External references** (pool protocols, hardware specs)
6. **Volatile memory** (session context - verify before use)

## Common Tasks and Workflows

### Building and Testing

```bash
./autogen.sh        # Only needed for git builds
./configure         # Optional flags: --disable-opencl, --enable-scrypt, etc.
make
make check          # Run tests if available
```

See `workflows.md` for complete procedures.

### Adding New Hardware Support

1. Add driver files following patterns in `driver-*.c`
2. Update `configure.ac` with new `--enable-<device>` option
3. Document in `concept-architecture-overview.md`
4. Test thoroughly before submitting PR

### Modifying Algorithms

NeoScrypt, Scrypt, and SHA-256d implementations have dedicated code:
- Kernel files: `neoscrypt.cl`, `scrypt.cl`, `diablo.cl`, etc.
- CPU implementations: `sha256_*.c`, `neoscrypt.c`
- Configuration: `configure.ac` (USE_NEOSCRYPT, USE_SCRYPT, USE_SHA256D)

## Reporting Issues

Use GitHub Issues with appropriate templates. Include:
- NSGminer version
- Hardware details
- OS and build environment
- Debug output (`-D -T --verbose`)
- Steps to reproduce

## Further Reading

- `docs/maintainer-wiki/` - Complete maintainer documentation
- API documentation in `API-README`
- FPGA documentation in `FPGA-README`
- Scrypt tuning in `SCRYPT-README`
- NEWS file - version history and changes

---

**Remember**: The wiki is the canonical source for maintainer knowledge. The memory bank is for temporary session use only. When in doubt, read the source code.