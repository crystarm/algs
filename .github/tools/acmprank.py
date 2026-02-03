#!/usr/bin/env python3
from __future__ import annotations

import html as html_module
import json
import re
import sys
from pathlib import Path
from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen

root = Path(__file__).resolve().parents[2]
stats_path = root / ".github" / "meta" / "stats.lson"

author_id = 541249
profile_url = f"https://acmp.ru/index.asp?main=user&id={author_id}"

fetch_urls = [
    f"https://acmp.ru/index.asp?main=user&id={author_id}",
    f"https://acmp.ru/index.asp?id={author_id}&main=user",
]

ua = "crystarm-algs-readme-bot/1.0 (+https://github.com/crystarm/algs)"


def fetch_html(url: str) -> str:
    req = Request(
        url,
        headers={
            "User-Agent": ua,
            "Accept": "text/html,*/*;q=0.9",
            "Accept-Language": "ru,en",
        },
    )
    with urlopen(req, timeout=25) as r:
        b = r.read()

    head = b[:4096].lower()
    enc = None
    m = re.search(rb"charset\s*=\s*([a-z0-9_\-]+)", head)
    if m:
        try:
            enc = m.group(1).decode("ascii", errors="ignore").strip()
        except Exception:
            enc = None

    if enc:
        try:
            return b.decode(enc, errors="replace")
        except Exception:
            pass

    s_utf8 = b.decode("utf-8", errors="replace")
    if ("Место" in s_utf8) or ("Place" in s_utf8) or ("Rank" in s_utf8):
        return s_utf8

    s_1251 = b.decode("cp1251", errors="replace")
    if ("Место" in s_1251) or ("Place" in s_1251) or ("Rank" in s_1251):
        return s_1251

    return s_utf8


def html_to_text(s: str) -> str:
    s = html_module.unescape(s)
    s = re.sub(r"(?is)<script.*?</script>", " ", s)
    s = re.sub(r"(?is)<style.*?</style>", " ", s)
    s = re.sub(r"(?s)<[^>]+>", " ", s)
    s = re.sub(r"\s+", " ", s).strip()
    return s


def parse_rank_by_rating(text: str) -> tuple[int, int] | None:
    m = re.search(
        r"(?:Место|Place|Rank)\s*[:,-]?\s*(\d+)\s*/\s*(\d+)",
        text,
        flags=re.I,
    )
    if m:
        return int(m.group(1)), int(m.group(2))

    m = re.search(
        r"(?:Место|Place|Rank)\s*[:,-]?\s*(\d+)\s+(?:из|out\s+of)\s+(\d+)",
        text,
        flags=re.I,
    )
    if m:
        return int(m.group(1)), int(m.group(2))

    return None


def load_json(p: Path) -> dict:
    if not p.exists():
        return {}
    return json.loads(p.read_text(encoding="utf-8"))


def write_json(p: Path, obj: dict) -> None:
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(json.dumps(obj, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


def main() -> int:
    last_err = None
    parsed = None

    for url in fetch_urls:
        try:
            raw = fetch_html(url)
            text = html_to_text(raw)
            parsed = parse_rank_by_rating(text)
            if parsed:
                break
        except (HTTPError, URLError, TimeoutError) as e:
            last_err = e
            continue
        except Exception as e:
            last_err = e
            continue

    if not parsed:
        print(f"acmprank: failed to fetch/parse; last_err={last_err}", file=sys.stderr)
        return 1

    rank, total = parsed
    if total <= 0 or rank <= 0:
        print(
            f"acmprank: invalid numbers rank_by_rating={rank}/{total}", file=sys.stderr
        )
        return 1

    pct = rank * 100.0 / total
    rank_text = f"{rank} of {total} (top {pct:.1f}%)"
    print(f"acmprank: parsed rank_by_rating_text={rank_text}")

    stats = load_json(stats_path)
    acmp = stats.get("acmp")
    if not isinstance(acmp, dict):
        acmp = {}

    acmp["author_id"] = author_id
    acmp["profile_url"] = profile_url
    acmp["rank_by_rating"] = rank
    acmp["rank_by_rating_total"] = total
    acmp["rank_by_rating_pct"] = round(pct, 1)
    acmp["rank_by_rating_text"] = rank_text

    stats["acmp"] = acmp
    write_json(stats_path, stats)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
