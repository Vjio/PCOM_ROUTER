#include "icmp.h"

// the router currently needs to handle icmp in only 3 instances:
// replying to an echo
// TTL expiring
// not finding a route to the destination
void handle_icmp(struct ether_hdr* ether_header, struct ip_hdr *ip_header, struct icmp_hdr *icmp_header,
	char buf[MAX_PACKET_LEN], size_t len, size_t interface, struct arp_table_entry *arp_table,
    uint8_t mtype) {
    
    // in case of sending an error msg, we need to remake the payload
    // with the IP_header of the dropped packet and its first 64 bits (of the IP_header's payload)
    if (mtype != ICMP_ECHO_REPLY) {
        char *remake_buf = calloc(1, sizeof(struct ip_hdr) + 8);
        memcpy(remake_buf, ip_header, sizeof(struct ip_hdr) + 8);
        memcpy((uint8_t *)icmp_header + sizeof(struct icmp_hdr), remake_buf, sizeof(struct ip_hdr) + 8);
        free(remake_buf);

        // update ip header len
        ip_header->tot_len = htons(2 * sizeof(struct ip_hdr) + sizeof(struct icmp_hdr) + 8);

        // update packet len
        len = sizeof(struct ether_hdr) + 2 * sizeof(struct ip_hdr) + sizeof(struct icmp_hdr) + 8;

        // handle extra union fields of ICMP
        if (mtype == ICMP_DEST_UNREACHABLE)
            icmp_header->un_t.gateway_addr = 0;
    }
    // for the echo reply, we keep the payload intact

    // update ETHER header
    memcpy(ether_header->ethr_dhost, ether_header->ethr_shost, 6);
	get_interface_mac(interface, ether_header->ethr_shost);

    // update IP header
    // change IP D_ADDR be the old S_ADDR
    ip_header->dest_addr = ip_header->source_addr;

    // change IP S_ADDR to be the interface's ADDR
    ip_header->source_addr = inet_addr(get_interface_ipv4(interface));

    // reset TTL
    ip_header->ttl = 64;
    // make sure protocol is set to ICMP
    ip_header->proto = IPPROTO_ICMP;

    // recalculate IP checksum
	ip_header->checksum = 0;
    ip_header->checksum = htons(checksum((uint16_t *)ip_header, sizeof(struct ip_hdr)));

    // set ICMP message type. no hton required since it's only 1 byte
    icmp_header->mtype = mtype;
    icmp_header->mcode = 0;

    // recalculate ICMP checksum
    icmp_header->check = 0;
    icmp_header->check = htons(checksum((uint16_t *)icmp_header, len - sizeof(struct ether_hdr) - sizeof(struct ip_hdr)));

    // send packet back on the receiving interface
	send_to_link(len, buf, interface);
}
