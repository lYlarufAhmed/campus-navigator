#include <stdio.h>
#include "dijkstra.h"

/* ================================================================
   DIJKSTRA'S ALGORITHM  —  O(V²)
   Simple array-based approach: at each step we scan the entire
   dist[] array to find the minimum.  This is O(V) per step and
   O(V²) overall.  For a campus graph with ≤ 100 locations this is
   perfectly fast; a binary-heap version would be O((V+E) log V)
   but adds significant complexity for no practical gain here.
   ================================================================ */

/* scan unvisited vertices for the one with smallest tentative distance */
static int min_vertex(const int dist[], const int visited[], int n)
{
    int min = INF, idx = -1;
    for (int v = 0; v < n; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            idx = v;
        }
    }
    return idx;  /* -1 when all remaining vertices are unreachable */
}

/*
 * Core Dijkstra.  Caller provides dist[] and parent[] arrays of size
 * at least g->num_locations.  After the call:
 *   dist[v]   = shortest distance from src to v  (INF = unreachable)
 *   parent[v] = predecessor of v on that path    (-1  = none)
 */
int dijkstra(const Graph *g, int src, int dist[], int parent[])
{
    int visited[MAX_LOCATIONS] = {0};

    for (int i = 0; i < g->num_locations; i++) {
        dist[i]   = INF;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int step = 0; step < g->num_locations; step++) {
        int u = min_vertex(dist, visited, g->num_locations);
        if (u == -1) break;   /* no more reachable vertices */
        visited[u] = 1;

        /*
         * Relaxation: if going through u gives a shorter path to
         * neighbour v, update dist[v] and record u as v's predecessor.
         */
        for (AdjNode *cur = g->adj[u]; cur; cur = cur->next) {
            int v = cur->dest;
            int w = cur->weight;
            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v]   = dist[u] + w;
                parent[v] = u;
            }
        }
    }
    return 0;
}

/* ================================================================
   PATH DISPLAY
   ================================================================ */

/* look up the stored weight for edge u→v in the adjacency list */
static int edge_weight(const Graph *g, int u, int v)
{
    for (AdjNode *cur = g->adj[u]; cur; cur = cur->next)
        if (cur->dest == v) return cur->weight;
    return 0;
}

/*
 * Recursive path printer.
 * Unwinds the parent[] chain back to src using recursion, then prints
 * each step forward as the stack unwinds, annotating each edge with
 * its weight so the user sees:
 *   Library --(5)--> Cafeteria --(7)--> Engineering_Block ...
 */
static void print_path(const Graph *g, int src, int dest, const int parent[])
{
    if (dest == src) {
        printf("%s", g->locations[src].name);
        return;
    }
    print_path(g, src, parent[dest], parent);
    int w = edge_weight(g, parent[dest], dest);
    printf(" --(%d)--> %s", w, g->locations[dest].name);
}

/*
 * Runs Dijkstra from src, then prints the full annotated path to dest
 * and the total cost.  Returns total distance, or -1 if unreachable.
 */
int dijkstra_path(const Graph *g, int src, int dest)
{
    int dist[MAX_LOCATIONS], parent[MAX_LOCATIONS];
    dijkstra(g, src, dist, parent);

    printf("  From: %s\n  To:   %s\n", g->locations[src].name,
                                        g->locations[dest].name);

    if (dist[dest] == INF) {
        printf("  Result: No path exists.\n");
        return -1;
    }

    printf("  Path:  ");
    print_path(g, src, dest, parent);
    printf("\n  Total distance: %d\n", dist[dest]);
    return dist[dest];
}

/* Print shortest distance from src to every other location. */
void dijkstra_print_all(const Graph *g, int src)
{
    int dist[MAX_LOCATIONS], parent[MAX_LOCATIONS];
    dijkstra(g, src, dist, parent);

    printf("  Shortest distances from [%s]:\n", g->locations[src].name);
    for (int i = 0; i < g->num_locations; i++) {
        if (i == src) continue;
        if (dist[i] == INF)
            printf("    -> %-22s  UNREACHABLE\n", g->locations[i].name);
        else
            printf("    -> %-22s  %d\n", g->locations[i].name, dist[i]);
    }
}
