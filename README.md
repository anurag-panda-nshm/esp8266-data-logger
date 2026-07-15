# ESP8266 Telemetry Gateway to Supabase

A secure, production-grade HTTPS telemetry bridge that ingests temperature data in both Celsius (°C) and Fahrenheit (°F) from an ESP8266 microchip reading a DS18B20 sensor, pipes it through a validation layer via FastAPI, and logs it into a real-time Supabase PostgreSQL database.

---

## 🛠️ System Architecture

```text
[ DS18B20 Sensor ] 
       │
       ▼ (OneWire via Pin D3/GPIO 0)
[ ESP8266 Client ]
       │
       ▼ (Secure HTTPS Post with Root CA Handshake)
[ FastAPI Cloud Backend ] 
       │
       ▼ (Validates Schema via Pydantic)
[ Supabase PostgreSQL ] ──► Live Real-time Dashboard
```

## 📂 Project Structure

```Plaintext
esp8266-logger-supabase/
├── main.py            # FastAPI Application & Pydantic Validation Models
├── pyproject.toml     # Packaging Metadata & FastAPI Cloud Deploy Blueprint
├── requirements.txt   # Standard PIP Dependency Manifest
└── README.md          # Project Documentation
```

## ⚙️ Backend Requirements & Installation

This project utilizes `pyproject.toml` for modern standard compliance. The core dependencies included are:

FastAPI [Standard] (Core API infrastructure & high-speed Uvicorn worker loops)
Supabase Python Client (Database connector)
Pydantic (Microchip payload structural integrity check)

### Local Development Setup

#### Clone or navigate to the project directory:

```Bash
cd api
```

#### Install all dependencies:

```Bash
pip install .
```

#### Alternatively, using standard requirements:

```Bash
pip install -r requirements.txt
```

#### Spin up your local development server:

```Bash
fastapi dev main.py
```

Your application will now be running locally at http://127.0.0.1:8000.

### ☁️ Deploying to FastAPI Cloud

Because your pyproject.toml contains the exact deployment metadata configuration entrypoint ([tool.fastapi]), moving this live to the cloud is simplified into a single instruction.

#### Trigger the automated build engine:

```Bash
fastapi deploy
```

#### Configure Cloud Environment Variables:
Once the build completes and gives you your live URL (e.g., https://api-logger.fastapicloud.dev), open your FastAPI Cloud UI dashboard dashboard and inject your secret database parameters:

```env
SUPABASE_URL = "https://your-project-id.supabase.co"

SUPABASE_KEY = "your-anon-or-service-role-key"
```

## 💾 Database Setup (Supabase SQL)
Run this schema initialization script inside your Supabase SQL Editor to construct the target data table and enable RLS protection matched with the FastAPI entry rules:

```SQL
-- Create telemetry storage architecture
CREATE TABLE temperature_logs (
    id BIGSERIAL PRIMARY KEY,
    created_at TIMESTAMPTZ DEFAULT NOW(),
    device_id TEXT NOT NULL,
    temp_c NUMERIC(5,2) NOT NULL,
    temp_f NUMERIC(5,2) NOT NULL
);

-- Active security layout
ALTER TABLE temperature_logs ENABLE ROW LEVEL SECURITY;

-- Turn on the Realtime broadcasting switch
ALTER PUBLICATION supabase_realtime ADD TABLE temperature_logs;

-- Allow FastAPI anonymous inserts securely via RLS
CREATE POLICY "Allow anonymous inserts via backend"
ON "public"."temperature_logs"
AS PERMISSIVE
FOR INSERT
TO anon
WITH CHECK (true);
```

## 🔌 Hardware Configuration Reference (ESP8266)
Sensor: DS18B20 (Waterproof/Transistor)

Data Wire Terminal: Connect to Pin D3 (GPIO 0).

Hardware Pull-up: A 4.7kΩ resistor must bridge the Data (D3) line to the VCC (3.3V) line to guarantee hardware line voltage stability.

Security Layer Method: Uses the Root Trust Anchor Method by embedding the Root CA string. This keeps the firmware working for up to 10–20 years without breaking when your specific domain SSL certificate renews.

## Finding Root CA Certificate for your API Domain

Once the build completes and gives you your live URL; Open PowerShell in Windows to execute these commands in your Windows PowerShell terminal to gather the validation values for your hosted FastAPI domain

### Install OpenSSL (If Not Installed)

#### Step 1: Install OpenSSL via Terminal
Right-click the Windows Start menu button and select Terminal (or PowerShell).

Type or paste the following command and press Enter:

```PowerShell
winget install Git.Git
```
Why Git? Git for Windows automatically installs a fully functional, official, up-to-date version of openssl.exe tucked inside its directories, saving you from having to download sketchy third-party .exe files from the internet.

#### Step 2: Add OpenSSL to your System Variables (Path)
To use the openssl command from anywhere in your terminal without typing the full directory pathway every time, you need to tell Windows where it lives.

Press the Windows Key, type Environment Variables, and press Enter.

Click the Environment Variables... button at the bottom right.

In the bottom window ("System variables"), scroll down, select the Path variable, and click Edit....

Click New on the right side, and paste this exact directory pathway:

```Plaintext
C:\Program Files\Git\usr\bin
```
Click OK on all three open windows to save the settings.

#### Step 3: Verify the Installation
Close your current Terminal window completely and open a fresh one to reload the environment pathways. Type the following command:

```PowerShell
openssl version
```
If it returns something like OpenSSL 3.x.x, it is completely ready to go!

### Finding The Root CA

```PowerShell
# Locate the highest numbered certificate block outputted on your screen. Copy everything from -----BEGIN CERTIFICATE----- through -----END CERTIFICATE-----.

echo $null | & "C:\Program Files\Git\usr\bin\openssl.exe" s_client -connect your_api_domain.fastapicloud.dev:443 -showcerts
```

## Contact Developer

**ANURAG PANDA**

[Portfolio](https://anuragpanda.in)

[Email](emailto:anuragpanda.dev@gmail.com)

# Build With ❤️ by Anurag Panda
