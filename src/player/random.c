#include <string.h>

#include "utils.h"
#include "player.h"

#define UNINITIALIZED 0

struct player {
  char *name;             //name of the strategy
  struct graph_t *graph;  //graph of the player
  size_t position[2];     //position of the two players on the board
  size_t num_walls;       //number of walls in the hand of the player
  enum color_t id;        //id of the player
};

struct player player = {.name= NULL, .graph = NULL, .position = {UNINITIALIZED, UNINITIALIZED},
			.id = -1, .num_walls = UNINITIALIZED};

/**
 * Return the name of the player strategy
 * @return the player name 
 */
char const* get_player_name()
{
  player.name = "randomStrat";
  return player.name;
}

/**
 * Initialize the player with informations given by the server if he is not already initialize :
 * @param id : the id of the player : WHITE or BLACK
 * @param graph : the graph to play with
 * @param num_walls : the number of walls in the hand of the player
 */
void initialize(enum color_t id, struct graph_t* graph, size_t num_walls)
{
  static int is_initialized = 0;

  if (!is_initialized) {
    player.graph = graph_copy(graph);  
  
    player.id = id;

    player.num_walls = num_walls;
  
    player.position[BLACK] = graph->num_vertices;
    player.position[WHITE] = graph->num_vertices;
    graph_free(graph);
  }
  else
    is_initialized = 1;
}

/**
 * Initialize and set a move
 * @param position : the new position of the player
 * @param edge1 : the first edge of the placed wall if a wall has been placed
 * @param edge2 : the second edge of the wall
 * @param id : the id of the player
 * @param movetype : type of the move
 * @return the move set
 */
struct move_t set_move(size_t position, struct edge_t edge1, struct edge_t edge2, size_t id, enum movetype_t movetype)
{
  struct move_t move;
  move.m = position;
  move.e[0] = edge1;
  move.e[1] = edge2;
  move.c = id;
  move.t = movetype;

  return move;
}

/**
 * Return the first move for a player : the player is put on one of his own vertices
 * @return the first move for a player
 */
struct move_t get_first_move()
{
  size_t starting_positions[player.graph->num_vertices];
  size_t nb_pos = 0;

  //searching for available positions
  for (size_t index = 0; index < player.graph->num_vertices; index++) {
    if (is_owned(player.graph, player.id, index) == 1) {
      starting_positions[nb_pos++] = index;
    }
  }

  //choice of an available position
  size_t random_index = (size_t) rand() % nb_pos;
  player.position[player.id] = starting_positions[random_index];

  //move to the position
  struct move_t first_move = set_move(player.position[player.id], no_edge(), no_edge(), player.id, MOVE);
  
  return first_move;
}

/**
 * Update the player with the move of the other player and return a move
 * @param previous_move : the previous move of the other player
 * @return the player move 
 */
struct move_t get_new_move()
{
  size_t nb_pos = 0;
  size_t pos[player.graph->num_vertices];

  size_t position_player1 = player.position[BLACK];
  size_t position_player2 = player.position[WHITE];
  
  //research of available positions
  for (size_t j = 0; j < player.graph->num_vertices; j++) {
    if (can_player_move_to(player.graph, j, player.id, position_player1, position_player2))
      pos[nb_pos++] = j;
  }

  //choice of an available position
  size_t ind = (size_t) rand() % nb_pos;
  player.position[player.id] = pos[ind];

  //move to the position
  struct move_t new_move = set_move(player.position[player.id], no_edge(), no_edge(), player.id, MOVE);
  
  return new_move;
}

/**
 * Return true if the player is not placed yet.
 * @return true if the player is not placed yet.
 */
int is_first_move()
{
  return player.position[player.id] == player.graph->num_vertices;
}

/**
 * Update the graph of the player with the move of the other player
 */
void update(struct move_t previous_move)
{
  if (previous_move.t == MOVE) {
    player.position[previous_move.c] = previous_move.m;
  }

  else if (previous_move.t == WALL) {
    add_wall(player.graph, previous_move.e);
  }
}

/**
 * Return the move of the active player
 * @param previous_move : previous move of the other player
 * @return the player move
 */
struct move_t play(struct move_t previous_move)
{
  if (is_first_move())
    return get_first_move();

  update(previous_move);

  return get_new_move();
}

/**
 * Free the player
 */
void finalize()
{
  graph_free(player.graph);
}
