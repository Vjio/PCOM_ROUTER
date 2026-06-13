#pragma once

#include <arpa/inet.h>
#include <string.h>
#include "queue.h"
#include "misc_protocol.h"
#include "ethernet.h"
#include "ipv4.h"
#include "lib.h"
#include "routing_table.h"

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


void handle_arp(struct ether_hdr *ether_header, struct arp_hdr *arp_header, 
    char buf[MAX_PACKET_LEN], size_t len, size_t interface, 
    struct arp_table_entry *arp_table, int *arp_table_len,
    struct route_table_entry *routing_table, int routing_table_len,
    struct queue *q);

void send_arp_request(struct route_table_entry *best_route, int interface,
    uint16_t opproto_typecode);
