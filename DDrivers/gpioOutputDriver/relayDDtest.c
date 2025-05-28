#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int pin = 0;
int state = 0;
char buffer[5];

int main(void)
{

    int fd0 = open("/dev/relayDD", O_WRONLY);

    pin = 23;
    state = 0;
    sprintf(buffer, "%d%d", pin, state);
    write(fd0, &buffer, 1);
    sleep(3);

    pin = 23;
    state = 1;
    sprintf(buffer, "%d%d", pin, state);
    write(fd0, &buffer, 1);
    sleep(3);

    pin = 24;
    state = 0;
    sprintf(buffer, "%d%d", pin, state);
    write(fd0, &buffer, 1);
    sleep(3);

    pin = 24;
    state = 1;
    sprintf(buffer, "%d%d", pin, state);
    write(fd0, &buffer, 1);

    close(fd0);
    return 0;

}
