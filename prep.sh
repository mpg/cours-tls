#!/bin/sh

# Get projects ready for being graded
# Assumes all projects are at the root

set -eu

rm -rf rendu
mkdir rendu
ln -s ../mbedtls rendu/mbedtls

( cd mbedtls/programs && make ssl/ssl_server2 )

( cd project-1 && ls ) > rendu/ref-ls

mpgdiff() {
    diff $1 ../../project-1/$1 > ${1}.mpg.diff
    diff -w $1 ../../project-1/$1 > ${1}.mpg.diff-w
}

for fn in *.zip *.tgz *.tar*; do
    name="${fn%%.*}"
    printf "${name}... "

    if [ -d "rendu/$name" ]; then false; fi
    cp -r project-1 "rendu/$name"

    cd "rendu/$name"

    if echo "$fn" | grep '\.zip$' > /dev/null; then
        unzip -o ../../$fn >/dev/null
    else
        tar xf ../../$fn
    fi

    if [ -e mitm_proxy.c ]; then :; else
        touch BAD-LAYOUT-DIR
        if [ -d project-1 ]; then
            cp -r project-1/* .
        fi
    fi

    if ls | grep '\.mpg\.diff'; then false; fi

    ls > ../tmp
    diff ../tmp ../ref-ls > NEW-FILE || true

    mpgdiff mitm_proxy.c || true
    mpgdiff mitm_proxy.c -b || true

    if mpgdiff vulnerable_client.c; then
        rm vulnerable_client.c.mpg.diff*
    else
        touch BAD-CLIENT-MODIFIED
    fi

    if mpgdiff Makefile; then
        rm Makefile.mpg.diff*

        if make > make.log 2> make.err; then
            if [ -s make.err ]; then
                touch BAD-WARNINGS
            else
                rm -f make.err
            fi
        else
            touch BAD-DOES-NOT-BUILD
        fi
    else
        touch BAD-MAKEFILE-MODIFIED
    fi

    cd ../..
    echo "done"
done
