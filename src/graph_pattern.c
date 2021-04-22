#include "graph_pattern.h"

int c_constraint(int size)
{
  return size > 0;
}

int t_constraint(int size)
{
  return size > 0 && size % 3 == 0;
}

int h_constraint(int size)
{
  return t_constraint(size);
}

int s_constraint(int size)
{
  return size > 0 && size % 5 == 0;
}

int abide_graph_constraint(char type, int size)
{
  switch (type)
  {
  case 'c':
    return c_constraint(size);
  case 't':
  case 'h':
    return t_constraint(size);
  case 's':
    return s_constraint(size);
  default:
    return 0;
  }
}

int get_direction_square(size_t i, size_t j, size_t m)
{
  if (i == j + 1 && (i / m == j / m))
  {
    return WEST;
  }
  if (i == j - 1 && (i / m == j / m))
  {
    return EAST;
  }
  if (i + m == j)
  {
    return SOUTH;
  }
  if (i - m == j)
  {
    return NORTH;
  }
  return NO_DIRECTION;
}

gsl_spmatrix_uint *fill_graph(size_t m, int (*is_in_graph)(size_t, size_t, size_t))
{
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(m * m, m * m);
  for (size_t x = 0; x < m; x++)
  {
    for (size_t j = 0; j < m; j++)
    {
      if (is_in_graph(j, x, m))
      {
        for (size_t k = 0; k < m * m; k++)
        {
          if (is_in_graph(k%m, k/m, m))
            gsl_spmatrix_uint_set(matrix, x * m + j, k, get_direction_square(x * m + j, k, m));
        }
      }
    }
  }
  return matrix;
}

int square(size_t x, size_t y, size_t m)
{
  (void)x;
  (void)y;
  (void)m;
  return 1;
}

gsl_spmatrix_uint *square_graph(size_t m)
{
  return fill_graph(m, square);
}

int is_in_rectangle(size_t x, size_t y, size_t begin_x, size_t begin_y, size_t size_x, size_t size_y)
{
  return (begin_x <= x && x < begin_x + size_x) && (begin_y <= y && y < begin_y + size_y);
}

int toric(size_t x, size_t y, size_t m)
{
  return !is_in_rectangle(x, y, (m / 3), (m / 3), (m / 3), (m / 3));
}

gsl_spmatrix_uint *t_graph(size_t m)
{
  return fill_graph(m, toric);
}

int h(size_t x, size_t y, size_t m)
{
  return !is_in_rectangle(x, y, m / 3, 0 , m/3, m/3) && !is_in_rectangle(x, y, m/3, 2*m/3, m/3, m/3);
}

gsl_spmatrix_uint *h_graph(size_t m)
{
  return fill_graph(m, h);
}

int snake(size_t x, size_t y, size_t m)
{
  return !is_in_rectangle(x, y, m / 5, 0, 4 * m / 5, m / 5) && !is_in_rectangle(x, y, m / 5, 4 * m / 5, 4 * m / 5, m / 5) && !is_in_rectangle(x, y, 0 , 2 * m / 5, 4 * m / 5, m / 5);
}

gsl_spmatrix_uint *s_graph(size_t m)
{
  return fill_graph(m, snake);
}

gsl_spmatrix_uint *matrix_position(size_t m)
{
  size_t num_vertices = m * m;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(2, num_vertices);

  for (size_t i = 0; i < m; i++)
  {
    gsl_spmatrix_uint_set(matrix, 0, i, 1);
    gsl_spmatrix_uint_set(matrix, 1, num_vertices - 1 - i, 1);
  }

  return matrix;
}

gsl_spmatrix_uint *h_matrix_position(size_t m)
{
  size_t num_vertices = m * m;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(2, num_vertices);

  for (size_t i = 0; i < m; i++)
  {
    if (i < m / 3 || i >= 2 * m / 3)
    {
      gsl_spmatrix_uint_set(matrix, 0, i, 1);
      gsl_spmatrix_uint_set(matrix, 1, num_vertices - 1 - i, 1);
    }
  }

  return matrix;
}


gsl_spmatrix_uint *s_matrix_position(size_t m)
{
  size_t num_vertices = m * m;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(2, num_vertices);

  for (size_t i = 0; i < m / 5; i++)
  {
    if (i < m / 3 || i >= 2 * m / 3)
    {
      gsl_spmatrix_uint_set(matrix, 0, i, 1);
      gsl_spmatrix_uint_set(matrix, 1, num_vertices - 1 - 4 * m / 5 - i, 1);
    }
  }
  return matrix;
}

struct graph_functions {
  gsl_spmatrix_uint *(*get_matrix_relation)(size_t);
  gsl_spmatrix_uint *(*get_matrix_position)(size_t);
};

struct graph_functions get_graph_functions(char type)
{
  static struct graph_functions graph_functions[] = {
      {square_graph, matrix_position},
      {t_graph, matrix_position},
      {h_graph, h_matrix_position},
      {s_graph, s_matrix_position},
      {NULL,NULL}
  };
  switch (type)
  {
  case 'c':
    return graph_functions[0];
  case 't':
    return graph_functions[1];
  case 'h':
    return graph_functions[2];
  case 's':
    return graph_functions[3];
  default:
    return graph_functions[4];
  }
}


struct graph_t *get_graph(char type, size_t width)
{
  struct graph_functions graph_functions = get_graph_functions(type);
  if (graph_functions.get_matrix_position == NULL || graph_functions.get_matrix_relation == NULL)
    return NULL;
  struct graph_t *graph = malloc(sizeof(struct graph_t));
  graph->num_vertices = width * width;
  graph->t = graph_functions.get_matrix_relation(width);
  graph->o = graph_functions.get_matrix_position(width);
  return graph;
}
