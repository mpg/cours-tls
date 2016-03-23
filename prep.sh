#!/bin/sh

# Get projects ready for being graded
# Assumes all zipped/tared projects are at the root

set -eu

rm -rf rendu
mkdir rendu
ln -s ../mbedtls rendu/mbedtls

( cd mbedtls/programs && make ssl/ssl_server2 )

mpgdiff() {
    diff $1 ../../project-1/$1 > ${1}.mpg.diff
    diff -w $1 ../../project-1/$1 > ${1}.mpg.diff-w
}

for fn in *.zip *.tgz *.tar*; do
    name="${fn%%.*}"
    printf "${name}... "

    mkdir "rendu/$name"
    cd "rendu/$name"

    if echo "$fn" | grep '\.zip$' > /dev/null; then
        unzip ../../$fn >/dev/null
    else
        tar xf ../../$fn
    fi

    if ls -R | egrep '\.mpg\.diff|GOOD|BAD|FILES'; then false; fi

    find . -type f | sed 's/^\.\///' > ../FILES
    mv ../FILES .

    if grep / FILES >/dev/null && [ ! -e mitm_proxy.c ]; then
        touch BAD-LAYOUT-DIR
        dir="$( sed 's/\/.*//' FILES | sort -u )"
        test -d "$dir"
        mv "$dir"/* .
        rmdir $dir
    fi

    if grep flexible_server * >/dev/null; then
        touch GOOD-SERVER
    fi

    if grep -i base64 * >/dev/null; then
        touch GOOD-BASE64
    fi

    if [ -e mitm_proxy.c ]; then
        mpgdiff mitm_proxy.c || true
    else
        touch BAD-NO-PROXY-SOURCE
    fi

    cp -S.BAD -b ../../project-1/vulnerable_client.c .
    cp -S.BAD -b ../../project-1/Makefile .

    if make > make.log 2> make.err; then
        if [ -s make.err ]; then
            touch BAD-WARNINGS
        else
            rm -f make.err
        fi
    else
        touch BAD-DOES-NOT-BUILD
    fi

    ls | egrep 'GOOD|BAD' >> FILES || true

    cd ../..
    echo "done"
done
