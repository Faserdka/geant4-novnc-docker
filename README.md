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
optical_detector/
├── src/                # C++ Source code (Detector, Physics, Action)
├── include/            # C++ Header files
├── scripts/            # Python automation & analysis scripts
├── config.txt          # Main control panel (Photon count)
├── start.sh            # Universal simulation launcher
├── build/              # Output directory (Graphs, CSVs, Binary)
└── vis.mac             # Visualization settings
```

---

## 💻 Quick Start

### 1. Launch the Environment
Follow the repository deployment instructions to start the Docker container. Open the web interface at `http://localhost:8080`.

### 2. Configure Your Run
Open **`workspace/optical_detector/config.txt`** and set the number of photons:
```text
1000
```

### 3. Run Simulation
You have two options:
*   **Auto-Run on Save**: Just save the `config.txt` file. The background watcher will detect the change and start the simulation.
*   **Manual Trigger**: Run the following command in the terminal:
    ```bash
    ./start.sh 5000
    ```

### 4. View Results
Results are generated automatically in the `build/` folder:
*   📊 **`results.png`**: Time-of-flight distribution graph.
*   📄 **`final_combined_data.csv`**: A clean, sorted table of all detected hits.
*   📝 **`analysis.log`**: Live statistics (efficiency, counts, etc.).

---

## ⚙️ Requirements

*   **Docker** & **Docker Compose**
*   **Browser** (for noVNC visualization)

Developed for advanced agentic coding and high-energy physics research.
