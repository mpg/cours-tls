#include "mbedtls/x509_crt.h"

/**
 * \brief       Compute the base64-encoded SHA-256 hash of a cert's public key
 *              The output is a printable, NULL-terminated string.
 *
 * \param crt   The certificate
 * \param buf   Output buffer
 * \param len   Size of buf
 *
 * \return      0 on success or a negative error code
 */
int x509_crt_pkhash( const mbedtls_x509_crt *crt, char *buf, size_t len );
