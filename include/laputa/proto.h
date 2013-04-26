#ifndef __LAPUTA_PROTO__
#define __LAPUTA_PROTO__

typedef struct {

	enum { 
		LA_MAX_CONTENT = LA_CONF_PAYLOAD - sizeof(uint64_t) - sizeof(uint8_t)
	};

	uint64_t 	sender;
	uint8_t  	len;
	uint8_t 	cont[LA_MAX_CONTENT];

} __attribute__((__packed__)) LaDATAGRAM;


class LaProto
{
public:
	LaDATAGRAM body;

public:
	static
	LaProto& datagram() {
		return inst();
	}

public:
	//
	// Write
	//
	LaProto& sendFrom(const char* sender) {
		inst().body.sender = la_conf_to_addr(sender);
		return inst();
	}
	LaProto& withContent(const uint8_t* content, int len) {
		inst().body.len = len > LaDATAGRAM::LA_MAX_CONTENT ? LaDATAGRAM::LA_MAX_CONTENT : len;
		memcpy(inst().body.cont, content, inst().body.len);
		return inst();
	}

	template<typename CHAN> 
	bool write(CHAN& chan) {
		return chan.write(&body, LA_CONF_PAYLOAD);
	}

public:
	//
	// Read
	//
	template<typename CHAN> 
	bool read(CHAN& chan) {
		return chan.read(&body, LA_CONF_PAYLOAD);
	}
	

private:
	static
	LaProto& inst() {
		static LaProto inst;
		return inst;
	}
};


#endif
