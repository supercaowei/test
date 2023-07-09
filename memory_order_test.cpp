#include <atomic>
#include <thread>
#include <assert.h>
#include <map>
#include <functional>

template<typename B>
void write_bool(B& b)
{
    b.store(true, std::memory_order_release);
}

template<typename B, typename I>
void read_x_then_y(B& x, B& y, I& z)
{
    // while(!x.load(std::memory_order_acquire));
    // if(y.load(std::memory_order_acquire))
    //     ++z;
    
    while(!x.load(std::memory_order_relaxed));
    if(y.load(std::memory_order_relaxed))
        ++z;
}

template<typename B>
void write_x_then_y(B& x, B& y)
{
    // x.store(true, std::memory_order_release);
    // y.store(true, std::memory_order_release);

    y.store(true, std::memory_order_relaxed);
    x.store(true, std::memory_order_relaxed);
}

int main()
{
    std::map<int, int> valueCount;
    for (int i = 0; i < 10000; i++) {
        std::atomic<bool> x(false), y(false);
        std::atomic<int> z(0);
        // bool x = false, y = false;
        // int z = 0;
        // std::thread a(std::bind(write_bool<decltype(x)>, std::ref(x)));
        // std::thread b(std::bind(write_bool<decltype(y)>, std::ref(y)));
        // std::thread c(std::bind(read_x_then_y<decltype(x), decltype(z)>, std::ref(x), std::ref(y), std::ref(z)));
        // std::thread d(std::bind(read_x_then_y<decltype(x), decltype(z)>, std::ref(y), std::ref(x), std::ref(z)));
        // a.join();
        // b.join();
        // c.join();
        // d.join();

        std::thread b(std::bind(read_x_then_y<decltype(x), decltype(z)>, std::ref(x), std::ref(y), std::ref(z)));
        std::thread a(std::bind(write_x_then_y<decltype(x)>, std::ref(x) , std::ref(y)));
        a.join();
        b.join();
        valueCount[z.load(std::memory_order_seq_cst)]++;
    }
    for (auto iter = valueCount.cbegin(); iter != valueCount.cend(); iter++) {
        printf("%d: %d\n", iter->first, iter->second);
    }
    return 0;
}