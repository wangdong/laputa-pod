#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <AM2321.h>
#include <weather.h>
#include <LowPower.h>
#include <laputa.h>


PCF8563 rtc;
AM2321  ac;
RF24    radio(LA_CONF_PIN_CE, LA_CONF_PIN_CSN);

volatile 
bool interrupt = false;


void setup(void)
{
    Serial.begin(LA_CONF_BAUD);
    Wire.begin();

    //
    // RTC
    //
    attachInterrupt(0, interrupt_timeup, FALLING);
    rtc.initClock();

    //
    // Radio
    //
    radio.begin();
    radio.setDataRate(RF24_2MBPS);
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
    delay(200);
}

void interrupt_timeup() {
}


void loop(void) {
    rtc.setTimer(TIMER_EVERY_MINUTE, 3);
    probeAndSend();
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}


void probeAndSend() {
    ac.read();
    if (ac.available()) {
        float data[LA_CONF_DATA_COUNT];
        data[LA_CONF_DATA_TEMP] = ac.temperature/10.0;
        data[LA_CONF_DATA_HUM]  = ac.humidity/10.0;
        data[LA_CONF_DATA_DEW]  = weather::dewPoint(ac.temperature/10.0, ac.humidity/10.0);

        Serial.println(data[LA_CONF_DATA_TEMP]);
        Serial.println(data[LA_CONF_DATA_HUM]);
        Serial.println(data[LA_CONF_DATA_DEW]);

        radio.powerUp();
        LaProto::datagram()
            .withContent((const uint8_t*)data, LA_CONF_DATA_SIZE)
            .sendFrom(LA_CONF_ADDR_POD2)
        .write(radio);
        radio.powerDown();
    }
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

