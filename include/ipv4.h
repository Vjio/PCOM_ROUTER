#pragma once

#include <arpa/inet.h>
#include <string.h>
#include "queue.h"
#include "protocols.h"
#include "lib.h"
#include "icmp.h"
#include "arp.h"
#include "routing_table.h"

void handle_ipv4(struct ether_hdr* ether_header, struct ip_hdr *ip_header, 
	char buf[MAX_PACKET_LEN], size_t len, size_t interface, 
	struct arp_table_entry *arp_table, int arp_table_len,
	struct route_table_entry *routing_table, int routing_table_len,
	struct queue *q);
