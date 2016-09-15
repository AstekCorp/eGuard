#include "Authenticate.h"
#include "astekcrypto.h"
#include "cryptoauthlib.h"
#include "atcacert/atcacert_client.h"
#include "atcacert/atcacert_host_hw.h"
#include "atcacert/atcacert_host_sw.h"
#include "cert_def_1_signer.h"
#include "cert_def_2_device.h"
#include "crypto/atca_crypto_sw_sha2.h"



/** \brief global variables for public keys */
uint8_t root_pub_key[64];
uint8_t signer_pub_key[64];
uint8_t device_public_key[64];
size_t  key_size = sizeof(root_pub_key);

/** \brief global variables for signer certificate */
uint8_t g_signer_cert[1024];
size_t  g_signer_cert_size = sizeof(g_signer_cert);

/** \brief global storage for device certificate */
uint8_t g_device_cert[1024];
size_t  g_device_cert_size = sizeof(g_device_cert);

/** \brief global storage for the challenge data to sign by the device */
uint8_t g_challenge[32];
uint8_t g_response[64];

ATCA_STATUS rebuild_certs(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	//Re-initialize cert size variable
	g_signer_cert_size = sizeof(g_signer_cert);
	g_device_cert_size = sizeof(g_device_cert);
	
	ret = atcacert_read_cert(&g_cert_def_1_signer, root_pub_key, &g_signer_cert, &g_signer_cert_size);
	if (ret != ATCA_SUCCESS) return ret;
	
	ret = atcacert_get_subj_public_key(&g_cert_def_1_signer, &g_signer_cert, g_signer_cert_size, signer_pub_key);
	if (ret != ATCA_SUCCESS) return ret;
	
	ret = atcacert_read_cert(&g_cert_def_2_device, signer_pub_key, &g_device_cert, &g_device_cert_size);
	if (ret != ATCA_SUCCESS) return ret;
	
	ret = atcacert_get_subj_public_key(&g_cert_def_2_device, &g_device_cert, g_device_cert_size, device_public_key);
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}

ATCA_STATUS cert_chain_verify(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	//Validate signer certificate against root public key
	ret = atcacert_verify_cert_hw(&g_cert_def_1_signer, &g_signer_cert, g_signer_cert_size, root_pub_key);
	if (ret != ATCA_SUCCESS) return ret;
	
	//Assumes signer key was already read from certificate in rebuilt certificate method
	
	//Validate the device cert against signer key
	ret = atcacert_verify_cert_hw(&g_cert_def_2_device, &g_device_cert, g_device_cert_size, signer_pub_key);
	if (ret != ATCA_SUCCESS) return ret;
	
	//return 0 if all succeeded
	return ret; 
}

ATCA_STATUS cert_chain_verify_sw(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	uint8_t signature[64];
	bool is_verified = false;
	
	ret = atcacert_verify_cert_sw(&g_cert_def_1_signer, &g_signer_cert, g_signer_cert_size, root_pub_key);
	if (ret != ATCA_SUCCESS) return ret;
	
	//Assumes signer key was already read from certificate in rebuilt certificate method
	
	//Validate the device cert against signer key
	ret = atcacert_verify_cert_sw(&g_cert_def_2_device, &g_device_cert, g_device_cert_size, signer_pub_key);
	if (ret != ATCA_SUCCESS) return ret;
	
	//return 0 if all succeeded
	return ret;
}

ATCA_STATUS generate_challenge(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	ret = atcacert_gen_challenge_hw(g_challenge);
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}

ATCA_STATUS generate_challenge_sw(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	ret = atcacert_gen_challenge_sw(g_challenge);
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}

ATCA_STATUS client_generate_response(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	ret = atcacert_get_response(g_cert_def_2_device.private_key_slot, g_challenge, g_response);
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}

ATCA_STATUS verify_response(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	//Assumes device key was already read from certificate in rebuilt certificate method
	
	ret = atcacert_verify_response_hw(device_public_key, g_challenge, g_response);
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}

ATCA_STATUS verify_response_sw(void)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	//Assumes device key was already read from certificate in rebuilt certificate method
	
	ret = atcacert_verify_response_sw(device_public_key, g_challenge, g_response);
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}

ATCA_STATUS authenticate_hw(ATCAIfaceCfg *host_cfg, ATCAIfaceCfg *client_cfg)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	
	atcab_wakeup();  // sanity check
	
	//Read root public key from host
	crypto_changedev(host_cfg);
	ret = atcab_read_pubkey(ROOT_PUBKEY_SLOT, &root_pub_key);
	if (ret != ATCA_SUCCESS) return ret;
	
	//rebuild certificates from client
	crypto_changedev(client_cfg);
	ret = rebuild_certs();
	if (ret != ATCA_SUCCESS) return ret;
	
	//Host verify chain -- Currently using SHA256_sw implementation so no comms with IC
	crypto_changedev(host_cfg);
	ret = cert_chain_verify();
	if (ret != ATCA_SUCCESS) return ret;
	
	//Host generate challenge
	crypto_changedev(host_cfg);  //sanity check to avoid client generating challenge
	ret = generate_challenge();
	if (ret != ATCA_SUCCESS) return ret;
	
	//Client generate response
	crypto_changedev(client_cfg);
	ret = client_generate_response();
	if (ret != ATCA_SUCCESS) return ret;
	
	//Host verify response
	crypto_changedev(host_cfg);
	ret = verify_response();
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}

ATCA_STATUS authenticate_sw(ATCAIfaceCfg *dev_cfg)
{
	ATCA_STATUS ret = ATCACERT_E_UNIMPLEMENTED;
	//Root pub key is already stored in FW - Cert files
	
	memcpy(root_pub_key, g_signer_1_ca_public_key, key_size);
	
	atcab_wakeup(); //Sanity check
	
	//rebuild certificates from client -- only chip to work with
	crypto_changedev(dev_cfg);
	ret = rebuild_certs();
	if (ret != ATCA_SUCCESS) return ret;
	
	//Verify certificates using SW functions
	ret = cert_chain_verify_sw();
	if (ret != ATCA_SUCCESS) return ret;
	
	//FW acting as host needs to generate random number
	ret = generate_challenge_sw();
	if (ret != ATCA_SUCCESS) return ret;
	
	//Client generate response
	ret = client_generate_response();
	if (ret != ATCA_SUCCESS) return ret;
	
	//Host(FW) verify response
	ret = verify_response_sw();
	if (ret != ATCA_SUCCESS) return ret;
	
	return ret;
}
 
/*brief: sign a unique buffer related to specific product
 *       This function allows you to guarantee only OEM products are allowed to interact
 *		 the system by signing the UID of the product and then using verify function during runtime
 *params[in] buffer:	 ptr to buffer to be signed
 *params[in] length:	 length of buffer
 *params[out] signature: ptr to signature of buffer
*/
ATCA_STATUS sign_product(uint8_t* buffer, size_t length, uint8_t* signature)
{
	ATCA_STATUS ret = ATCA_UNIMPLEMENTED;
	
	auto uint8_t tbs_digest[32];
	
	//SW SHA256 of message to generate tbs_digest
	ret = atcac_sw_sha2_256(buffer, length, tbs_digest);
	
	
	//Check parameters before signing data
	if (PRODUCT_SIGNER_PRIV_KEY_SLOT > 15 || &tbs_digest == NULL || signature == NULL)
	{
		return ATCACERT_E_BAD_PARAMS;	
	}
	atcab_sign(PRODUCT_SIGNER_PRIV_KEY_SLOT, &tbs_digest, signature);
}

/*brief: Verify product signature generated by sign_product function
 *       This function allows you to guarantee only OEM products are allowed to interact
 *		 the system by signing the UID of the product and then using verify function during runtime
 *params[in] buffer:	 ptr to buffer to be signed
 *params[in] length:	 length of buffer
 *params[in] signature:	 ptr to signature of buffer
 *params[in] public_key: ptr to public key of key pair used to create signature
 *return:	Success if everything succeeded and verified or error code if fail
*/
ATCA_STATUS verify_product(uint8_t* buffer, size_t length, uint8_t* signature, uint8_t* public_key)
{
	ATCA_STATUS ret = ATCA_UNIMPLEMENTED;
	auto uint8_t tbs_digest[32];
	
	//SW SHA256 of message to generate tbs_digest
	ret = atcac_sw_sha2_256(buffer, length, tbs_digest);
	if (ret != ATCA_SUCCESS)
	{
		return ret;
	}
	
	ret = atcacert_verify_response_hw(public_key, tbs_digest, signature);	
	if (ret != ATCA_SUCCESS)
	{
		return ret;
	}	
}