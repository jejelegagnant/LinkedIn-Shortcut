#!/bin/bash

# Ensure it is run as root so snap set works
if [ "$EUID" -ne 0 ]; then
  echo "Error: This setup script must be run as root."
  echo "Usage: sudo linkedin-button.setup"
  exit 1
fi

echo "=== LinkedIn Button Configuration ==="

# 1. Auto-detect the user who ran sudo
TARGET_USER=${SUDO_USER:-$(whoami)}
echo "[+] Detected target user: $TARGET_USER"

# 2. Help the user find their keyboard
echo ""
echo "[?] Scanning for attached keyboards..."
echo "------------------------------------------------"
# This parses the input devices and prints only the Name and corresponding event node for keyboards
grep -E 'Name=|Handlers=' /proc/bus/input/devices | grep -B1 'kbd' | grep -Eo 'Name=".*"|event[0-9]+'
echo "------------------------------------------------"
echo ""
read -p "Type the event node for your keyboard (e.g., event3): " EVENT_NODE
KBD_PATH="/dev/input/$EVENT_NODE"

# 3. Ask for the browser
read -p "Enter your preferred browser executable (default: firefox): " BROWSER
BROWSER=${BROWSER:-firefox}

# 4. Apply configurations via snap set
echo ""
echo "Applying configurations..."
snap set linkedin-shortcut keyboard="$KBD_PATH"
snap set linkedin-shortcut browser="$BROWSER"
snap set linkedin-shortcut user="$TARGET_USER"

# 5. Restart the daemon to apply changes
snap restart linkedin-shortcut
echo "Done! The daemon is now listening in the background."
