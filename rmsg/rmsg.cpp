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


RF24 radio(RPI_V2_GPIO_P1_18, RPI_V2_GPIO_P1_26, BCM2835_SPI_SPEED_8MHZ);
std::string server_addr = "L1N00";

inline 
void setup() {
    radio.begin();
    radio.setDataRate(RF24_2MBPS);
    radio.enableDynamicPayloads();
    radio.setAutoAck(true);
    radio.setChannel(24);
    radio.setRetries(15, 15);
    radio.openWritingPipe((uint64_t)*server_addr.c_str());
    radio.startListening();
    radio.stopListening();
}

inline 
void send(std::string msg) {
    if (radio.write(msg.c_str(), msg.size()))
    	puts("sent");
    else
    	puts("failed");
}

int main(int argc, char** argv) {
    if (argc == 1)
        return 0;
        
    setup();
    if (argc > 1 && strcmp(argv[1], "-i") == 0) {
        radio.printDetails();
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "-s") == 0) {
		server_addr = argv[2];
	    send(std::string(argv[3]));
    }
    else
	    send(std::string(argv[1]));

    return 0;
}
