#!/system-bin/sh

FILE="/app-data/default_settings_restored"
if [ ! -f "$FILE" ]; then
    touch "$FILE"
    cp -r /app-misc/eink-config/* /app-data/
fi

# Wait for inkbox to exit
sleep 3
echo 2 > /sys/class/graphics/fb0/rotate
echo 2 > /sys/class/graphics/fb0/rotate
sleep 1

# No env -i please...
DEVICE="${DEVICE}" DEVICE_CODENAME="${DEVICE_CODENAME}" QT_FONT_DPI=${QT_FONT_DPI} PATH="/app-bin:/system-bin" LD_LIBRARY_PATH="/system-lib/lib:/system-lib/qt/lib:/app-lib/" QT_QPA_PLATFORM="kobo:keyboard:mouse" LC_ALL="en_US" QT_PLUGIN_PATH="/system-lib/qt/plugins/" XDG_CONFIG_HOME="/app-data/" XDG_RUNTIME_DIR="/app-data/" QT_SCALE_FACTOR=0.55 /system-lib/lib/ld-linux-armhf.so.3 /app-bin/rssguard.bin

echo 3 > /sys/class/graphics/fb0/rotate
echo 3 > /sys/class/graphics/fb0/rotate
sleep 3
