#ifndef __LAPUTA_PROTO__
#define __LAPUTA_PROTO__

typedef struct {

	enum { 
		LA_MAX_CONTENT = LA_CONF_PAYLOAD - sizeof(uint64_t) - sizeof(uint8_t)
	};

	uint64_t 	sender;
	uint8_t  	len;
	uint8_t 	content[LA_MAX_CONTENT];

} __attribute__((__packed__)) LaDATAGRAM;


class LaProto
{
public:
	LaDATAGRAM datagram;

public:
	//
	// Write
	//
	static
	LaProto& beginWrite() {
		return inst();
	}
	LaProto& sendFrom(const char* sender) {
		inst().datagram.sender = la_conf_to_addr(sender);
		return inst();
	}
	LaProto& withContent(const uint8_t* content, int len) {
		inst().datagram.len = len > LaDATAGRAM::LA_MAX_CONTENT ? LaDATAGRAM::LA_MAX_CONTENT : len;
		memcpy(inst().datagram.content, content, inst().datagram.len);
		return inst();
	}

	template<typename CHAN> 
	bool write(CHAN& chan) {
		return chan.write(&datagram, LA_CONF_PAYLOAD);
	}

public:
	//
	// Read
	//
	template<typename CHAN> 
	bool read(CHAN& chan) {
		return chan.read(&datagram, LA_CONF_PAYLOAD);
	}

private:
	LaProto() {
		memset(&datagram, 0, sizeof(LaDATAGRAM));
	}
	static
	LaProto& inst() {
		static LaProto inst;
		return inst;
	}
};


#endif
