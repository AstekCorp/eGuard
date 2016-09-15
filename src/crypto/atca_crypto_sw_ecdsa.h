#ifndef ATCA_CRYPTO_SW_ECDSA_H
#define ATCA_CRYPTO_SW_ECDSA_H

#include "atca_crypto_sw.h"
#include <stddef.h>
#include <stdint.h>

/* Curve selection options. */
#define ECC_CURVE secp256r1

#define secp128r1 16
#define secp192r1 24
#define secp256r1 32
#define secp384r1 48

#ifndef ECC_CURVE
    #define ECC_CURVE secp256r1
#endif

#if (ECC_CURVE != secp128r1 && ECC_CURVE != secp192r1 && ECC_CURVE != secp256r1 && ECC_CURVE != secp384r1)
    #error "Must define ECC_CURVE to one of the available curves"
#endif

#define ECC_BYTES ECC_CURVE

#ifdef __cplusplus
extern "C"
{
#endif

/* ecc_make_key() function.
Create a public/private key pair.
    
Outputs:
    p_publicKey  - Will be filled in with the public key.
    p_privateKey - Will be filled in with the private key.

Returns 1 if the key pair was generated successfully, 0 if an error occurred.
*/
int ecc_make_key(uint8_t p_publicKey[ECC_BYTES+1], uint8_t p_privateKey[ECC_BYTES]);

/* ecdh_shared_secret() function.
Compute a shared secret given your secret key and someone else's public key.
Note: It is recommended that you hash the result of ecdh_shared_secret before using it for symmetric encryption or HMAC.

Inputs:
    p_publicKey  - The public key of the remote party.
    p_privateKey - Your private key.

Outputs:
    p_secret - Will be filled in with the shared secret value.

Returns 1 if the shared secret was generated successfully, 0 if an error occurred.
*/
int ecdh_shared_secret(const uint8_t p_publicKey[ECC_BYTES+1], const uint8_t p_privateKey[ECC_BYTES], uint8_t p_secret[ECC_BYTES]);

/* ecdsa_sign() function.
Generate an ECDSA signature for a given hash value.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and pass it in to
this function along with your private key.

Inputs:
    p_privateKey - Your private key.
    p_hash       - The message hash to sign.

Outputs:
    p_signature  - Will be filled in with the signature value.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int ecdsa_sign(const uint8_t p_privateKey[ECC_BYTES], const uint8_t p_hash[ECC_BYTES], uint8_t p_signature[ECC_BYTES*2]);

/* ecdsa_verify() function.
Verify an ECDSA signature.

Usage: Compute the hash of the signed data using the same hash as the signer and
pass it to this function along with the signer's public key and the signature values (r and s).

Inputs:
    p_publicKey - The signer's public key
    p_hash      - The hash of the signed data.
    p_signature - The signature value.

Returns 1 if the signature is valid, 0 if it is invalid.
*/                                          //33 bytes                       32 bytes                              64 bytes
int ecdsa_verify(const uint8_t p_publicKey[ECC_BYTES*2], const uint8_t p_hash[ECC_BYTES], const uint8_t p_signature[ECC_BYTES*2]);

#ifdef __cplusplus
} /* end of extern "C" */
#endif


/** \defgroup atcac_ Software crypto methods (atcac_)
 *
 * \brief
 * These methods provide a software implementation of various crypto
 * algorithms
 *
   @{ */

#define ATCA_ECC_P256_FIELD_SIZE       (256 / 8)
#define ATCA_ECC_P256_PRIVATE_KEY_SIZE (ATCA_ECC_P256_FIELD_SIZE)
#define ATCA_ECC_P256_PUBLIC_KEY_SIZE  (ATCA_ECC_P256_FIELD_SIZE * 2)
#define ATCA_ECC_P256_SIGNATURE_SIZE   (ATCA_ECC_P256_FIELD_SIZE * 2)

#ifdef __cplusplus
extern "C" {
#endif
				
int atcac_sw_ecdsa_verify_p256( const uint8_t msg[ATCA_ECC_P256_FIELD_SIZE],				//32 btes
                                const uint8_t signature[ATCA_ECC_P256_SIGNATURE_SIZE],		//64 bytes
                                const uint8_t public_key[ATCA_ECC_P256_PUBLIC_KEY_SIZE]);	//64 bytes

#ifdef __cplusplus
}
#endif

/** @} */
#endif