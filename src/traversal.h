#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include "graph.h"

/* --- DFS: reachability --- */
void dfs_visit(const Graph *g, int v, int visited[]);
int  dfs_all_reachable(const Graph *g, int src);
void dfs_print_reachability(const Graph *g, int src);

/* --- BFS: fewest stops --- */
int  bfs_fewest_stops(const Graph *g, int src, int dest);

#endif /* TRAVERSAL_H */
