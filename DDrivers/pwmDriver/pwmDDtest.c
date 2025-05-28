#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

unsigned int freq = 0;
unsigned int duty = 0;
char buffer[9];

int main(void)
{
    int fd0 = open("/dev/pwmDD", O_WRONLY);

    freq = 50;
    duty = 9.5;

    snprintf(buffer, sizeof(buffer), "%05u%03u", freq, duty);
    write(fd0, &buffer, 9);
    
    sleep(3);

    freq = 50;
    duty = 7;

    snprintf(buffer, sizeof(buffer), "%05u%03u", freq, duty);
    write(fd0, &buffer, 9);

    close(fd0);
    return 0;

}
