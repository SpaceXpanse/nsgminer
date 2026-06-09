# Maintainer Wiki

Welcome to the NSGminer maintainer wiki. This documentation provides canonical information for anyone working on the NSGminer codebase.

## Purpose

This wiki serves as the single source of truth for:
- Technical architecture and design decisions
- Build and development workflows
- Testing and release procedures
- Hardware support and algorithms
- API specifications and protocols

## Structure

The wiki is organized into several key pages:

- **index.md**: This catalog of all wiki pages
- **tech-stack.md**: Complete technology inventory with evidence links
- **workflows.md**: Standard procedures for building, testing, and releasing
- **decisions.md**: Record of architectural and technical decisions with rationales
- **open-work.md**: Known issues, gaps, and unfinished work
- **concept-architecture-overview.md**: High-level system design and organization
- **agent-guide.md**: Guidance for AI agents and automated workflows
- **log.md**: Chronological record of wiki updates

## How to Use

### For New Maintainers

1. Start with [Tech stack](tech-stack.md) to understand the project's dependencies
2. Review [Workflows](workflows.md) to learn build/test/release procedures
3. Check [Open work](open-work.md) for current issues that need attention
4. Consult [Architecture overview](concept-architecture-overview.md) to understand code organization

### For AI Agents

1. Read [Agent guide](agent-guide.md) for specific operating procedures
2. Always verify information against source code first
3. Use the source-of-truth hierarchy: code → configure.ac/Makefile.am → wiki → README/NEWS
4. Update relevant wiki pages when fixing bugs or adding features
5. Remember: Volatile memory (`.kilocode/rules/memory-bank/`) is NON-CANONICAL

### For Contributors

1. Ensure you understand the [Source-of-Truth Policy](ag​​ents.md#source-of-truth-policy) from AGENTS.md
2. Follow coding standards and maintain backward compatibility
3. Test changes before committing (build + basic functionality)
4. Document changes in relevant wiki pages and NEWS file
5. Respect GPLv3 licensing for all contributions

## Source-of-Truth Hierarchy

When gathering information, use this priority order:

1. **Source code** (ultimate truth) - The actual implementation is always authoritative
2. **Build configuration** (configure.ac, Makefile.am) - Build options and dependencies
3. **Committed documentation** (this wiki) - Maintainer-approved information
4. **Project-level docs** (README, NEWS, AUTHORS) - User-facing documentation
5. **External references** (pool protocols, hardware specs) - Third-party specifications
6. **Volatile memory** (`.kilocode/rules/memory-bank/`) - Session context only (verify before use)

## Contributing to the Wiki

When updating wiki pages:

1. **Verify first**: Check source code to ensure accuracy
2. **Cite sources**: Link to specific files, functions, or evidence
3. **Be concise**: Focus on actionable information maintainers need
4. **Update index.md**: Add new pages to the catalog
5. **Log changes**: Update [Log](log.md) with a brief entry
6. **Validate**: Ensure code references are accurate and links work
7. **Commit**: Changes should be committed alongside code changes

## Related Resources

- **AGENTS.md**: Complete maintainer/agent operating manual (root directory)
- **CLAUDE.md**: Quick reference for Claude Code (defers to AGENTS.md)
- **API-README**: RPC API documentation
- **FPGA-README**: Extended details about FPGA mining
- **SCRYPT-README**: Scrypt mining tuning guide
- **README**: User-facing project documentation
- **NEWS**: Version history and changes

## Getting Help

- Review existing wiki pages thoroughly
- Check source code comments and documentation
- Consult GitHub Issues for known problems
- Refer to external documentation (OpenCL specs, hardware manuals)

---

**Remember**: The wiki is canonical. When in doubt, read the source code.