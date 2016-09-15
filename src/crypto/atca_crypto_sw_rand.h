#ifndef ATCA_CRYPTO_SW_RAND_H
#define ATCA_CRYPTO_SW_RAND_H

#include "atca_crypto_sw.h"
#include <stddef.h>
#include <stdint.h>

/** \defgroup atcac_ Software crypto methods (atcac_)
 *
 * \brief
 * These methods provide a software implementation of various crypto
 * algorithms
 *
   @{ */
#ifdef __cplusplus
extern "C" {
#endif

uint8_t seed[32];
	
int atcac_sw_random(uint8_t* data, size_t data_size);


#ifdef __cplusplus
}
#endif
/** @} */

#endif