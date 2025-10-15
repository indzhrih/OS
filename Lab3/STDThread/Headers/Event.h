#ifndef LAB3_WINAPI_EVENT_H
#define LAB3_WINAPI_EVENT_H

#include <mutex>
#include <condition_variable>
#include <atomic>

class Event {
public:
    Event(bool manualReset, bool initialState);
    ~Event();

    void set();
    void reset();
    void wait();
    bool is_set() const;

private:
    bool manualReset;
    std::atomic<bool> signaled;
    mutable std::mutex mtx;
    std::condition_variable cv;
};

#endif // LAB3_WINAPI_EVENT_H
