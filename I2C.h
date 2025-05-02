#ifndef I2C_H
#define I2C_H

#include "std_types.h"





/************************************************************************/
/*                        CONFIGURATIONS                                */
/************************************************************************/

/* I2C Prescaler Options (choose ONE) */
#define I2C_PRESCALER      1     /* Options: 1, 4, 16, 64 */

/*  I2C SCL CLK SPEED TO GIVE IT TO I2C_vINIT() */
#define I2C_SCL_100_KHZ         100000
#define I2C_SCL_400_KHZ         400000



/************************************************************************/
/*                        REGISTERS                                     */
/************************************************************************/

#define TWBR   (*(volatile u8*)0x20)  /* Bit Rate Register  */
#define TWSR   (*(volatile u8*)0x21)  /* Status Register */
#define TWAR   (*(volatile u8*)0x22)  /* Address  Register */
#define TWDR   (*(volatile u8*)0x23)  /* Data Register */
#define TWCR   (*(volatile u8*)0x56)  /* Control  Register */
#define TWAMR  (*(volatile u8*)0x57)  /* Address Mask Register */

/************************************************************************/
/*                    BIT DEFINITIONS OF REGISTERS                      */
/************************************************************************/

/******************************** TWCR – TWI Control Register ********************************/
#define TWIE    0   /** Interrupt Enable */
#define TWEN    2   /** TWI Enable */
#define TWWC    3   /** Write Collision Flag */
#define TWSTO   4   /** Stop Condition Bit */
#define TWSTA   5   /** Start Condition Bit */
#define TWEA    6   /** Enable Acknowledge */
#define TWINT   7   /** Interrupt Flag */

/******************************** TWSR – TWI Status Register ********************************/
#define TWPS0   0   /** Prescaler bit 0 */
#define TWPS1   1   /** Prescaler bit 1 */
/** Bits 7:3 hold the status code (mask with 0xF8 to read) */

/******************************** TWAR – TWI (Slave) Address Register ********************************/
#define TWGCE   0   /** General Call Recognition Enable */
/** Bits 7:1 are address bits (slave address) */


/************************************************************************/
/*                          STATUS CODES                                */
/************************************************************************/

/******************************** Master Transmitter Mode ********************************/
#define START_SENT             0x08  /** START condition transmitted */
#define REPEATED_START_SENT    0x10  /** Repeated START transmitted */
#define MT_SLA_W_ACK           0x18  /** SLA+W transmitted, ACK received */
#define MT_SLA_W_NACK          0x20  /** SLA+W transmitted, NACK received */
#define MT_DATA_ACK            0x28  /** Data byte transmitted, ACK received */
#define MT_DATA_NACK           0x30  /** Data byte transmitted, NACK received */
#define MT_ARB_LOST            0x38  /** Arbitration lost */

/******************************** Master Receiver Mode ********************************/
#define MR_ARB_LOST            0x38  /** Arbitration lost in SLA+R or data */
#define MR_SLA_R_ACK           0x40  /** SLA+R transmitted, ACK received */
#define MR_SLA_R_NACK          0x48  /** SLA+R transmitted, NACK received */
#define MR_DATA_ACK            0x50  /** Data received, ACK returned */
#define MR_DATA_NACK           0x58  /** Data received, NACK returned */

/******************************** Slave Receiver Mode ********************************/
#define SR_SLA_W_ACK           0x60  /** SLA+W received, ACK returned */
#define SR_ARB_LOST_SLA_W_ACK  0x68  /** Arbitration lost, SLA+W received */
#define SR_GCALL_ACK           0x70  /** General call received, ACK returned */
#define SR_ARB_LOST_GCALL_ACK  0x78  /** Arbitration lost, general call received */
#define SR_DATA_ACK            0x80  /** Data received after SLA+W, ACK returned */
#define SR_DATA_NACK           0x88  /** Data received, NACK returned */
#define SR_GCALL_DATA_ACK      0x90  /** Data after general call, ACK returned */
#define SR_GCALL_DATA_NACK     0x98  /** Data after general call, NACK returned */
#define SR_STOP_OR_REP_START   0xA0  /** STOP or repeated START received while addressed as slave */

/******************************** Slave Transmitter Mode ********************************/
#define ST_SLA_R_ACK           0xA8  /** SLA+R received, ACK returned */
#define ST_ARB_LOST_SLA_R_ACK  0xB0  /** Arbitration lost, SLA+R received */
#define ST_DATA_ACK            0xB8  /** Data transmitted, ACK received */
#define ST_DATA_NACK           0xC0  /** Data transmitted, NACK received */
#define ST_LAST_DATA_ACK       0xC8  /** Last data byte transmitted, ACK received */

/******************************** Miscellaneous ********************************/
#define NO_INFO                0xF8  /** No relevant state information */
#define BUS_ERROR              0x00  /** Illegal START/STOP condition */




/************************************************************************/
/*                        FUNCTIONS PROTOTYPES                          */
/************************************************************************/

/**
 * brief : Initializes the I2C (TWI) peripheral as master.
 * 
 * Configures the TWI bitrate and enables the TWI module.
 * Assumes master mode with default prescaler and address (if any).
 */
void I2C_vInit(u32 SCL_Frequency);

/**
 * brief : Sends a START condition on the I2C bus.
 * 
 * Waits for the TWINT flag to be set indicating the START condition has been transmitted.
 * Used to begin communication with a slave.
 */
void I2C_vStart();

/**
 * brief : Sends a STOP condition on the I2C bus.
 * 
 * Terminates the ongoing communication on the I2C bus.
 * Releases the bus so other devices can communicate.
 */
void I2C_vStop();

/**
 * brief : Transmits a byte of data over the I2C bus.
 * 
 * param : data The byte to be transmitted to the slave.
 * Loads the byte into TWDR, clears TWINT, and waits for transmission to complete.
 */
void I2C_writeByte(u8 data);

/**
 * brief : Reads a byte from the I2C bus and sends ACK after reception.
 * 
 * return : u8 The received byte from the slave.
 * Sends an ACK to allow continued reading of more bytes.
 */
u8 I2C_ReadByte_ACK();

/**
 * brief : Reads a byte from the I2C bus and sends NACK after reception.
 * 
 * return : u8 The received byte from the slave.
 * Sends a NACK indicating the master will not read more data.
 */
u8 I2C_ReadByte_NACK();



/************************************************************************/
/*                        FUNCTIONS LIKE MACRO                          */
/************************************************************************/

/*  I2C_ENABLE()  */
#define I2C_ENABLE() ( TWCR = (1 << TWEN) )

#define I2C_START_CONDITION()  ( TWCR = (1 << TWSTA) )

#define I2C_CLEAR_TWINT_FLAG() ( TWCR = (1 << TWINT) )



#endif /* I2C_H */
