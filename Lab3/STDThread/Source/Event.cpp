#include "../Headers/Event.h"

Event::Event(bool manualReset, bool initialState)
    : manualReset(manualReset), signaled(initialState) {}

Event::~Event() {}

void Event::set() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        signaled.store(true, std::memory_order_release);
    }
    cv.notify_all();
}

void Event::reset() {
    std::lock_guard<std::mutex> lock(mtx);
    signaled.store(false, std::memory_order_release);
}

void Event::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    while (!signaled.load(std::memory_order_acquire))cv.wait(lock);

    if (!manualReset) signaled.store(false, std::memory_order_release);
}

bool Event::is_set() const {
    return signaled.load(std::memory_order_acquire);
}
