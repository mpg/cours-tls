MBEDTLS_DIR=../mbedtls
LIB_DIR=$(MBEDTLS_DIR)/library
INC_DIR=$(MBEDTLS_DIR)/include
PRG_DIR=$(MBEDTLS_DIR)/programs

CFLAGS=-std=c99 -Wall -Wextra -Os -I $(INC_DIR)

LIBS=$(LIB_DIR)/libmbedtls.a $(LIB_DIR)/libmbedx509.a $(LIB_DIR)/libmbedcrypto.a
BINS=pinning_client flexible_server cert_info

all: $(BINS)

pinning_client: pinning_client.c $(LIBS)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

cert_info: cert_info.c $(LIBS)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

flexible_server: $(PRG_DIR)/ssl/ssl_server2.c $(LIBS)
	$(MAKE) -C $(PRG_DIR) ssl/ssl_server2
	cp $(PRG_DIR)/ssl/ssl_server2 flexible_server

$(LIBS):
	$(MAKE) -C $(MBEDTLS_DIR) lib

force_libs:
	$(MAKE) -C $(MBEDTLS_DIR) lib

mrproper:
	rm -f $(BINS)

.PHONY: force_libs mrproper
