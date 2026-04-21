#pragma once

#include <arpa/inet.h>
#include <string.h>
#include "queue.h"
#include "protocols.h"
#include "lib.h"
#include "routing_table.h"

#define OP_REQUEST 1
#define OP_REPLY 2

void handle_arp(struct ether_hdr *ether_header, struct arp_hdr *arp_header, 
    char buf[MAX_PACKET_LEN], size_t len, size_t interface, 
    struct arp_table_entry *arp_table, int *arp_table_len,
    struct route_table_entry *routing_table, int routing_table_len,
    struct queue *q);
void send_arp_request(struct route_table_entry *best_route, int interface,
    uint16_t opproto_typecode);
