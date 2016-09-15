/**
 * \file
 * \brief low-level HAL - methods used to setup indirection to physical layer interface.
 * this level does the dirty work of abstracting the higher level ATCAIFace methods from the
 * low-level physical interfaces.  Its main goal is to keep low-level details from bleeding into
 * the logical interface implemetation.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \atmel_crypto_device_library_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel integrated circuit.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \atmel_crypto_device_library_license_stop
 */


/* when incorporating ATCA HAL into your application, you need to adjust the #defines in atca_hal.h to include
 * and exclude appropriate interfaces - this optimizes memory use when not using a specific iface implementation in your application */

#include "cryptoauthlib.h"
#include "atca_hal.h"

/** \brief Standard HAL API for ATCA to initialize a physical interface
 * \param[in] cfg pointer to ATCAIfaceCfg object
 * \param[in] hal pointer to ATCAHAL_t intermediate datastructure
 */

ATCA_STATUS hal_iface_init( ATCAIfaceCfg *cfg, ATCAHAL_t *hal )
{
	// Because C isn't a real object oriented language or dynamically typed, some switch in the overall system is unavoidable
	// The key here is to provide the flexibility to include just the types of interfaces you want/need without breaking the
	// object model.  The former is needed so in an embedded, constrained memory environment, you don't have to pay the price
	// (in terms of memory) for interfaces you don't use in your application.
	ATCA_STATUS status = ATCA_COMM_FAIL;

	switch (cfg->iface_type) {
	case ATCA_I2C_IFACE:
		#ifdef ATCA_HAL_I2C
		hal->halinit = &hal_i2c_init;
		hal->halpostinit = &hal_i2c_post_init;
		hal->halreceive = &hal_i2c_receive;
		hal->halsend = &hal_i2c_send;
		hal->halsleep = &hal_i2c_sleep;
		hal->halwake = &hal_i2c_wake;
		hal->halidle = &hal_i2c_idle;
		hal->halrelease = &hal_i2c_release;
		hal->hal_data = NULL;

		status = ATCA_SUCCESS;
		#endif
		break;
	case ATCA_SWI_IFACE:
		#ifdef ATCA_HAL_SWI
		hal->halinit = &hal_swi_init;
		hal->halpostinit = &hal_swi_post_init;
		hal->halreceive = &hal_swi_receive;
		hal->halsend = &hal_swi_send;
		hal->halsleep = &hal_swi_sleep;
		hal->halwake = &hal_swi_wake;
		hal->halidle = &hal_swi_idle;
		hal->halrelease = &hal_swi_release;
		hal->hal_data = NULL;

		status = ATCA_SUCCESS;
		#endif
		break;
	case ATCA_UART_IFACE:
		#ifdef ATCA_HAL_UART
		// TODO - initialize UART iface
		#endif
		#ifdef ATCA_HAL_KIT_CDC
		hal->halinit = &hal_kit_cdc_init;
		hal->halpostinit = &hal_kit_cdc_post_init;
		hal->halreceive = &hal_kit_cdc_receive;
		hal->halsend = &hal_kit_cdc_send;
		hal->halsleep = &hal_kit_cdc_sleep;
		hal->halwake = &hal_kit_cdc_wake;
		hal->halidle = &hal_kit_cdc_idle;
		hal->halrelease = &hal_kit_cdc_release;
		hal->hal_data = NULL;

		status = ATCA_SUCCESS;
		#endif
		break;
	case ATCA_SPI_IFACE:
		#ifdef ATCA_HAL_SPI
		// TODO - initialize SPI iface
		#endif
		break;
	case ATCA_HID_IFACE:
		#ifdef ATCA_HAL_KIT_HID
		hal->halinit = &hal_kit_hid_init;
		hal->halpostinit = &hal_kit_hid_post_init;
		hal->halreceive = &hal_kit_hid_receive;
		hal->halsend = &hal_kit_hid_send;
		hal->halsleep = &hal_kit_hid_sleep;
		hal->halwake = &hal_kit_hid_wake;
		hal->halidle = &hal_kit_hid_idle;
		hal->halrelease = &hal_kit_hid_release;
		hal->hal_data = NULL;

		status = ATCA_SUCCESS;
		#endif
		break;
	}
	return status;
}

/** \brief releases a physical interface, HAL knows how to interpret hal_data
 * \param[in] ifacetype - the type of physical interface to release
 * \param[in] hal_data - pointer to opaque hal data maintained by HAL implementation for this interface type
 */

ATCA_STATUS hal_iface_release( ATCAIfaceType ifacetype, void *hal_data )
{
	ATCA_STATUS status = ATCA_GEN_FAIL;

	switch (ifacetype) {
	case ATCA_I2C_IFACE:
			#ifdef ATCA_HAL_I2C
		status = hal_i2c_release(hal_data);
			#endif
		break;
	case ATCA_SWI_IFACE:
			#ifdef ATCA_HAL_SWI
		status = hal_swi_release(hal_data);
			#endif
		break;
	case ATCA_UART_IFACE:
			#ifdef ATCA_HAL_UART
		// TODO - release HAL UART
			#endif
			#ifdef ATCA_HAL_KIT_CDC
		status = hal_kit_cdc_release(hal_data);
			#endif
		break;
	case ATCA_SPI_IFACE:
			#ifdef ATCA_HAL_SPI
		// TODO - release HAL SPI
			#endif
		break;
	case ATCA_HID_IFACE:
			#ifdef ATCA_HAL_KIT_HID
		status = hal_kit_hid_release(hal_data);
			#endif
		break;
	}

	return status;
}


/** \brief wake up CryptoAuth device using I2C bus
 * \param[in] iface  interface to logical device to wakeup
 */
ATCA_STATUS hal_i2c_wake(ATCAIface iface)
{
	ATCAIfaceCfg *cfg = atgetifacecfg(iface);
	uint8_t currentaddress = cfg->atcai2c.slave_address;
	uint32_t bdrt = cfg->atcai2c.baud;
	uint8_t data[4], expected[4] = { 0x04, 0x11, 0x33, 0x43 };
	uint16_t rlength = 4;
	
	if ( bdrt != 100000 )  // if not already at 100KHz, change it
	change_i2c_speed( iface, 100000 );
	
	if (cfg->atcai2c.slave_address != 0x00) //change slave address to 0x00 if not already, for wake need to pull sda for twhi+twlo
		cfg->atcai2c.slave_address = 0x00;
	
	i2c_master_write (iface,&data[0],0);    // part will NACK, so don't check for status
	
	cfg->atcai2c.slave_address = currentaddress; //restore original address to cfg
	atca_delay_us(cfg->wake_delay);     // wait tWHI + tWLO which is configured based on device type and configuration structure
	
	hal_i2c_receive (iface,&data[0],&rlength);
	
	// if necessary, revert baud rate to what came in.
	if ( bdrt != 100000 )
	change_i2c_speed( iface, bdrt );

	if ( memcmp( data, expected, 4 ) == 0 )
	return ATCA_SUCCESS;

	return ATCA_COMM_FAIL;
}

/** \brief idle CryptoAuth device using I2C bus
 * \param[in] iface  interface to logical device to idle
 */
ATCA_STATUS hal_i2c_idle(ATCAIface iface)
{
	uint8_t data = 0x02, datalength = 1;  // idle word address value and length
	
	if (i2c_master_write(iface,&data,datalength) != ATCA_SUCCESS) 
		return ATCA_COMM_FAIL;

	return ATCA_SUCCESS;
}

/** \brief sleep CryptoAuth device using I2C bus
 * \param[in] iface  interface to logical device to sleep
 */
ATCA_STATUS hal_i2c_sleep(ATCAIface iface)
{
	uint8_t data = 0x01, datalength = 1;  // sleed word address value and length
	
	if (i2c_master_write(iface,&data,datalength) != ATCA_SUCCESS) 
	return ATCA_COMM_FAIL;
	
	return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t *txdata, int txlength)
{
	// for this implementation of I2C with CryptoAuth chips, txdata is assumed to have ATCAPacket format

	// other device types that don't require i/o tokens on the front end of a command need a different hal_i2c_send and wire it up instead of this one
	// this covers devices such as ATSHA204A and ATECCx08A that require a word address value pre-pended to the packet
	// txdata[0] is using _reserved byte of the ATCAPacket
	txdata[0] = 0x03;   // insert the Word Address Value, Command token
	txlength++;         // account for word address value byte.
	

	//	statusCode = i2c_master_write_packet_wait(&i2c_master_instance, &packet);
	//if ( i2c_master_write_packet_wait_no_stop( &(i2c_hal_data[bus]->i2c_master_instance), &packet) != STATUS_OK)
	//if ( i2c_master_write_packet_wait(&(i2c_hal_data[bus]->i2c_master_instance), &packet) != STATUS_OK )
	if (i2c_master_write(iface, txdata, txlength) != ATCA_SUCCESS)
	return ATCA_COMM_FAIL;

	return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_receive( ATCAIface iface, uint8_t *rxdata, uint16_t *rxlength)
{
	return i2c_master_read(iface, rxdata, rxlength);
}