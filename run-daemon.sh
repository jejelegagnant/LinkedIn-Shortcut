#!/bin/sh

KBD=$(snapctl get keyboard)
BROWSER=$(snapctl get browser)
TARGET_USER=$(snapctl get user)

[ -z "$KBD" ] && KBD="/dev/input/event0"
[ -z "$BROWSER" ] && BROWSER="firefox"
[ -z "$TARGET_USER" ] && TARGET_USER="root"

exec $SNAP/bin/linkedin_daemon -k "$KBD" -b "$BROWSER" -u "$TARGET_USER"
