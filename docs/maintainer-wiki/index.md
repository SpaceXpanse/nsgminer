# Maintainer Wiki Index

This wiki contains durable, committed documentation for NSGminer maintainers and contributors.

## Pages

- [README](README.md) — Overview and getting started
- [Decision log](decisions.md) — Dated maintainer decisions
- [Open work](open-work.md) — Unresolved items and known gaps
- [Tech stack](tech-stack.md) — Technology inventory with evidence
- [Workflows](workflows.md) — Standard procedures (build, test, release)
- [Concept: Architecture overview](concept-architecture-overview.md) — High-level system design
- [Agent guide](agent-guide.md) — Agent operating notes (defers to AGENTS.md)
- [Log](log.md) — Chronological record of wiki updates

## Canonical Source-of-Truth

Always consult the wiki before making changes. For protocol/implementation details, verify against:
1. Source code (ultimate truth)
2. Build configuration (configure.ac, Makefile.am)
3. Committed documentation (this wiki)
4. Project README, NEWS, AUTHORS
5. External references (pool protocols, hardware specs)

## Getting Started

If you're new to NSGminer development:
1. Read this index and the [README](README.md)
2. Review [Tech stack](tech-stack.md) to understand dependencies
3. Follow [Workflows](workflows.md) for build/test procedures
4. Check [Open work](open-work.md) for current issues
5. Consult [Agent guide](agent-guide.md) for AI-assisted workflows

## Maintenance

When updating the wiki:
- Add new pages to this index
- Index existing pages if they're missing
- Keep the structure flat unless categories become necessary
- Update the [Log](log.md) with significant changes
- Validate changes against source code before committing