"""Fails if a relative link or image in the docs is broken, or a case study is missing from the index."""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
SKIP_DIRS = {".git", "templates"}  # templates contain placeholder paths on purpose
LINK = re.compile(r'\]\(([^)\s]+)\)|src="([^"]+)"')
errors = []

for md in ROOT.rglob("*.md"):
    if SKIP_DIRS.intersection(md.relative_to(ROOT).parts):
        continue
    text = md.read_text(encoding="utf-8")
    for m in LINK.finditer(text):
        target = (m.group(1) or m.group(2)).split("#", 1)[0]
        if not target or re.match(r"^[a-z][a-z0-9+.-]*:", target, re.I):
            continue  # pure anchor, or external URL / mailto
        if not (md.parent / target).resolve().exists():
            line = text.count("\n", 0, m.start()) + 1
            errors.append(f"{md.relative_to(ROOT)}:{line}: broken link -> {target}")

index = (ROOT / "README.md").read_text(encoding="utf-8")
for case in sorted(p for p in (ROOT / "case-studies").iterdir() if p.is_dir()):
    if f"./case-studies/{case.name}/" not in index:
        errors.append(f"README.md: case study missing from index -> {case.name}")

print("\n".join(errors) if errors else "All relative links resolve and every case study is indexed.")
sys.exit(1 if errors else 0)
