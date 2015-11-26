#include "x509_pkhash.h"

#include <string.h>

int x509_crt_pkhash( const mbedtls_x509_crt *crt, char *buf, size_t len )
{
    (void) crt;

    strncpy( buf, "TODO", len );

    return( 0 );
}
