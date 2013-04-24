#include <unistd.h>
#include <librf24-bcm/RF24.h>
#include <laputa-pod.h>

#define CONF_PROBE_TIMEOUT   (5 * 60) /* secs */
#define CONF_PROBE_CYCLE      1       /* secs */


RF24 radio(RPI_V2_GPIO_P1_26, RPI_V2_GPIO_P1_15, BCM2835_SPI_SPEED_8MHZ);


inline 
void setup() {
	radio.begin();
	radio.setDataRate(RF24_2MBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setPayloadSize(CONF_PAYLOAD);
	radio.setChannel(CONF_CHANNEL);
	radio.openReadingPipe(0, CONF_ADDR_CENTER);

	radio.startListening();
}

inline 
void loop() {
	int time = 0;
	while (time < CONF_PROBE_TIMEOUT) {

		if (radio.available()) {
			float data[CONF_DATA_COUNT];
			radio.read((char*)&data, CONF_DATA_SIZE);

			printf("/%s?temp=%f&hum=%f", CONF_ADDR_POD0, data[CONF_DATA_TEMP], data[CONF_DATA_HUM]);

			break;
		}
		
		time += CONF_PROBE_CYCLE;
		sleep(CONF_PROBE_CYCLE);
		fputc('.', stderr);
	}
}


int main(int argc, char** argv) {
	setup();
	if (argc > 1 && strcmp(argv[1], "-d") == 0) {
		radio.printDetails();
		return 0;
	}

	loop();

	return 0;
}
