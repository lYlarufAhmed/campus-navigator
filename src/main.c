#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "union_find.h"
#include "sorting.h"

int main(void)
{
    Graph *g = create_graph();
    load_from_file(g, "data/campus.txt");
    print_graph(g);

    /* --- Section 2 demo: connectivity check via Union-Find --- */
    UnionFind uf;
    uf_build_from_graph(&uf, g);

    int pairs[][2] = { {0, 7}, {0, 6}, {3, 5} };
    int npairs = (int)(sizeof pairs / sizeof pairs[0]);
    printf("=== Connectivity Check ===\n");
    for (int i = 0; i < npairs; i++) {
        int a = pairs[i][0], b = pairs[i][1];
        printf("  %s <-> %s : %s\n",
               g->locations[a].name,
               g->locations[b].name,
               uf_connected(&uf, a, b) ? "CONNECTED" : "NOT CONNECTED");
    }
    printf("\n");

    /* --- Section 3 demo: sort roads by weight --- */
    Edge copy1[MAX_EDGES], copy2[MAX_EDGES];
    memcpy(copy1, g->edges, g->num_roads * sizeof(Edge));
    memcpy(copy2, g->edges, g->num_roads * sizeof(Edge));

    printf("=== Roads Sorted by Weight — QuickSort ===\n");
    sort_edges_quick(copy1, g->num_roads);
    print_edges(g, copy1, g->num_roads);

    printf("\n=== Roads Sorted by Weight — MergeSort ===\n");
    sort_edges_merge(copy2, g->num_roads);
    print_edges(g, copy2, g->num_roads);
    printf("\n");

    free_graph(g);
    return 0;
}
