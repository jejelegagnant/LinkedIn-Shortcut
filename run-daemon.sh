#!/bin/bash
KBD_PATH=$(snapctl get keyboard)

if [ -z "$KBD_PATH" ]; then
    echo "Unconfigured."
    exit 1
fi

exec $SNAP/bin/daemon -k "$KBD_PATH"
