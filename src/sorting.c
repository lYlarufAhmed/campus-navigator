#include <stdio.h>
#include <string.h>
#include "sorting.h"

/* ================================================================
   QUICKSORT  —  O(n log n) average, O(n²) worst, in-place
   Uses Lomuto partition: last element is always the pivot.
   ================================================================ */

static void swap_edge(Edge *a, Edge *b)
{
    Edge tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
 * Lomuto partition scheme:
 *   - pivot  = arr[high].weight
 *   - i      tracks the boundary: everything left of i+1 is <= pivot
 *   - j      scans forward; whenever arr[j] <= pivot we expand the
 *             left region by swapping arr[j] into position i+1
 *   - finally swap pivot into its sorted position
 */
static int partition(Edge arr[], int low, int high)
{
    int pivot = arr[high].weight;
    int i     = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].weight <= pivot) {
            i++;
            swap_edge(&arr[i], &arr[j]);
        }
    }
    swap_edge(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort_edges(Edge arr[], int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort_edges(arr, low,    pi - 1);
        quicksort_edges(arr, pi + 1, high);
    }
}

void sort_edges_quick(Edge arr[], int n)
{
    if (n > 1)
        quicksort_edges(arr, 0, n - 1);
}

/* ================================================================
   MERGESORT  —  O(n log n) guaranteed, stable, not in-place
   Uses a static scratch buffer so no heap allocation is needed.
   ================================================================ */

/*
 * Merge two already-sorted halves arr[left..mid] and arr[mid+1..right]
 * into one sorted run.  The scratch buffer holds a copy of the left half
 * so we can overwrite arr[] in place without losing data.
 *
 * We copy the entire window [left..right] into tmp[], then walk two
 * pointers through tmp and write the smaller element back to arr[].
 */
static void merge(Edge arr[], int left, int mid, int right)
{
    static Edge tmp[MAX_EDGES];
    int len = right - left + 1;
    memcpy(tmp, arr + left, len * sizeof(Edge));

    int i = 0;              /* pointer into tmp's left half  */
    int j = mid - left + 1; /* pointer into tmp's right half */
    int k = left;           /* write position in arr         */

    while (i <= mid - left && j < len) {
        if (tmp[i].weight <= tmp[j].weight)
            arr[k++] = tmp[i++];
        else
            arr[k++] = tmp[j++];
    }
    while (i <= mid - left) arr[k++] = tmp[i++];
    while (j <  len)         arr[k++] = tmp[j++];
}

void mergesort_edges(Edge arr[], int left, int right)
{
    if (left < right) {
        int mid = left + (right - left) / 2; /* avoids int overflow */
        mergesort_edges(arr, left,    mid);
        mergesort_edges(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void sort_edges_merge(Edge arr[], int n)
{
    if (n > 1)
        mergesort_edges(arr, 0, n - 1);
}

/* ================================================================
   DISPLAY
   ================================================================ */

void print_edges(const Graph *g, const Edge arr[], int n)
{
    printf("  %-22s  %-22s  Weight\n", "From", "To");
    printf("  %-22s  %-22s  ------\n", "----", "--");
    for (int i = 0; i < n; i++) {
        printf("  %-22s  %-22s  %d\n",
               g->locations[arr[i].src].name,
               g->locations[arr[i].dest].name,
               arr[i].weight);
    }
}
