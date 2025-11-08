"""Fetch, filter, and extract lat,lon."""
from __future__ import annotations
from datetime import datetime
from .client import SocrataClient
from .config import REPORT_DATE_FIELD, INCIDENT_TYPE_CANDIDATES, LOCATION_FIELD_CANDIDATES, LAT_CANDIDATES, LON_CANDIDATES
from .filters import is_relevant_incident

def build_soql(since: str | None, until: str | None) -> dict[str, str]:
    soql: dict[str, str] = {}
    where = []
    def _iso(s: str) -> str:
        try:
            return datetime.strptime(s, "%Y-%m-%d").strftime("%Y-%m-%dT00:00:00")
        except ValueError:
            return s
    if since:
        where.append(f"{REPORT_DATE_FIELD} >= '{_iso(since)}'")
    if until:
        where.append(f"{REPORT_DATE_FIELD} <= '{_iso(until)}'")
    if where:
        soql["$where"] = " AND ".join(where)
    return soql

def extract_lat_lon(row: dict):
    for lf in LOCATION_FIELD_CANDIDATES:
        loc = row.get(lf)
        if isinstance(loc, dict):
            lat, lon = loc.get("latitude"), loc.get("longitude")
            if lat is not None and lon is not None:
                try:
                    return (float(lat), float(lon))
                except (TypeError, ValueError):
                    pass
    lat_val = next((row.get(f) for f in LAT_CANDIDATES if f in row), None)
    lon_val = next((row.get(f) for f in LON_CANDIDATES if f in row), None)
    if lat_val is not None and lon_val is not None:
        try:
            return (float(lat_val), float(lon_val))
        except (TypeError, ValueError):
            return None
    return None

def run_pipeline(endpoint: str, exclude_keywords: list[str], since: str | None = None, until: str | None = None, chunk: int = 50000, max_rows: int | None = None, verbose: bool = False):
    client = SocrataClient()
    soql = build_soql(since, until)
    coords = []
    for row in client.get_all(endpoint, soql=soql, chunk=chunk, max_rows=max_rows):
        if not is_relevant_incident(row, INCIDENT_TYPE_CANDIDATES, exclude_keywords):
            continue
        pair = extract_lat_lon(row)
        if pair is None:
            continue
        lat, lon = pair
        if abs(lat) < 1e-9 and abs(lon) < 1e-9:
            continue
        coords.append((lat, lon))
        if verbose and len(coords) % 1000 == 0:
            print(f"[pipeline] kept {len(coords)}")
    if verbose:
        print(f"[pipeline] finished: kept={len(coords)}")
    return coords
