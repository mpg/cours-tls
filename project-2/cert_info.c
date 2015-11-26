/*
 *  Certificate reading application
 *
 *  Simplified version of mbed TLS's programs/x509/cert_app
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_fprintf    fprintf
#define mbedtls_printf     printf
#endif

#if !defined(MBEDTLS_X509_CRT_PARSE_C) || !defined(MBEDTLS_FS_IO)
int main( void )
{
    mbedtls_printf( "MBEDTLS_X509_CRT_PARSE_C and/or MBEDTLS_FS_IO "
                    "not defined.\n");
    return( 0 );
}
#else

#include "mbedtls/x509_crt.h"
#include "x509_pkhash.h"

#define USAGE "usage: cert_info filename.crt\n"


int main( int argc, char *argv[] )
{
    int ret = 0;
    const char *filename;
    char buf[1024];
    mbedtls_x509_crt crt;

    mbedtls_x509_crt_init( &crt );

    if( argc != 2 )
    {
        mbedtls_printf( USAGE );
        return( 1 );
    }

    filename = argv[1];

    /*
     * 1. Load the certificate
     */
    mbedtls_printf( "\n  . Loading certificate '%s' ...", filename );
    fflush( stdout );

    ret = mbedtls_x509_crt_parse_file( &crt, filename );
    if( ret < 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509_crt_parse_file returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\n" );

    /*
     * 2. Print the certificate
     */
    mbedtls_printf( "  . Certificate information:\n" );

    ret = mbedtls_x509_crt_info( buf, sizeof( buf ), "     ", &crt );
    if( ret < 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509_crt_info returned %d\n\n", ret );
        goto exit;
    }
    ret = 0;

    mbedtls_printf( "%s\n", buf );

    /*
     * 3. Print de certificate's public key hash
     */
    mbedtls_printf( "  . Certificate public key hash: " );

    ret = x509_crt_pkhash( &crt, buf );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  x509_crt_pkhash returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_printf( "%s\n", buf );

exit:
    mbedtls_x509_crt_free( &crt );

#if defined(_WIN32)
    mbedtls_printf( "  + Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( ret != 0 );
}
#endif /* MBEDTLS_X509_CRT_PARSE_C && MBEDTLS_FS_IO */
