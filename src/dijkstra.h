#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

/* Fills dist[] and parent[] from src. Returns 0. */
int  dijkstra(const Graph *g, int src, int dist[], int parent[]);

/* Prints the shortest path src→dest with per-edge weights and total cost. */
int  dijkstra_path(const Graph *g, int src, int dest);

/* Prints the shortest distance from src to every other location. */
void dijkstra_print_all(const Graph *g, int src);

#endif /* DIJKSTRA_H */
