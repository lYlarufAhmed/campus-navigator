#include <stdio.h>
#include <string.h>
#include "mst.h"
#include "union_find.h"
#include "sorting.h"

/* ================================================================
   KRUSKAL'S ALGORITHM  —  O(E log E)
   Strategy: greedily add the cheapest edge that does NOT form a cycle.

   Step 1 — Sort all edges by weight ascending.       (MergeSort, O(E log E))
   Step 2 — Init Union-Find: every location is its own component.
   Step 3 — Walk the sorted list:
              • If the edge connects two DIFFERENT components → include it,
                union the components.
              • If both endpoints are already in the SAME component → skip
                (adding it would create a cycle).
   Step 4 — Stop when we have V-1 edges (a spanning tree) or exhaust
             the list (disconnected graph → spanning forest).

   Correctness relies on the Cut Property: for any cut of the graph,
   the minimum-weight crossing edge is safe to add to any MST.
   ================================================================ */

int kruskal(const Graph *g, Edge mst_edges[], int *mst_size)
{
    /* work on a private copy so g->edges[] stays in original order */
    Edge sorted[MAX_EDGES];
    memcpy(sorted, g->edges, g->num_roads * sizeof(Edge));
    sort_edges_merge(sorted, g->num_roads);   /* O(E log E) */

    UnionFind uf;
    uf_init(&uf, g->num_locations);           /* each location is its own set */

    int total = 0;
    *mst_size = 0;

    for (int i = 0; i < g->num_roads; i++) {
        int u = sorted[i].src;
        int v = sorted[i].dest;

        /*
         * uf_connected checks if u and v share a root in O(α(n)) ≈ O(1).
         * If they do, this edge would close a cycle — skip it.
         * If they don't, this edge safely bridges two components — take it.
         */
        if (!uf_connected(&uf, u, v)) {
            mst_edges[(*mst_size)++] = sorted[i];
            uf_union(&uf, u, v);
            total += sorted[i].weight;

            /* a spanning tree of V vertices has exactly V-1 edges */
            if (*mst_size == g->num_locations - 1)
                break;
        }
    }

    /* if we collected fewer than V-1 edges the graph is disconnected */
    return (*mst_size == g->num_locations - 1) ? total : -1;
}

void kruskal_print(const Graph *g)
{
    Edge mst[MAX_EDGES];
    int  mst_size = 0;

    int total = kruskal(g, mst, &mst_size);

    printf("  %-22s  %-22s  Weight\n", "From", "To");
    printf("  %-22s  %-22s  ------\n", "----", "--");
    for (int i = 0; i < mst_size; i++) {
        printf("  %-22s  %-22s  %d\n",
               g->locations[mst[i].src].name,
               g->locations[mst[i].dest].name,
               mst[i].weight);
    }

    if (total == -1) {
        printf("\n  WARNING: Graph is disconnected — this is a spanning FOREST.\n");
        printf("  Forest edges: %d  |  Total cost: %d\n", mst_size, 0);
        /* recalculate cost for forest */
        int cost = 0;
        for (int i = 0; i < mst_size; i++) cost += mst[i].weight;
        printf("  Forest cost: %d\n", cost);
    } else {
        printf("\n  Spanning tree edges : %d  (need V-1 = %d)\n",
               mst_size, g->num_locations - 1);
        printf("  Minimum total cost  : %d\n", total);
    }
}
