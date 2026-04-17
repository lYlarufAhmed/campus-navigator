#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "graph.h"

typedef struct {
    int parent[MAX_LOCATIONS];
    int rank[MAX_LOCATIONS];
    int n;
} UnionFind;

void uf_init(UnionFind *uf, int n);
int  uf_find(UnionFind *uf, int x);          /* path-compressed find        */
void uf_union(UnionFind *uf, int x, int y);  /* union by rank               */
int  uf_connected(UnionFind *uf, int x, int y);
void uf_build_from_graph(UnionFind *uf, const Graph *g);

#endif /* UNION_FIND_H */
