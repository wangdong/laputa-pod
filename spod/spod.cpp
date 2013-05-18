#include <unistd.h>
#include <librf24-bcm/RF24.h>
#include <laputa.h>

#define LA_CONF_SCAN_TIMEOUT   (5 * 60) /* secs */
#define LA_CONF_SCAN_CYCLE     30      /* secs */


RF24 radio(RPI_V2_GPIO_P1_26, RPI_V2_GPIO_P1_15, BCM2835_SPI_SPEED_8MHZ);


inline 
void setup() {
	radio.begin();
	radio.setDataRate(RF24_250KBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setPayloadSize(LA_CONF_PAYLOAD);
	radio.setChannel(LA_CONF_CHANNEL);
	radio.openReadingPipe(0, la_conf_to_addr(LA_CONF_ADDR_LAPUTA));

	radio.startListening();
}

inline 
void loop() {
	int time = 0;
	while (time < LA_CONF_SCAN_TIMEOUT) {

		if (radio.available()) {

			LaProto::datagram().receive(radio);

			const LaDATAGRAM& body = LaProto::datagram().body;

			float data[LA_CONF_DATA_COUNT];
			memcpy((char*)&data, body.cont, LA_CONF_DATA_COUNT * sizeof(float));

			printf("%s,%f,%f,%f", 
				la_addr_to_conf(body.sender), 
				data[LA_CONF_DATA_TEMP], 
				data[LA_CONF_DATA_HUM], 
				data[LA_CONF_DATA_DEW]);

			break;
		}
		
		time += LA_CONF_SCAN_CYCLE;
		sleep(LA_CONF_SCAN_CYCLE);
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
