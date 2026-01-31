#!/usr/bin/env python3
from __future__ import annotations

import json
import re
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

BEGIN = "<!-- TASKS_TABLE_BEGIN -->"
END = "<!-- TASKS_TABLE_END -->"

LANG = {
    ".cpp": "C++",
    ".cc": "C++",
    ".c": "C",
    ".py": "Python",
    ".cs": "C#",
    ".php": "PHP",
    ".sql": "SQL",
}

SOURCES = [
    ("acmp", "ACMP"),
    ("coderun", "CodeRun"),
    ("timus", "Timus"),
]


@dataclass(frozen=True)
class row:
    src: str
    sid: str
    lang: str
    sol: str
    stmt: str


def is_task_file(p: Path) -> bool:
    if not p.is_file():
        return False
    if p.name.startswith("."):
        return False
    return p.suffix.lower() in LANG


def mk_stmt(src_dir: str, sid: str) -> str:
    if src_dir == "acmp":
        if re.fullmatch(r"\d+", sid):
            return f"https://acmp.ru/index.asp?main=task&id_task={int(sid)}"
        return ""

    if src_dir == "timus":
        if re.fullmatch(r"\d+", sid):
            return f"https://acm.timus.ru/problem.aspx?space=1&num={int(sid)}"
        return ""

    if src_dir == "coderun":
        if re.fullmatch(r"\d+", sid):
            return f"https://coderun.yandex.ru/catalog?search={int(sid)}"
        return f"https://coderun.yandex.ru/problem/{sid}"

    return ""


def read_text(p: Path) -> str:
    return p.read_text(encoding="utf-8")


def write_text(p: Path, s: str) -> None:
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(s, encoding="utf-8")


def gen_table(rows: list[row]) -> str:
    out = []
    out.append("| Source | ID | Lang | Solution | Statement |")
    out.append("|---|---:|---|---|---|")
    for r in rows:
        sol_md = f"[{r.sol}]({r.sol})"
        stmt_md = f"[link]({r.stmt})" if r.stmt else ""
        out.append(f"| {r.src} | {r.sid} | {r.lang} | {sol_md} | {stmt_md} |")
    return "\n".join(out) + "\n"


def sort_key(r: row):
    if re.fullmatch(r"\d+", r.sid):
        return (r.src, 0, int(r.sid))
    return (r.src, 1, r.sid)


def main() -> int:
    rows: list[row] = []

    for src_dir, src_name in SOURCES:
        d = ROOT / src_dir
        if not d.is_dir():
            continue
        for p in sorted(d.rglob("*")):
            if not is_task_file(p):
                continue
            sid = p.stem
            lang = LANG[p.suffix.lower()]
            sol = str(p.relative_to(ROOT)).replace("\\", "/")
            stmt = mk_stmt(src_dir, sid)
            rows.append(row(src_name, sid, lang, sol, stmt))

    rows.sort(key=sort_key)

    stats = {
        "tasks_total": len(rows),
        "by_source": {},
    }
    for r in rows:
        stats["by_source"][r.src] = stats["by_source"].get(r.src, 0) + 1

    stats_path = ROOT / ".github" / "meta" / "stats.lson"
    write_text(stats_path, json.dumps(stats, ensure_ascii=False, indent=2) + "\n")

    readme_path = ROOT / "README.md"
    text = read_text(readme_path)

    if BEGIN not in text or END not in text:
        raise SystemExit(f"README.md: markers not found: {BEGIN} / {END}")

    table_md = gen_table(rows)
    new_block = f"{BEGIN}\n<!-- generated: do not edit by hand -->\n\n{table_md}\n{END}"

    pattern = re.escape(BEGIN) + r".*?" + re.escape(END)
    text2 = re.sub(pattern, new_block, text, flags=re.S)
    write_text(readme_path, text2)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
