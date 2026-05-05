# 🌌 Geant4 Optical Detector Simulation with Auto-Analysis

A professional, Dockerized Geant4 environment for high-fidelity optical detector simulations. This project features a real-time analysis pipeline, automatic data merging, and a user-friendly configuration system.

---

## 🚀 Key Features

*   **Geant4 11.3.0 Engine**: Full-scale physics engine with pre-installed datasets.
*   **Real-time Visualization**: Access the simulation GUI via any web browser (noVNC).
*   **Automatic Analysis**: Background Python service that updates graphs and tables immediately after each run.
*   **One-Click Launcher**: Trigger simulations with a simple command or by saving a config file.
*   **Multithreading Support**: Automatic merging of data from multiple CPU threads into a single dataset.

---

## 🛠️ Project Structure

```text
workspace/optical_detector/
├── src/                # C++ Source code (Detector, Physics, Action)
├── include/            # C++ Header files
├── scripts/            # Python automation & analysis scripts (auto-launcher, analysis)
├── config.txt          # Main control panel (Photon count)
├── start.sh            # Universal simulation launcher
├── build/              # Output directory (Graphs, CSVs, Binary)
└── vis.mac             # Visualization settings
```

---

## ⚙️ Prerequisites

*   **Docker**
*   **Docker Compose**
*   **Browser** (for noVNC visualization)

---

## 💻 Quick Start

### 1. Build and Launch the Environment

Start the environment using Docker Compose. This will build the Geant4 image, start the VNC server, and mount the `workspace` directory.

```bash
docker-compose up -d --build
```

Wait a few minutes for the initial build to complete. Once finished, open the web interface at:
**`http://localhost:8080`**

### 2. Configure Your Run

Open **`workspace/optical_detector/config.txt`** and set the number of photons:
```text
1000
```

### 3. Run Simulation

You have two options:
*   **Auto-Run on Save**: The system has a background watcher script (`scripts/auto_launcher.py`). Just save the `config.txt` file, and it will detect the change and automatically start the simulation.
*   **Manual Trigger**: Run the following command inside the terminal of the Docker container:
    ```bash
    cd /workspace/optical_detector
    ./start.sh 5000
    ```

*(Note: The `auto_launcher.py` and `analyze_hits.py` should be running to use the auto-run and auto-analysis features. Check the container's processes if it's not working.)*

### 4. View Results

Results are generated automatically in the `build/` (or current working) folder:
*   📊 **`results.png`**: Time-of-flight distribution graph.
*   📄 **`final_combined_data.csv`**: A clean, sorted table of all detected hits.
*   📝 **`analysis.log`**: Live statistics (efficiency, counts, etc.).

---

## 💡 Ideas for Improvement

1. **Service Integration:** Currently, Python scripts (`auto_launcher.py`, `analyze_hits.py`) appear to need manual execution or integration. You could add them to the `entrypoint.sh` or use `supervisor` or `systemd` inside the Docker image to automatically launch them in the background upon container startup.
2. **Environment Variables:** Allow dynamic port mapping and VNC password configuration via a `.env` file instead of hardcoding them in `docker-compose.yml`.
3. **Data Volume Management:** Currently, output data is stored directly in the repository via the bind mount. Consider setting up a dedicated `/data` volume or `.gitignore` rules for the `build/` directory and CSV outputs to avoid accidentally committing large datasets to Git.
4. **Error Handling:** Add more robust error handling in `start.sh` and Python scripts, e.g., verifying that the Geant4 setup script `geant4.sh` is successfully sourced.
5. **JupyterLab Integration:** For advanced data analysis, integrating a JupyterLab server alongside noVNC could allow researchers to run custom Python notebooks directly on the simulation output.

---

Developed for advanced agentic coding and high-energy physics research.