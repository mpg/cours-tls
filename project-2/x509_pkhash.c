#include "x509_pkhash.h"

#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/base64.h"

int x509_crt_pkhash( const mbedtls_x509_crt *crt, char *buf, size_t len )
{
    int ret;
    unsigned char pkey[1024]; /* All keys are ECC, so way shorter */

    if( ( ret = mbedtls_pk_write_pubkey_der( (mbedtls_pk_context *) &crt->pk,
                                             pkey, sizeof( pkey ) ) ) < 0 )
    {
        return( ret );
    }

    size_t pklen = ret;
    unsigned char *start = pkey + sizeof( pkey ) - pklen;

    unsigned char shaout[32];
    mbedtls_sha256( start, pklen, shaout, 0 );

    size_t b64len;
    if( ( ret = mbedtls_base64_encode( (unsigned char *) buf, len,
                                        &b64len, shaout, 32 ) ) != 0 )
    {
        return( ret );
    }

    if( b64len >= len )
    {
        buf[len-1] = '\0';
        return( -1 );
    }

    buf[b64len] = '\0';

    return( 0 );
}
