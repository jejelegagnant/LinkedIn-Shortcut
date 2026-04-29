#!/bin/bash
echo "=== Setup ==="
echo "Enter keyboard path (/dev/input/eventX):"
read -e KBD_PATH

FIFO_PATH="/var/snap/linkedin-shortcut/common/trigger.fifo"

# Clean up old tests and create the Named Pipe
rm -rf /var/snap/linkedin-shortcut/common/ipc
rm -f $FIFO_PATH
mkfifo $FIFO_PATH
chmod 666 $FIFO_PATH

snapctl set keyboard="$KBD_PATH"
snapctl restart "${SNAP_NAME}.daemon"

echo "Done. Remember to connect raw-input!"
