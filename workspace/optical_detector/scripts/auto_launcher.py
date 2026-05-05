import os
import time
import subprocess

CONFIG_FILE = "/workspace/optical_detector/config.txt"
LAUNCHER_SCRIPT = "/workspace/optical_detector/start.sh"

def get_config_value():
    try:
        with open(CONFIG_FILE, 'r') as f:
            return f.read().strip()
    except Exception as e:
        print(f"Error reading config: {e}")
        return None

def main():
    print(f"--- AUTO-LAUNCHER STARTED ---")
    print(f"Watching {CONFIG_FILE}...")
    print("Change the number in that file and SAVE it to trigger simulation.")
    
    last_mtime = os.path.getmtime(CONFIG_FILE)
    
    while True:
        try:
            current_mtime = os.path.getmtime(CONFIG_FILE)
            if current_mtime > last_mtime:
                last_mtime = current_mtime
                photons = get_config_value()
                
                if photons and photons.isdigit():
                    print(f"\n[AUTO] Detected change! Starting simulation with {photons} photons...")
                    # Запускаем наш скрипт start.sh
                    subprocess.run([LAUNCHER_SCRIPT, photons])
                    print("[AUTO] Simulation finished. Waiting for next change...")
                else:
                    print(f"[AUTO] Warning: Invalid value in config.txt: '{photons}'")
            
            time.sleep(1)
        except KeyboardInterrupt:
            break
        except Exception as e:
            print(f"Error in auto-launcher: {e}")
            time.sleep(5)

if __name__ == "__main__":
    main()
