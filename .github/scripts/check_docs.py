"""Fails if a relative link or image in the docs is broken, or a case study or project is missing from the index."""
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
for group in ("case-studies", "technical-projects"):
    folder = ROOT / group
    if not folder.is_dir():
        continue
    for item in sorted(p for p in folder.iterdir() if p.is_dir()):
        if f"./{group}/{item.name}/" not in index:
            errors.append(f"README.md: missing from index -> {group}/{item.name}")

print("\n".join(errors) if errors else "All relative links resolve and every case study and project is indexed.")
sys.exit(1 if errors else 0)
