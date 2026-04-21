#pragma once

#include <arpa/inet.h>
#include <string.h>
#include "protocols.h"
#include "lib.h"
#include "icmp.h"

void handle_icmp(struct ether_hdr* ether_header, struct ip_hdr *ip_header, struct icmp_hdr *icmp_header,
    char buf[MAX_PACKET_LEN], size_t len, size_t interface, struct arp_table_entry *arp_table,
    uint8_t mtype);
