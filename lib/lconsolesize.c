
#include <sys/ioctl.h>
void getConsoleSize(struct winsize *w)
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
}