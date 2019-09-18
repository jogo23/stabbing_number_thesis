
// taken from: http://www.martinbroadhurst.com/spanning-trees-of-a-graph-in-c.html
// computes all possible labeled spanning trees on n vertices

// Attention: this program takes about 10 minutes to compute all spanning trees.
// If there exists a file all_spanning_trees_9.txt, it will be overwritten

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
 
typedef struct {
    unsigned int first;
    unsigned int second;
} edge;
 
typedef void (*treefn)(const edge *, unsigned int);

unsigned int num_trees = 0;


void connected_components_recursive(const edge *edges, unsigned int n, 
        int *components, unsigned int order, unsigned int vertex,
        unsigned int component)
{
    unsigned int i;
    /* Put this vertex in the current component */
    components[vertex] = component; 
    for (i = 0; i < n; i++) {
        if (edges[i].first == vertex || edges[i].second == vertex) {
            /* Adjacent */
            const unsigned int neighbour = edges[i].first == vertex ?
                    edges[i].second : edges[i].first;
            if (components[neighbour] == -1) {
                /* Not yet visited */
                connected_components_recursive(edges, n, components, order, neighbour, component);
            }
        }
    }
}
 
unsigned int connected_components(const edge *edges, unsigned int n, unsigned int order, 
        int **components)
{
    unsigned int i;
    unsigned int component = 0;
    *components = malloc(order * sizeof(int));
    if (components == NULL) {
        return 0;
    }
    for (i = 0; i < order; i++) {
        (*components)[i] = -1;
    }
     
    for (i = 0; i < order; i++) {
        if ((*components)[i] == -1) {
            connected_components_recursive(edges, n, *components, order, i, component);
            component++;
        }
    }
    return component;
}

 
/* Check if vertices v1 and v2 are in different components in the tree */
static unsigned int different_components(const edge *tree, unsigned int t, unsigned int order,
        unsigned int v1, unsigned int v2)
{
    int *components;
    unsigned int different;
    connected_components(tree, t, order, &components);
    different = components[v1] != components[v2];
    free(components);
    return different;
}
 
static void spanning_trees_recursive(const edge *edges, unsigned int n, unsigned int order, 
        edge *tree, unsigned int t, int predecessor, treefn fun)
{
    if (t == order - 1) {
        /* Found a tree */
        num_trees = num_trees + 1;
        fun(tree, order - 1);
    }
    else {
        unsigned int e;
        for (e = predecessor + 1; e < n; e++) {
            if (t == 0 /* First edge */
                || different_components(tree, t, order, 
                    edges[e].first, edges[e].second))
            {
                tree[t] = edges[e];   
                spanning_trees_recursive(edges, n, order, tree, t + 1, e, fun);
            }
        }
    }
}
 
void spanning_trees(const edge *edges, unsigned int n, unsigned int order, treefn fun)
{
    edge *tree;
    tree = malloc((n - 1) * sizeof(edge));
    if (tree == NULL) {
        return;
    }
    spanning_trees_recursive(edges, n, order, tree, 0, -1, fun);
    free(tree);
}




/* Calculate the nth triangular number T(n) */
unsigned int triangular_number(unsigned int n)
{
    return (n * (n + 1)) / 2;
}
 
/* Construct a complete graph on v vertices */
edge *complete_graph(unsigned int v)
{
    const unsigned int n = triangular_number(v - 1);
    unsigned int i, j, k;
    edge *edges = malloc(n * sizeof(edge));
    if (edges != NULL) {
        for (i = 0, k = 0; i < v - 1; i++) {
            for (j = i + 1; j < v; j++) {
                edges[k].first = i;
                edges[k].second = j;
                k++;
            }
        }
    }
    return edges;
}
 
/* Print the edges in a tree */
static void print_tree(const edge *tree, unsigned int n)
{
    unsigned int e;
    for (e = 0; e < n; e++) {
        //printf("(%u, %u) ", tree[e].first, tree[e].second);
        printf("%u %u ", tree[e].first, tree[e].second);
    }
    putchar('\n');
}


static void save_tree(const edge *tree, unsigned int n)
{
    FILE *fp;

    fp = fopen("all_spanning_trees_9.txt", "a+");

    char tmp[5];

    unsigned int e;
    for (e = 0; e < n; e++) {
        sprintf(tmp, "%d", tree[e].first);
        fputs(tmp, fp);
        fputs(" ", fp);
        sprintf(tmp, "%d", tree[e].second);
        fputs(tmp, fp);
        fputs(" ", fp);
    }

    fputs("\n", fp);
    fclose(fp);
}
 
 
int main(void)
{
    printf("%s\n", "Takes about 10 minutes.");

    const unsigned int v = 9;
    const unsigned int n = triangular_number(v - 1);
    edge *edges;
     
    edges = complete_graph(v);
    if (edges == NULL) {
        return 1;
    }

    const unsigned int n_trees = pow(v, v-2);
    FILE *fp;
    fp = fopen("all_spanning_trees_9.txt", "w+");
    char tmp[12];
    sprintf(tmp, "%d", n_trees);
    fputs(tmp, fp);
    fputs("\n", fp);
    fclose(fp);

    spanning_trees(edges, n, v, save_tree);
    free(edges);

    assert(num_trees == n_trees);

    //printf("%u\n", num_trees);
 
    return 0;
}





