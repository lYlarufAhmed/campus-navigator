#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "union_find.h"
#include "sorting.h"
#include "traversal.h"
#include "dijkstra.h"
#include "mst.h"

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

    /* --- Section 6 demo: Kruskal's MST --- */
    printf("=== Kruskal's MST: Minimum Cost Road Network ===\n");
    kruskal_print(g);
    printf("\n");

    /* --- Section 5 demo: Dijkstra shortest weighted path --- */
    printf("=== Dijkstra: Shortest Weighted Path ===\n");
    dijkstra_path(g, 0, 7);   /* Library -> Hostel      */
    printf("\n");
    dijkstra_path(g, 2, 4);   /* Main_Gate -> Science_Block */
    printf("\n");
    printf("=== Dijkstra: All Distances from Library ===\n");
    dijkstra_print_all(g, 0);
    printf("\n");

    /* --- Section 4 demo: DFS reachability + BFS fewest stops --- */
    printf("=== DFS: Are All Locations Reachable from Library? ===\n");
    dfs_print_reachability(g, 0);
    printf("\n");

    printf("=== BFS: Fewest Stops ===\n");
    printf("  Library -> Hostel:\n");
    bfs_fewest_stops(g, 0, 7);
    printf("\n  Main_Gate -> Sports_Complex:\n");
    bfs_fewest_stops(g, 2, 5);
    printf("\n");

    free_graph(g);
    return 0;
}
