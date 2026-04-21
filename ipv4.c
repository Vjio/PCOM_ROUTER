#include <ipv4.h>

void handle_ipv4(struct ether_hdr* ether_header, struct ip_hdr *ip_header, 
	char buf[MAX_PACKET_LEN], size_t len, size_t interface, 
	struct arp_table_entry *arp_table, int arp_table_len,
	struct route_table_entry *routing_table, int routing_table_len,
	struct queue *q) {

	// STEP 1: verify checksum
	uint16_t initial_checksum = ip_header->checksum;
	ip_header->checksum = 0;

	// wrong ckecksum-> drop packet
	if (initial_checksum != ntohs(checksum((uint16_t *)ip_header, sizeof(struct ip_hdr)))) {
		printf("incorrect cheksum. packet dropped\n");
		return;
	}
	
	// STEP 2: check if this router is the destination
	uint32_t interface_ip = inet_addr(get_interface_ip(interface));
	if (interface_ip == ip_header->dest_addr) {
		// this project's router will only respond to ICMP messages
		if (ip_header->proto != ICMP_PROTOCOL_ID)
			return;

		// answer to ICMP request
		struct icmp_hdr *icmp_header = (struct icmp_hdr *)(buf + sizeof(struct ether_hdr) + sizeof(struct ip_hdr));

		handle_icmp(ether_header, ip_header, icmp_header, buf, len, interface, arp_table,
		ICMP_ECHO_REPLY);

		return;
	}


	// router is not the destination, prepare packet for forwarding
	// STEP 3: handle TTL
	if (ip_header->ttl <= 1) {
		// handle time exceeded
		struct icmp_hdr *icmp_header = (struct icmp_hdr *)(buf + sizeof(struct ether_hdr) + sizeof(struct ip_hdr));

		handle_icmp(ether_header, ip_header, icmp_header, buf, len, interface, arp_table,
		ICMP_TIME_EXCEEDED);

		return;
	}

	// TTL is valid-> decrement and update checksum
	ip_header->ttl--;
	ip_header->checksum = 0;	// just to be sure
	ip_header->checksum = htons(checksum((uint16_t *)ip_header, sizeof(struct ip_hdr)));

	// STEP 4: search for dest IP addr in routing table
	struct route_table_entry *best_route = get_best_route(ip_header->dest_addr,
		routing_table, routing_table_len);

	// if nothing is found, drop packet and a return a ICMP "Dest unreachable" to sender
	if (best_route == NULL) {
		// handle dest unreachable
		struct icmp_hdr *icmp_header = (struct icmp_hdr *)(buf + sizeof(struct ether_hdr) + sizeof(struct ip_hdr));

		handle_icmp(ether_header, ip_header, icmp_header, buf, len, interface, arp_table,
		ICMP_DEST_UNREACHABLE);

		return;
	}

	// found a path => STEP 5: resfresh ethernet header
	// rewrite S_ADDR to be the interface's one
	get_interface_mac(best_route->interface, ether_header->ethr_shost);

	// rewrite D_ADDR to be receiving router's MAC
	// get next hop's mac
	struct arp_table_entry *next_hop_entry = get_arp_table_entry(best_route->next_hop,
		arp_table, arp_table_len);

	// arp table miss. put packet in queue and send an ARP request
	if (next_hop_entry == NULL) {
		char *buf_copy = malloc(len);
		memcpy(buf_copy, buf, len);
		queue_enq(q, (void *)buf_copy);

		send_arp_request(best_route, best_route->interface, ARP_PROTOCOL_TYPE_IP);
		return;
	}
	// set ether_header DHOST to the found mac
	memcpy(ether_header->ethr_dhost, next_hop_entry->mac, 6);

	// all done, send the packet
	send_to_link(len, buf, best_route->interface);
}
