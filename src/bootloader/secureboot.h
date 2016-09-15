#ifndef SECUREBOOT_H_
#define SECUREBOOT_H_

#define SIGNER_CERT_SIZE 506
#define DEVICE_CERT_SIZE 428

#include "cryptoauthlib.h"



typedef struct secureboot_params {
	uint8_t appimg_signature[64];
	uint32_t appimg_size;
	uint8_t root_pubkey[64];
	uint8_t signer_cert[SIGNER_CERT_SIZE];
	uint8_t device_cert[DEVICE_CERT_SIZE];
} secureboot_params;


ATCA_STATUS get_boot_params(secureboot_params* params, uint8_t* app_tbs_digest, uint32_t app_size);
ATCA_STATUS secureboot_check(secureboot_params* params, uint8_t* AppImage);

#endif /* SECUREBOOT_H_ */