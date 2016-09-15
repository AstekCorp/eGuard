#include "atca_hal.h"
#include "hal/atca_hal.h"
#include "astek_hal.h"
#include "atca_device.h"


/** \brief default configuration for crypto IC devices */
ATCAIfaceCfg cfg_508a_host = {
	.iface_type				= ATCA_I2C_IFACE,
	.devtype				= ATECC508A,
	.atcai2c.slave_address	= 0xF0,
	.atcai2c.bus			= 2,
	.atcai2c.baud			= 400000,
	//.atcai2c.baud = 100000,
	.wake_delay				= 800,
	.rx_retries				= 20
};

ATCAIfaceCfg cfg_508a_client = {
	.iface_type				= ATCA_I2C_IFACE,
	.devtype				= ATECC508A,
	.atcai2c.slave_address	= 0xE0,
	.atcai2c.bus			= 2,
	.atcai2c.baud			= 400000,
	//.atcai2c.baud = 100000,
	.wake_delay				= 800,
	.rx_retries				= 20

};

ATCAIfaceCfg cfg_508a_borecleaner = {
	.iface_type				= ATCA_I2C_IFACE,
	.devtype				= ATECC508A,
	.atcai2c.slave_address	= 0xC0,
	.atcai2c.bus			= 2,
	.atcai2c.baud			= 400000,
	//.atcai2c.baud = 100000,
	.wake_delay				= 800,
	.rx_retries				= 20

};


/*Timer functions*/
void atca_delay_ms(uint32_t delay)
{

}

void atca_delay_us(uint32_t delay)
{

}

void atca_delay_10us(uint32_t delay)
{


}


/** \brief initialize an I2C interface using given config
 * \param[in] hal - opaque ptr to HAL data
 * \param[in] cfg - interface configuration
 */
ATCA_STATUS hal_i2c_init(void *hal, ATCAIfaceCfg *cfg)
{

}

/** \brief HAL implementation of I2C post init
 * \param[in] iface  instance
 * \return ATCA_STATUS
 * \This is a place holder for possible post init hardware requirements
 * \if this function is not used it must return ATCA_SUCCESS
 */
ATCA_STATUS hal_i2c_post_init(ATCAIface iface)
{ 
return ATCA_SUCCESS;
}
	
/** \brief HAL implementation of I2C Master Write
 * \param[in] iface     instance
 * \param[in] txdata    pointer to space to bytes to send
 * \param[in] txlength  number of bytes to send
 * \return ATCA_STATUS
 */
ATCA_STATUS i2c_master_write(ATCAIface iface, uint8_t *txdata, int txlength)
{	

}


/** \brief HAL implementation of I2C Master Read function
 * \param[in] iface     instance
 * \param[in] rxdata    pointer to space to receive the data
 * \param[in] rxlength  ptr to expected number of receive bytes to request
 * \return ATCA_STATUS
 */
ATCA_STATUS i2c_master_read( ATCAIface iface, uint8_t *rxdata, uint16_t *rxlength)
{

	
}



/** \brief method to change the bus speed of I2C
 * \param[in] iface  interface on which to change bus speed
 * \param[in] speed  baud rate (typically 100000 or 400000)
 */
void change_i2c_speed( ATCAIface iface, uint32_t speed )
{


}



/***** FUNCTIONS BELOW ARE OPTIONAL DEPENDING ON FUNCTIONALITIES USED ****/

/** \brief manages reference count on given bus and releases resource if no more references exist
 * \param[in] hal_data - opaque pointer to hal data structure - known only to the HAL implementation
 */
ATCA_STATUS hal_i2c_release( void *hal_data )
{
	/* implement your code here */
	return ATCA_UNIMPLEMENTED;
}

/** \brief discover i2c buses available for this hardware
 * this maintains a list of logical to physical bus mappings freeing the application
 * of the a-priori knowledge
 * \param[in] i2c_buses - an array of logical bus numbers
 * \param[in] max_buses - maximum number of buses the app wants to attempt to discover
 */
ATCA_STATUS hal_i2c_discover_buses(int i2c_buses[], int max_buses)
{
	/* implement your code here */
	return ATCA_UNIMPLEMENTED;
}

/** \brief discover any CryptoAuth devices on a given logical bus number
 * \param[in]  busNum  logical bus number on which to look for CryptoAuth devices
 * \param[out] cfg     pointer to head of an array of interface config structures which get filled in by this method
 * \param[out] found   number of devices found on this bus
 */
ATCA_STATUS hal_i2c_discover_devices(int busNum, ATCAIfaceCfg cfg[], int *found )
{
	/* implement your code here */
	return ATCA_UNIMPLEMENTED;
}

 /* brief return software generated 32 byte random number
  * param[out] random_number - ptr to space to receive the random number
 */
ATCA_STATUS hal_random_number(uint8_t* random_number)
{
	/* implement your code here */
	return ATCA_UNIMPLEMENTED;
}