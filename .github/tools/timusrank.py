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

author_id = 404938
profile_url = f"https://acm.timus.ru/author.aspx?id={author_id}"

fetch_urls = [
    f"https://acm.timus.ru/author.aspx?id={author_id}&locale=en",
    f"https://timus.online/author.aspx?id={author_id}&locale=en",
]

ua = "crystarm-algs-readme-bot/1.0 (+https://github.com/crystarm/algs)"


def fetch_html(url: str) -> str:
    req = Request(
        url,
        headers={
            "User-Agent": ua,
            "Accept": "text/html,*/*;q=0.9",
            "Accept-Language": "en",
        },
    )
    with urlopen(req, timeout=25) as r:
        b = r.read()
    return b.decode("utf-8", errors="replace")


def html_to_text(s: str) -> str:
    s = html_module.unescape(s)
    s = re.sub(r"(?is)<script.*?</script>", " ", s)
    s = re.sub(r"(?is)<style.*?</style>", " ", s)
    s = re.sub(r"(?s)<[^>]+>", " ", s)
    s = re.sub(r"\s+", " ", s).strip()
    return s


def parse_rank_by_rating(text: str) -> tuple[int, int] | None:
    # allow optional punctuation after label
    m = re.search(
        r"Rank by rating\s*[:,-]?\s*(\d+)\s+out of\s+(\d+)",
        text,
        flags=re.I,
    )
    if m:
        return int(m.group(1)), int(m.group(2))

    # fallback for ru locale (just in case)
    m = re.search(
        r"Место\s+по\s+рейтингу\s*(\d+)\s*из\s*(\d+)",
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
        print(f"timusrank: failed to fetch/parse; last_err={last_err}", file=sys.stderr)
        return 1

    rank, total = parsed
    print(f"timusrank: parsed rank_by_rating={rank}/{total}")

    stats = load_json(stats_path)
    timus = stats.get("timus")
    if not isinstance(timus, dict):
        timus = {}

    timus["author_id"] = author_id
    timus["profile_url"] = profile_url
    timus["rank_by_rating"] = rank
    timus["rank_by_rating_total"] = total
    timus["rank_by_rating_text"] = f"{rank} of {total}"

    stats["timus"] = timus
    write_json(stats_path, stats)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
