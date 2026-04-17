#include <stdio.h>
#include "graph.h"

int main(void)
{
    Graph *g = create_graph();
    load_from_file(g, "data/campus.txt");
    print_graph(g);
    free_graph(g);
    return 0;
}
