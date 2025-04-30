/**/


#include "I2C.h"

/*    MCU FREQUENCY     */
#define F_CPU 8000000UL
    
    
    
    
    
    
void I2C_vInit(u32 SCL_Frequency)
{
    /*  
      Steps:

      1- Set SCL frequency using TWBR and prescaler bits (in TWSR).

      2- Enable TWI by setting TWEN in TWCR.
    
    */
    
    u8 TWPS_Bits = 0 ;
    
    /* Set TWSR bits based on macro */
    #if I2C_PRESCALER == 1
        TWSR &= ~( (1 << TWPS0) | (1 << TWPS1) );  /* TWPS = 00 */
        TWPS_Bits = 0;
    #elif I2C_PRESCALER == 4
        TWSR = (TWSR & 0xFC) | (1 << TWPS0);     /* TWPS = 01 */
        TWPS_Bits = 1;
    #elif I2C_PRESCALER == 16
        TWSR = (TWSR & 0xFC) | (1 << TWPS1);     /* TWPS = 10 */
        TWPS_Bits = 2;
    #elif I2C_PRESCALER == 64
        TWSR = (TWSR & 0xFC) | (1 << TWPS0) | (1 << TWPS1); /* TWPS = 11  */
        TWPS_Bits = 3;
    #else
        #error "Invalid I2C_PRESCALER value! Use 1, 4, 16, or 64."
    #endif
    
    /*  SCL EQUATION  */
    TWBR = ( ( F_CPU / ( 2 * SCL_Frequency * ( 1 << ( 2 * TWPS_Bits ) ) ) - 8 ) ; 
    
    I2C_ENABLE() ;
    
    
}




void I2C_vStart();




void I2C_vStop();




void I2C_writeByte();




u8 I2C_ReadByte_ACK();




u8 I2C_ReadByte_NACK();
