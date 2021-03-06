#include "utils.h"

struct graph_t* graph_copy(struct graph_t* graph)
{
    struct graph_t* cp = malloc(sizeof(struct graph_t));

    cp->num_vertices = graph->num_vertices;

    cp->t = gsl_spmatrix_uint_alloc(graph->t->size1, graph->t->size2);
    gsl_spmatrix_uint_memcpy(cp->t, graph->t);

    cp->o = gsl_spmatrix_uint_alloc(graph->o->size1, graph->o->size2);
    gsl_spmatrix_uint_memcpy(cp->o, graph->o);

    return cp;
}

void graph_free(struct graph_t *graph)
{
  gsl_spmatrix_uint_free(graph->t);
  gsl_spmatrix_uint_free(graph->o);
  free(graph);
}

void *load_library(char *path)
{
  void *lib;
  lib = dlopen(path, RTLD_LAZY);
  if (!lib)
    return NULL;
  return lib;
}

void *load_function(void *lib, const char *name)
{
  void *function = dlsym(lib, name);
  if (!function)
    return NULL;
  return function;
}

enum color_t get_next_player(enum color_t id)
{
  return (id == BLACK) ? WHITE : BLACK;
}

const char *get_name_type_player(enum color_t id)
{
    return id == BLACK ? "BLACK" : "WHITE";
}

int is_owned(struct graph_t *graph, enum color_t player_id, size_t position)
{
	if (!is_vertex_in_graph(graph, position))
		return 0;
    return gsl_spmatrix_uint_get(graph->o, player_id, position) == 1;
}

int is_winning(struct graph_t *graph, enum color_t player_id, size_t position)
{
	if (!is_vertex_in_graph(graph, position))
		return 0;
  	return is_owned(graph, get_next_player(player_id), position);
}

int get_connection_type(struct graph_t* graph, size_t from, size_t to)
{
    return gsl_spmatrix_uint_get(graph->t, from, to);
}

void set_connection_type(struct graph_t *graph, size_t from, size_t to, int connection)
{
    gsl_spmatrix_uint_set(graph->t, from, to, connection);
}

int is_connected(struct graph_t* graph, size_t from, size_t to)
{
	if (!(is_vertex_in_graph(graph, from) && is_vertex_in_graph(graph, to)))
		return 0;
    int connection_type = get_connection_type(graph, from, to);
    return MAX_DIRECTION > connection_type  && connection_type > NO_DIRECTION;
}

int is_vertex_in_graph(struct graph_t* graph, size_t vertex)
{
    return vertex < graph->t->size1;
}

int is_horizontal_connection(struct graph_t* graph, size_t from, size_t to)
{
	if (!(is_vertex_in_graph(graph, from) && is_vertex_in_graph(graph, to)))
		return 0;
    int relation = get_connection_type(graph, from, to);
    return relation == EAST || relation == WEST;
}

int is_vertical_connection(struct graph_t* graph, size_t from, size_t to)
{
	if (!(is_vertex_in_graph(graph, from) && is_vertex_in_graph(graph, to)))
		return 0;
    int relation = get_connection_type(graph, from, to);
    return relation == NORTH || relation == SOUTH;
}

enum wall_orientation_t get_wall_orientation_opposed(enum wall_orientation_t orientation) 
{
    switch (orientation)
    {
    case POINT_TO_NORTH:
        return POINT_TO_SOUTH;
    case POINT_TO_SOUTH:
        return POINT_TO_NORTH;
    case POINT_TO_EAST:
        return POINT_TO_WEST;
    case POINT_TO_WEST:
        return POINT_TO_EAST;
    default:
        return NOT_CONNECTED;
    }
}

void add_wall(struct graph_t* graph, struct edge_t e[])
{
    if (is_horizontal_connection(graph, e[0].fr, e[0].to))
    {
        enum wall_orientation_t orientation_e1 = e[0].fr > e[1].fr ? POINT_TO_NORTH : POINT_TO_SOUTH;
        enum wall_orientation_t orientation_e2 = get_wall_orientation_opposed(orientation_e1);
        set_connection_type(graph, e[0].fr, e[0].to, orientation_e1);
        set_connection_type(graph, e[0].to, e[0].fr, orientation_e1);
        set_connection_type(graph, e[1].fr, e[1].to, orientation_e2);
        set_connection_type(graph, e[1].to, e[1].fr, orientation_e2);
    }
    else
    {
        enum wall_orientation_t orientation_e1 = e[0].fr > e[1].fr ? POINT_TO_WEST : POINT_TO_EAST;
        enum wall_orientation_t orientation_e2 = get_wall_orientation_opposed(orientation_e1);
        set_connection_type(graph, e[0].fr, e[0].to, orientation_e1);
        set_connection_type(graph, e[0].to, e[0].fr, orientation_e1);
        set_connection_type(graph, e[1].fr, e[1].to, orientation_e2);
        set_connection_type(graph, e[1].to, e[1].fr, orientation_e2);
    }
}

int is_path_existing(struct graph_t *graph, int visited[], size_t position, enum color_t player_id, size_t opponent_position)
{
    visited[position] = 1;

    if (is_winning(graph, player_id, position) && position != opponent_position)
        return 1;

    for (size_t j = 0; j < graph->t->size1; j++)
		if (is_connected(graph, position, j) && !visited[(int)j] && is_path_existing(graph, visited, j, player_id, opponent_position))
			return 1;

    return 0;
}

int is_player_blocked(struct graph_t* graph, size_t position, enum color_t player_id, size_t opponent_position)
{
	if (!is_vertex_in_graph(graph, position))
		return 1;

    int visited[graph->t->size1];
    for (size_t i = 0; i < graph->t->size1; i++)
        visited[i] = 0;

    return !is_path_existing(graph, visited, position, player_id, opponent_position);
}

int is_overlapping_wall(struct graph_t* graph, size_t min_e1, size_t max_e1, size_t min_e2, size_t max_e2)
{
    return (get_connection_type(graph, min_e1, min_e2) == POINT_TO_EAST && get_connection_type(graph, max_e1, max_e2) == POINT_TO_WEST)
        || (get_connection_type(graph, min_e1, min_e2) == POINT_TO_SOUTH && get_connection_type(graph, max_e1, max_e2) == POINT_TO_NORTH);
}

// size of e is 2
int can_add_wall(struct graph_t* graph, struct edge_t e[], size_t p1_position, size_t p2_position)
{
	// Vertices must be on graph
	if (!(is_vertex_in_graph(graph, e[0].fr) && is_vertex_in_graph(graph, e[0].to)
			&& is_vertex_in_graph(graph, e[1].fr) && is_vertex_in_graph(graph, e[1].to)))
		return 0;

	
	// Check if vertices are connected AND if it won't overlap an other wall
	if (!(is_connected(graph, e[0].fr, e[0].to) && is_connected(graph, e[1].fr, e[1].to)))
		return 0;

	size_t max_e1 = e[0].fr > e[0].to ? e[0].fr : e[0].to;
	size_t min_e1 = e[0].to < e[0].fr ? e[0].to : e[0].fr;
	size_t max_e2 = e[1].fr > e[1].to ? e[1].fr : e[1].to;
	size_t min_e2 = e[1].to < e[1].fr ? e[1].to : e[1].fr;

	// Check vertices relation
	if (is_horizontal_connection(graph, e[0].fr, e[0].to) && is_horizontal_connection(graph, e[1].fr, e[1].to))
	{
		if (!((is_connected(graph, max_e1, max_e2) <= is_vertical_connection(graph, max_e1, max_e2))
				|| (is_connected(graph, min_e1, min_e2) <= is_vertical_connection(graph, min_e1, min_e2)))
				|| is_overlapping_wall(graph, min_e1, max_e1, min_e2, max_e2))
			return 0;

		if (!is_connected(graph, max_e1, max_e2) && get_connection_type(graph, max_e1, max_e2) < POINT_TO_NORTH)
			return 0;

		if (!is_connected(graph, min_e1, min_e2) && get_connection_type(graph, min_e1, min_e2) < POINT_TO_NORTH)
			return 0;

	}
	else if (is_vertical_connection(graph, e[0].fr, e[0].to) && is_vertical_connection(graph, e[1].fr, e[1].to)) // North-South relation
	{
		if (!((is_connected(graph, max_e1, max_e2) <= is_horizontal_connection(graph, max_e1, max_e2))
				|| (is_connected(graph, min_e1, min_e2) <= is_horizontal_connection(graph, min_e1, min_e2)))
				|| is_overlapping_wall(graph, min_e1, max_e1, min_e2, max_e2))
			return 0;

		if (!is_connected(graph, max_e1, max_e2) && get_connection_type(graph, max_e1, max_e2) < POINT_TO_NORTH)
			return 0;

		if (!is_connected(graph, min_e1, min_e2) && get_connection_type(graph, min_e1, min_e2) < POINT_TO_NORTH)
			return 0;
	}
	else {
	  return 0;
	}
	// Check if players aren't blocked
	struct graph_t* graph_with_wall = graph_copy(graph);
	add_wall(graph_with_wall, e);
	if (is_player_blocked(graph_with_wall, p1_position, BLACK, p2_position)
			|| is_player_blocked(graph_with_wall, p2_position, WHITE, p1_position))
	{
		graph_free(graph_with_wall);
		return 0;
	}
	graph_free(graph_with_wall);

	// The wall doesn't violate any rules, hence it's a legal move
	return 1;
}

int is_vertex_available(size_t vertex, size_t p1_position, size_t p2_position)
{
    return vertex != p1_position && vertex != p2_position;
}

int is_connection_existing(struct graph_t* graph, size_t i, int direction)
{
    for (size_t j = 0; j < graph->t->size1; j++)
        if (get_connection_type(graph, i, j) == direction)
            return 1;

    return 0;
}

int can_player_move_to(struct graph_t* graph, size_t vertex, enum color_t active_player, size_t p1_position, size_t p2_position)
{
    // Check if position is on the board
    if (!is_vertex_in_graph(graph, vertex))
        return 0;

    // Check if the opponent isn't already on it
    if (!is_vertex_available(vertex, p1_position, p2_position))
        return 0;

    size_t initial_position = active_player == 0 ? p1_position : p2_position;
    size_t opponent_position = active_player == 0 ? p2_position : p1_position;

    // The position is directly accessible, hence the player can move to it
    if (is_connected(graph, initial_position, vertex))
        return 1;

    // The position isn't directly accessible
    // Check if player are facing each other, and can jump to the position
    if (is_connected(graph, initial_position, opponent_position))
    {
        // Check if the opponent is next to the position
        if (!is_connected(graph, opponent_position, vertex))
            return 0;

        // The opponent is next to the position

        // Check if position is behind the opponent
        if (get_connection_type(graph, initial_position, opponent_position) == get_connection_type(graph, opponent_position, vertex))
            return 1;

        // There's a wall/edge of the board behind the opponent player
        if (!is_connection_existing(graph, opponent_position, get_connection_type(graph, initial_position, opponent_position)))
            return 1;
    }

    /*
    size_t new_position_p1 = active_player == BLACK ? vertex : p1_position;
	size_t new_position_p2 = active_player == WHITE ? vertex : p2_position;
	// Check if players aren't blocked
	if (is_player_blocked(graph, new_position_p1, BLACK, new_position_p2)
			|| is_player_blocked(graph, new_position_p2, WHITE, new_position_p1))
		return 0;
	*/
    // The position isn't accessible
    return 0;
}

int is_move_legal(struct graph_t* graph, struct move_t* move, size_t p1_position, size_t p2_position, size_t num_walls)
{
    if (move->t == WALL)
        return can_add_wall(graph, move->e, p1_position, p2_position) && num_walls != 0 && num_walls != 0;
    else if (move->t == MOVE)
        return can_player_move_to(graph, move->m, move->c, p1_position, p2_position);
    else
        return 0;
}
