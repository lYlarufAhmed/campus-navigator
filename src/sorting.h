#ifndef SORTING_H
#define SORTING_H

#include "graph.h"

/* --- recursive entry points (exposed so Kruskal's can call them too) --- */
void quicksort_edges(Edge arr[], int low, int high);
void mergesort_edges(Edge arr[], int left, int right);

/* --- convenience wrappers: just pass the array and its length --- */
void sort_edges_quick(Edge arr[], int n);
void sort_edges_merge(Edge arr[], int n);

void print_edges(const Graph *g, const Edge arr[], int n);

#endif /* SORTING_H */
