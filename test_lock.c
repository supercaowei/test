#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>
#include <sys/time.h>

static int64_t cur_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
}

int main() {
    atomic_flag m4 = ATOMIC_FLAG_INIT;
    int64_t t1 = cur_us();
    for (int i = 0; i < 10000000; ) {
        while (atomic_flag_test_and_set(&m4));
        i++;
        atomic_flag_clear(&m4);
    }
    printf("C spin lock cost %lldμs\n", (cur_us() - t1));

    return 0;
}

