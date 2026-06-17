#pragma once

#include <stdint.h>

// https://elixir.bootlin.com/linux/v6.11.2/source/include/uapi/linux/icmpv6.h
struct icmpv6_hdr {
	uint8_t type;       // message type
	uint8_t code;       // message code
	uint16_t checksum;

	union {
		// useful for quickly zeroing out the whole union
		uint32_t data32[1];
		uint16_t data16[2];
		uint8_t  data8[4];

		// for pings
		struct icmpv6_echo {
			uint16_t id;
			uint16_t seq;
		} echo;

		// for ND protocol neighbour
		struct icmp6_nd_advt {
#if __BYTE_ORDER == __LITTLE_ENDIAN
					uint32_t		reserved:5,
									override:1,
									solicited:1,
									router:1,
									reserved2:24;
#elif __BYTE_ORDER == __BIG_ENDIAN
					uint32_t		router:1,
									solicited:1,
									override:1,
									reserved:29;
#else
	#error "Unknown byte order"
#endif					
		} nd_advt;

		// for ND protocol router
		struct icmpv6_nd_ra {
					uint8_t			hop_limit;
#if __BYTE_ORDER == __LITTLE_ENDIAN
					uint8_t			reserved:3,
									router_pref:2,
									home_agent:1,
									other:1,
									managed:1;

#elif __BYTE_ORDER == __BIG_ENDIAN
					uint8_t			managed:1,
									other:1,
									home_agent:1,
									router_pref:2,
									reserved:3;
#else
	#error "Unknown byte order"
#endif	
					uint16_t		rt_lifetime;
		} nd_ra;
	} icmp6_data;
};
