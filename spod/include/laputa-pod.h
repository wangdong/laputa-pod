#ifndef __LAPUTA_POD__
#define __LAPUTA_POD__


#define CONF_DATA_COUNT		3 /* 1:temp, 2:hum, 3:dew */
#define CONF_DATA_SIZE		(CONF_DATA_COUNT * sizeof(float))
#define CONF_DATA_TEMP		0
#define CONF_DATA_HUM		1
#define CONF_DATA_DEW		2
#define CONF_PROBE_DELAY	5000

#define CONF_ADDR_CENTER	(0xF0F0F0F0E1LL)

#define CONF_ADDR_POD0		"QC00N000"
#define CONF_ADDR_POD1		"QNC00002"
#define CONF_ADDR_POD2		"QNC00003"
#define CONF_ADDR_POD3		"QNC00004"
#define CONF_ADDR_POD4		"QNC00005"
#define CONF_ADDR_POD5		"QNC00006"
#define CONF_ADDR_POD6		"QNC00007"
#define CONF_PIN_DHT11		4

#define CONF_PIN_CE  		8
#define CONF_PIN_CSN 		7
#define CONF_CHANNEL 		10
#define CONF_PAYLOAD 		CONF_DATA_SIZE
#define CONF_BAUD    		9600


#endif