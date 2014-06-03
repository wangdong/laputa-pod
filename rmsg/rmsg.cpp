#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <librf24-bcm/RF24.h>
#include <laputa.h>
#include <hiredis.h>
#include <string>
#include <iostream>

struct nRF24Config
{
	uint32_t CSN;
	uint32_t CE;	
};

enum RADIO {
	nRF24D00,
	nRF24D01,
	nRF24MAX,
};

const nRF24Config radioArray[nRF24MAX] = { 
	{RPI_V2_GPIO_P1_16, RPI_V2_GPIO_P1_24}, 
	{RPI_V2_GPIO_P1_18, RPI_V2_GPIO_P1_26},
};

RF24 radio(
    radioArray[nRF24D01].CSN, 
    radioArray[nRF24D01].CE, 
    BCM2835_SPI_SPEED_8MHZ
    );

inline 
void setup() {
    radio.begin();
    radio.setDataRate(RF24_2MBPS);
    radio.setChannel(24);
    radio.enableDynamicPayloads();
    radio.setRetries(8, 5);
    radio.openWritingPipe(0x00feedf00d12ull);
    radio.startListening();
}

inline 
void send(std::string msg) {
    radio.stopListening();
    if (radio.write(msg.c_str(), msg.size()))
    	puts("sent");
    else
    	puts("failed");
    radio.startListening();
}

int main(int argc, char** argv) {
    if (argc == 1)
        return 0;
        
    setup();
    if (argc > 1 && strcmp(argv[1], "-i") == 0)
        radio.printDetails();
    else
	    send(std::string(argv[1]));

    return 0;
}
