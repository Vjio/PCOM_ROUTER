#pragma once

#include <stdint.h>

// https://elixir.bootlin.com/linux/v4.6/source/include/uapi/linux/ipv6.h#L114
/* IPv6 header*/
struct ip6_hdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t  traffic_class_upper : 4,
             version : 4;
    uint8_t  flow_label_upper : 4,
             traffic_class_lower : 4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t  version : 4,
             traffic_class_upper : 4;
    uint8_t  traffic_class_lower : 4,
             flow_label_upper : 4;
#else
    #error "Unknown byte order"
#endif
    uint16_t flow_label_lower;
    uint16_t payload_len;
    uint8_t  next_hdr;
    uint8_t  hop_limit;
    uint8_t  source_addr[16];
    uint8_t  dest_addr[16];
};

