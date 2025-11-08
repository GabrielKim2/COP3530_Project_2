"""HTTP client for Socrata with paging."""
from __future__ import annotations
import time, typing as t, requests
from .config import APP_TOKEN

class SocrataClient:
    def __init__(self, app_token: str | None = None, timeout: int = 30, backoff_base: float = 1.5):
        self.session = requests.Session()
        self.timeout = timeout
        self.backoff_base = backoff_base
        token = (app_token or APP_TOKEN or "").strip()
        if token:
            self.session.headers.update({"X-App-Token": token})

    def get_page(self, endpoint: str, params: dict[str, t.Any], retries: int = 3) -> list[dict]:
        attempt = 0
        while True:
            try:
                resp = self.session.get(endpoint, params=params, timeout=self.timeout)
            except requests.RequestException:
                if attempt >= retries:
                    raise
                attempt += 1
                time.sleep(self.backoff_base ** attempt)
                continue
            if resp.status_code == 200:
                try:
                    data = resp.json()
                    return data if isinstance(data, list) else []
                except ValueError:
                    if attempt >= retries:
                        raise
            elif resp.status_code in (429,500,502,503,504):
                if attempt >= retries:
                    resp.raise_for_status()
                attempt += 1
                time.sleep(self.backoff_base ** attempt)
            else:
                resp.raise_for_status()

    def get_all(self, endpoint: str, soql: dict[str, t.Any], chunk: int = 50000, max_rows: int | None = None):
        offset = 0
        fetched = 0
        while True:
            params = dict(soql)
            params["$limit"] = chunk
            params["$offset"] = offset
            page = self.get_page(endpoint, params)
            if not page:
                break
            for row in page:
                yield row
                fetched += 1
                if max_rows is not None and fetched >= max_rows:
                    return
            offset += len(page)
            if len(page) < chunk:
                break
