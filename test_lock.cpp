#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <mutex>
#include <atomic>

static int64_t cur_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
}

int main()
{
    std::mutex m1;
    int64_t t1 = cur_us();
    for (int i = 0; i < 10000000; ) {
        m1.lock();
        i++;
        m1.unlock();
    }
    std::cout << "std::mutex cost " << (cur_us() - t1) << "μs\n";

    pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
    t1 = cur_us();
    for (int i = 0; i < 10000000; ) {
        pthread_mutex_lock(&m2);
        i++;
        pthread_mutex_unlock(&m2);
    }
    std::cout << "pthread_mutex_t cost " << (cur_us() - t1) << "μs\n";

    std::atomic_flag m3 = ATOMIC_FLAG_INIT;
    t1 = cur_us();
    for (int i = 0; i < 10000000; ) {
        while (m3.test_and_set(std::memory_order_acquire));
        i++;
        m3.clear(std::memory_order_release);
    }
    std::cout << "C++ spin lock cost " << (cur_us() - t1) << "μs\n";

    volatile int m4 = 0;
    t1 = cur_us();
    for (int i = 0; i < 10000000; ) {
        while (m4);
        m4 = 1;
        i++;
        m4 = 0;
    }
    std::cout << "fake lock cost " << (cur_us() - t1) << "μs\n";

    return 0;
}
