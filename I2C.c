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




void I2C_vStart()
{
    /* variable for storing the status code */
    u8 status_code ;
    
    /*  Start Condition   */
    I2C_START_CONDITION() ; 

    /*  Enabling The I2C  */
    I2C_ENABLE() ;

    /*  Clearing the TWINT Flag   */
    I2C_CLEAR_TWINT_FLAG() ;
    
    /*  Waiting on the TWINT Flag   */
    while( !( TWCR & (1 << TWINT) ) ) ;
    
    /*  Reading the Status Code */
    status_code = ( TWCR & 0xF8 ) ;
    
    if( status_code != 0x08 && status_code != 0x10)
    {
        /*  Handle the error if needed   */
    }

}




void I2C_vStop()
{
    /*
        Steps to Send a STOP Condition:

    1- Set TWSTO (Stop Condition bit) and TWEN (Enable TWI) in the TWCR register.

    2- Also set TWINT to 1 to trigger the operation.

    3- (Optional) Wait a short time to ensure the STOP condition is sent before another I2C action.
    
    */
    
    /* Send STOP condition  */
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);

    /* (Optional) Small delay to allow STOP to complete before next action  */
    while (TWCR & (1 << TWSTO));  /* Wait until STOP bit is cleared by hardware */





}




void I2C_writeByte(u8 Data_Byte)
{

    u8 status; 
    /*

    Steps:

    1- Load the data into TWDR

    2- Clear TWINT to start transmission

    3- Wait until TWINT is set again

    4- (Optionally) Check TWSR for 0x28 (data transmitted, ACK received)
    
    */
    
    /*  Loading The Data in the TWDR Register */
    TWDR = Data_Byte ;
    
    /*  Enable I2C  */
    I2C_ENABLE();
    
    /*  Clear The Interrupt Flag  */
    I2C_CLEAR_TWINT_FLAG();
    
    /*  Wait until TWINT is set again */
    while (!(TWCR & (1 << TWINT)));

    /*  Check TWSR for 0x28 (data transmitted, ACK received)  */
    status = ( TWSR & 0xF8 ) ;
    
    /*  
      0x18 = SLA+W transmitted and ACK received

      0x28 = Data byte transmitted and ACK received 
    */
    
    if (status != 0x28 && status != 0x18)  /* Data or SLA+W ACK */
    {
        /* Optionally handle NACK or error  */
    }

    return status;
    
    
    



}




u8 I2C_ReadByte_ACK();




u8 I2C_ReadByte_NACK();
