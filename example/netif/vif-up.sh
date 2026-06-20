#!/bin/sh

set -e

IFNAME="${1:-ferrum0}"

IP=192.168.50.1
PORT=9000
PREFIX=24

SYSFS="/sys/class/net/${IFNAME}/ferrum"

if [ ! -d "/sys/class/net/${IFNAME}" ]; then
	echo "interface ${IFNAME} not found, is the module loaded?" >&2
	exit 1
fi

echo "$IP"   > "${SYSFS}/ip"
echo "$PORT" > "${SYSFS}/port"

ip addr add "${IP}/${PREFIX}" dev "$IFNAME" 2>/dev/null || true

echo 1 > "${SYSFS}/enable"

echo "ferrum: ${IFNAME} up, ${IP}:${PORT}"
