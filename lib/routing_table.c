#include "routing_table.h"

struct route_table_entry *get_best_route(uint32_t ip_dest, struct route_table_entry *routing_table, int routing_table_len) {
	struct route_table_entry *best_route = NULL;

	for (int i = 0; i < routing_table_len; i++)
		if ((ip_dest & routing_table[i].mask) == routing_table[i].prefix) {
				best_route = &routing_table[i];
                break;
            }

	return best_route;
}

int compare(const void *a, const void *b) {
    struct route_table_entry *ra = (struct route_table_entry *)a;
    struct route_table_entry *rb = (struct route_table_entry *)b;

    if (ntohl(ra->mask) > ntohl(rb->mask)) {
        return -1;
    } else if (ntohl(ra->mask) < ntohl(rb->mask)) {
        return 1;
    } else {
        if (ntohl(ra->prefix) > ntohl(rb->prefix)) return -1;
        if (ntohl(ra->prefix) < ntohl(rb->prefix)) return 1;
        
        return 0;
    }
}

void sort_routing_table(struct route_table_entry *routing_table, int routing_table_len) {
    qsort(routing_table, routing_table_len, sizeof(struct route_table_entry), compare);
}