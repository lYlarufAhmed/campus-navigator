#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "union_find.h"
#include "sorting.h"
#include "traversal.h"
#include "dijkstra.h"
#include "mst.h"

/* ================================================================
   INPUT HELPERS
   ================================================================ */

/* discard the rest of the current input line after a bad read */
static void clear_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* print all location ids + names so the user knows what to type */
static void print_location_list(const Graph *g)
{
    printf("\n  Locations:\n");
    for (int i = 0; i < g->num_locations; i++)
        printf("    [%2d]  %s\n", i, g->locations[i].name);
}

/*
 * Shows the location list, then prompts until the user enters a valid id
 * or -1 to cancel.  Returns the id, or -1 on cancel.
 */
static int pick_location(const Graph *g, const char *prompt)
{
    print_location_list(g);
    int id;
    for (;;) {
        printf("  %s (0-%d, -1 to cancel): ", prompt, g->num_locations - 1);
        if (scanf("%d", &id) != 1) { clear_stdin(); continue; }
        clear_stdin();
        if (id == -1) return -1;
        if (id >= 0 && id < g->num_locations) return id;
        printf("  Invalid id — try again.\n");
    }
}

/* ================================================================
   MENU DISPLAY
   ================================================================ */

static void print_menu(void)
{
    printf("\n");
    printf("  ================================================\n");
    printf("             CAMPUS NAVIGATOR                     \n");
    printf("  ================================================\n");
    printf("   1.  Shortest path            (Dijkstra)\n");
    printf("   2.  Minimum cost network     (Kruskal MST)\n");
    printf("   3.  Check all reachable      (DFS)\n");
    printf("   4.  Path with fewest stops   (BFS)\n");
    printf("   5.  Sort roads by weight     (QuickSort / MergeSort)\n");
    printf("   6.  Check two locations connected (Union-Find)\n");
    printf("   7.  Show all distances from a location\n");
    printf("   8.  Print campus map\n");
    printf("   0.  Exit\n");
    printf("  ================================================\n");
    printf("  Choice: ");
}

/* ================================================================
   MENU HANDLERS  — one function per menu option
   ================================================================ */

static void menu_dijkstra(const Graph *g)
{
    printf("\n--- Shortest Weighted Path (Dijkstra) ---\n");
    int src = pick_location(g, "SOURCE     ");
    if (src == -1) return;
    int dst = pick_location(g, "DESTINATION");
    if (dst == -1) return;
    printf("\n");
    dijkstra_path(g, src, dst);
}

static void menu_mst(const Graph *g)
{
    printf("\n--- Minimum Cost Road Network (Kruskal's MST) ---\n\n");
    kruskal_print(g);
}

static void menu_dfs(const Graph *g)
{
    printf("\n--- Are All Locations Reachable? (DFS) ---\n");
    int src = pick_location(g, "START location");
    if (src == -1) return;
    printf("\n");
    dfs_print_reachability(g, src);
}

static void menu_bfs(const Graph *g)
{
    printf("\n--- Fewest-Stops Path (BFS) ---\n");
    int src = pick_location(g, "SOURCE     ");
    if (src == -1) return;
    int dst = pick_location(g, "DESTINATION");
    if (dst == -1) return;
    printf("\n");
    bfs_fewest_stops(g, src, dst);
}

static void menu_sort(const Graph *g)
{
    printf("\n--- Sort Roads by Weight ---\n");
    printf("  1 = QuickSort   2 = MergeSort\n  Algorithm: ");

    int alg;
    if (scanf("%d", &alg) != 1) { clear_stdin(); return; }
    clear_stdin();
    if (alg != 1 && alg != 2) { printf("  Invalid choice.\n"); return; }

    Edge copy[MAX_EDGES];
    memcpy(copy, g->edges, g->num_roads * sizeof(Edge));

    if (alg == 1) {
        sort_edges_quick(copy, g->num_roads);
        printf("\n  Roads sorted by weight [QuickSort]:\n");
    } else {
        sort_edges_merge(copy, g->num_roads);
        printf("\n  Roads sorted by weight [MergeSort]:\n");
    }
    print_edges(g, copy, g->num_roads);
}

static void menu_union_find(const Graph *g)
{
    printf("\n--- Are Two Locations Connected? (Union-Find) ---\n");
    int a = pick_location(g, "FIRST location ");
    if (a == -1) return;
    int b = pick_location(g, "SECOND location");
    if (b == -1) return;

    UnionFind uf;
    uf_build_from_graph(&uf, g);

    printf("\n  %s  <-->  %s : %s\n",
           g->locations[a].name,
           g->locations[b].name,
           uf_connected(&uf, a, b) ? "CONNECTED" : "NOT CONNECTED");
}

static void menu_all_distances(const Graph *g)
{
    printf("\n--- All Shortest Distances from a Location (Dijkstra) ---\n");
    int src = pick_location(g, "SOURCE");
    if (src == -1) return;
    printf("\n");
    dijkstra_print_all(g, src);
}

/* ================================================================
   ENTRY POINT
   ================================================================ */

int main(void)
{
    Graph *g = create_graph();
    load_from_file(g, "data/campus.txt");

    printf("\n  Campus loaded: %d locations, %d roads.\n",
           g->num_locations, g->num_roads);
    print_graph(g);

    int running = 1;
    while (running) {
        print_menu();

        int choice;
        if (scanf("%d", &choice) != 1) { clear_stdin(); continue; }
        clear_stdin();

        switch (choice) {
            case 1: menu_dijkstra(g);      break;
            case 2: menu_mst(g);           break;
            case 3: menu_dfs(g);           break;
            case 4: menu_bfs(g);           break;
            case 5: menu_sort(g);          break;
            case 6: menu_union_find(g);    break;
            case 7: menu_all_distances(g); break;
            case 8: print_graph(g);        break;
            case 0: running = 0;           break;
            default:
                printf("  Unknown option — enter 0 to 8.\n");
        }
    }

    printf("\n  Goodbye!\n\n");
    free_graph(g);
    return 0;
}
