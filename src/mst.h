#ifndef MST_H
#define MST_H

#include "graph.h"

/*
 * Runs Kruskal's algorithm on g.
 * Fills mst_edges[] with the chosen edges; sets *mst_size to the count.
 * Returns total MST cost, or -1 if the graph is disconnected (spanning
 * forest only — mst_edges[] still holds the forest edges).
 */
int kruskal(const Graph *g, Edge mst_edges[], int *mst_size);

/* Convenience wrapper: runs Kruskal's and prints the full result. */
void kruskal_print(const Graph *g);

#endif /* MST_H */
