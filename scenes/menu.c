
void loadMenu(Camera *camera, Scene *scene, struct winsize *w)
{
    setCameraHeaderText(camera, "#3d-xo v1.0# Menu", 17);
    setCameraPopupText(camera, "[S - sandbox]", 13);
    enableCameraPopup(camera);

    render(scene, camera, w);

    char command = 'q';
    do
    {
        command = getch();

        switch (command)
        {
        case 's':
        case 'S':
            loadSandBoxScene(camera, scene, w);
            break;
        default:
            continue;
        }

        setCameraHeaderText(camera, "#3d-xo v1.0# Menu", 17);
        setCameraPopupText(camera, "[S - singlplayer]", 17);
        enableCameraPopup(camera);

        for (int i = 0; i < scene->height * scene->width; i++)
        {
            scene->layers[0].data[i] = VOID_SYMBOL[i % 3];
        }
        render(scene, camera, w);
    } while (command != 'q');
}
