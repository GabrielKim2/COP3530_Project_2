"""Export lat,lon from dataGNV Crime Responses."""
from __future__ import annotations

if __package__ in (None, ""):
    import os, sys
    sys.path.append(os.path.dirname(os.path.dirname(__file__)))
    from datagnv.config import DEFAULT_ENDPOINT, DEFAULT_EXCLUDE, DEFAULT_CHUNK
    from datagnv.pipeline import run_pipeline
    from datagnv.writer import write_latlon_txt
else:
    from .config import DEFAULT_ENDPOINT, DEFAULT_EXCLUDE, DEFAULT_CHUNK
    from .pipeline import run_pipeline
    from .writer import write_latlon_txt

import argparse

def parse_args():
    p = argparse.ArgumentParser(description="Export lat,lon from dataGNV Crime Responses")
    p.add_argument("--endpoint", default=DEFAULT_ENDPOINT)
    p.add_argument("--since", default=None)
    p.add_argument("--until", default=None)
    p.add_argument("--exclude", default=",".join(DEFAULT_EXCLUDE))
    p.add_argument("--out", default="coords.txt")
    p.add_argument("--chunk", type=int, default=DEFAULT_CHUNK)
    p.add_argument("--max-rows", type=int, default=None)
    p.add_argument("--verbose", action="store_true")
    return p.parse_args()

def main():
    args = parse_args()
    exclude_keywords = [w.strip() for w in (args.exclude or "").split(",") if w.strip()]
    coords = run_pipeline(endpoint=args.endpoint, exclude_keywords=exclude_keywords, since=args.since, until=args.until, chunk=args.chunk, max_rows=args.max_rows, verbose=args.verbose)
    write_latlon_txt(coords, args.out)
    if args.verbose:
        print(f"Wrote {len(coords)} coordinates to {args.out}")

if __name__ == "__main__":
    main()
