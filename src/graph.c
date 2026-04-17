#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/* ---------- creation / teardown ---------- */

Graph *create_graph(void)
{
    Graph *g = calloc(1, sizeof(Graph));
    if (!g) { perror("calloc"); exit(EXIT_FAILURE); }
    return g;
}

void free_graph(Graph *g)
{
    for (int i = 0; i < g->num_locations; i++) {
        AdjNode *cur = g->adj[i];
        while (cur) {
            AdjNode *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free(g);
}

/* ---------- building the graph ---------- */

void add_location(Graph *g, const char *name)
{
    int id = g->num_locations;
    g->locations[id].id = id;
    strncpy(g->locations[id].name, name, NAME_LEN - 1);
    g->adj[id] = NULL;
    g->num_locations++;
}

/* prepend a neighbour node to src's adjacency list */
static void add_adj(Graph *g, int src, int dest, int weight)
{
    AdjNode *node = malloc(sizeof(AdjNode));
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->dest   = dest;
    node->weight = weight;
    node->next   = g->adj[src];
    g->adj[src]  = node;
}

/*
 * Undirected edge: we add two directed adjacency entries (src->dest,
 * dest->src) but only one entry in the flat edges[] array so Kruskal's
 * doesn't double-count.
 */
void add_edge(Graph *g, int src, int dest, int weight)
{
    if (src < 0 || dest < 0 ||
        src >= g->num_locations || dest >= g->num_locations) {
        fprintf(stderr, "add_edge: invalid vertex ids %d %d\n", src, dest);
        return;
    }
    if (g->num_roads >= MAX_EDGES) {
        fprintf(stderr, "add_edge: MAX_EDGES reached\n");
        return;
    }

    add_adj(g, src, dest, weight);
    add_adj(g, dest, src, weight);

    g->edges[g->num_roads].src    = src;
    g->edges[g->num_roads].dest   = dest;
    g->edges[g->num_roads].weight = weight;
    g->num_roads++;
}

/* ---------- lookup ---------- */

/* returns id on match, -1 if not found */
int find_location(const Graph *g, const char *name)
{
    for (int i = 0; i < g->num_locations; i++)
        if (strcmp(g->locations[i].name, name) == 0)
            return i;
    return -1;
}

/* ---------- display ---------- */

void print_graph(const Graph *g)
{
    printf("\n=== Campus Map (%d locations, %d roads) ===\n",
           g->num_locations, g->num_roads);
    for (int i = 0; i < g->num_locations; i++) {
        printf("  [%d] %-20s -> ", i, g->locations[i].name);
        AdjNode *cur = g->adj[i];
        if (!cur) { printf("(no roads)\n"); continue; }
        while (cur) {
            printf("%s(%d)", g->locations[cur->dest].name, cur->weight);
            if (cur->next) printf(", ");
            cur = cur->next;
        }
        printf("\n");
    }
    printf("\n");
}

/* ---------- file loading ---------- */

/*
 * File format:
 *   LOCATIONS <n>
 *   <id> <Name_no_spaces>
 *   ...
 *   ROADS <m>
 *   <src_id> <dest_id> <weight>
 *   ...
 */
void load_from_file(Graph *g, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror(filename); return; }

    int n, m;
    char buf[NAME_LEN];

    if (fscanf(fp, " LOCATIONS %d", &n) != 1) goto bad;
    for (int i = 0; i < n; i++) {
        int id;
        if (fscanf(fp, " %d %49s", &id, buf) != 2) goto bad;
        add_location(g, buf);
    }

    if (fscanf(fp, " ROADS %d", &m) != 1) goto bad;
    for (int i = 0; i < m; i++) {
        int s, d, w;
        if (fscanf(fp, " %d %d %d", &s, &d, &w) != 3) goto bad;
        add_edge(g, s, d, w);
    }

    fclose(fp);
    printf("Loaded campus from '%s'.\n", filename);
    return;

bad:
    fprintf(stderr, "load_from_file: bad format in '%s'\n", filename);
    fclose(fp);
}
