#ifndef _QUOR_GRAPH_H_
#define _QUOR_GRAPH_H_

#include <stddef.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spblas.h>

#include "move.h"

struct graph_t {
  size_t num_vertices;  // Number of vertices in the graph
  gsl_spmatrix* t;      // Sparse matrix of size n*n,
                        // t[i][j] > 0 means there is an edge from i to j
                        // t[i][j] == 1 means that j is NORTH of i
                        // t[i][j] == 2 means that j is SOUTH of i
                        // t[i][j] == 3 means that j is WEST  of i
                        // t[i][j] == 4 means that j is EAST  of i
  gsl_spmatrix* o;      // Sparse matrix of size 2*n, one line per player
                        // o[p][i] == 1 means that the vertex i is owned by p
  size_t p[2];          // Positions of the players, SIZE_MAX if none
};

#endif // _QUOR_GRAPH_H_