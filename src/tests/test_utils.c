#include "utils.h"
#include "test.h"

int test__is_winning_true_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    ASSERT_TRUE(is_winning(graph1, WHITE, 0) == 1);
    graph_free(graph1);

    return 1;
}

int test__is_winning_false_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    ASSERT_TRUE(is_winning(graph1, BLACK, 4) == 0);
    graph_free(graph1);

    return 1;
}

int test__is_owned_with_owner_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    ASSERT_TRUE(is_owned(graph1, BLACK, 0) == 1);
    graph_free(graph1);
    return 1;
}

int test__is_owned_with_opponent_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    ASSERT_TRUE(is_owned(graph1, WHITE, 0) == 0);
    graph_free(graph1);
    return 1;
}

int test__is_owned_with_a_vertex_in_a_hole_h_board()
{
    struct graph_t* graph1 = get_graph('h', 3);
    ASSERT_TRUE(is_owned(graph1, BLACK, 1) == 0);
    ASSERT_TRUE(is_owned(graph1, WHITE, 1) == 0);
    graph_free(graph1);
    return 1;
}

int test__get_connection_type_square_board()
{
    struct graph_t* graph1 = get_graph('c', 2);
    for (size_t i = 0; i < 4; i++)
        ASSERT_TRUE(get_connection_type(graph1, i, i) == NO_DIRECTION);
    ASSERT_TRUE(get_connection_type(graph1, 0, 1) == EAST);
    ASSERT_TRUE(get_connection_type(graph1, 0, 2) == SOUTH);
    ASSERT_TRUE(get_connection_type(graph1, 0, 3) == NO_DIRECTION);
    ASSERT_TRUE(get_connection_type(graph1, 1, 0) == WEST);
    ASSERT_TRUE(get_connection_type(graph1, 1, 2) == NO_DIRECTION);
    ASSERT_TRUE(get_connection_type(graph1, 1, 3) == SOUTH);
    ASSERT_TRUE(get_connection_type(graph1, 2, 0) == NORTH);
    ASSERT_TRUE(get_connection_type(graph1, 2, 1) == NO_DIRECTION);
    ASSERT_TRUE(get_connection_type(graph1, 2, 3) == EAST);
    ASSERT_TRUE(get_connection_type(graph1, 3, 0) == NO_DIRECTION);
    ASSERT_TRUE(get_connection_type(graph1, 3, 1) == NORTH);
    ASSERT_TRUE(get_connection_type(graph1, 3, 2) == WEST);

    graph_free(graph1);
    return 1;
}

int test__set_connection_type()
{
    struct graph_t* graph1 = get_graph('c', 2);
    set_connection_type(graph1, 0, 1, NO_DIRECTION);
    ASSERT_TRUE(get_connection_type(graph1, 0, 1) == NO_DIRECTION);
    graph_free(graph1);
    return 1;
}

int test__is_connected_with_connected_vertices()
{
    struct graph_t* graph1 = get_graph('c', 2);
    ASSERT_TRUE(is_connected(graph1, 0, 1) == 1);
    graph_free(graph1);
    return 1;
}

int test__is_connected_with_unconnected_vertices()
{
    struct graph_t* graph1 = get_graph('c', 2);
    ASSERT_TRUE(is_connected(graph1, 0, 3) == 0);
    graph_free(graph1);
    return 1;
}

int test__is_connected_with_wall_between_vertices()
{
    struct graph_t* graph1 = get_graph('c', 3);
    struct edge_t e1[2] = {
            { 0, 1 },
            { 3, 4 }
    };
    add_wall(graph1, e1);
    ASSERT_TRUE(is_connected(graph1, 0, 1) == 0);
    graph_free(graph1);
    return 1;
}

int test__is_vertex_in_graph_with_vertex_in_graph()
{
    struct graph_t* graph1 = get_graph('c', 2);
    ASSERT_TRUE(is_vertex_in_graph(graph1, 0) == 1);
    graph_free(graph1);
    return 1;
}

int test__is_vertex_in_graph_with_vertex_out()
{
    struct graph_t* graph1 = get_graph('h', 3);
    ASSERT_TRUE(is_vertex_in_graph(graph1, 1) == 0);
    graph_free(graph1);
    return 1;
}

int test__is_horizontal_connection_with_vertex_east_west()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_TRUE(is_horizontal_connection(graph1, 0, 1));
	graph_free(graph1);
	return 1;
}

int test__is_horizontal_connection_with_vertex_north_south()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_FALSE(is_horizontal_connection(graph1, 0, 4));
	graph_free(graph1);
	return 1;
}

int test__is_horizontal_connection_with_vertex_unconnected()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_FALSE(is_horizontal_connection(graph1, 0, 6));
	graph_free(graph1);
	return 1;
}

int test__is_vertical_connection_with_vertex_east_west()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_FALSE(is_vertical_connection(graph1, 0, 1));
	graph_free(graph1);
	return 1;
}

int test__is_vertical_connection_with_vertex_north_south()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_TRUE(is_vertical_connection(graph1, 0, 4));
	graph_free(graph1);
	return 1;
}

int test__is_vertical_connection_with_vertex_unconnected()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_FALSE(is_vertical_connection(graph1, 0, 6));
	graph_free(graph1);
	return 1;
}

int test__add_wall()
{
	struct edge_t e1[2] = {
			{ 0, 1 },
			{ 2, 3 }
	};

	struct graph_t *graph1 = get_graph('c', 2);

	add_wall(graph1, e1);

	ASSERT_TRUE(get_connection_type(graph1, 0, 1) == POINT_TO_SOUTH);
	ASSERT_TRUE(get_connection_type(graph1, 2, 3) == POINT_TO_NORTH);

	struct edge_t e2[2] = {
			{ 1, 3 },
			{ 0, 2 }
	};

	add_wall(graph1, e2);
	ASSERT_TRUE(get_connection_type(graph1, 1, 3) == POINT_TO_WEST);
	ASSERT_TRUE(get_connection_type(graph1, 0, 2) == POINT_TO_EAST);

	struct edge_t e1bis[2] = {
			{ 2, 3 },
			{ 0, 1 },
	};

	struct graph_t *graph2 = get_graph('c', 2);

	add_wall(graph2, e1bis);

	ASSERT_TRUE(get_connection_type(graph2, 0, 1) == POINT_TO_SOUTH);
	ASSERT_TRUE(get_connection_type(graph2, 2, 3) == POINT_TO_NORTH);

	struct edge_t e2bis[2] = {
			{ 0, 2 },
			{ 1, 3 },
	};

	add_wall(graph2, e2bis);
	ASSERT_TRUE(get_connection_type(graph2, 1, 3) == POINT_TO_WEST);
	ASSERT_TRUE(get_connection_type(graph2, 0, 2) == POINT_TO_EAST);

	graph_free(graph1);
	graph_free(graph2);

	return 1;
}

int test__is_path_existing_with_a_plain_square_board()
{
	struct graph_t* graph1 = get_graph('c', 4);
	int visited1[16] = { 0 };
	ASSERT_TRUE(is_path_existing(graph1, visited1, 0, BLACK, 5));
	int visited2[16] = { 0 };
	ASSERT_TRUE(is_path_existing(graph1, visited2, 5, WHITE, 1));
	graph_free(graph1);
	return 1;
}

int test__is_path_existing_without_a_path()
{
	struct graph_t* graph1 = get_graph('c', 4);
	struct edge_t e1[2] = {
			{ 0, 4 },
			{ 1, 5 }
	};
	add_wall(graph1, e1);
	struct edge_t e2[2] = {
			{ 2, 6 },
			{ 3, 7 }
	};
	add_wall(graph1, e2);

	int visited1[16] = { 0 };
	ASSERT_FALSE(is_path_existing(graph1, visited1, 0, BLACK, 12));
	int visited2[16] = { 0 };
	ASSERT_FALSE(is_path_existing(graph1, visited2, 12, WHITE, 0));
	int visited3[16] = { 0 };
	ASSERT_TRUE(is_path_existing(graph1, visited3, 8, BLACK, 12));
	graph_free(graph1);

	return 1;
}

int test__is_player_blocked_with_a_plain_square_board()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_FALSE(is_player_blocked(graph1, 0, BLACK, 5));
	ASSERT_FALSE(is_player_blocked(graph1, 5, WHITE, 1));
	graph_free(graph1);
	return 1;
}

int test__is_player_blocked_without_a_path()
{
	struct graph_t* graph1 = get_graph('c', 4);
	struct edge_t e1[2] = {
			{ 0, 4 },
			{ 1, 5 }
	};
	add_wall(graph1, e1);
	struct edge_t e2[2] = {
			{ 2, 6 },
			{ 3, 7 }
	};
	add_wall(graph1, e2);

	ASSERT_TRUE(is_player_blocked(graph1, 0, BLACK, 12));
	ASSERT_TRUE(is_player_blocked(graph1, 12, WHITE, 0));
	ASSERT_FALSE(is_player_blocked(graph1, 8, BLACK, 12));
	graph_free(graph1);

	return 1;
}

int test__can_add_wall()
{
    struct edge_t e1[2] = {
            { 0, 1 },
            { 2, 3 }
    };

    struct graph_t *graph1 = get_graph('c', 2);
    ASSERT_TRUE(can_add_wall(graph1, e1, 0, 1));


    struct edge_t e21[2] = {
            { 0, 3 },
            { 1, 4 }
    };

    struct graph_t *graph2 = get_graph('c', 3);
    ASSERT_TRUE(can_add_wall(graph2, e21, 1, 7));

    add_wall(graph2, e21);

    struct edge_t e22[2] = {
            { 2, 5 },
            { 1, 4 }
    };
    ASSERT_FALSE(can_add_wall(graph2, e22, 1, 7));

    struct edge_t e23[2] = {
            { 3, 4 },
            { 6, 7 }
    };
    ASSERT_TRUE(can_add_wall(graph2, e23, 1, 7));
    add_wall(graph2, e23);

    struct edge_t e24[2] = {
            { 1, 4 },
            { 2, 5 }
    };
    ASSERT_FALSE(can_add_wall(graph2, e24, 1, 7));

    struct edge_t e31[2] = {
            { 4, 8 },
            { 5, 9 }
    };

    struct graph_t *graph3 = get_graph('c', 4);
    ASSERT_TRUE(can_add_wall(graph3, e31, 0, 15));
    add_wall(graph3, e31);

    struct graph_t* graph4 = get_graph('c', 4);
    struct edge_t e41[2] = {
			{ 3, 2 },
			{ 7, 6 }
    };
    struct edge_t e42[2] = {
			{ 10, 9 },
			{ 6, 5 }
    };
    struct edge_t e43[2] = {
			{ 10, 14 },
			{ 9, 13 }
    };
	ASSERT_TRUE(can_add_wall(graph4, e41, 2, 13));
    add_wall(graph4, e41);
	ASSERT_TRUE(can_add_wall(graph4, e42, 2, 12));
    add_wall(graph4, e42);
	ASSERT_TRUE(can_add_wall(graph4, e43, 2, 13));
	add_wall(graph4, e43);

	struct graph_t* graph5 = get_graph('c', 5);
	struct edge_t e51[2] = {
            { 10, 15 },
            { 11, 16 }
	};
	struct edge_t e52[2] = {
            { 12, 17 },
            { 13, 18 }
	};
	struct edge_t e53[2] = {
            { 11, 12 },
            { 16, 17 }
	};
	ASSERT_TRUE(can_add_wall(graph5, e51, 1, 20));
	add_wall(graph5, e51);
	ASSERT_TRUE(can_add_wall(graph5, e52, 1, 20));
	add_wall(graph5, e52);
    ASSERT_TRUE(can_add_wall(graph5, e53, 1, 20));
    add_wall(graph5, e53);

    struct graph_t* graph6 = get_graph('c', 5);
    struct edge_t e61[2] = {
            { 6, 7 },
            { 11, 12 }
    };
    struct edge_t e62[2] = {
            { 11, 16 },
            { 12, 17 }
    };
    struct edge_t e63[2] = {
            { 16, 17 },
            { 21, 22 }
    };
    ASSERT_TRUE(can_add_wall(graph6, e61, 1, 20));
    add_wall(graph6, e61);
    ASSERT_TRUE(can_add_wall(graph6, e62, 1, 20));
    add_wall(graph6, e62);
    ASSERT_TRUE(can_add_wall(graph6, e63, 1, 20));
    add_wall(graph6, e63);

    struct graph_t* graph7 = get_graph('c', 4);
    struct edge_t e71[2] = {
            { 1, 5 },
            { 2, 6 }
    };
    struct edge_t e72[2] = {
            { 6, 10 },
            { 7, 11 }
    };
    struct edge_t e73[2] = {
            { 10, 14 },
            { 11, 15 }
    };
    struct edge_t e74[2] = {
            { 8, 9 },
            { 12, 13 }
    };
    struct edge_t e75[2] = {
            { 8, 12 },
            { 9, 13 }
    };
    ASSERT_TRUE(can_add_wall(graph7, e71, 0, 15));
    add_wall(graph7, e71);
    ASSERT_TRUE(can_add_wall(graph7, e72, 0, 15));
    add_wall(graph7, e72);
    ASSERT_TRUE(can_add_wall(graph7, e73, 0, 15));
    add_wall(graph7, e73);
    ASSERT_TRUE(can_add_wall(graph7, e74, 0, 15));
    add_wall(graph7, e74);
    ASSERT_FALSE(can_add_wall(graph7, e75, 0, 15))

	struct graph_t* graph8 = get_graph('c', 4);
	struct edge_t e81[2] = {
			{ 0, 1 },
			{ 4, 5 }
	};
	struct edge_t e82[2] = {
			{ 0, 4 },
			{ 1, 5 }
	};
	add_wall(graph8, e81);
	ASSERT_FALSE(can_add_wall(graph8, e82, 3, 15));

	struct graph_t* graph9 = get_graph('c', 4);
	struct edge_t e91[2] = {
			{ 4, 8 },
			{ 7, 11 }
	};
	struct edge_t e92[2] = {
			{ 4, 8 },
			{ 6, 10 }
	};
	ASSERT_FALSE(can_add_wall(graph9, e91, 0, 12));
	ASSERT_FALSE(can_add_wall(graph9, e92, 0, 12));

	struct edge_t e93[2] = {
			{ 4, 8 },
			{ 5, 9 }
	};
	ASSERT_TRUE(can_add_wall(graph9, e93, 0, 12));
	add_wall(graph9, e93);
	ASSERT_FALSE(can_add_wall(graph9, e91, 0, 12));
	ASSERT_FALSE(can_add_wall(graph9, e92, 0, 12));

	graph_free(graph1);
    graph_free(graph2);
    graph_free(graph3);
    graph_free(graph4);
    graph_free(graph5);
    graph_free(graph6);
    graph_free(graph7);
    graph_free(graph8);
	graph_free(graph9);

    return 1;
}

int test_can_player_move_to()
{
    struct graph_t* graph1 = get_graph('c', 3);
    ASSERT_TRUE(can_player_move_to(graph1, 1, 0, 4, 2) == 1);
    ASSERT_TRUE(can_player_move_to(graph1, 0, 1, 1, 6) == 0);
    ASSERT_TRUE(can_player_move_to(graph1, 2, 0, 0, 1) == 1);

    struct graph_t* graph2 = get_graph('c', 4);
    struct edge_t e1[2] = {
            { 4, 5 },
            { 8, 9 }
    };
    add_wall(graph2, e1);
    ASSERT_TRUE(can_player_move_to(graph2, 9, 0, 6, 5) == 1);
    ASSERT_TRUE(can_player_move_to(graph2, 1, 0, 6, 5) == 1);
    ASSERT_TRUE(can_player_move_to(graph2, 4, 0, 6, 5) == 0);

    struct graph_t* graph3 = get_graph('c', 5);
    ASSERT_TRUE(can_player_move_to(graph3, 15, BLACK, 11, 10) == 1);

    graph_free(graph1);
    graph_free(graph2);
    graph_free(graph3);

    return 1;
}

int test_is_move_legal()
{
    struct graph_t* graph1 = get_graph('c', 3);

    struct move_t m1 = { 1, { no_edge(), no_edge() }, MOVE, BLACK };
    ASSERT_TRUE(is_move_legal(graph1, &m1, 4, 2, 1) == 1);

    struct move_t m2 = { 0, { no_edge(), no_edge() }, MOVE, WHITE };
    ASSERT_TRUE(is_move_legal(graph1, &m2, 1, 6, 1) == 0);

    struct move_t m3 = { 2, { no_edge(), no_edge() }, MOVE, BLACK };
    ASSERT_TRUE(is_move_legal(graph1, &m3, 0, 1, 1) == 1);


    struct graph_t* graph2 = get_graph('c', 4);
    struct edge_t e1[2] = {
            { 4, 5 },
            { 8, 9 }
    };
    add_wall(graph2, e1);

    struct move_t m4 = { 9, { no_edge(), no_edge() }, MOVE, BLACK };
    ASSERT_TRUE(is_move_legal(graph2, &m4, 6, 5, 1) == 1);

    struct move_t m5 = { 1, { no_edge(), no_edge() }, MOVE, BLACK };
    ASSERT_TRUE(is_move_legal(graph2, &m5, 6, 5, 1) == 1);

    struct move_t m6 = { 4, { no_edge(), no_edge() }, MOVE, BLACK };
    ASSERT_TRUE(is_move_legal(graph2, &m6, 6, 5, 1) == 0);

    struct move_t m7 = { 4, { no_edge(), no_edge() }, WALL, BLACK };
    ASSERT_TRUE(is_move_legal(graph2, &m7, 6, 5, 0) == 0);

    graph_free(graph1);
    graph_free(graph2);

    return 1;
}

void tests__utils_functions()
{
    TEST_FUNCTION(test__is_winning_true_square_board);
    TEST_FUNCTION(test__is_winning_false_square_board);
    TEST_FUNCTION(test__is_owned_with_owner_square_board);
    TEST_FUNCTION(test__is_owned_with_opponent_square_board);
    TEST_FUNCTION(test__is_owned_with_a_vertex_in_a_hole_h_board);
    TEST_FUNCTION(test__get_connection_type_square_board);
    TEST_FUNCTION(test__set_connection_type);
    TEST_FUNCTION(test__is_connected_with_connected_vertices);
    TEST_FUNCTION(test__is_connected_with_unconnected_vertices);
    TEST_FUNCTION(test__is_connected_with_wall_between_vertices);
    TEST_FUNCTION(test__is_vertex_in_graph_with_vertex_in_graph);

    // Test not working, was already commented
    // TEST_FUNCTION(test__is_vertex_in_graph_with_vertex_out);

	TEST_FUNCTION(test__is_horizontal_connection_with_vertex_east_west);
	TEST_FUNCTION(test__is_horizontal_connection_with_vertex_north_south);
	TEST_FUNCTION(test__is_horizontal_connection_with_vertex_unconnected);
	TEST_FUNCTION(test__is_vertical_connection_with_vertex_east_west);
	TEST_FUNCTION(test__is_vertical_connection_with_vertex_north_south);
	TEST_FUNCTION(test__is_vertical_connection_with_vertex_unconnected);
	TEST_FUNCTION(test__add_wall);
	TEST_FUNCTION(test__is_path_existing_with_a_plain_square_board);
	TEST_FUNCTION(test__is_path_existing_without_a_path);
	TEST_FUNCTION(test__is_player_blocked_with_a_plain_square_board);
	TEST_FUNCTION(test__is_player_blocked_without_a_path);
    TEST_FUNCTION(test__can_add_wall);
    TEST_FUNCTION(test_can_player_move_to);
    TEST_FUNCTION(test_is_move_legal);
}