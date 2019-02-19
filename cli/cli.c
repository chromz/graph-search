// Rodrigo Custodio

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
			a_star_solve(board, sudoku_goaltest, sudoku_expand,
				     sudoku_path_cost, sudoku_heuristic,
				     sudoku_free_board_void);
		struct sudoku_board *final = result->elm;
		if (final == NULL) {
			printf("Error\n");
		}
		printf("Solution: \n");
		sudoku_print_board(final);
		sudoku_free_board(&board);
	} else {
		printf(usage, argv[0]);
		return 1;
	}
	return 0;
}
