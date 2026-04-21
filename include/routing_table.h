#pragma once

#include <arpa/inet.h>
#include <string.h>
#include "protocols.h"
#include "lib.h"

struct route_table_entry *get_best_route(uint32_t ip_dest, struct route_table_entry *routing_table, int routing_table_len);
void sort_routing_table(struct route_table_entry *routing_table, int routing_table_len);
int compare(const void *a, const void *b);
