import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

def main():
    csv_files = glob.glob('hits*.csv')
    if not csv_files:
        print("No CSV files found.")
        return
        
    dfs = []
    for f in csv_files:
        try:
            dfs.append(pd.read_csv(f, comment='#', names=['Energy', 'GlobalTime']))
        except Exception as e:
            print(f"Error reading CSV {f}: {e}")
    if not dfs:
        return
    df = pd.concat(dfs, ignore_index=True)

    # run.mac uses 100 photons
    total_photons = 100
    detected_photons = len(df)
    collection_efficiency = (detected_photons / total_photons) * 100

    print(f"Total simulated photons: {total_photons}")
    print(f"Detected photons: {detected_photons}")
    print(f"Collection Efficiency: {collection_efficiency:.2f}%")

    plt.figure(figsize=(8, 5))
    plt.hist(df['GlobalTime'], bins=20, color='cyan', edgecolor='black')
    plt.title(f'Time of Flight (Collection Efficiency: {collection_efficiency:.2f}%)')
    plt.xlabel('Global Time (ns)')
    plt.ylabel('Count')
    plt.grid(True, alpha=0.5)
    
    plt.tight_layout()
    plt.savefig('results.png', dpi=300)
    print("Saved results.png")

if __name__ == "__main__":
    main()
