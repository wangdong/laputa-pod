#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT11.h>
#include <laputa-pod.h>


dht11 DHT11;
RF24 radio(LA_CONF_PIN_CE, LA_CONF_PIN_CSN);

void setup(void)
{
	Serial.begin(LA_CONF_BAUD);

	radio.begin();
	radio.setDataRate(RF24_2MBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setPayloadSize(LA_CONF_PAYLOAD);
	radio.setChannel(LA_CONF_CHANNEL);
	radio.openWritingPipe(la_conf_to_addr(LA_CONF_ADDR_LAPUTA));

	radio.startListening();

	printf_begin();
	radio.printDetails();
}



void loop(void) {
	radio.powerUp();
	probeAndSendByDHT11();
	radio.powerDown();
	delay(LA_CONF_PROBE_DELAY);
}


bool probeAndSendByDHT11() {
	if (DHT11.read(LA_CONF_PIN_DHT11) == DHTLIB_OK) {
		float data[LA_CONF_DATA_COUNT];
		data[LA_CONF_DATA_TEMP] = (double)DHT11.temperature;
		data[LA_CONF_DATA_HUM]  = (double)DHT11.humidity;
		data[LA_CONF_DATA_DEW]  = (double)helper_dewPoint(DHT11.temperature, DHT11.humidity);

		radio.write((byte*)data, LA_CONF_DATA_SIZE);

		Serial.print("TEMP: ");
		Serial.println(data[LA_CONF_DATA_TEMP]);

		return true;
	}
	return false;
}


//Celsius to Fahrenheit conversion
double helper_Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

// fast integer version with rounding
//int Celcius2Fahrenheit(int celcius)
//{
//  return (celsius * 18 + 5)/10 + 32;
//}


//Celsius to Kelvin conversion
double helper_Kelvin(double celsius)
{
	return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double helper_dewPoint(double celsius, double humidity)
{
	// RATIO was originally named A0, possibly confusing in Arduino context
	double RATIO = 373.15 / (273.15 + celsius);
	double SUM = -7.90298 * (RATIO - 1);
	SUM += 5.02808 * log10(RATIO);
	SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
	SUM += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM - 3) * humidity;
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double helper_dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity/100);
	double Td = (b * temp) / (a - temp);
	return Td;
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

