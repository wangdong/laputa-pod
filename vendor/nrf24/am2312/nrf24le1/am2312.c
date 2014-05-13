#include "am2312.h"

#include <string.h>
#include <nrf24le1.h>
#include <nordic_common.h>
#include <hal_delay.h>
#include <hal_w2.h>


//
// 这些神秘的代码来自nRFGo SDK的hal_w2.c
//   - 可以注释其中用GPIO模拟产生一个8bit的部分，经验证，
//     这样做，在保持总线时序正常的情况下，也能完成重置。这里保留先。
//   - 至于为何搞这样一通就能reset，W2CON0要反复
//     在0x03, 0x07间变来变去，原因不明，
//     但不这样，的确不能重置。
//
static
void hal_w2_soft_reset()
{
#ifndef W2_SOFT_RESET_NOT_AVAILABLE
  uint8_t pulsecount, w2_freq;

  // Store the selected 2-wire frequency 
  w2_freq = W2CON0 & 0x0C;
  // Prepare the GPIO's to take over SDA & SCL
  HAL_W2_CLEAR_SDA_SCL;
  HAL_W2_OVERRIDE_SDA_SCL(1, 1);
  //P0DIR = 0xFF;
  
  // Reset 2-wire. SCL goes high.
  W2CON0 = 0x03;
  W2CON0 = 0x07;
  
  // Disable 2-wire.
  W2CON0 = 0x06;

  // SDA and SCL are now under software control, and both are high. 
  // Complete first SCL pulse.
  //P0DIR = 0xEF;
  HAL_W2_OVERRIDE_SDA_SCL(1, 0);
  
  // SCL low
  delay_us(5);
  //P0DIR = 0xCF;
  HAL_W2_OVERRIDE_SDA_SCL(0, 0);
  
  // SDA low
  // Create SCL pulses for 7 more data bits and ACK/NACK
  delay_us(5);
  for( pulsecount = 0; pulsecount < 8; pulsecount++ )
  {
    //P0DIR = 0xDF;
    HAL_W2_OVERRIDE_SDA_SCL(0, 1);
    delay_us(5);
    //P0DIR = 0xCF;
    HAL_W2_OVERRIDE_SDA_SCL(0, 0);
    delay_us(5);
  }
  
  // Generating stop condition by driving SCL high
  delay_us(5);
  //P0DIR = 0xDF;
  HAL_W2_OVERRIDE_SDA_SCL(0, 1);
  
  // Drive SDA high
  delay_us(5);
  //P0DIR = 0xFF;
  HAL_W2_OVERRIDE_SDA_SCL(1, 1);

  // Work-around done. Return control to 2-wire.
  W2CON0 = 0x07;
  
  // Reset 2-wire and return to master mode at the frequency selected before calling this function
  W2CON0 = 0x03;
  W2CON0 = 0x03 | w2_freq;
#endif
}


static 
uint16_t helper_crc16(unsigned char *ptr, unsigned char len) {
	uint16_t crc = 0xFFFF; 
	uint8_t  i   = 0;
	while(len--) {
		crc ^= *ptr++; 
		for(i = 0 ; i < 8 ; i++) {
			if(crc & 0x01) {
				crc >>= 1;
				crc  ^= 0xA001; 
			}
			else {
				crc >>= 1;
			} 
		}
	}
	return crc; 
}

static 
void helper_read(uint8_t* buff, uint8_t address, uint8_t firstReg, uint8_t regCount) {
	// wakeup
	hal_w2_write(address, 0, 0);

	// command
	buff[0] = PARAM_AM2321_READ;
	buff[1] = firstReg;
	buff[2] = regCount;
	if (hal_w2_write(address, buff, 3)) {
		// read
		delay_us(1600);
		hal_w2_read(address, buff, 2 + regCount + 2); // cmd + reg Array + crc
		hal_w2_soft_reset();
	}
}

am2312_ACInfo am2312_read_ac() {
	enum { len = 32 };
	
	uint8_t buff[len];
	uint16_t crc;
	am2312_ACInfo info;

	memset(&buff, 0, len);
	memset(&info, 0, sizeof(am2312_ACInfo));

	helper_read(buff, I2C_ADDR_AM2321, REG_AM2321_HUMIDITY_MSB, 4);

	info.humidity     = buff[2] << 8;
	info.humidity    += buff[3];

	info.temperature  = buff[4] << 8;
	info.temperature += buff[5];

	crc  = buff[6] ;
	crc |= buff[7] << 8;

	info.crc = crc;
	info.crcCalc = helper_crc16(buff, 6);
	info.isOK = (info.crc == info.crcCalc);

	return info;
}

