from fastapi import FastAPI, HTTPException, status
from pydantic import BaseModel
from dotenv import load_dotenv
from supabase import create_client, Client
import os

# 1. Load the .env file parameters explicitly into system memory at boot
load_dotenv()

# 2. Extract the variables safely using the native os module
SUPABASE_URL = os.getenv("SUPABASE_URL")
SUPABASE_KEY = os.getenv("SUPABASE_KEY")

# Safety Check: Stop the server immediately if configurations are missing
if not SUPABASE_URL or not SUPABASE_KEY:
    raise RuntimeError("Missing configuration parameters! Please check your .env environment variables.")

app = FastAPI(title="DS18B20 IoT Gateway")

supabase: Client = create_client(SUPABASE_URL, SUPABASE_KEY)

class TemperaturePayload(BaseModel):
    device_id: str
    temp_c: float
    temp_f: float

@app.get("/")
def index():
    return {
        "Message": "Welcome to ESP8266 Data Logger API",
        "Endpoints": {
            "/api/temperature": "POST Temperature Data from DS18B20",
            "/docs": "View & Test the API"
        },
        "created_by": "ANURAG PANDA"
    }

@app.post("/api/temperature", status_code=status.HTTP_201_CREATED)
async def log_temperature(data: TemperaturePayload):
    try:
        response = supabase.table("temperature_logs").insert({
            "device_id": data.device_id,
            "temp_c": data.temp_c,
            "temp_f": data.temp_f
        }).execute()
        return {"status": "success", "data": response.data}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
