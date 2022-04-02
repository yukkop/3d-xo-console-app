#include <stdio.h>
#include <stdlib.h>

#define TILE_WIDTH 15
#define TILE_HEIGHT 3
#define FIELD_HEIGHT 3
#define FIELD_WIDTH 3

struct Tile
{
    wchar_t data[TILE_HEIGHT][TILE_WIDTH];
};

struct Tile emptyTiles[FIELD_HEIGHT][FIELD_WIDTH], xTiles[FIELD_HEIGHT][FIELD_WIDTH], oTiles[FIELD_HEIGHT][FIELD_WIDTH];

void ReadTilesFromFile(char *fileName, struct Tile tiles[FIELD_HEIGHT][FIELD_WIDTH])
{
    printf("Function \"ReadTilesFromFile\", fileName: \"%s\"\n", fileName);
    FILE *file = fopen(fileName, "rb");
    wchar_t *buffer;
    if ((buffer = malloc(sizeof(wchar_t))) == NULL)
    {
        printf("Error: No enough memory\n");
        getchar();
        return;
    }
    else
    {
        if (file == NULL)
        {
            printf("Error: File cannot be opened\n");
            getchar();
            return;
        }
        for (int mapLine = 0; mapLine < FIELD_HEIGHT; mapLine++)
        {
            for (int tileLine = 0; tileLine < TILE_HEIGHT; tileLine++)
            {
                for (int mapColumn = 0; mapColumn < FIELD_WIDTH; mapColumn++)
                {
                    for (int tileColumn = 0; tileColumn < TILE_WIDTH; tileColumn++)
                    {
                        fread(buffer, sizeof(wchar_t), 1, file);
                        if (*buffer == '\n')
                        {
                            tileColumn--;
                            // printf("%s", buffer);
                        }
                        else
                        {
                            if (tiles[mapLine][mapColumn].data[tileLine][tileColumn] = malloc(sizeof(wchar_t)) == NULL)
                            {
                                printf("Error: No enough memory\n");
                                getchar();
                                return;
                            }
                            else
                            {
                                tiles[mapLine][mapColumn].data[tileLine][tileColumn] = *buffer;
                            }
                        }
                    }
                    printf("%s", tiles[mapLine][mapColumn].data[tileLine]);
                }
            }
        }
    }

    fclose(file);
}

void ReadTest(char *fileName)
{
    FILE *file = fopen(fileName, "r");

    for (int mapLine = 0; mapLine < TILE_HEIGHT * FIELD_HEIGHT; mapLine++)
    {
        char *line_part = (char *)malloc(sizeof(char) * TILE_WIDTH * FIELD_WIDTH);
        if (line_part == NULL)
        {
            fputs("Ошибка памяти", stderr);
            exit(2);
        }
        fread(line_part, sizeof(char), TILE_WIDTH * FIELD_WIDTH, file);
        printf("%s", line_part);
        free(line_part);
    }

    fclose(file);
}

void ReadTest2(char *fileName)
{
    FILE *file = fopen(fileName, "rb");

    for (int mapLine = 0; mapLine < TILE_HEIGHT * FIELD_HEIGHT; mapLine++)
    {
        char *line_part = (char *)malloc(sizeof(char) * TILE_WIDTH * FIELD_WIDTH);
        fgets(line_part, TILE_WIDTH * FIELD_WIDTH, file);
        printf("%s", line_part);
    }

    fclose(file);
}

void Test()
{
    char *map = "⠀⠀⠀⠀⠀⠀⠀⣤⠤⠤⠤⠤⠤⠤⠤⠤⠤⣤⠤⠤⠤⠤⠤⠤⠤⠤⠤⣤⠤⠤⠤⠤⠤⠤⠤⠤⠤⣤⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⣤⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⣤⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀\n⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀\n⠀⠀⣤⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⣿⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⣿⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⣤⠀⠀\n⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀\n⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤\n⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛";
    printf("%s", map);
}

int main(int argc, char *argv[])
{
    argc = 2;
    printf("%i \n", argc);
    if (argc == 2)
    {
        printf("%s \n", "dывапролд");

        char *emptyTileFile = "empty_tiles.txt", *xTilesFile = "x_tiles", *oTilesFile = "o_tiles";

        ReadTilesFromFile("output.txt", oTiles);
    }
    else
    {
        FILE *fp, *fw;
        wchar_t *buffer;

        if ((buffer = malloc(sizeof(wchar_t))) == NULL)
        {
            printf("Error: No enough memory\n");
            getchar();
            return 1;
        }
        else
        {
            fp = fopen("x_tiles", "rb");
            fw = fopen("output.txt", "wb");

            if (fp == NULL || fw == NULL)
            {
                printf("Error: File cannot be opened\n");
                getchar();
                return 1;
            }
            else
            {
                while (fread(buffer, sizeof(wchar_t), 1, fp) > 0)
                {
                    fwrite(buffer, sizeof(wchar_t), 1, fw);
                    printf("%s", buffer);
                }
            }
            getchar();
        }
    }
}