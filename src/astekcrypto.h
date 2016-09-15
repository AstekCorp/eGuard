#ifndef ASTEKCRYPTO_H_
#define ASTEKCRYPTO_H_

#include "cryptoauthlib.h"
#include "bootloader/secureboot.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ATCAIfaceCfg cfg_508a_host;
extern ATCAIfaceCfg cfg_508a_client;
extern ATCAIfaceCfg cfg_508a_borecleaner;

ATCA_STATUS crypto_getrev( char *verstr );
ATCA_STATUS crypto_init();
ATCA_STATUS crypto_changedev(ATCAIfaceCfg *cfg);
ATCA_STATUS crypto_boreclean(ATCAIfaceCfg *cfg);

ATCA_STATUS crypto_authenticate_sw(ATCAIfaceCfg *dev_cfg);
ATCA_STATUS crypto_authenticate_hw(ATCAIfaceCfg *host_cfg, ATCAIfaceCfg *client_cfg);

ATCA_STATUS crypto_sign_product();
ATCA_STATUS crypto_verify_product();

/*brief: generates all parameters to be saved into flash for secure boot operation
 *param[out] params			- ptr to secure boot parameters structure
 *param[in]  app_tbs_digest - ptr to application to be signed digest
 *param[in]  app_size       - ptr to application image size value
 *return     ATCA_STATUS
*/
ATCA_STATUS crypto_secureboot_getparams(secureboot_params* params, uint8_t* app_tbs_digest, uint32_t app_size);

/*brief: Checks application to be run before jumping to application section. 
 *This should be called in booloader section before authorizing application to run
 *param[in] params			- ptr to secure boot parameters structure, generate from generate boot method using astek usb signer
 *param[in] AppImage		- ptr to application image
 *return     ATCA_STATUS
*/
ATCA_STATUS crypto_secureboot_check(secureboot_params* params,uint8_t* AppImage);

#ifdef __cplusplus
}
#endif

#endif /* ASTEKCRYPTO_H_ */