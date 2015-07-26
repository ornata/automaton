/* automata.c
* Generates a cellular automaton with NROWS rows and NCOLS cols given a rule. */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#define NROWS 200
#define NCOLS 201

/* Prints out the cellular automaton */
void print_grid(int8_t grid[NROWS][NCOLS], FILE* ofp)
{
    int i, j;
    for (i = 0; i < NROWS; i++)
    {
        for (j = 1; j < NCOLS-1; j++)
        {
            fprintf(ofp, "%d ", grid[i][j]);
        }
        fprintf(ofp, "\n");
    }
}

/* Decides the colour for the cell directly below grid[i][j] based
* off 
*     1. The binary value that left, centre and right form
*     2. Whether or bot the corresponding bit in rule is set or not.
*/
int eval_middle(int8_t grid[NCOLS], int index, int8_t rule)
{
    int8_t left, right, centre, nbrs;
    left = grid[index - 1];
    right = grid[index + 1];
    centre = grid[index];
    nbrs = (left << 2) | (centre << 1) | (right);
    return (rule >> nbrs) & 0x1;
}

/* Calls eval_middle to determine whether or not the current cell is
* black (1) or white (0).
*/
void build_automata(int8_t grid[NROWS][NCOLS], int8_t rule)
{
    int i, j;
    for (i = 0; i < NROWS-1; i++) {
        for (j = 1; j < NCOLS-1; j++) {
            grid[i+1][j] = eval_middle(grid[i], j, rule);
        }
    }
}

/* Reads in an integer representing a rule for cellular automaton generation.
* Sets the starting point for the automaton to be row 0's NCOLS/2-th element.
* Calls build_automata and outputs an image automaton.pbm after it completes.
* To increase automata size, increase NROWS or NCOLS. */
int main(int argc, char* argv[])
{
    int8_t grid[NROWS][NCOLS];
    int i, j;
    uint8_t rule;
    FILE* ofp;

    fprintf(stdout, "Input an integer between 0 and 255.\n");
    scanf("%2" SCNu8, &rule);

    // read in the rule from stdin
    for (i = 0; i < NROWS; i++)
        for (j = 0; j < NCOLS; j++)
            grid[i][j] = 0;

    grid[0][NCOLS/2] = 1;
    build_automata(grid, rule);

    ofp = fopen("automaton.pbm", "w");

    if (ofp == NULL)
    {
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