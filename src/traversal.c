#include <stdio.h>
#include <string.h>
#include "traversal.h"

/* ================================================================
   DFS  —  O(V + E)
   Visits every location reachable from v by following adjacency
   list edges.  visited[] is caller-allocated so the same array
   can be reused across multiple calls without extra allocation.
   ================================================================ */

void dfs_visit(const Graph *g, int v, int visited[])
{
    visited[v] = 1;
    for (AdjNode *cur = g->adj[v]; cur; cur = cur->next)
        if (!visited[cur->dest])
            dfs_visit(g, cur->dest, visited);
}

/* returns 1 if every location is reachable from src, 0 otherwise */
int dfs_all_reachable(const Graph *g, int src)
{
    int visited[MAX_LOCATIONS] = {0};
    dfs_visit(g, src, visited);
    for (int i = 0; i < g->num_locations; i++)
        if (!visited[i]) return 0;
    return 1;
}

/*
 * Runs DFS from src, then reports which locations were reached and
 * which (if any) were not.  Useful for the menu "check all reachable"
 * feature as well as debugging disconnected graphs.
 */
void dfs_print_reachability(const Graph *g, int src)
{
    int visited[MAX_LOCATIONS] = {0};
    dfs_visit(g, src, visited);

    printf("  DFS from [%s]:\n", g->locations[src].name);
    int all = 1;
    for (int i = 0; i < g->num_locations; i++) {
        printf("    [%s]  %s\n",
               visited[i] ? "REACHED    " : "UNREACHABLE",
               g->locations[i].name);
        if (!visited[i]) all = 0;
    }
    printf("  Result: %s\n",
           all ? "All locations are reachable."
               : "Some locations are NOT reachable from this point.");
}

/* ================================================================
   BFS  —  O(V + E)
   Explores level by level (one stop at a time), so the first time
   BFS reaches the destination it has used the minimum number of
   edges (stops), regardless of edge weights.

   Queue: simple array used as a queue with head/tail indices.
   parent[]: parent[v] = u means "BFS reached v from u".
             Initialised to -1; used to reconstruct the path.
   ================================================================ */

static void print_path(const Graph *g, int src, int dest, const int parent[])
{
    if (dest == src) {
        printf("%s", g->locations[src].name);
        return;
    }
    print_path(g, src, parent[dest], parent);
    printf(" -> %s", g->locations[dest].name);
}

/*
 * Returns the number of stops (edges) on the shortest hop-count path,
 * or -1 if dest is unreachable from src.
 * Prints the full path and stop count on success.
 */
int bfs_fewest_stops(const Graph *g, int src, int dest)
{
    if (src == dest) {
        printf("  Source and destination are the same: %s\n",
               g->locations[src].name);
        return 0;
    }

    int visited[MAX_LOCATIONS] = {0};
    int parent[MAX_LOCATIONS];
    int dist[MAX_LOCATIONS];
    int queue[MAX_LOCATIONS];

    memset(parent, -1, sizeof parent);
    memset(dist,   -1, sizeof dist);

    int head = 0, tail = 0;
    queue[tail++] = src;
    visited[src]  = 1;
    dist[src]     = 0;

    while (head < tail) {
        int u = queue[head++];

        for (AdjNode *cur = g->adj[u]; cur; cur = cur->next) {
            int v = cur->dest;
            if (visited[v]) continue;

            visited[v] = 1;
            parent[v]  = u;
            dist[v]    = dist[u] + 1;

            if (v == dest) {
                printf("  Fewest-stops path: ");
                print_path(g, src, dest, parent);
                printf("\n  Stops (edges): %d\n", dist[dest]);
                return dist[dest];
            }
            queue[tail++] = v;
        }
    }

    printf("  No path found from %s to %s.\n",
           g->locations[src].name, g->locations[dest].name);
    return -1;
}
