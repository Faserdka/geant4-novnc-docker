import pandas as pd
import matplotlib.pyplot as plt
import glob
import os
import time
from tabulate import tabulate

def analyze():
    # 1. Получаем количество фотонов
    total_photons = 100
    if os.path.exists('run_info.txt'):
        try:
            with open('run_info.txt', 'r') as f:
                total_photons = int(f.read().strip())
        except:
            pass

    # 2. Собираем данные
    csv_files = glob.glob('hits*.csv')
    if not csv_files:
        return False
        
    dfs = []
    for f in csv_files:
        try:
            data = pd.read_csv(f, comment='#', names=['Energy_eV', 'Time_ns'])
            if not data.empty:
                dfs.append(data)
        except Exception as e:
            print(f"Error reading {f}: {e}")
            
    if not dfs:
        return False
        
    # 3. Форматирование таблицы
    df = pd.concat(dfs, ignore_index=True)
    df = df.sort_values(by='Time_ns').reset_index(drop=True)
    
    # Округляем значения
    df['Energy_eV'] = df['Energy_eV'].round(6)
    df['Time_ns'] = df['Time_ns'].round(3)
    
    # Подготавливаем финальную таблицу для сохранения
    df_final = df.rename(columns={
        'Energy_eV': 'Energy [eV]',
        'Time_ns': 'Arrival Time [ns]'
    })
    
    # Сохраняем в CSV (Hit ID будет индексом)
    df_final.to_csv('final_combined_data.csv', index_label='Hit ID')
    
    # 4. Обновляем график
    detected_photons = len(df)
    if total_photons == 0: total_photons = 1
    efficiency = (detected_photons / total_photons) * 100

    plt.figure(figsize=(10, 6))
    plt.hist(df['Time_ns'], bins=30, color='royalblue', edgecolor='black', alpha=0.7)
    plt.title(f'Detector Hits Analysis\nSimulated: {total_photons} | Detected: {detected_photons} | Efficiency: {efficiency:.2f}%')
    plt.xlabel('Arrival Time (ns)')
    plt.ylabel('Number of Photons')
    plt.grid(True, linestyle=':', alpha=0.6)
    
    plt.tight_layout()
    plt.savefig('results.png', dpi=300)
    plt.close()
    
    # Красивый вывод в лог
    print("\n" + "="*40)
    print(f" UPDATE: {time.ctime()}")
    print("-"*40)
    print(f" Total Simulated: {total_photons}")
    print(f" Total Detected:  {detected_photons}")
    print(f" Efficiency:      {efficiency:.2f}%")
    print("-"*40)
    print(" Preview of Combined Data:")
    print(tabulate(df_final.head(10), headers='keys', tablefmt='simple'))
    print("="*40 + "\n")
    
    return True

def main():
    print("Starting polished automatic analysis...")
    last_mtime = 0
    while True:
        current_mtime = 0
        files = glob.glob('hits*.csv') + glob.glob('run_info.txt')
        if files:
            current_mtime = max(os.path.getmtime(f) for f in files)
        
        if current_mtime > last_mtime:
            if analyze():
                last_mtime = current_mtime
        
        time.sleep(2)

if __name__ == "__main__":
    main()
