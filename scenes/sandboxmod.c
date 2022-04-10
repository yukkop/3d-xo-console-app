
void cutTileSHeetToTiles(Tile *tiles, char *tileSheet)
{
    // printf("Make tiles \n");
    for (int yTile = 0; yTile < FIELD_HEIGHT; yTile++)
    {

        for (int xTile = 0; xTile < FIELD_WIDTH; xTile++)
        {
            for (int yChar = 0; yChar < TILE_HEIGHT; yChar++)
            {
                for (int xChar = 0; xChar < TILE_WIDTH * UNICOD_COSTIL; xChar++)
                {
                    tiles[yTile * FIELD_WIDTH + xTile].data[yChar * TILE_WIDTH * UNICOD_COSTIL + xChar] =
                        tileSheet[/* x */ (xChar + xTile * TILE_WIDTH * UNICOD_COSTIL) + /* y */ (yChar + yTile * TILE_HEIGHT) * FIELD_WIDTH * TILE_WIDTH * UNICOD_COSTIL];

                    // printf("%c", tileSheet[/* x */ (xChar + xTile * TILE_WIDTH * UNICOD_COSTIL) + /* y */ (yChar + yTile * TILE_HEIGHT) * FIELD_WIDTH * TILE_WIDTH * UNICOD_COSTIL]);
                }
                // printf("\n");
            }
        }
    }
}

void fillScene(Tile *defaultTiles, Layer *layer)
{
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++)
    {
        int x = i % FIELD_WIDTH;
        int y = i / FIELD_WIDTH;
        // printf("%i:%i\n", x, y);
        inputTileToLayer(
            &defaultTiles[i],
            TILE_WIDTH * UNICOD_COSTIL,
            TILE_HEIGHT,
            x,
            y,
            layer,
            FIELD_WIDTH);
    }
}

void chageSelectedTile(PointInt *selectedTilePoint, Scene *scene, PointInt newPoint)
{
    inputTileToLayer(
        &transparencyTile,
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);

    selectedTilePoint->x = newPoint.x;
    selectedTilePoint->y = newPoint.y;

    inputTileToLayer(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);
}

PointInt returnEmptyTilePointTowards(enum minimapSymbols *minimap, PointInt *selectedTilePoint, int xDirrection, int yDirrection)
{
    // int xOffset = xDirrection;
    // int yOffset = yDirrection;
    int x = selectedTilePoint->x + xDirrection;
    if (x >= FIELD_WIDTH)
        x = 0;
    else if (x < 0)
        x = FIELD_WIDTH - 1;

    int y = selectedTilePoint->y + yDirrection;
    if (y >= FIELD_HEIGHT)
        y = 0;
    else if (y < 0)
        y = FIELD_HEIGHT - 1;
    PointInt answer;
    answer.x = x;
    answer.y = y;
    return answer;
}

void findEmptyTilePoint(enum minimapSymbols *minimap, PointInt *selectedTilePoint)
{
    int lengs = FIELD_WIDTH * FIELD_HEIGHT;
    for (int i = lengs / 2; i < lengs; i++)
    {
        if (minimap[i] == emptySymbol)
        {
            selectedTilePoint->x = i % FIELD_WIDTH;
            selectedTilePoint->y = i / FIELD_WIDTH;
            // printf("%i, %i", selectedTilePoint->x, selectedTilePoint->y);
            return;
        }
        else if (minimap[lengs / 2 - (i - lengs / 2)] == emptySymbol)
        {
            selectedTilePoint->x = (lengs / 2 - (i - lengs / 2)) % FIELD_WIDTH;
            selectedTilePoint->y = (lengs / 2 - (i - lengs / 2)) / FIELD_WIDTH;
            return;
        }
    }
    selectedTilePoint->x = FIELD_WIDTH / 2;
    selectedTilePoint->y = FIELD_HEIGHT / 2;
}

void put(PointInt *selectedTilePoint, enum minimapSymbols *minimap, Scene *scene, Camera *camera, struct winsize *w)
{
    char command = 'q';
    inputTileToLayer(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);

    render(scene, camera, w);
    do
    {
        command = getch();
        disableCameraPopup(camera);
        switch (command)
        {
        case 37: // LeftArrow
        case 'a':
        case 'A':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, -1, 0));
            break;
        case 38: // UpArrow
        case 'w':
        case 'W':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, 0, -1));
            break;
        case 39: // RightArrow
        case 'd':
        case 'D':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, +1, 0));
            break;
        case 40: // DowndArrow
        case 's':
        case 'S':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, 0, +1));
            break;
        case 'o':
        case 'O':

            inputTileToLayer(
                &oTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                TILE_WIDTH * UNICOD_COSTIL,
                TILE_HEIGHT,
                selectedTilePoint->x,
                selectedTilePoint->y,
                &scene->layers[0],
                FIELD_WIDTH);
            minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] = oSymbol;
            command = 'q';
            break;
        case 'x':
        case 'X':
            inputTileToLayer(
                &xTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                TILE_WIDTH * UNICOD_COSTIL,
                TILE_HEIGHT,
                selectedTilePoint->x,
                selectedTilePoint->y,
                &scene->layers[0],
                FIELD_WIDTH);

            minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] = xSymbol;
            command = 'q';
            break;
        case 'c':
        case 'C':
            if (minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] != emptySymbol)
            {
                inputTileToLayer(
                    &emptyTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                    TILE_WIDTH * UNICOD_COSTIL,
                    TILE_HEIGHT,
                    selectedTilePoint->x,
                    selectedTilePoint->y,
                    &scene->layers[0],
                    FIELD_WIDTH);

                minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] = emptySymbol;
            }
            command = 'q';
            break;
        default:
            continue;
        }
        setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [o - put o] [x - put x] [c - clear tile] [w,a,s,d - mark movement]", 90);
        render(scene, camera, w);
    } while (command != 'q');

    inputTileToLayer(
        &transparencyTile,
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);
}

void loadSandBoxScene(Camera *camera, Scene *scene, struct winsize *w)
{

    disableCameraPopup(camera);
    for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
    {
        emptyTiles[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
        xTiles[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
        oTiles[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
        chosenTile[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
    }

    cutTileSHeetToTiles(emptyTiles, EMPTY_TILES);
    cutTileSHeetToTiles(oTiles, O_TILES);
    cutTileSHeetToTiles(xTiles, X_TILES);
    cutTileSHeetToTiles(chosenTile, CHOSEN_TILES);
    transparencyTile = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
    transparencyTile.data = TRANSPARENCY_TILE;

    fillScene(emptyTiles, &scene->layers[0]);

    enum minimapSymbols minimap[FIELD_HEIGHT * FIELD_WIDTH];
    memset(minimap, emptySymbol, sizeof(enum minimapSymbols) * FIELD_HEIGHT * FIELD_WIDTH);

    PointInt selectedTilePoint;
    selectedTilePoint.x = FIELD_WIDTH / 2;
    selectedTilePoint.y = FIELD_HEIGHT / 2;

    char command = 'q';
    setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [p - put] [w,a,s,d - camera movement]", 61);

    render(scene, camera, w);
    do
    {
        command = getch();
        disableCameraPopup(camera);

        switch (command)
        {
        case 'p':
        case 'P':
            setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [o - put o] [x - put x] [c - clear tile] [w,a,s,d - mark movement]", 90);
            findEmptyTilePoint(minimap, &selectedTilePoint);
            put(&selectedTilePoint, minimap, scene, camera, w);
            break;
        case 'w':
        case 'W':
            setCameraOffset(camera, 0, -1);
            break;
        case 'a':
        case 'A':
            setCameraOffset(camera, -1, 0);
            break;
        case 's':
        case 'S':
            setCameraOffset(camera, 0, +1);
            break;
        case 'd':
        case 'D':
            setCameraOffset(camera, +1, 0);
            break;
        case 'r':
        case 'R':
            break;
        case 'q':
        case 'Q':
            setCameraHeaderText(camera, "#3d-xo v1.0# Back to menu [y/N]", 31);
            setCameraPopupText(camera, "are you sure? progress will not be saved", 40);
            enableCameraPopup(camera);
            render(scene, camera, w);
            command = getch();
            if (command == 'y' || command == 'Y')
            {
                command = 'q';
                break;
            }
            else if (command == 'q' || command == 'Q')
            {
                command = 'n';
            }
            disableCameraPopup(camera);
            render(scene, camera, w);
            break;
        default:
            continue;
        }

        setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [p - put] [w,a,s,d - camera movement]", 61);
        render(scene, camera, w);
    } while (command != 'q');
}