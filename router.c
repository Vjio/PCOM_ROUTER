#include "misc_protocol.h"
#include "queue.h"
#include "routing_table.h"
#include "lib.h"
#include "arp.h"
#include "ipv4.h"
#include "ipv6.h"

int main(int argc, char *argv[])
{
	char buf[MAX_PACKET_LEN];

	// Do not modify this line
	init(argv + 2, argc - 2);

	FILE *file = fopen("routerOutput", "w");
	fprintf(file, "Router starting\n");
	fflush(file);

	// allocate ARP & routing tables
	struct route_table_entry *routing_table = malloc(sizeof(struct route_table_entry) * 100000);
	int routing_table_len;
	struct arp_table_entry *arp_table = malloc(sizeof(struct arp_hdr) * 100000);
	int arp_table_len = -1;

	// read routing table & sort it
	routing_table_len = read_rtable(argv[1], routing_table);
	sort_routing_table(routing_table, routing_table_len);

	// allocate arp queue
	struct queue *arp_q = create_queue();

	while (1) {

		size_t interface;
		size_t len;

		interface = recv_from_any_link(buf, &len);
		DIE(interface < 0, "recv_from_any_links");

    /* Note that packets received are in network order,
		any header field which has more than 1 byte will need to be conerted to
		host order. For example, ntohs(eth_hdr->ether_type). The oposite is needed when
		sending a packet on the link, */
		// read failed, try again
		if (interface == -1)
			continue;

		fprintf(file, "Received packet\n");
		fflush(file);

		// check if the message received is large enough
		if (len < sizeof(struct ether_hdr) + sizeof(struct ip_hdr)) {
			fprintf(file, "PACKET DROPPED because len=%ld\n", len);
			fflush(file);
			continue;
		}
	
		// extract ethernet header from buf
		struct ether_hdr *ether_header = (struct ether_hdr*) buf;

		// check if packet was broadcasted
		if (memcmp((ether_header->ethr_dhost), BROADCAST_MAC, 6) == 0) {
			fprintf(file, "RECOGNIZED BROADCAST\n");
			goto ethernet_header_correct;
		}

		// check if packet destination is this router
		uint8_t interface_mac[8];
		get_interface_mac(interface, interface_mac);
		if (memcmp(ether_header->ethr_dhost, interface_mac, 6) == 0)
			goto ethernet_header_correct;

		// drop packet, MAC addr are corrupted or just wrong
		fprintf(file, "PACKET DROPPED, ETHER HEADER WRONG!\n");
		fflush(file);
		continue;

ethernet_header_correct:

		// call function depending on next header after ethernet 
		uint16_t ethr_type = ntohs(ether_header->ethr_type);
		switch (ethr_type) {
		case ETHR_TYPE_IPV4:
			struct ip_hdr *ip_header = (struct ip_hdr*) (buf + sizeof(struct ether_hdr));
			handle_ipv4(ether_header, ip_header, buf, len, interface, arp_table, arp_table_len,
				routing_table, routing_table_len, arp_q);
			break;
		
		case ETHER_TYPE_IPV6:
			struct ipv6_hdr *ipv6_header = (struct ipv6_hdr*) (buf + sizeof(struct ether_hdr));
			handle_ipv6(ether_header, ipv6_header, buf, len, interface);
			break;

		case ETHR_TYPE_ARP:
			struct arp_hdr *arp_header = (struct arp_hdr*) (buf + sizeof(struct ether_hdr));
			handle_arp(ether_header, arp_header, buf, len, interface, arp_table, &arp_table_len, 
				routing_table, routing_table_len, arp_q);
			
		default:
			// packet uses a different network protocol, not included in this project
			// drop ze packet
			fprintf(file, "unrecognized protocol, packet dropped\n");
			fflush(file);
			break;
		}
	}

	// free allocated mem
	fclose(file);
	free(routing_table);
	free(arp_table);
	while (!queue_empty(arp_q)) 
		queue_deq(arp_q);
	free(arp_q);
}
