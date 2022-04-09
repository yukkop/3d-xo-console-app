
void loadMenu(Camera *camera, Scene *scene, struct winsize *w)
{

    setCameraHeaderText(camera, "#3d-xo v1.0# Menu", 17);
    setCameraPopupText(camera, "[S - singlplayer]", 17);
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

        render(scene, camera, w);
    } while (command != 'q');
}
