#ifndef __AM2312_H__
#define __AM2312_H__

#include <stdint.h>
#include <stdbool.h>

#define I2C_ADDR_AM2321                 (0xB8 >> 1)          //AM2321温湿度计I2C地址
#define PARAM_AM2321_READ                0x03                //读寄存器命令
#define REG_AM2321_HUMIDITY_MSB          0x00                //湿度寄存器高位
#define REG_AM2321_HUMIDITY_LSB          0x01                //湿度寄存器低位
#define REG_AM2321_TEMPERATURE_MSB       0x02                //温度寄存器高位
#define REG_AM2321_TEMPERATURE_LSB       0x03                //温度寄存器低位
#define REG_AM2321_DEVICE_TYPE   		 0x08                //16位设备型号高8位
#define REG_AM2321_DEVICE_ID_BIT_24_31   0x0B                //32位设备ID高8位


typedef struct tagACInfo {
	bool     isOK;
	int16_t  temperature;
	uint16_t humidity;
	uint16_t crc;
	uint16_t crcCalc;
} am2312_ACInfo;

am2312_ACInfo am2312_read_ac();

#endif
