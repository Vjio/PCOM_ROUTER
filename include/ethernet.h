#pragma once

#include <stdint.h>

#define ETHR_TYPE_IPV4  0x0800
#define ETHR_TYPE_ARP   0x0806
#define ETHER_TYPE_IPV6 0x86DD

/* Ethernet frame header*/
struct ether_hdr {
    uint8_t  ethr_dhost[6]; // destination MAC address
    uint8_t  ethr_shost[6]; // sender MAC address
    uint16_t ethr_type;     // encapsulated protocol identification
};
