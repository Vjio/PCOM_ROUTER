#pragma once

#include <arpa/inet.h>
#include <string.h>
#include "ipv4.h"
#include "ethernet.h"
#include "lib.h"

#define ICMP_PROTOCOL_ID            1
#define ICMP_ECHO_REPLY             0
#define ICMP_DEST_UNREACHABLE       3
#define ICMP_TIME_EXCEEDED          11

struct icmp_hdr
{
  uint8_t mtype;                /* message type */
  uint8_t mcode;                /* type sub-code */
  uint16_t check;               /* checksum */
  union {
	// for pings
	struct {
	  uint16_t        id;
	  uint16_t        seq;
	} echo_t;                        /* echo datagram*/
	// for redirects
	uint32_t        gateway_addr;
	// for fragmentation needed errors
	struct {
	  uint16_t        __unused;
	  uint16_t        mtu;
	} frag_t;
  } un_t;
};

void handle_icmp(struct ether_hdr* ether_header, struct ip_hdr *ip_header, 
	struct icmp_hdr *icmp_header, char buf[MAX_PACKET_LEN], size_t len, 
	size_t interface, struct arp_table_entry *arp_table, uint8_t mtype);
