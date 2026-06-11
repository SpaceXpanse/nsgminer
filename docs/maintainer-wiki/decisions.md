# Decision Log

Records of architectural and technical decisions made during NSGminer development and maintenance.

## Format

Each decision entry should include:
- **Date**: When the decision was made (YYYY-MM-DD)
- **Title**: Brief descriptive title
- **Context**: Problem or requirement being addressed
- **Options Considered**: Alternatives that were evaluated
- **Decision**: What was chosen and why
- **Consequences**: Outcomes, trade-offs, or follow-up actions
- **Evidence**: Links to source code, discussions, or external references

## Entries

### NSGminer Maintainer Wiki Alignment and Evidence Correction
- **Date**: 2026-06-11
- **Title**: NSGminer maintainer wiki alignment and evidence correction
- **Context**: A routine review of `docs/maintainer-wiki/` was performed against the actual source code and build configuration to ensure accuracy. Several evidence line references in `tech-stack.md` were found to be incorrect.
- **Options Considered**:
  1. Leave documentation as-is with known inaccuracies
  2. Update documentation to match actual source code and build configuration
  3. Regenerate entire wiki from source analysis
- **Decision**: Option 2. Updated `tech-stack.md` evidence line reference from `configure.ac:120-350` to `configure.ac:145-510` to correctly match the feature detection section verified against actual `configure.ac` content.
- **Consequences**: Maintainer wiki remains accurate and trustworthy. Future updates should verify evidence references against actual code before committing.
- **Evidence**: Verified against actual `configure.ac` in repository root; feature detection section spans lines 145-510.

### Header Validation and Enhanced Logging for Share Submission

- **Date**: 2025-06-10
- **Title**: Header Validation and Enhanced Logging for Share Submission
- **Context**: Miners experienced share rejections due to invalid headers and endianness mismatches. Debugging was hampered by insufficient logging information, making it difficult to identify the root cause of invalid shares.
- **Options Considered**:
  1. Add comprehensive header validation with detailed error messages
  2. Implement logging that includes header byte patterns for both accepted and rejected shares
  3. Add debug mode that dumps raw header data for forensic analysis
- **Decision**: Implemented all three approaches: enhanced validation checks in [`work_decode()`](miner.c) and [`gen_stratum_work()`](miner.c) to catch malformed headers early, added explicit error logging that distinguishes between header length issues, endianness mismatches, and padding errors, and created a `--debug-headers` mode that outputs raw header bytes for detailed diagnostics.
- **Consequences**: Reduced invalid share rate by enabling miners to detect configuration errors. Maintainers and users can now diagnose header format issues faster through targeted log messages. Slight performance overhead when `--debug-headers` is enabled, but minimal impact in normal operation.
- **Evidence**: [`miner.c`](miner.c) – changes in `work_decode()`, `gen_stratum_work()`, and `submit_share()`; new validation logic and logging statements.

## Adding a Decision

When documenting a new decision:

When documenting a new decision:

1. Add entry to this file in chronological order (newest at top)
2. Follow the format above
3. Link to relevant source files or commit hashes
4. Keep it concise but complete enough to understand the rationale
5. Update any affected wiki pages (architecture, workflows, etc.)

## Related

- See [Open work](open-work.md) for unresolved questions
- See [Concept: Architecture overview](concept-architecture-overview.md) for current design
- Consult source code for implementation details