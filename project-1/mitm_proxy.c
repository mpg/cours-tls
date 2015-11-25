/*
 *  TCP proxy used to perform a MitM attack
 *
 *  Somewhat based on programs/test/udp_proxy.c from mbed TLS
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2015 Manuel Pégourié-Gonnard
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

/*
 * Warning: this is an internal utility program we use for tests.
 * It does break some abstractions from the NET layer, and is thus NOT an
 * example of good general usage.
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#define mbedtls_printf     printf
#endif

#if !defined(MBEDTLS_NET_C)
#include <stdio.h>
int main( void )
{
    mbedtls_printf( "MBEDTLS_NET_C not defined.\n" );
    return( 0 );
}
#else

#include "mbedtls/net.h"
#include "mbedtls/error.h"
#include "mbedtls/ssl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* For select() */
#if (defined(_WIN32) || defined(_WIN32_WCE)) && !defined(EFIX64) && \
    !defined(EFI32)
#include <winsock2.h>
#include <windows.h>
#if defined(_MSC_VER)
#if defined(_WIN32_WCE)
#pragma comment( lib, "ws2.lib" )
#else
#pragma comment( lib, "ws2_32.lib" )
#endif
#endif /* _MSC_VER */
#else /* ( _WIN32 || _WIN32_WCE ) && !EFIX64 && !EFI32 */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif /* ( _WIN32 || _WIN32_WCE ) && !EFIX64 && !EFI32 */

#define MAX_MSG_SIZE        16384 + 2048 /* max record size */

#define SERVER_ADDR         "localhost"
#define SERVER_PORT         "5556"
#define LISTEN_ADDR         "localhost"
#define LISTEN_PORT         "4433"

enum direction {
    c2s,
    s2c
};

/*
 * Forward a packet, possibly altering it in the process
 */
int forward( mbedtls_net_context *from,
             mbedtls_net_context *to,
             enum direction dir )
{
    unsigned char buf[MAX_MSG_SIZE];
    int ret;
    size_t len;

    if( ( ret = mbedtls_net_recv( from, buf, sizeof( buf ) ) ) < 0 )
        return( ret );

    if( ret == 0 )
        return( MBEDTLS_ERR_NET_CONN_RESET );

    len = (size_t) ret;
    printf( "forwarding %d bytes (%s)\n", ret, dir == c2s ? "c2s" : "s2c" );

    if( ( ret = mbedtls_net_send( to, buf, len ) ) < 0 )
        return( ret );

    if( ret == 0 )
        return( MBEDTLS_ERR_NET_CONN_RESET );

    /* Be lazy, don't handle partial writes, but still detect them */
    if( (size_t) ret != len )
        return( -1 );

    return 0;
}

int main( void )
{
    int ret;
    int really_exit = 0;

    mbedtls_net_context listen_fd, client_fd, server_fd;

    int nb_fds;
    fd_set read_fds;

    mbedtls_net_init( &listen_fd );
    mbedtls_net_init( &client_fd );
    mbedtls_net_init( &server_fd );

    /*
     * 1. Setup the listening TCP socket
     */
    mbedtls_printf( "  . Bind on TCP/%s/%s ...",
                    LISTEN_ADDR, LISTEN_PORT );
    fflush( stdout );

    if( ( ret = mbedtls_net_bind( &listen_fd, LISTEN_ADDR, LISTEN_PORT,
                                  MBEDTLS_NET_PROTO_TCP ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_net_bind returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\n" );

    /*
     * 2. Wait until a client connects
     */
accept:
    mbedtls_printf( "  . Waiting for a remote connection ..." );
    fflush( stdout );

    if( ( ret = mbedtls_net_accept( &listen_fd, &client_fd,
                                    NULL, 0, NULL ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_net_accept returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\n" );

    /*
     * 3. Connect to the server
     */
    mbedtls_printf( "  . Connect to server on TCP/%s/%s ...",
                    SERVER_ADDR, SERVER_PORT );
    fflush( stdout );

    if( ( ret = mbedtls_net_connect( &server_fd, SERVER_ADDR, SERVER_PORT,
                                     MBEDTLS_NET_PROTO_TCP ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_net_connect returned %d\n\n", ret );
        really_exit = 1;
        goto exit;
    }

    mbedtls_printf( " ok\n" );

    /*
     * 4. Forward packets until some party disconnects (more precisely, until
     * an error occurs) or a new client connects
     */
    nb_fds = client_fd.fd;
    if( nb_fds < server_fd.fd )
        nb_fds = server_fd.fd;
    ++nb_fds;

    while( 1 )
    {
        FD_ZERO( &read_fds );
        FD_SET( server_fd.fd, &read_fds );
        FD_SET( client_fd.fd, &read_fds );

        if( ( ret = select( nb_fds, &read_fds, NULL, NULL, NULL ) ) <= 0 )
        {
            perror( "select" );
            really_exit = 1;
            goto exit;
        }

        if( FD_ISSET( client_fd.fd, &read_fds ) )
        {
            if( ( ret = forward( &client_fd, &server_fd, c2s ) ) != 0 )
                goto exit;
        }

        if( FD_ISSET( server_fd.fd, &read_fds ) )
        {
            if( ( ret = forward( &server_fd, &client_fd, s2c ) ) != 0 )
                goto exit;
        }
    }

    /*
     * Only exit if we got a bad enough error,
     * otherwise loop to accepting a new client
     */
exit:

#ifdef MBEDTLS_ERROR_C
    if( ret != 0 )
    {
        char error_buf[100];
        mbedtls_strerror( ret, error_buf, 100 );
        mbedtls_printf( "Last error was: -0x%04X - %s\n\n", - ret, error_buf );
        fflush( stdout );
    }
#endif

    mbedtls_net_free( &client_fd );
    mbedtls_net_free( &server_fd );
    if( ! really_exit )
        goto accept;

    mbedtls_net_free( &listen_fd );

#if defined(_WIN32)
    mbedtls_printf( "  Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    return( ret != 0 );
}

#endif /* MBEDTLS_NET_C */
