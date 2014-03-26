#include <stdio.h>
#include <stdlib.H>
#include <string.h>
#include <limits.h>
#ifndef UTILS_H
#define UTILS_H
#include "utils.h"
#endif

#define CELL_INNER_WIDTH (11) // strlen(MAX_INT) = 10 + 1 (for negative sign)
#define CELL_HPADDING (2)     // horizontal
#define CELL_HBORDER (2)
#define CELL_WIDTH (CELL_INNER_WIDTH + CELL_HPADDING + CELL_HBORDER)
#define CELL_HEIGHT (3)

// Holds the raw canvas on which to draw
typedef struct Canvas {
    Env     t;
    char    *c;
} Canvas;

// Cell
typedef struct Cell {
    int     column;
    int     row;
    int     value;
    char    contents[CELL_HEIGHT][CELL_WIDTH+1];
} Cell;

typedef struct Grid {
    int     columns;
    int     rows;
    int     cellCount;
    // Position
    int     x;
    int     y;
    int     position;
    // Cells
    Cell    *cells;
} Grid;

Canvas canvasInit(Env terminal);
Cell cellInit(int column, int row, int value);
void cellDraw(Cell cell);
void cellReadValue(Cell **cell, Grid *grid);

Grid gridInit(Canvas canvas, int limit);
Grid gridInitMax(Canvas canvas);
void gridChangePosition(Grid *grid, int x, int y);
void gridDraw(Grid *grid);
