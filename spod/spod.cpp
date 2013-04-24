#include <cstdlib>
#include <iostream>
#include <librf24-bcm/RF24.h>
#include <laputa-pod.h>


RF24 radio(RPI_V2_GPIO_P1_26, RPI_V2_GPIO_P1_15, BCM2835_SPI_SPEED_8MHZ);

inline
void setup() {
	radio.begin();
	radio.setDataRate(RF24_2MBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setPayloadSize(CONF_PAYLOAD);
	radio.setChannel(CONF_CHANNEL);
	radio.openWritingPipe(CONF_ADDR_CENTER);

	radio.startListening();

	radio.printDetails();
}

inline
void probe() {
	for (;;) {
		if (radio.available()) {
			float data[CONF_DATA_COUNT];
			radio.read((char*)&data, CONF_DATA_SIZE);
			printf("/%s?temp=%f&hum=%f", CONF_ADDR_POD0, data[CONF_DATA_TEMP], data[CONF_DATA_HUM]);
			break;
		}
	}
}

int main(int argc, char** argv) {
	setup();
	probe();

	return 0;
}
