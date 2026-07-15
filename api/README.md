## 📂 Project Structure

```Plaintext
api/
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
