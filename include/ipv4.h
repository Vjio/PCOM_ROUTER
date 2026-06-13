#pragma once

#include <arpa/inet.h>
#include <string.h>
#include "queue.h"
#include "lib.h"
#include "arp.h"
#include "routing_table.h"

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
    uint32_t   source_addr; // source ip addr
    uint32_t   dest_addr;   // dest ip addr
};

void handle_ipv4(struct ether_hdr* ether_header, struct ip_hdr *ip_header, 
	char buf[MAX_PACKET_LEN], size_t len, size_t interface, 
	struct arp_table_entry *arp_table, int arp_table_len,
	struct route_table_entry *routing_table, int routing_table_len,
	struct queue *q);
