# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

# make all
# reinstall the library after each recompilation
all: spod

spod: spod.cpp
	g++ -Wall -lrf24-bcm -I../include ${CCFLAGS} -o spod $^

# clear build files
clean:
	rm -rf *.o spod

install: all
	install -m 0755 spod /usr/local/bin
	install -m 0755 script/spod-put-cosm-feed /usr/local/bin
