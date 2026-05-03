# Geant4 with noVNC inside Docker

This repository contains the configuration files to deploy a Geant4 physical engine simulation environment accessible directly in the browser via noVNC (no local installation required).

## Architecture

- **Base Image**: Ubuntu 24.04 LTS
- **Physics Engine**: Geant4 11.3.0 (with complete installed datasets)
- **Framework Support**: Qt6, OpenGL, and X11
- **Desktop Environment**: Fluxbox (lightweight window manager)
- **Display Server**: TigerVNC server + WebSockify (noVNC)
- **Build Threads**: Configured for 3 threads (`make -j3`) to avoid Out-of-Memory (OOM) crashes on 11GB RAM machines.

## Usage

1. Clone the repository and navigate into the folder:
   ```bash
   git clone https://github.com/Faserdka/geant4-novnc-docker.git
   cd geant4-novnc-docker
   ```

2. Start the container with Docker Compose:
   ```bash
   docker compose up --build -d
   ```

3. Open your browser and navigate to:
   ```
   http://localhost:8080
   ```

4. Click **Connect**, enter the default password (`geant4pass`), and enjoy using Geant4!
