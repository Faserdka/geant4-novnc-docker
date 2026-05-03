#!/bin/bash
set -e

export USER=root
export HOME=/root

# Настройка пароля VNC
mkdir -p $HOME/.vnc
echo "${VNC_PASSWORD:-"PASSWORD"}" | vncpasswd -f > $HOME/.vnc/passwd
chmod 600 $HOME/.vnc/passwd

# Создание скрипта xstartup для Fluxbox
cat << 'EOF' > $HOME/.vnc/xstartup
#!/bin/sh
xsetroot -solid "#1e1e1e" # Темный фон
# Запускаем терминал при старте для удобства
xterm -geometry 80x24+10+10 -ls -title "Geant4 Terminal" &
exec fluxbox
EOF
chmod +x $HOME/.vnc/xstartup

# Удаление возможных блокировок X11 (если контейнер был перезапущен)
rm -rf /tmp/.X1-lock /tmp/.X11-unix/X1 /tmp/.X11-unix

# Запуск VNC-сервера TigerVNC
vncserver :1 -geometry 1280x768 -depth 24

# Запуск noVNC
echo "Starting noVNC web server on port ..."
# Используем websockify для проксирования VNC в WebSockets
exec websockify --web /usr/share/novnc "80@ localhost:"80"
