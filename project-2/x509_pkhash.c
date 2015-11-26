#include "x509_pkhash.h"

#include <string.h>

int x509_crt_pkhash( const mbedtls_x509_crt *crt, char *buf )
{
    (void) crt;

    strcpy( buf, "TODO" );

    return( 0 );
}
