# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.



#configure_file(include/fota_config.h.in ${BINARY_TOP_DIR}/include/fota_config.h)



#设置包含头文�?
target_include_directories(${target} PUBLIC ../../api/include)
target_include_directories(${target} PUBLIC ../components/newlib/include)
target_include_directories(${target} PUBLIC inc)

target_sources(${target} PRIVATE
	src/aes.c
	src/asn1.c
	src/bigint.c
	src/crypto_misc.c
	src/gen_cert.c
	src/hmac.c
	src/loader.c
	src/md5.c
	src/openssl.c
	src/os_port.c
	src/rc4.c
	src/rsa.c
	src/sha1.c
	src/sha256.c
	src/sha384.c
	src/sha512.c
	src/ssllib.c
	src/tls1.c
	src/tls1_clnt.c
	src/x509.c
	src/p12.c
)


