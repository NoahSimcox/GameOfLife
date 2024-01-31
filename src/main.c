#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>
#include <graphx.h>
#include <keypadc.h>
#include <gfx/gfx.h>
#include <stdbool.h>
#include <sys/timers.h>

#define CELL_WIDTH 4
#define CELL_HEIGHT 4

bool step();
void draw();
void play();

typedef struct{
    int x;
    int y;
} vec2_t;
typedef struct{
    bool alive;
} tile;
typedef struct{
    vec2_t vec;
    bool alive;
} cell;

tile screen[60][80];
vec2_t selector = {GFX_LCD_WIDTH / 2, GFX_LCD_HEIGHT / 2};
vec2_t start_cells[50];
cell cells[100];

int main() {

    gfx_Begin();
    gfx_SetDrawBuffer(); // Draw to the buffer to avoid rendering artifacts
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    while (step()) { // No rendering allowed in step!
        draw(); // As little non-rendering logic as possible
        gfx_SwapDraw(); // Queue the buffered frame to be displayed
    }

    gfx_End();
    return 0;
}

int increment2 = 0;
int increment = 0;
bool step(){

    kb_Scan();
    static bool left, right, up, down, prev_key_left, prev_key_right, prev_key_up, prev_key_down;
    left = kb_Data[7] & kb_Left;
    right = kb_Data[7] & kb_Right;
    up = kb_Data[7] & kb_Up;
    down = kb_Data[7] & kb_Down;

    if (kb_Data[6] & kb_Clear)
        return false;
    
    if (left && !prev_key_left)
        selector.x -= 4;
    if (right && !prev_key_right)
        selector.x += 4;
    if (up && !prev_key_up)
        selector.y -= 4;
    if (down && !prev_key_down)
        selector.y += 4;
    
    prev_key_left = left;
    prev_key_right = right;
    prev_key_up = up;
    prev_key_down = down;

    if (kb_Data[1] & kb_Del){
        screen[selector.y / 4][selector.x / 4].alive = true;
        start_cells[increment] = selector;
        increment++;
    }

    if (kb_Data[1] & kb_Mode){
        increment--;
        screen[start_cells[increment].y / 4][start_cells[increment].x / 4].alive = false;
    }

    if (kb_Data[6] & kb_Enter){

        play();

        for (int i = 0; i < GFX_LCD_HEIGHT / 4; i++){
            for (int j = 0; j < GFX_LCD_WIDTH / 4; j++){

                screen[i][j].alive = false;
            }
        }
    
        increment = 0;
        increment2 = 0;
    }

    return true;
}


void draw(){

    gfx_FillScreen(0);

    gfx_SetColor(2);
    for (int i = 0; i < increment; i++){
        gfx_FillRectangle(start_cells[i].x, start_cells[i].y, CELL_WIDTH, CELL_HEIGHT);
    }

    gfx_SetColor(1);
    gfx_FillRectangle(selector.x, selector.y, CELL_WIDTH, CELL_HEIGHT);

}


void play(){

    while(!(kb_Data[5] & kb_Vars)){

        increment2 = 0;
        
        for (int i = 0; i < GFX_LCD_HEIGHT / 4; i++){
            for (int j = 0; j < GFX_LCD_WIDTH / 4; j++){

                int neighbors = 0;
                vec2_t move;
                move.x = 0;
                move.y = -1;

                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;
                move.x++;
                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;
                move.y++;
                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;
                move.y++;
                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;
                move.x--;
                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;
                move.x--;
                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;
                move.y--;
                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;
                move.y--;
                if (screen[i + move.y][j + move.x].alive)
                    neighbors++;


                if (screen[i][j].alive && (neighbors < 2 || neighbors > 3)){
                    cells[increment2].vec.x = j;
                    cells[increment2].vec.y = i;
                    cells[increment2].alive = false;
                    increment2++;
                } else if (screen[i][j].alive && (neighbors == 3 || neighbors == 2)){
                    cells[increment2].vec.x = j;
                    cells[increment2].vec.y = i;
                    cells[increment2].alive = true;
                    increment2++;
                } else if (!(screen[i][j].alive) && neighbors == 3){
                    cells[increment2].vec.x = j;
                    cells[increment2].vec.y = i;
                    cells[increment2].alive = true;
                    increment2++;
                }
            }
        }

        delay(250);

        gfx_FillScreen(0);

        for (int i = 0; i < increment2; i++){

            if (cells[i].alive){
                gfx_SetColor(2);
                gfx_FillRectangle(cells[i].vec.x * 4, cells[i].vec.y * 4, CELL_WIDTH, CELL_HEIGHT);
                screen[cells[i].vec.y][cells[i].vec.x].alive = true;
                continue;
            }

            gfx_SetColor(0);
            gfx_FillRectangle(cells[i].vec.x * 4, cells[i].vec.y * 4, CELL_WIDTH, CELL_HEIGHT);

            screen[cells[i].vec.y][cells[i].vec.x].alive = false;
        }

        gfx_SwapDraw();
        kb_Scan();
    }
}