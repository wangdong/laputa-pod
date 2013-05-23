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

#define LA_CONF_SCAN_CYCLE      1       /* secs */


class MQ {
	redisContext* mq;
public:
	MQ() {
		mq = NULL;
	}
public:
	void send(const char* listName, const char* nodeName, float temp, float hum, float dewpoint) {
		time_t now;
		time(&now);
		if (connect()) {
		    redisCommand(mq, "RPUSH %s %ld,%s,%.2f,%.2f,%.2f", 
		    	listName, now, nodeName, temp, hum, dewpoint);
		}
	}
private:
	bool connect() {
		if (mq == NULL) {
			mq = redisConnect("127.0.0.1", 6379);
			if (mq != NULL && mq->err)
	    		printf("Error: %s\n", mq->errstr);
	    }
	    return mq != NULL;
	}
};

void termsig_handler(int signo) 
{ 
	if(signo == SIGTERM) 
	/* catched signal sent by kill(1) command */ 
  	{ 
		syslog(LOG_INFO, "terminated"); 
		closelog(); 
		exit(0); 
  	}
}

int fork_daemon(void) 
{
	pid_t pid; 
	if((pid = fork()) < 0) 
  		return -1;
	else if(pid != 0) 
  		exit(0); /* parent exit */ 

	openlog("spod", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "started"); 

	/* child continues */ 
	setsid(); /* become session leader */ 
	umask(0); /* clear file mode creation mask */ 
	close(0);
	close(1);
	close(2);

	signal(SIGTERM, termsig_handler);

	return 0;
}


RF24 radio(RPI_V2_GPIO_P1_26, RPI_V2_GPIO_P1_15, BCM2835_SPI_SPEED_8MHZ);
MQ mq;


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
	while (true) {
		if (radio.available()) {
			LaProto::datagram().receive(radio);

			const LaDATAGRAM& body = LaProto::datagram().body;

			float data[LA_CONF_DATA_COUNT];
			memcpy((char*)&data, body.cont, LA_CONF_DATA_COUNT * sizeof(float));
			
			char buf[256] = {0};
			sprintf(buf, "%s,%f,%f,%f", 
				la_addr_to_conf(body.sender), 
				data[LA_CONF_DATA_TEMP], 
				data[LA_CONF_DATA_HUM], 
				data[LA_CONF_DATA_DEW]);
			syslog(LOG_INFO, buf);

			mq.send(LA_CONF_QUEUE_NAME, 
				la_addr_to_conf(body.sender), 
				data[LA_CONF_DATA_TEMP], 
				data[LA_CONF_DATA_HUM], 
				data[LA_CONF_DATA_DEW]);
		}
		sleep(LA_CONF_SCAN_CYCLE);
		fputc('.', stderr);
	}
}


int main(int argc, char** argv) {
	setup();
	if (argc > 1 && strcmp(argv[1], "-i") == 0) {
		radio.printDetails();
		return 0;
	}
	if (argc > 1 && strcmp(argv[1], "-d") == 0) {
		if (fork_daemon() != 0)
			return 0;
	}

	loop();

	return 0;
}
