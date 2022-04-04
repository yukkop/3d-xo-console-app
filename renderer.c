
#include "constants.h"
#include "lib/types.c"
#include <unistd.h>
#include <sys/ioctl.h>

void drowOnScreen(Camera *camera, char field[], int width, int height)
{
    for (int y = 0; y < camera->height; y++)
    {
        if (y >= height)
            for (int x = 0; x < camera->width; x++)
            {
                printf("%s", VOID_SYMBOL);
            }
        else
            for (int x = 0; x < camera->width * UNICOD_COSTIL; x++)
            {
                if (x >= width)
                    continue;
                else
                    printf("%c", field[camera->xOffset + x + (camera->yOffset + y) * width]);
            }
        if (y <= camera->height - 2) // Не переходить на новую строку на последней линии
            printf("\n");
    }
}

void render(char *field, Camera *camera, struct winsize *w)
{
    system("clear");
    ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
    camera->xOffset = 0;
    camera->yOffset = 0;
    camera->width = w->ws_col;
    camera->height = w->ws_row;

    drowOnScreen(camera, field, FIELD_WIDTH * TILE_WIDTH, FIELD_HEIGHT * TILE_HEIGHT);
}