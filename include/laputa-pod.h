#ifndef __LAPUTA_POD__
#define __LAPUTA_POD__


//
// Network
//
#define LA_CONF_ADDR_LAPUTA	"QS00N000"

#define LA_CONF_ADDR_POD0	"QC00N000"
#define LA_CONF_ADDR_POD1	"QC00N001"
#define LA_CONF_ADDR_POD2	"QC00N002"
#define LA_CONF_ADDR_POD3	"QC00N003"
#define LA_CONF_ADDR_POD4	"QC00N004"
#define LA_CONF_ADDR_POD5	"QC00N005"
#define LA_CONF_ADDR_POD6	"QC00N006"
#define LA_CONF_ADDR_POD7	"QC00N007"

inline
uint64_t la_conf_to_addr(const char* addr) {
	return *(uint64_t*)addr;
}


//
// AC Data Pack
//
#define LA_CONF_DATA_COUNT	3 /* 1:temp, 2:hum, 3:dew */
#define LA_CONF_DATA_SIZE	(LA_CONF_DATA_COUNT * sizeof(float))
#define LA_CONF_DATA_TEMP	0
#define LA_CONF_DATA_HUM	1
#define LA_CONF_DATA_DEW	2
#define LA_CONF_PROBE_DELAY	5000


//
// DHT11
//
#define LA_CONF_PIN_DHT11		4


//
// nRF24L01
//
#define LA_CONF_PIN_CE  		8
#define LA_CONF_PIN_CSN 		7
#define LA_CONF_CHANNEL 		10
#define LA_CONF_PAYLOAD 		LA_CONF_DATA_SIZE


//
// Serial
//
// AC Data Pack
//
//
#define LA_CONF_BAUD    		9600


#endif