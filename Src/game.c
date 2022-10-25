#include <stdio.h>
#include "cprocessing.h"

#define GOL_GRID_COLS 30
#define GOL_GRID_ROWS 30
#define GOL_GRID_BUFFERS 2

#define GOL_ALIVE 1
#define GOL_DEAD 0

#define TRUE 1
#define FALSE 0

int gIsPaused;
int gGrids[GOL_GRID_BUFFERS][GOL_GRID_ROWS][GOL_GRID_COLS];
int draw;

/* Feel free to declare your own variables here */
float width, height;

int neighbours(int buffer, int row, int col) {

    int count = 0;

    if ( !((row + 1) > 30) && !((row - 1) < 0) && !((col + 1) > 30) && !((col - 1) < 0)) {

        //top of grid
        if (gGrids[buffer][row - 1][col] == GOL_ALIVE) {
            count++;
        }

        //bottom of grid
        if (gGrids[buffer][row + 1][col] == GOL_ALIVE) {
            count++;
        }

        //left of grid
        if (gGrids[buffer][row][col - 1] == GOL_ALIVE) {
            count++;
        }

        //right of grid
        if (gGrids[buffer][row][col + 1] == GOL_ALIVE) {
            count++;
        }

        //bottom left of grid
        if (gGrids[buffer][row + 1][col - 1] == GOL_ALIVE) {
            count++;
        }

        //top left of grid
        if (gGrids[buffer][row - 1][col - 1] == GOL_ALIVE) {
            count++;
        }

        //bottom right of grid
        if (gGrids[buffer][row - 1][col + 1] == GOL_ALIVE) {
            count++;
        }

        //top right of grid
        if (gGrids[buffer][row + 1][col + 1] == GOL_ALIVE) {
            count++;
        }
       

    }

    return count;
}


void game_init(void)
{
    /* Set every grids' cells as 'dead' */
    for (int row = 0; row < GOL_GRID_ROWS; ++row) {
        for (int col = 0; col < GOL_GRID_COLS; ++col) {
            for (int i = 0; i < GOL_GRID_BUFFERS; ++i) {
                gGrids[i][row][col] = GOL_DEAD;
            }
        }
    }

    /*********************************************************
    *  Let's start with gGrids[0] as the 'reference grid'
    *  and gGrids[1] as the 'displaying grid'.
    *
    *  We initialize gGrids[0] with a simple 'glider'.
    *********************************************************/

    gGrids[0][1][2] = GOL_ALIVE;
    gGrids[0][2][3] = GOL_ALIVE;
    gGrids[0][3][1] = GOL_ALIVE;
    gGrids[0][3][2] = GOL_ALIVE;
    gGrids[0][3][3] = GOL_ALIVE;

    /* We start unpaused */
    gIsPaused = FALSE;

    draw = 0;

    /* Initialization of your other variables here */
    width = CP_System_GetWindowWidth() / 30.0f;
    height = CP_System_GetWindowHeight() / 30.0f;
}

void game_update(void)
{

    CP_Color squareColour = CP_Color_Create(100, 100, 100, 255);
    CP_Settings_Fill(squareColour);

    CP_Settings_RectMode(CP_POSITION_CENTER);
    CP_Settings_StrokeWeight(0.5f);

    for (float y = 6.5f; y <= CP_System_GetWindowHeight(); y += height) {
        for (float x = 6.5f; x <= CP_System_GetWindowWidth(); x += width) {
            CP_Graphics_DrawRect(x, y, width, height);
        }
    }

    if (CP_Input_KeyTriggered(KEY_ANY)) {
        /* Case whereany key is pressed */
        if (gIsPaused == FALSE) {
            gIsPaused = TRUE;
        }
        else {
            gIsPaused = FALSE;
        }
    }

    if (gIsPaused == TRUE) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        if (CP_Input_MouseClicked() == TRUE) {
            float click_x = CP_Input_GetMouseX();
            float click_y = CP_Input_GetMouseY();

            for (int row = 0; row < GOL_GRID_ROWS; ++row) {
                for (int col = 0; col < GOL_GRID_COLS; ++col) {
                    float area_center_x = 6.5f + width * (float)row;
                    float area_center_y = 6.5f + height * (float)col;

                    if (click_x <= (area_center_x + (width / 2)) && click_x >= (area_center_x - (width / 2))
                        && click_y <= (area_center_y + (height / 2)) && click_y >= (area_center_y - (height / 2))) {
                        if (gGrids[draw][row][col] == GOL_ALIVE) {
                            gGrids[draw][row][col] = GOL_DEAD;
                        }
                        else {
                            gGrids[draw][row][col] = GOL_ALIVE;
                        }
                    }
                    
                }
            }

        }
    }
    else {
        for (int row = 0; row < GOL_GRID_ROWS; ++row) {
            for (int col = 0; col < GOL_GRID_COLS; ++col) {
                int neighbour = neighbours(draw, row, col);

                if (gGrids[draw][row][col] == GOL_ALIVE) {
                    if (neighbour < 2 || neighbour > 3) {
                        gGrids[!draw][row][col] = GOL_DEAD;
                    }
                    else {
                        gGrids[!draw][row][col] = GOL_ALIVE;
                    }

                }
                else {
                    if (neighbour == 3) {
                        gGrids[!draw][row][col] = GOL_ALIVE;
                    }
                    else {
                        gGrids[!draw][row][col] = GOL_DEAD;
                    }
                }  
            }
        }
        draw = !draw;
    }

    for (int row = 0; row < GOL_GRID_ROWS; ++row) {
        for (int col = 0; col < GOL_GRID_COLS; ++col) {

            if (gGrids[draw][row][col] == GOL_ALIVE) {
                CP_Color squareColour = CP_Color_Create(255, 255, 255, 255);
                CP_Settings_Fill(squareColour);
                CP_Graphics_DrawRect(6.5f + width * (float)row, 6.5f + height * (float)col, width, height);
            }
            else {
                CP_Color squareColour = CP_Color_Create(100, 100, 100, 255);
                CP_Settings_Fill(squareColour);
                CP_Graphics_DrawRect(6.5f + width * (float)row, 6.5f + height * (float)col, width, height);
            }

        }
    }
}

void game_exit(void)
{

}
