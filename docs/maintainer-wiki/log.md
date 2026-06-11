# Wiki Update Log

Chronological record of significant updates to the maintainer wiki.

## 2026-06-11: Wiki Accuracy Audit and Evidence Correction

**Performed by**: Documentation Specialist maintenance review
**Purpose**: Ensure wiki accuracy by cross-referencing documentation with actual source code and build configuration.

### Changes Made

- **tech-stack.md**: Corrected `configure.ac` evidence line reference from `120-350` to `145-510` to match the actual feature detection section in the repository.
- **decisions.md**: Added new decision entry documenting this evidence correction and its rationale.
- **open-work.md**: Added explicit item for missing automated documentation validation script (no `docs:check` equivalent currently implemented).
- **log.md**: Updated to record this review (this entry).

### Verification

All existing wiki pages were reviewed for alignment with:
- Actual `configure.ac` content
- Actual `Makefile.am` content
- Actual project directory structure

No missing required pages were found. `index.md` already catalogs all existing pages.

## 2025-06-09: Initial Wiki Setup

**Created by**: Wiki initialization process
**Purpose**: Establish canonical documentation structure for NSGminer

### Pages Created

- `index.md` - Wiki catalog and getting started guide
- `README.md` - Maintainer wiki overview and usage instructions
- `tech-stack.md` - Complete technology inventory with evidence links
- `workflows.md` - Standard build/test/release procedures
- `decisions.md` - Decision log (initially empty)
- `open-work.md` - Known issues and improvement areas
- `concept-architecture-overview.md` - High-level system design
- `agent-guide.md` - AI agent operating procedures

### Concurrent Changes

- Created `AGENTS.md` - Complete maintainer/agent operating manual
- Created `CLAUDE.md` - Quick reference deferring to AGENTS.md
- Updated `.gitignore` to exclude `.kilocode/` directory
- Initialized volatile memory bank:
  - `.kilocode/rules/memory-bank/brief.md` - Project brief
  - `.kilocode/rules/memory-bank/context.md` - Current context
  - `.kilocode/rules/memory-bank/active.md` - Active tasks

### Notes

This initialization was performed automatically as part of project documentation setup. The wiki structure follows standard Kilo Code conventions with source-of-truth hierarchy: code → build configs → wiki → project docs.

All pages were created based on analysis of:
- `configure.ac` - Build system and feature flags
- `Makefile.am` - Source file organization
- `README` - User-facing documentation
- Source code structure and headers

### Open Questions

- Should CHANGELOG.md be created/updated? (No existing file detected)
- Actual test infrastructure unknown (need to inspect `Makefile.am` targets)
- Security audit status unclear
- Hardware-specific documentation may need expansion

## Future Updates

When adding new pages or making significant changes:
1. Update this log with date, author, and summary
2. Update `index.md` catalog if new pages added
3. Validate all links and references
4. Commit alongside code changes where applicable

---

**Format**: YYYY-MM-DD: Brief description
**Order**: Most recent at top
**Level of detail**: One paragraph per major update