#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "reader.h"
#include "grid.h"

int main(int argc, char **argv) {

    int     limit   = -1,
            message = 0;

    // Read the limit of cells
    message = interpretArguments(argc, argv, &limit);
    if (limit < 0) {
        printf("How many cells do you want to work with (0 - unlimited): ");
        readIntRangeDownTo(&limit, 0);
        printf("\n");
    }

    // Check for how many cells could be drawn
    Env     terminal = envGet();
    Canvas  canvas = canvasInit(terminal);

    canvasErase(canvas);

    Grid    grid;
    if (limit)
        grid = gridInit(canvas, limit);
    else
        grid = gridInitMax(canvas);

    gridDraw(&grid);
	gridHeadersDraw(&grid);

    char command = getChar();
    Cell *activeCell 	= NULL,
		 *formulaCell 	= NULL,
		 *targetCell 	= NULL;

    while (command != ':') {
        // UI for navigating between cells
        switch(command) {
            // Cursor keys
            case 68:
                // Left
                gridChangePosition(&grid, grid.x-1, grid.y);
                break;
            case 65:
                // Up
                gridChangePosition(&grid, grid.x, grid.y-1);
                break;
            case 67:
                // Right
                gridChangePosition(&grid, grid.x+1, grid.y);
                break;
            case 66:
                // Bottom
                gridChangePosition(&grid, grid.x, grid.y+1);
                break;
            case 27:
            case 91:
                // Command bytes that go along with the cursor keys
                
                break;
            // Enter - new line
            case '\n':
                // Start reading input
                if (!activeCell && !formulaCell) {
                    activeCell = &(grid.cells[grid.position]);
                    cellReadValue(&activeCell, &grid);
                } else {
                    activeCell = NULL;
                }
                break;
			case '=':
				formulaCell = &(grid.cells[grid.position]);
				// Start reading references to other functions
				printf("=");
				int row = 1;
				readInt(&row, 0, grid.rows-3);
				char col = 'A';
				col = getChar();
				char namesC[][3] = {"A", "B", "C", "D",
									"E", "F", "G", "H",
									"I", "J", "K", "L",
									"M", "N", "R", "P"};
				while (col < 'A' || col > namesC[grid.columns-1][0]) {
					col = getChar();
				}
				printf("%c", col);
				fflush(stdout);
				// Find the cell the user entered
				int x = (int)(col - 'A'),
					y = row;

				targetCell = &(grid.cells[grid.columns * y + x]);
				formulaCell->refCells++;
				formulaCell->refs = realloc(formulaCell->refs, sizeof(Cell) * formulaCell->refCells);
				formulaCell->refs[formulaCell->refCells] = *targetCell;
				// Fill the formula cell with content from target
    			gridChangePosition(&grid, formulaCell->column, formulaCell->row);
				char buf[10];
				sprintf(buf, "%d", targetCell->value);
				printf("%s", buf);

				// Return home
				formulaCell = NULL;
				break;
            default:
                break;
        }
        command = getChar();
    }

    // TODO UI for reading a function
    // TODO calculations

    return 0;
}
