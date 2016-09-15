#include <stdint.h>
#include "astekcrypto.h"
#include "cryptoauthlib.h"
#include "authentication/Authenticate.h"
#include "bootloader/secureboot.h"
#include "atcacert/atcacert_host_hw.h"

char astek_version[] = { "20160913" };  // change for each release, yyyymmdd

extern ATCADevice _gDevice;
extern ATCACommand _gCommandObj;
extern ATCAIface _gIface;

ATCAIfaceCfg *gCfg = NULL; //Store device configuration

/** \brief returns a version string for the astek crypto release.
 *  The format of the version string returned is "yyyymmdd"
 * \param[out] verstr ptr to space to receive version string
 * \return ATCA_STATUS
 */
ATCA_STATUS crypto_getrev( char *verstr )
{
	strcpy( verstr, astek_version );
	return ATCA_SUCCESS;
}

ATCA_STATUS crypto_info(uint8_t *revision)
{
	return atcab_info(revision);
}

ATCA_STATUS crypto_sernum(uint8_t *sernum)
{
	return atcab_read_serial_number(sernum);
}

ATCA_STATUS crypto_init()
{
	gCfg = &cfg_ateccx08a_i2c_default;
	gCfg->devtype = ATECC508A;
	
	return atcab_init(gCfg);
}

ATCA_STATUS cryto_read_config(uint8_t *config_data)
{
	atcab_read_ecc_config_zone(config_data);
}

ATCA_STATUS crypto_read_zone(uint8_t zone, uint8_t slot, uint8_t block, uint8_t offset, uint8_t *data, uint8_t len)
{
	return atcab_read_zone(zone, slot, block, offset, *data, len);
}

ATCA_STATUS crypto_changedev(ATCAIfaceCfg *cfg)
{
	return atcab_init(cfg);
}

ATCA_STATUS crypto_boreclean(ATCAIfaceCfg *cfg)
{
	ATCA_STATUS status;

	crypto_changedev(cfg);
	
	status = atcab_wakeup(); //try to wake - tests i2c and delay function

	return status;
}

ATCA_STATUS crypto_chageaddr(uint8_t slave_address)
{	
	ATCAIfaceCfg *cfg = atgetifacecfg(_gIface);
	cfg->atcai2c.slave_address = slave_address;
	
	return ATCA_SUCCESS;
}

ATCA_STATUS crypto_authenticate_hw(ATCAIfaceCfg *host_cfg, ATCAIfaceCfg *client_cfg)
{
	return authenticate_hw(host_cfg, client_cfg);
}

ATCA_STATUS crypto_authenticate_sw(ATCAIfaceCfg *dev_cfg)
{
	return authenticate_sw(dev_cfg);
}

ATCA_STATUS crypto_gen_challenge(uint8_t g_challenge[32])
{	
	ATCA_STATUS ret;
	ret = atcacert_gen_challenge_hw(g_challenge);
		
	return ret;
}

ATCA_STATUS crypto_secureboot_getparams(secureboot_params* params, uint8_t* app_tbs_digest, uint32_t app_size)
{
	return get_boot_params(params, app_tbs_digest, app_size);	
}

/*brief: Checks application to be run before jumping to application section. 
 *This should be called in booloader section before authorizing application to run
 *param[in] params			- ptr to secure boot parameters structure, generate from generate boot method using astek usb signer
 *param[in] AppImage		- ptr to application image
 *return     ATCA_STATUS
*/
ATCA_STATUS crypto_secureboot_check(secureboot_params* params,uint8_t* AppImage)
{
	return secureboot_check(params, AppImage);
}

