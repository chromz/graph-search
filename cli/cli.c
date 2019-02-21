// Rodrigo Custodio

#include "fifteen/fifteen.h"
#include "sudoku/sudoku.h"

#include "algorithms/a_star.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
	char *usage = "USAGE: %s (sudoku|sixteen) <source>\n";
	if (argc != 3) {
		printf(usage, argv[0]);
		return 1;
	}
	if (strcmp(argv[1], "sudoku") == 0) {
		struct sudoku_board *board = sudoku_read(argv[2]);
		if (board == NULL) {
			printf("ERROR: Invalid input\n");
			printf(usage, argv[0]);
			return 1;
		}
		printf("Solving sudoku... \n");
		sudoku_print_board(board);
		struct a_star_node *result = 
			a_star_solve(board, 
				     true,
				     sudoku_goaltest,
				     sudoku_expand, sudoku_compare,
				     sudoku_path_cost, sudoku_heuristic,
				     sudoku_free_board_void);
		struct sudoku_board *final = result->elm;
		if (final == NULL) {
			printf("Error\n");
		}
		printf("Solution: \n");
		sudoku_print_board(final);
		sudoku_free_board(&board);
		free_a_star_node(result, sudoku_free_board_void);
	} else if (strcmp(argv[1], "fifteen") == 0) {
		struct fifteen_board *board = fifteen_read(argv[2]);
		if (board == NULL) {
			printf("ERROR: Invalid input \n");
			printf(usage, argv[0]);
			return 1;
		}
		printf("Checking if board is solvable \n");
		fifteen_print_board(board);
		if (!fifteen_is_valid(board)) {
			printf("Puzzle is not solvable\n");
			fifteen_free_board(&board);
			return 1;
		}
		printf("Puzzle is solvable :)\n");
		struct a_star_node *result = 
			a_star_solve(board, 
				     false,
				     fifteen_goaltest,
				     fifteen_expand, fifteen_compare,
				     fifteen_path_cost, fifteen_heuristic,
				     fifteen_free_board_void);
		struct fifteen_board *final = result->elm;
		if (final == NULL) {
			printf("Error\n");
		}
		printf("Solution: \n");
		fifteen_print_board(final);
		fifteen_free_board(&board);
		free_a_star_node(result, fifteen_free_board_void);
	} else {
		printf(usage, argv[0]);
		return 1;
	}
	return 0;
}
