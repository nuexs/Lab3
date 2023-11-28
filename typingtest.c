#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#define PASSWORDSIZE 12

typedef unsigned long long int tick64_t;
typedef unsigned long int tick32_t;

tick32_t getSecond(){
	tick32_t tick = 0ul;
#if defined(WIN32) || defined(WIN64)
	tick = getSecond();
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	tick = (ts.tv_sec * 1000ul) + (ts.tv_nsec / 1000ul / 1000ul);
#endif
	return tick;
}

int main(void)
{
    int fd;
    int nread, cnt = 0, errcnt = 0;
    char ch, text[] = "The magic thing is that you can change it.";
    struct termios init_attr, new_attr;

    fd = open(ttyname(fileno(stdin)), O_RDWR);
    tcgetattr(fd, &init_attr);
   
    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO; /* ~(ECHO | ECHOE | ECHOK | ECHONL); */
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &new_attr) != 0)
    {
        fprintf(stderr, "Unable to set terminal properties.\n");
    }
    printf("Please enter the following sentence as it is.\n%s\n", text);
   
    int startTime = getSecond();

    while ((nread = read(fd, &ch, 1)) > 0 && ch != '\n')
    {
        if (ch == text[cnt++])
            write(fd, &ch, 1);
        else
        {
            write(fd, "*", 1);
            errcnt++;
        }
    }

    double endTime = getSecond();
    double typingSec = fmod(((endTime - startTime) / 1000), 60);
    

    printf("\ntime : %.5lf sec\n", typingSec);
    printf("typing %d /sec\n", (int)(((sizeof(text) / sizeof(int)) / typingSec) * 60));
    printf("The number of typing errors is %d\n", errcnt);
    tcsetattr(fd, TCSANOW, &init_attr);
    close(fd);
}
