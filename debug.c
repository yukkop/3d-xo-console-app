

void drowBox(char *arr, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%c", arr[x + y * width]);
        }
        if (y <= height - 2) // Не переходить на новую строку на последней линии
            printf("\n");
    }
}