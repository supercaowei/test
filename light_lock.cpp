#include <atomic>
#include <mutex>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <memory>
#include <stdarg.h>

class LockInterface {
public:
    LockInterface() = default;
    LockInterface(const LockInterface& lock) = delete;
    LockInterface(LockInterface&& lock) = delete;
    LockInterface& operator=(const LockInterface& lock) = delete;
    LockInterface& operator=(LockInterface&& lock) = delete;
    virtual ~LockInterface() = default;
    virtual const char* name() = 0;
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

void MyPrint(bool enable, const char* fmt, ...) {
    if (enable) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

class LightLock : public LockInterface {
public:
    const char* name() override {
        return "LightLock";
    }

    void lock() override {
        MyPrint(false, "%s try lock\n", name());
        while (flag.test_and_set(std::memory_order_acquire));
        MyPrint(false, "%s locked\n", name());
    }

    void unlock() override {
        MyPrint(false, "%s try unlock\n", name());
        flag.clear(std::memory_order_release);
        MyPrint(false, "%s unlocked\n", name());
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

class PThreadLock : public LockInterface {
public:
    PThreadLock() {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~PThreadLock() {
        pthread_mutex_destroy(&mutex_);
    }

    const char* name() override {
        return "PThreadLock";
    }

    void lock() override {
        MyPrint(false, "%s try lock\n", name());
        pthread_mutex_lock(&mutex_);
        MyPrint(false, "%s locked\n", name());
    }

    void unlock() override {
        MyPrint(false, "%s try unlock\n", name());
        pthread_mutex_unlock(&mutex_);
        MyPrint(false, "%s unlocked\n", name());
    }

private:
    pthread_mutex_t mutex_;
};

class StdLock : public LockInterface {
public:
    const char* name() override {
        return "StdLock";
    }

    void lock() override {
        MyPrint(false, "%s try lock\n", name());
        mutex_.lock();
        MyPrint(false, "%s locked\n", name());
    }

    void unlock() override {
        MyPrint(false, "%s try unlock\n", name());
        mutex_.unlock();
        MyPrint(false, "%s unlocked\n", name());
    }

private:
    std::mutex mutex_;
};

class GccLock : public LockInterface {
public:
    GccLock() {
        __asm__ __volatile__ ("" ::: "memory");
        lock_ = 0;
    }

    const char* name() override {
        return "GccLock";
    }

    void lock() override {
        MyPrint(false, "%s try lock\n", name());
        while (1) {
            int i;
            for (i=0; i < 10000; i++) {
                if (__sync_bool_compare_and_swap(&lock_, 0, 1)) {
                    return;
                }
            }
            sched_yield();
        }
        MyPrint(false, "%s locked\n", name());
    }

    void unlock() override {
        MyPrint(false, "%s try unlock\n", name());
        __asm__ __volatile__ ("" ::: "memory");
        lock_ = 0;
        MyPrint(false, "%s unlocked\n", name());
    }

private:
    int lock_;
};

/* 
class SpinLock : public LockInterface {
public:
    SpinLock() {
        pthread_spin_init(&lock_, PTHREAD_PROCESS_PRIVATE);
    }

    ~SpinLock() {
        pthread_spin_destroy(&lock_);
    }

    const char* name() override {
        return "SpinLock";
    }

    void lock() override {
        MyPrint(false, "%s try lock\n", name());
        pthread_spin_lock(&lock_);
        MyPrint(false, "%s locked\n", name());
    }

    void unlock() override {
        MyPrint(false, "%s try unlock\n", name());
        pthread_spin_unlock(&lock_);
        MyPrint(false, "%s unlock\n", name());
    }

private:
    pthread_spinlock_t lock_;
}; */

class FakeLock : public LockInterface {
public:
    const char* name() override {
        return "FakeLock";
    }

    void lock() override {
        MyPrint(false, "%s try lock\n", name());
        MyPrint(false, "%s locked\n", name());
    }

    void unlock() override {
        MyPrint(false, "%s try unlock\n", name());
        MyPrint(false, "%s unlock\n", name());
    }
};

class LockGuard {
public:
    LockGuard(LockInterface& lock) : lock_(lock) {
        lock_.lock();
    }

    virtual ~LockGuard() {
        lock_.unlock();
    }

    LockGuard() = delete;
    LockGuard(const LockGuard& lock) = delete;
    LockGuard(LockGuard&& lock) = delete;
    LockGuard& operator=(const LockGuard& lock) = delete;
    LockGuard& operator=(LockGuard&& lock) = delete;

private:
    LockInterface& lock_;
};

int64_t getCurrentTimeUs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void* threadProc(void *opaque) {
    int64_t t1 = getCurrentTimeUs();
    LockInterface* lock = (LockInterface*)opaque;
    pthread_t tid = pthread_self();
    MyPrint(true, "Thread %d start threadProc with lock %s\n", tid, lock->name());
    for (int i = 0; i < 100; i++) {
        LockGuard LockGuard(*lock);
        MyPrint(true, "Thread %d lock %s before sleep %d\n", tid, lock->name(), i);
        usleep(1000);
        MyPrint(true, "Thread %d lock %s after sleep %d\n", tid, lock->name(), i);
    }
    int64_t t2 = getCurrentTimeUs();
    MyPrint(true, "Thread %d lock %s cost %lld us\n", tid, lock->name(), (t2 - t1));
    return NULL;
}

int main() {
    std::unique_ptr<LockInterface> lock(new LightLock);
    pthread_t threads[2] = {0};
    for (auto &thread : threads) {
        pthread_create(&thread, NULL, threadProc, lock.get());
    }
    for (auto &thread : threads) {
        pthread_join(thread, NULL);
    }
    return 0;
}
