#include <sys/time.h>
#include <unistd.h>

int main()
{
    int i = 1000;
    while (i) 
    {
        struct timeval currentTime;
        gettimeofday(&currentTime,NULL);
        printf("UTC in ms: %lld\n", (currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000));
        sleep(1);
        i--;
    }
}