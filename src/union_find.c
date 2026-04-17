#include "union_find.h"

void uf_init(UnionFind *uf, int n)
{
    uf->n = n;
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;   /* each node is its own set */
        uf->rank[i]   = 0;
    }
}

/*
 * Path compression: on the way back up the recursion, point every visited
 * node directly at the root.  This flattens the tree so future finds are
 * nearly O(1).
 */
int uf_find(UnionFind *uf, int x)
{
    if (uf->parent[x] != x)
        uf->parent[x] = uf_find(uf, uf->parent[x]);
    return uf->parent[x];
}

/*
 * Union by rank: attach the shorter tree under the taller one.
 * Only increment rank when two equal-rank trees merge.
 * Together with path compression this gives O(α(n)) amortised — effectively
 * constant for any realistic campus size.
 */
void uf_union(UnionFind *uf, int x, int y)
{
    int rx = uf_find(uf, x);
    int ry = uf_find(uf, y);
    if (rx == ry) return;   /* already in the same set */

    if (uf->rank[rx] < uf->rank[ry])
        uf->parent[rx] = ry;
    else if (uf->rank[rx] > uf->rank[ry])
        uf->parent[ry] = rx;
    else {
        uf->parent[ry] = rx;
        uf->rank[rx]++;
    }
}

int uf_connected(UnionFind *uf, int x, int y)
{
    return uf_find(uf, x) == uf_find(uf, y);
}

/* convenience: populate a UnionFind from every edge already in the graph */
void uf_build_from_graph(UnionFind *uf, const Graph *g)
{
    uf_init(uf, g->num_locations);
    for (int i = 0; i < g->num_roads; i++)
        uf_union(uf, g->edges[i].src, g->edges[i].dest);
}
