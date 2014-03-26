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

    char command = getChar();
    Cell *activeCell = NULL;

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
                if (!activeCell) {
                    activeCell = &(grid.cells[grid.position]);
                    cellReadValue(&activeCell, &grid);
                    //printf("grid(%d, %d)\n", grid.x, grid.y);
                } else {
                    activeCell = NULL;
                }
                break;
            default:
                break;
                //continue;
                //printf("%d\n", command);
        }
        command = getChar();
    }

    // TODO UI for navigating between the cells
    // TODO Number input
    // TODO calculations

    return 0;
}
