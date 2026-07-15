from fastapi import FastAPI, HTTPException, status
from pydantic import BaseModel
from supabase import create_client, Client
import os

app = FastAPI(title="DS18B20 IoT Gateway")

# Replace with your actual project keys from Supabase Settings -> API
SUPABASE_URL = "https://jsexnrtdgvdjrkmnpnvo.supabase.co"
SUPABASE_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImpzZXhucnRkZ3ZkanJrbW5wbnZvIiwicm9sZSI6ImFub24iLCJpYXQiOjE3ODQwODUxODMsImV4cCI6MjA5OTY2MTE4M30.Rgh4seOomE5vVdtN00itvrgJ-nK_OP2dP8SEm83a-2E"

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
