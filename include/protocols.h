#pragma once
#include <unistd.h>
#include <stdint.h>

#define BROADCAST_MAC (uint8_t[]){0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

#define ETHR_TYPE_IPV4  0x800
#define ETHR_TYPE_ARP   0x806

#define ICMP_PROTOCOL_ID      	1
#define ICMP_ECHO_REPLY		    	0
#define ICMP_DEST_UNREACHABLE	  3
#define ICMP_TIME_EXCEEDED		  11

#define ARP_REQUEST             1
#define ARP_REPLY               2
#define ARP_HARDWARE_TYPE_ETH   1
#define ARP_HARDWARE_LEN_ETH    6
#define ARP_PROTOCOL_TYPE_IP    0x800
#define ARP_PROTOCL_LEN_IP      4

/* Ethernet ARP packet from RFC 826 */
struct arp_hdr {
	uint16_t hw_type;   /* Format of hardware address */
	uint16_t proto_type;   /* Format of protocol address */
	uint8_t hw_len;    /* Length of hardware address */
	uint8_t proto_len;    /* Length of protocol address */
	uint16_t opcode;    /* ARP opcode (command) */
	uint8_t shwa[6];  /* Sender hardware address */
	uint32_t sprotoa;   /* Sender IP address */
	uint8_t thwa[6];  /* Target hardware address */
	uint32_t tprotoa;   /* Target IP address */
} __attribute__((packed));

/* Ethernet frame header*/
struct ether_hdr {
    uint8_t  ethr_dhost[6]; // destination MAC address
    uint8_t  ethr_shost[6]; // sender MAC address
    uint16_t ethr_type;     // encapsulated protocol identification
};

/* IP Header */
struct ip_hdr {
    // this means that version uses 4 bits, and ihl 4 bits
    uint8_t    ihl:4, ver:4;   // we use version = 4
    uint8_t    tos;         // Nu este relevant pentru temă (set pe 0)
    uint16_t   tot_len;     // total length = ipheader + data
    uint16_t   id;          // Nu este relevant pentru temă, (set pe 4)
    uint16_t   frag;        // Nu este relevant pentru temă, (set pe 0)
    uint8_t    ttl;         // Time to Live -> to avoid loops, we will decrement
    uint8_t    proto;       // Identificator al protocolului encapsulat (e.g. ICMP)
    uint16_t   checksum;    // checksum     -> Since we modify TTL,
    uint32_t   source_addr; // Adresa IP sursă  
    uint32_t   dest_addr;   // Adresa IP destinație
};

struct icmp_hdr
{
  uint8_t mtype;                /* message type */
  uint8_t mcode;                /* type sub-code */
  uint16_t check;               /* checksum */
  union
  {
    struct
    {
      uint16_t        id;
      uint16_t        seq;
    } echo_t;                        /* echo datagram.  Vom folosi doar acest câmp din union*/
    uint32_t        gateway_addr;        /* Gateway address. Nu este relevant pentru tema */
    struct
    {
      uint16_t        __unused;
      uint16_t        mtu;
    } frag_t;                        /* Nu este relevant pentru tema */
  } un_t;
};
