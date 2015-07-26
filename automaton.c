/* automata.c
* Generates a cellular automaton with NROWS rows and NCOLS cols given a rule. */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#define NROWS 200
#define NCOLS 201

/* Prints out the cellular automaton */
void print_grid(uint8_t grid[NROWS][NCOLS], FILE* ofp)
{
    int i, j;
    
    for (i = 0; i < NROWS; i++) {
        for (j = 1; j < NCOLS-1; j++) {
            fprintf(ofp, "%d ", grid[i][j]);
        }
        fprintf(ofp, "\n");
    }
}

/* Calls eval_middle to determine whether or not the current cell is
* black (1) or white (0).
*/
void build_automata(uint8_t grid[NROWS][NCOLS], uint8_t rule)
{
    int i, j;
    int8_t nbrs;

    for (i = 0; i < NROWS-1; i++) {
        for (j = 1; j < NCOLS-1; j++) {
            nbrs = (grid[i][j - 1] << 2) | (grid[i][j] << 1) | (grid[i][j + 1]);
            grid[i+1][j] = (rule >> nbrs) & 0x1;
        }
    }
}

/* Reads in an integer representing a rule for cellular automaton generation.
* Sets the starting point for the automaton to be row 0's NCOLS/2-th element.
* Calls build_automata and outputs an image automaton.pbm after it completes.
* To increase automata size, increase NROWS or NCOLS. */
int main(int argc, char* argv[])
{
    uint8_t grid[NROWS][NCOLS];
    int i, j;
    uint8_t rule;
    FILE* ofp;

    fprintf(stdout, "Input an integer between 0 and 255.\n");
    scanf("%2" SCNu8, &rule);

    for (i = 0; i < NROWS; i++)
        for (j = 0; j < NCOLS; j++)
            grid[i][j] = 0;

    grid[0][NCOLS/2] = 1; // initial grid configuration
    build_automata(grid, rule);
    ofp = fopen("automaton.pbm", "w");

    if (ofp == NULL) {
        fprintf(stderr, "Could not create image.\n");
        exit(-1);
    }

    fprintf(ofp, "P1\n");
    fprintf(ofp, "%d %d\n", NROWS-1, NCOLS-2);
    print_grid(grid, ofp);
    fclose(ofp);
    fprintf(stdout, "Output to 'automaton.pbm.'\n");

    return 0;
}