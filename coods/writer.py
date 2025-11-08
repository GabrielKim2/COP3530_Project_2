"""Write lat,lon to text file."""
from __future__ import annotations
import pathlib

def write_latlon_txt(coords, out_path):
    out_path = pathlib.Path(out_path)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", encoding="utf-8") as f:
        for lat, lon in coords:
            f.write(f"{lat},{lon}\n")
