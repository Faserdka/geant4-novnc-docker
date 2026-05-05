import pandas as pd
import glob
import os
from tabulate import tabulate

def combine_data():
    # Поиск всех файлов от потоков
    csv_files = glob.glob('hits*.csv')
    
    if not csv_files:
        print("Ошибка: Файлы hits*.csv не найдены. Сначала запустите симуляцию.")
        return

    print(f"Найдено файлов: {len(csv_files)}")
    
    dfs = []
    for f in csv_files:
        try:
            # Читаем данные, убираем строки с комментариями (#)
            df_part = pd.read_csv(f, comment='#', names=['Energy_eV', 'Time_ns'])
            # Добавляем колонку с именем исходного файла (номером потока)
            thread_id = f.split('_t')[-1].split('.')[0]
            df_part['Thread'] = thread_id
            dfs.append(df_part)
        except Exception as e:
            print(f"Не удалось прочитать {f}: {e}")

    if not dfs:
        print("Данные в файлах отсутствуют.")
        return

    # Объединяем всё в одну таблицу
    full_df = pd.concat(dfs, ignore_index=True)
    
    # Сортируем по времени для логичности
    full_df = full_df.sort_values(by='Time_ns').reset_index(drop=True)
    
    # Добавляем ID хита
    full_df.index.name = 'Hit_ID'
    
    # Сохраняем в CSV
    output_file = 'final_combined_data.csv'
    full_df.to_csv(output_file)
    
    # Выводим красивую таблицу (первые 15 строк)
    print("\n--- ИТОГОВАЯ ТАБЛИЦА (ПРЕВЬЮ) ---")
    print(tabulate(full_df.head(15), headers='keys', tablefmt='psql'))
    
    print(f"\nВсего зарегистрировано хитов: {len(full_df)}")
    print(f"Данные успешно объединены и сохранены в: {os.getcwd()}/{output_file}")

if __name__ == "__main__":
    combine_data()
