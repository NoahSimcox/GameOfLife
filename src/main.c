#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>
#include <graphx.h>
#include <keypadc.h>
#include <gfx/gfx.h>
#include <stdbool.h>

#define RECT_WIDTH 4
#define RECT_HEIGHT 4

bool step();
void draw();
typedef struct{
    int x;
    int y;
} vec2_t;

vec2_t arr[100];
vec2_t selector = {GFX_LCD_WIDTH / 2, GFX_LCD_HEIGHT / 2};

int main() {

    gfx_Begin();
    gfx_SetDrawBuffer(); // Draw to the buffer to avoid rendering artifacts

    while (step()) { // No rendering allowed in step!
        draw(); // As little non-rendering logic as possible
        gfx_SwapDraw(); // Queue the buffered frame to be displayed
    }

    gfx_End();
    return 0;
}


bool step(){

    kb_Scan();
    bool left, right, up, down, prev_key;
    left = kb_Data[7] == kb_Left;
    right = kb_Data[7] == kb_Right;
    up = kb_Data[7] == kb_Up;
    down = kb_Data[7] == kb_Down;

    if (kb_Data[6] & kb_Clear)
        return false;
    
    if (left && !prev_key)
        selector.x--;
    if (right && !prev_key)
        selector.x++;
    if (up && !prev_key)
        selector.y--;
    if (down && !prev_key)
        selector.y--;
    
    if (kb_Data[1] & kb_Del)
        

}


void draw(){

    gfx_FillScreen(0);

}