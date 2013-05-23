#include <avr/wdt.h>

#include <LowPower.h>
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <AM2321.h>
#include <Weather.h>

#include <laputa.h>


//
// 本传感器的地址
//
#define LA_CONF_SELF_ADDR LA_CONF_ADDR_POD0

//
// 外围器件
//
PCF8563   rtc;
AM2321    ac;
RF24      radio(LA_CONF_PIN_CE, LA_CONF_PIN_CSN);
LaResetor resetor(LA_CONF_RESET_CYCLE);


void setup(void)
{
    Serial.begin(LA_CONF_BAUD);

    //
    // RTC & Interruption
    //
    attachInterrupt(0, interrupt_timeup, FALLING);
    rtc.initClock();

    //
    // Radio
    //
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.setPayloadSize(LA_CONF_PAYLOAD);
    radio.setChannel(LA_CONF_CHANNEL);
    radio.openWritingPipe(la_conf_to_addr(LA_CONF_ADDR_LAPUTA));

    radio.startListening();

    //
    // Debug dump
    //
    printf_begin();
    radio.printDetails();

    radio.powerDown();
}

void loop(void) {
    rtc.setTimer(LA_CONF_REPORT_CYCLE);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);
    rtc.clearTimer();
    report();
    resetor.wokenup();
}

void report() {
    ac.read();
    if (ac.available()) {
        float data[LA_CONF_DATA_COUNT];
        data[LA_CONF_DATA_HUM] = ac.humidity/10.0;
        data[LA_CONF_DATA_TEMP] = ac.temperature/10.0;
        data[LA_CONF_DATA_DEW] = weather::dewPoint(data[LA_CONF_DATA_TEMP], data[LA_CONF_DATA_HUM]);

        // Serial.println(data[LA_CONF_DATA_TEMP]);
        // Serial.println(data[LA_CONF_DATA_HUM]);
        // Serial.println(data[LA_CONF_DATA_DEW]);

        radio.powerUp();
        LaProto::datagram()
            .withContent((const uint8_t*)data, LA_CONF_DATA_SIZE)
            .sendFrom(LA_CONF_SELF_ADDR)
            .send(radio);
        radio.powerDown();
    }
}

void interrupt_timeup() {
}


/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
 
/**
 * @file printf.h
 *
 * Setup necessary to direct stdout to the Arduino Serial library, which
 * enables 'printf'
 */

#ifndef __PRINTF_H__
#define __PRINTF_H__

#ifdef ARDUINO

int serial_putc( char c, FILE * ) 
{
  Serial.write( c );

  return c;
} 

void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

#else
#error This example is only for use on Arduino.
#endif // ARDUINO

#endif // __PRINTF_H__

//
// END OF FILE
//
