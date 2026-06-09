# Agent Guide

Guidance for AI agents (including Kilo Code) working on the NSGminer codebase.

## Quick Reference

- **Primary manual**: `AGENTS.md` (complete operating procedures)
- **Wiki**: `docs/maintainer-wiki/` (canonical documentation)
- **Memory bank**: `.kilocode/rules/memory-bank/` (NON-CANONICAL)
- **Source-of-truth**: Code > configure.ac/Makefile.am > wiki > README/NEWS

## Agent Operating Principles

All agents MUST:

1. **Read AGENTS.md first** - Contains complete policies and workflows
2. **Check the wiki before acting** - Consult `docs/maintainer-wiki/` for technical details
3. **Verify against source code** - Never trust documentation blindly; verify with code
4. **Preserve existing code** - Don't remove unrelated functionality
5. **Document changes** - Update wiki and NEWS for user-visible changes
6. **Respect licensing** - All contributions GPLv3-compatible
7. **Use memory bank appropriately** - It's local-only, gitignored, NON-CANONICAL

## Source-of-Truth Hierarchy

When seeking information, consult in this order:

1. **Source code** (`*.c`, `*.h`, `*.cl`) - Ultimate authority
2. **Build configuration** (`configure.ac`, `Makefile.am`) - Build options, dependencies
3. **Committed wiki** (`docs/maintainer-wiki/`) - Maintainer-approved documentation
4. **Project docs** (`README`, `NEWS`, `AUTHORS`) - User-facing information
5. **External references** - Pool protocols, hardware specs (cite with URLs)
6. **Volatile memory** (`.kilocode/rules/memory-bank/`) - Session context only

## Pre-Flight Checklist

Before making any changes:

- [ ] Read relevant wiki pages (tech-stack.md, workflows.md, concept-architecture-overview.md)
- [ ] Check AGENTS.md for current policies
- [ ] Review source code to verify assumptions
- [ ] Ensure changes align with project architecture
- [ ] Identify which documentation needs updating
- [ ] Verify build/test workflow will still work
- [ ] Consider backward compatibility

## Common Tasks

### Adding a New Feature

1. **Design**: Consider if it fits project scope; check open-work.md for related items
2. **Implementation**:
   - Follow existing code patterns (read similar driver/algorithm code)
   - Add appropriate configure flags if new optional feature
   - Update Makefile.am to include new files
3. **Documentation**:
   - Update README for user-facing changes
   - Update wiki (tech-stack.md, workflows.md, architecture) as needed
   - Add NEWS entry describing the change for users
4. **Testing**: Build and perform basic smoke tests; document test procedure

### Fixing a Bug

1. **Reproduce**: Understand the exact conditions causing the bug
2. **Locate**: Use codebase search to find relevant code
3. **Analyze**: Read surrounding code and understand root cause
4. **Fix**: Minimal change to address issue without breaking other functionality
5. **Test**: Verify fix resolves issue and doesn't introduce regressions
6. **Document**: Update NEWS with bug fix description; if fix reveals design issue, log decision in decisions.md

### Updating Documentation

1. **Verify**: Check source code to confirm technical details
2. **Cite**: Include evidence links (file:line references when possible)
3. **Update index.md**: Add new pages to catalog
4. **Log changes**: Update log.md with date and summary
5. **Validate**: Ensure code/config references are accurate

## File Reference

### Critical Files to Read

- `configure.ac` - Build configuration, feature flags, dependency checks
- `Makefile.am` - Source file organization, build rules
- `miner.c` - Main application logic
- `miner.h` - Core data structures and driver interface
- `driver-*.c` - Examples of driver implementation patterns
- `sha256_*.c`, `neoscrypt.c` - Algorithm implementations

### Documentation Files

- `README` - User documentation (also check for `*README` in subdirectories)
- `NEWS` - Version history and user-facing changes
- `AUTHORS` - Contributors
- `COPYING` - License terms (GPLv3)

### Configuration Examples

- `example.conf` - Sample configuration file
- `API-README` - RPC API documentation
- `FPGA-README` - FPGA-specific setup and usage
- `SCRYPT-README` - Scrypt algorithm tuning

## Code Style and Conventions

- **Follow existing style**: Indentation, naming, brace placement
- **Comments**: Explain "why", not "what" (code should be self-explanatory)
- **Error handling**: Check return values; use `applog` for messages
- **Memory**: Free resources on all error paths; use `calloc`/`realloc` carefully
- **Thread safety**: Use mutexes (`mtx_` variables) for shared data
- **Portability**: Use platform abstraction (see `compat.h`, `util.c`)
- **Strings**: Use `strdup`, `realloc_strcat`, not manual `malloc`+`strcpy`

## Testing Guidelines

### Before Committing

1. **Build**:
   ```bash
   ./autogen.sh  # if from git
   ./configure [your options]
   make -j$(nproc)
   ```
   Ensure no warnings/errors.

2. **Basic functionality**:
   ```bash
   ./nsgminer --version
   ./nsgminer -n  # List devices
   ```
   Verify expected outputs.

3. **Integration** (if applicable):
   - Test with a mining pool (use test pool if available)
   - Verify device detection and initialization
   - Check that shares are accepted

### Test Infrastructure

If `make check` exists, run it:
```bash
make check
```

If not, check for test programs in `tests/` or similar.

## Validation Commands

Check if documentation consistency scripts exist:
```bash
# Look for docs:check or similar
ls -1 *.md  # If package.json exists (unlikely for C project)
```

If none exist, manually verify:
- All code references in docs match actual code
- Configuration options documented correctly
- Build commands work as documented

## Memory Bank Usage

The `.kilocode/rules/memory-bank/` is for **temporary, session-scoped context only**:

- `brief.md` - Project summary (already created; update if major changes)
- `context.md` - Current work state (update when working on different tasks)
- `active.md` - Active tasks and questions (track ongoing work)

**Do NOT put**:
- Canonical information (goes in wiki)
- Secrets or credentials (never commit these)
- Completed work (should be committed to wiki/code)

**Update memory bank when**:
- Switching to a new task
- Discovering important context not yet in wiki
- Tracking open questions
- Before completing task to summarize progress

## Red Flags

Be cautious of:

- **Old dependencies**: jansson, libblkmaker may have CVEs; check current versions
- **Build failures**: Don't commit code that breaks `./configure && make`
- **Documentation drift**: Keep wiki in sync with code changes
- **API breakage**: RPC API should maintain backward compatibility
- **License compliance**: Ensure contributions are GPLv3-compatible

## Communication

When submitting changes:

- **Commit messages**: Be concise but descriptive; reference issue numbers
- **Pull requests**: Include summary, rationale, testing done
- **Wiki updates**: Commit alongside code changes when related
- **Respect maintainers**: Changes should be minimal, focused, well-tested

## Getting Help

If stuck:

1. **Search existing wiki** - Information may already be documented
2. **Read source code** - Often the best reference
3. **Check external docs** - OpenCL spec, ADL documentation, hardware manuals
4. **Consult AGENTS.md** - Contains extended guidance and examples
5. **Ask user** - If clarification needed, use `ask_followup_question`

## Maintenance Mode Reminder

NSGminer is in **maintenance mode** (last release 2017). Prioritize:

1. **Documentation** - Preserve institutional knowledge
2. **Bug fixes** - Critical issues only
3. **Security** - Address vulnerabilities if discovered
4. **Compatibility** - Ensure builds on modern systems

Avoid adding major new features unless there's strong justification and maintainer consensus.

---

**Remember**: The source code is always the ultimate authority. When in doubt, read it.