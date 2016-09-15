
#include "atca_crypto_sw_rand.h"
#include "basic/atca_basic.h"
#include "Authentication/cert_def_1_signer.h"
#include "atca_crypto_sw_sha2.h"
#include "hal/atca_hal.h"
#include <stdlib.h>



extern const uint8_t g_signer_1_ca_public_key[64];

static void onetimepad(uint8_t* msg, uint8_t* key, int length)
{
	int i;
	for (i=0; i<length; i++)
	*(msg + i) ^= *(key + i);
}


/** \brief return software generated random number
 * \param[out] data       ptr to space to receive the random number
 * \param[in]  data_size  size of data buffer
 * return ATCA_STATUS
 */
int atcac_sw_random(uint8_t* data, size_t data_size)
{
	ATCA_STATUS ret;
	uint8_t nonce[32], randnum[32];
	
	//Request random number from IC
	ret = atcab_random(nonce);
	
	//Good idea to onetimepad nonce with random number generated from customer (HAL function)
	hal_random_number(&randnum);
	onetimepad(nonce,randnum, 32);
	
	if ((seed[0] == 0) && (seed[10] == 0))
	{
		memcpy(seed, g_signer_1_ca_public_key, 32);				//if seed hasn't been initialized use root public key
	}
	
	onetimepad(nonce,seed,32);									//one time pad to create cipher seed with nonce
	
	ret = atcac_sw_sha2_256(nonce, sizeof(nonce), seed);		//create digest of seed and store in nonce variable
	if (ret != ATCA_SUCCESS)
	return ret;
	
	ret = atcac_sw_sha2_256(seed, sizeof(seed), nonce);			//create digest of seed and store in nonce variable
	if (ret != ATCA_SUCCESS)
	return ret;
	
	memcpy(data, nonce, data_size);
	
	return ATCA_SUCCESS;
}

