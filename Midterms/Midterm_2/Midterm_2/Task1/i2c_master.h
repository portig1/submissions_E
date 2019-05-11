#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#define I2C_READ 0x01
#define I2C_WRITE 0x00

#define TWBR TWBR0 //_SFR_MEM8(0xB8)
#define TWSR TWSR0 //_SFR_MEM8(0xB9)
#define TWAR TWAR0 //_SFR_MEM8(0xBA)
#define TWDR TWDR0 //_SFR_MEM8(0xBB)
#define TWCR TWCR0 //_SFR_MEM8(0xBC)
#define TWAMR TWAMR0 //_SFR_MEM8(0xBD)
#define TWI_vect_num TWI0_vect_num //24
#define TWI_vect TWI0_vect //_VECTOR(24)

void i2c_init(void);
uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
void i2c_stop(void);

#endif // I2C_MASTER_H
