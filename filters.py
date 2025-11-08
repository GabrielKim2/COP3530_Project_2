"""Relevance filter for incidents."""
from __future__ import annotations

def is_relevant_incident(row: dict, fields: list[str], exclude: list[str]) -> bool:
    text = None
    for f in fields:
        v = row.get(f)
        if isinstance(v, str):
            text = v
            break
    if not text:
        return True
    u = text.upper()
    for kw in exclude:
        if kw.strip() and kw.strip().upper() in u:
            return False
    return True
