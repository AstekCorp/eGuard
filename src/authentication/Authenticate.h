#ifndef AUTHENTICATE_H_
#define AUTHENTICATE_H_
#include "cryptoauthlib.h"

#define ROOT_PUBKEY_SLOT 0xD
#define PRODUCT_SIGNER_PRIV_KEY_SLOT 0X3

ATCA_STATUS authenticate_hw(ATCAIfaceCfg *host_cfg, ATCAIfaceCfg *client_cfg);
ATCA_STATUS authenticate_sw(ATCAIfaceCfg *client_cfg);

ATCA_STATUS sign_product(uint8_t* buffer, size_t length, uint8_t* signature);
ATCA_STATUS verify_product(uint8_t* buffer, size_t length, uint8_t* signature, uint8_t* public_key);




#endif /* AUTHENTICATE_H_ */