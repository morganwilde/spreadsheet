#include "grid.h"

Canvas canvasInit(Env terminal) {
    Canvas c;
    c.t = terminal;
    c.c = (char *)malloc((c.t.h * c.t.w + 1) * sizeof(char));

    // Initialize the grid to ' '
    int i, j, index;
    for (i = 0; i < c.t.h; i++) {
        for (j = 0; j < c.t.w; j++) {
            index = i * c.t.w + j;
            c.c[index] = ' ';
        }
    }
    c.c[strlen(c.c)] = '\0';

    return c;
}

void canvasErase(Canvas c) {
    // This -> c.c holds a grid full of empty spaces
    printf("%s", c.c);
    // Get the cursor back to the initial position (0,0)
    printf("\x1B[0;0H");
    fflush(stdout);
}

Cell cellInit(int column, int row, int value) {
    Cell c;
    c.column = column;
    c.row = row;
    c.value = value;

    // Draw the cell
    int left    = column * CELL_WIDTH,
        right   = left + CELL_WIDTH,
        top     = row * CELL_HEIGHT,
        bottom  = top + CELL_HEIGHT;

    int i, j;
    for (i = 0; i < CELL_HEIGHT; i++) {

        for (j = 0; j < CELL_WIDTH; j++) {
            if ((i == 0 && j == 0) ||
                (i == (CELL_HEIGHT-1) && j == 0) ||
                (i == 0 && j == (CELL_WIDTH-1)) ||
                (i == (CELL_HEIGHT-1) && j == (CELL_WIDTH-1))) {
                c.contents[i][j] = '+';
            } else if ((j == 0 || j == (CELL_WIDTH-1))) {
                c.contents[i][j] = '|';
            } else {
                if (i == 1 && (j >= CELL_HPADDING/2 && j <= ((CELL_WIDTH-1)-CELL_HPADDING/2))) {
                    // The content part of the cell
                    /*
                    if (j == 3)
                        c.contents[i][j] = (char)((int)'0')+c.column;
                    else if (j == 5)
                        c.contents[i][j] = (char)((int)'0')+c.row;
                    else 
                        c.contents[i][j] = ' ';
                    */
                     c.contents[i][j] = ' ';

                } else if (i == 1) {
                    c.contents[i][j] = ' '; // horizontal padding
                } else {
                    c.contents[i][j] = '-';
                }
            }
        }
        c.contents[i][j] = '\0';
    }

    return c;
}

void cellDraw(Cell cell) {
    int cursorX = cell.column * (CELL_WIDTH-1),
        cursorY = cell.row * (CELL_HEIGHT-1);

    int i, j;
    for (i = 0; i < CELL_HEIGHT; i++) {
        printf("\x1B[%d;%dH", cursorY+(i+1), cursorX+1);
        printf("%s", cell.contents[i]);
    }
    printf("\x1B[%d;%dH", cursorY+1, cursorX+1);

    fflush(stdout);
}

void cellReadValue(Cell **cell, Grid *grid) {
    // Store a new value
    cell[0]->value = readIntReturn();
    // After reading, move the cursor back
    gridChangePosition(grid, cell[0]->column, cell[0]->row);

    // Reset the activeCell to NULL
    *cell = NULL;
}

Grid gridInit(Canvas canvas, int limit) {
    Grid grid;
    grid.columns = canvas.t.w / (CELL_WIDTH-1);
    grid.rows = canvas.t.h / (CELL_HEIGHT-1);
    grid.cellCount = limit;
    grid.x = 0;
    grid.y = 0;
    grid.position = grid.columns * grid.y + grid.x;
    grid.cells = malloc(limit * sizeof(Cell));

    int i, j;
    for (i = 0; i < grid.rows; i++) {
        for (j = 0; j < grid.columns; j++) {
            if ((i+1)*(j+1) <= limit) {
                grid.cells[i*grid.columns + j] = cellInit(j, i, 0);
            } else {
                break;
            }
        }
        if ((i+1)*(j+1) > limit) {
            break;
        }
    }

    return grid;
}

Grid gridInitMax(Canvas canvas) {
    int width   = canvas.t.w / (CELL_WIDTH-1),
        height  = canvas.t.h / (CELL_HEIGHT-1),
        limit   = width * height;
    
    return gridInit(canvas, limit);
}

void gridChangePosition(Grid *grid, int x, int y) {
    // Check for bounds
    if (x >= 0 && x < grid->columns) {
        grid->x = x;
    }
    if (y >= 0 && y < (grid->rows-1)) {
        grid->y = y;
    }
    grid->position = grid->columns * grid->y + grid->x;

    printf("\x1B[%d;%dH", (grid->y+1)*(CELL_HEIGHT-1), (grid->x+1)*(CELL_WIDTH-1) - (CELL_WIDTH-4));
    fflush(stdout);
}

void gridDraw(Grid *grid) {
    int i, j;
    for (i = 0; i < grid->rows; i++) {
        for (j = 0; j < grid->columns; j++) {
            if ((i+1)*(j+1) <= grid->cellCount) {
                cellDraw(grid->cells[i*grid->columns + j]);
            } else {
                break;
            }
        }
        if ((i+1)*(j+1) > grid->cellCount) {
            break;
        }
    }
    gridChangePosition(grid, 0, 0);
}
