## Objective

Maintain a Scout Lite firmware based on the official ESP32 Marauder C5 target with minimal long-term divergence from upstream.

## Mandatory Rules

1. Prefer existing upstream implementations.
2. Verify missing functionality before porting.
3. Do not rewrite Marauder core unnecessarily.
4. Preserve standard Marauder CLI behavior.
5. Preserve SigRoam FAP compatibility.
6. Treat hardware assumptions as unverified until confirmed by schematics or tests.
7. Keep patches small and independently testable.
8. Submit reusable fixes upstream when suitable.
9. Never auto-merge unverified upstream changes.
10. Never publish production firmware without explicit approval and hardware validation.

## Workflow

For every change:

- Identify relevant upstream source and commit.
- Explain the root cause or feature gap.
- Propose the smallest maintainable change.
- Build the C5 target and run available tests.
- Record test evidence and remaining risks.
- Update the compatibility matrix.
- Open a PR for review.

## Priority

Upstream compatibility > reliability > maintainability > feature count.
