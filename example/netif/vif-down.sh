#!/bin/sh

set -e

IFNAME="${1:-ferrum0}"
SYSFS="/sys/class/net/${IFNAME}/ferrum"

if [ ! -d "/sys/class/net/${IFNAME}" ]; then
	echo "interface ${IFNAME} not found" >&2
	exit 1
fi

echo 0 > "${SYSFS}/enable"
ip addr flush dev "$IFNAME" 2>/dev/null || true

echo "ferrum: ${IFNAME} down"
