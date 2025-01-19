/**
* libnet
* 
* Purpose: high-loaded reliable and unreliable network data transmittion
* Author: Deryabin K.
* Date: 30.12.24
* Version: 0.1
*/
#pragma once
#include <string.h>
#include <stdint.h>
#include <cassert>
#include <typeinfo>
#include "smalldelegate.h"

#define LIBNET_DEFAULT (-1)

typedef void* (*pfn_memalloc_func)(size_t count, size_t size, int flags);
typedef void  (*pfn_memfree_func)(void* p_ptr);

typedef uint16_t net_slot_t;

/**
* init struct
*/
struct libnet_init {
	pfn_memalloc_func p_memallocfn;
	pfn_memfree_func p_memfreefn;
};

/**
* libnet operations status
*/
enum LIBNET_STATUS {
	LIBNET_STATUS_OK = 0,
	LIBNET_STATUS_INVALID_PARAMETER,
	LIBNET_STATUS_OUT_OF_MEMORY,
};

/**
* network address struct
*/
class net_address
{
public:
	enum NA_TYPE {
		NA_NONE = 0,
		NA_IP,
		NA_BROADCAST,
		NA_LOOPBACK,
	};

private:
	NA_TYPE        m_type;
	char           m_addr[32];
	unsigned short m_port;
public:
	net_address() : m_type(NA_NONE) {
		memset(m_addr, 0, sizeof(m_addr));
		m_port = 0;
	}
};

/**
* net channel properties
*/
struct channel_props_s {
	size_t unk;
};

/**
* net channel data
*/
class channel_data {
protected:
	net_address m_address;
	float       m_avg_bytes_per_sec;
	void       *m_vuserdata[8];
public:
	channel_data() : m_avg_bytes_per_sec(0.f) {
		memset(m_vuserdata, 0, sizeof(m_vuserdata));
	}
	inline void* get_userdata(int idx) { return m_vuserdata[idx]; }
	inline void  set_userdata(int idx, void* p_userdata) { m_vuserdata[idx] = p_userdata; }
};

/* base tag-class for netmessage handler class */
class netmessage_handler
{
public:
	netmessage_handler() {};
};

/* decl netmessage delegate */
class cnetworkmessagebase;
using _netmsg_delegate = small_sc_delegate<bool, netmessage_handler, cnetworkmessagebase*>;

/**
* network messages interface
*/
class inetworkmessage
{
public:
	/* retreives network message id */
	virtual int  get_id() = 0;
	/* write message to buffer */
	virtual bool write(void *wrbuffer) = 0;
	/* read message from buffer */
	virtual bool read(void* rdbuffer) = 0;
	/* execute message handler */
	virtual bool handle(_netmsg_delegate &delegate) = 0;
	/* string message name */
	virtual const char* name() = 0;
	/* THREAD UNSAFE! convert readed netmessage to string info message */
	virtual const char *string_info(char *p_dst, size_t dstlen) = 0;
};

/* base network message */
class cnetworkmessagebase : public inetworkmessage {
	bool m_breliable;
public:
	cnetworkmessagebase() : m_breliable(true) {}
	inline bool is_reliable() { return m_breliable; }
	virtual bool handle(_netmsg_delegate &delegate) {
		assert(delegate.is_valid() && "net message delegate is not valid!");
		return delegate(this);
	}
};

template<int id, class _derived_class>
class cnetmessage_helper
{
public:
	cnetmessage_helper() = default;
	/* retreives network message id */
	virtual int get_id() { return id; }
	/* string message name */
	virtual const char* name() { return typeid(_derived_class).name(); }
};

/**
* example netmessage
*/
class netmsg_example : public cnetworkmessagebase, public cnetmessage_helper<0, netmsg_example>
{
	int m_field1;
public:
	netmsg_example() : m_field1(0) {}

};

/**
* net channel iface
*/
class ichannel : public channel_data
{
public:
	/* channel information */
	inline const channel_data *get_channel_info() { return this; }
	virtual void set_properties(channel_props_s& p_src) = 0;
	virtual void set_properties(const channel_props_s &p_src) = 0;

	/* network messages */
	virtual bool register_message(cnetworkmessagebase *p_netmsg_instance, const _netmsg_delegate &handler) = 0;
	virtual bool unregister_message(cnetworkmessagebase* p_netmsg_instance, const _netmsg_delegate& handler) = 0;

	/* send raw data */
	virtual bool send(void *p_data, uint32_t size) = 0;
	/* send buffer */
	virtual bool send(void *wrbuffer) = 0;
	/* send network message */
	virtual bool send(inetworkmessage *p_netmsg) = 0;
};

class ilibnet
{
public:
	virtual LIBNET_STATUS initialize(const libnet_init &init) = 0;
	virtual LIBNET_STATUS deinitialize() = 0;

	/* server */

	/* client */
};
