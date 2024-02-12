/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include "aes.h"
#include "fsl_crc.h"
#include "aescrc_cfg.h"

/*****************************************************************************/
/* Defines:                                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Private functions:                                                        */
/*****************************************************************************/
void InitCRC32(void);
void Set_AESCRC(uint8_t *plaintext, uint8_t padded_msg[]);
void Unset_AESCRC(uint8_t *plaintext, uint8_t padded_msg[]);