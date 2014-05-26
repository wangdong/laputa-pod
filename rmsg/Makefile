# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

# make all
# reinstall the library after each recompilation
all: rmsg 

rmsg: rmsg.cpp
	g++ -Wall -lrf24-bcm -lhiredis -I../include -I../vendor/libxively/src -I../vendor/hiredis ${CCFLAGS} -o rmsg $^

# clear build files
clean:
	rm -rf *.o rmsg 

install: all
	install -m 0755 rmsg /usr/local/bin
