#ifndef __LAPUTA_BASIC__
#define __LAPUTA_BASIC__


//
// Network
//
#define LA_CONF_ADDR_LEN    sizeof(uint64_t)

#define LA_CONF_ADDR_LAPUTA "QS00N000"

#define LA_CONF_ADDR_POD0   "QC00N000"
#define LA_CONF_ADDR_POD1   "QC00N001"
#define LA_CONF_ADDR_POD2   "QC00N002"
#define LA_CONF_ADDR_POD3   "QC00N003"
#define LA_CONF_ADDR_POD4   "QC00N004"
#define LA_CONF_ADDR_POD5   "QC00N005"
#define LA_CONF_ADDR_POD6   "QC00N006"
#define LA_CONF_ADDR_POD7   "QC00N007"

inline
uint64_t la_conf_to_addr(const char* addr) {
    return *(uint64_t*)addr;
}

inline
const char* la_addr_to_conf(uint64_t addr) {
    static char conf[LA_CONF_ADDR_LEN + 1];
    memcpy(conf, &addr, LA_CONF_ADDR_LEN);
    conf[LA_CONF_ADDR_LEN] = 0;
    return conf;
}

//
// Queue
//
#define LA_CONF_QUEUE_NAME LA_CONF_ADDR_LAPUTA

//
// AC Data Pack
//
#define LA_CONF_DATA_COUNT      3 /* 1:temp, 2:hum, 3:dew */
#define LA_CONF_DATA_SIZE       (LA_CONF_DATA_COUNT * sizeof(float))
#define LA_CONF_DATA_TEMP       0
#define LA_CONF_DATA_HUM        1
#define LA_CONF_DATA_DEW        2


//
// nRF24L01
//
#define LA_CONF_PIN_CE          7
#define LA_CONF_PIN_CSN         8
#define LA_CONF_CHANNEL         10
#define LA_CONF_PAYLOAD         32


//
// Serial
//
#define LA_CONF_BAUD            9600


//
// 传感器采样周期
//
#define LA_CONF_REPORT_CYCLE_MINS 2 /*mins*/
#define LA_CONF_REPORT_CYCLE      TIMER_EVERY_MINUTE, LA_CONF_REPORT_CYCLE_MINS /*mins*/

#endif

