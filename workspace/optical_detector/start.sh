#!/bin/bash

# Проверяем, введено ли число
if [ -z "$1" ]
then
      echo "Ошибка: Укажите количество фотонов."
      echo "Пример использования: ./start.sh 1000"
      exit 1
fi

PHOTONS=$1
BASE_DIR="/workspace/optical_detector"

echo "==========================================="
echo "   ЗАПУСК СИМУЛЯЦИИ: $PHOTONS ФОТОНОВ"
echo "==========================================="

# Создаем временный макрос (используем абсолютный путь)
cat <<EOF > ${BASE_DIR}/tmp_run.mac
/control/verbose 0
/run/verbose 0
/run/initialize
/run/beamOn $PHOTONS
EOF

# Запускаем Geant4 (используем абсолютные пути)
source /usr/local/geant4/bin/geant4.sh
cd ${BASE_DIR}/build
./optical_detector ${BASE_DIR}/tmp_run.mac

echo "-------------------------------------------"
echo " Симуляция завершена."
echo "==========================================="
