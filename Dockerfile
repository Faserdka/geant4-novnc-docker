FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Установка системных зависимостей, компиляторов, Qt6, X11, Fluxbox, TigerVNC и noVNC
RUN apt-get update && apt-get install -y \
    build-essential cmake wget git nano vim \
    libexpat1-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxmu-dev \
    qt6-base-dev qt6-declarative-dev qt6-3d-dev \
    tigervnc-standalone-server tigervnc-common fluxbox xterm \
    python3 python3-websockify net-tools \
    novnc websockify \
    && rm -rf /var/lib/apt/lists/*

# Симлинк для удобного запуска noVNC и установки страницы по умолчанию
RUN ln -s /usr/share/novnc /opt/novnc && \
    ln -s /usr/share/novnc/vnc.html /usr/share/novnc/index.html

# Скачивание и сборка Geant4 11.3.0
WORKDIR /opt/geant4
RUN wget https://github.com/Geant4/geant4/archive/refs/tags/v11.3.0.tar.gz && \
    tar -xzf v11.3.0.tar.gz && \
    rm v11.3.0.tar.gz && \
    mkdir geant4-build

WORKDIR /opt/geant4/geant4-build
# Ограничиваем сборку до 3 потоков, чтобы уложиться в 11 ГБ ОЗУ и не вызвать OOM
RUN cmake -DCMAKE_INSTALL_PREFIX=/usr/local/geant4 \
          -DGEANT4_BUILD_MULTITHREADED=ON \
          -DGEANT4_INSTALL_DATA=ON \
          -DGEANT4_USE_OPENGL_X11=ON \
          -DGEANT4_USE_QT=ON \
          -DGEANT4_USE_QT_QT6=ON \
          ../geant4-11.3.0 && \
    make -j3 && \
    make install

# Очистка исходников для небольшого уменьшения веса (оставляем датасеты и бинарники)
RUN rm -rf /opt/geant4/geant4-build

# Настройка примера B1 на рабочем столе пользователя root
WORKDIR /root/Desktop/exampleB1
RUN cp -r /opt/geant4/geant4-11.3.0/examples/basic/B1/* . && \
    mkdir build && cd build && \
    /bin/bash -c "source /usr/local/geant4/bin/geant4.sh && cmake .. && make -j3"

# Добавляем настройку окружения Geant4 в bashrc, чтобы он был доступен сразу при открытии терминала
RUN echo "source /usr/local/geant4/bin/geant4.sh" >> /root/.bashrc

# Полная замена стилей и внедрение кастомного Geant4 UI
COPY custom_ui.html /tmp/custom_ui_js.html
COPY easter_egg.png /usr/share/novnc/app/images/easter_egg.png
RUN cat /tmp/custom_ui_js.html >> /usr/share/novnc/vnc.html

# Подготовка скрипта запуска
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Копируем исходный код детектора в образ
COPY workspace/optical_detector /opt/optical_detector_src

# Собираем "вшитую" версию проекта
RUN mkdir -p /opt/optical_detector_build && \
    cd /opt/optical_detector_build && \
    /bin/bash -c "source /usr/local/geant4/bin/geant4.sh && \
    cmake /opt/optical_detector_src && \
    make -j3"

# Создаем удобный алиас для запуска вшитой версии
RUN echo 'alias run_detector="/opt/optical_detector_build/optical_detector"' >> /root/.bashrc

# Рабочая директория (которую мы будем монтировать)
WORKDIR /workspace

EXPOSE "80"

ENTRYPOINT ["/entrypoint.sh"]
