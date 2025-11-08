"""Config: reads DATAGNV_APP_TOKEN and defines defaults."""
import os
try:
    from dotenv import load_dotenv  # type: ignore
    load_dotenv()
except Exception:
    pass

APP_TOKEN = os.getenv("IEj4K25gFNx45KbKKeRSKoJDE", "").strip()
DEFAULT_ENDPOINT = "https://data.cityofgainesville.org/resource/gvua-xt9q.json"
DEFAULT_EXCLUDE = ["PARKING","TRAFFIC STOP","CIVIL","ALARM","ASSIST","LOST PROPERTY","FOUND PROPERTY"]
DEFAULT_CHUNK = 50000
REPORT_DATE_FIELD = "report_date"
INCIDENT_TYPE_CANDIDATES = ["incident_type","type","incident","incidenttype","offense"]
LOCATION_FIELD_CANDIDATES = ["location","mapped_location","geolocation","point"]
LAT_CANDIDATES = ["latitude","lat"]
LON_CANDIDATES = ["longitude","lon","lng","long"]
