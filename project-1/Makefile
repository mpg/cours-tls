CFLAGS=-Wall -Wextra -Os

MBEDTLS_DIR=../mbedtls
LIB_DIR=$(MBEDTLS_DIR)/library
INC_DIR=$(MBEDTLS_DIR)/include
PRG_DIR=$(MBEDTLS_DIR)/programs

LIBS=$(LIB_DIR)/libmbedtls.a $(LIB_DIR)/libmbedx509.a $(LIB_DIR)/libmbedcrypto.a

all: vulnerable_client flexible_server

vulnerable_client: vulnerable_client.c $(LIBS)
	$(CC) $(CFLAGS) -I $(INC_DIR) $< $(LIBS) -o $@

flexible_server: $(PRG_DIR)/ssl/ssl_server2.c $(LIBS)
	$(MAKE) -C $(PRG_DIR) ssl/ssl_server2
	cp $(PRG_DIR)/ssl/ssl_server2 flexible_server

$(LIBS): force_libs

force_libs:
	$(MAKE) -C $(MBEDTLS_DIR) lib

.PHONY: force_libs
