

#include "secureboot.h"
#include "atcacert/atcacert_client.h"
#include "atcacert/atcacert_host_hw.h"
#include "atcacert/atcacert_def.h"
#include "crypto/atca_crypto_sw_sha2.h"
#include "Authentication/cert_def_1_signer.h"
#include "Authentication/cert_def_2_device.h"


extern const uint8_t g_signer_1_ca_public_key[64];


/*brief: generates all parameters to be saved into flash for secure boot operation
 *param[out] params			- ptr to secure boot parameters structure
 *param[in]  app_tbs_digest - ptr to application to be signed digest
 *param[in]  app_size       - ptr to application image size value
 *return     ATCA_STATUS
*/
ATCA_STATUS get_boot_params(secureboot_params* params, uint8_t* app_tbs_digest, uint32_t app_size)
{
	ATCA_STATUS ret = ATCA_UNIMPLEMENTED;
	/** \brief global variables for signer certificate */
	uint8_t g_signer_cert[1024];
	size_t  g_signer_cert_size = sizeof(g_signer_cert);
	uint8_t signer_pubkey[64];
	
	/** \brief global storage for device certificate */
	uint8_t g_device_cert[1024];
	size_t  g_device_cert_size = sizeof(g_device_cert);
	
	//Retrieve root public key
	memcpy(params->root_pubkey, g_signer_1_ca_public_key, 64);
	
	//Generate signer certificate
	ret = atcacert_read_cert(&g_cert_def_1_signer, params->root_pubkey, &g_signer_cert, &g_signer_cert_size);
	if (ret != ATCA_SUCCESS) return ret;
	
	//Extract signer public key
	ret = atcacert_get_subj_public_key(&g_cert_def_1_signer, &g_signer_cert, g_signer_cert_size, signer_pubkey);
	if (ret != ATCA_SUCCESS) return ret;
	
	//Sanity check for signer certificate size
	if (g_signer_cert_size > SIGNER_CERT_SIZE)
	{
		return ATCACERT_E_UNEXPECTED_ELEM_SIZE;
	}else
	{
		//Retrieve signer certificate
		memcpy(params->signer_cert, g_signer_cert, SIGNER_CERT_SIZE);
	}

	//Generate device certificate
	ret = atcacert_read_cert(&g_cert_def_2_device, signer_pubkey, &g_device_cert, &g_device_cert_size);
	if (ret != ATCA_SUCCESS) return ret;
	
	//Sanity check for device certificate size
	if (g_device_cert_size > DEVICE_CERT_SIZE)
	{
		return ATCACERT_E_UNEXPECTED_ELEM_SIZE;
	}else
	{
		//Retrieve device certificate
		memcpy(params->device_cert, g_device_cert, DEVICE_CERT_SIZE);
	}
	
	//Assign application image size
	params->appimg_size = app_size;
	
	//sign application image digest
	ret = atcacert_get_response(g_cert_def_2_device.private_key_slot, app_tbs_digest, params->appimg_signature);
	if (ret != ATCA_SUCCESS) return ret;
	
	
	return ret;
}

/*brief: Checks application to be run before jumping to application section. 
 *This should be called in booloader section before authorizing application to run
 *param[in] params			- ptr to secure boot parameters structure, generate from generate boot method using astek usb signer
 *param[in] AppImage		- ptr to application image
 *return     ATCA_STATUS
*/
ATCA_STATUS secureboot_check(secureboot_params* params, uint8_t* AppImage)
{
	ATCA_STATUS ret = ATCA_UNIMPLEMENTED;
	uint8_t signer_pubkey[64];
	uint8_t device_pubkey[64];
	uint8_t digest[32];
	bool is_verified = false;
	
	//Check root public keys match
	if ( memcmp( &(params->root_pubkey), &g_signer_1_ca_public_key, 64 ) != 0 )
	{
		return ATCA_INVALID_ID;
	}
	
	/*Validate signer certificate against root public key*/
	ret = atcacert_verify_cert_hw(&g_cert_def_1_signer, &params->signer_cert, SIGNER_CERT_SIZE, params->root_pubkey);
	if (ret != ATCA_SUCCESS) return ret;
	
	/*Extract signer public key*/
	ret = atcacert_get_subj_public_key(&g_cert_def_1_signer, &params->signer_cert, SIGNER_CERT_SIZE, signer_pubkey);
	if (ret != ATCA_SUCCESS) return ret;
	
	/*Validate the device cert against signer key*/
	ret = atcacert_verify_cert_hw(&g_cert_def_2_device, &params->device_cert, DEVICE_CERT_SIZE, signer_pubkey);
	if (ret != ATCA_SUCCESS) return ret;
	
	/*Extract device public key.*/
	ret = atcacert_get_subj_public_key(&g_cert_def_2_device, &params->device_cert, DEVICE_CERT_SIZE, device_pubkey);
	if (ret != ATCA_SUCCESS) return ret;
	
	//Calculate app digest
	ret = atcac_sw_sha2_256(AppImage, params->appimg_size, digest);
	if (ret != ATCACERT_E_SUCCESS)
	return ret;
	
	//Verify digest against signature
	ret = atcab_verify_extern(&digest, &(params->appimg_signature), &device_pubkey, &is_verified);
	if (ret != ATCA_SUCCESS)
	return ret;

	return is_verified ? ATCACERT_E_SUCCESS : ATCACERT_E_VERIFY_FAILED;
}


