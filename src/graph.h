#ifndef GRAPH_H
#define GRAPH_H

#define MAX_LOCATIONS 100
#define MAX_EDGES     500
#define NAME_LEN      50
#define INF           999999

/* ---------- core structs ---------- */

typedef struct {
    int  id;
    char name[NAME_LEN];
} Location;

/* one node in an adjacency list */
typedef struct AdjNode {
    int dest;
    int weight;
    struct AdjNode *next;
} AdjNode;

/* an undirected edge stored flat — used by Kruskal's and sorting */
typedef struct {
    int src, dest, weight;
} Edge;

typedef struct {
    int        num_locations;
    int        num_roads;
    Location   locations[MAX_LOCATIONS];
    AdjNode   *adj[MAX_LOCATIONS];   /* adj[i] = head of i's neighbour list */
    Edge       edges[MAX_EDGES];     /* flat list of all roads              */
} Graph;

/* ---------- function declarations ---------- */

Graph *create_graph(void);
void   add_edge(Graph *g, int src, int dest, int weight);
void   add_location(Graph *g, const char *name);
int    find_location(const Graph *g, const char *name);
void   print_graph(const Graph *g);
void   load_from_file(Graph *g, const char *filename);
void   free_graph(Graph *g);

#endif /* GRAPH_H */
