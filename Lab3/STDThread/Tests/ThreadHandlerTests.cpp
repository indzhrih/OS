#include <bandit/bandit.h>
#include "../Headers/ThreadHandler.h"
#include "../Headers/Thread.h"
#include "../Headers/Event.h"
#include "../Headers/Array.h"
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>

using namespace bandit;
using namespace snowhouse;

go_bandit([]{
    describe("threadWorker function", []{
        struct TestThreadResources {
            Event* startEvent = nullptr;
            Event* endEvent = nullptr;
            Event* stopEvent = nullptr;
            std::mutex* criticalSection = nullptr;
            Array* array = nullptr;

            TestThreadResources(bool createArray = true) {
                startEvent = new Event(true, true);
                endEvent = new Event(true, true);
                stopEvent = new Event(true, false);
                criticalSection = new std::mutex();
                if (createArray) {
                    array = new Array(1);
                }
            }

            ~TestThreadResources() {
                delete startEvent;
                delete endEvent;
                delete stopEvent;
                delete criticalSection;
                delete array;
                startEvent = endEvent = stopEvent = nullptr;
                criticalSection = nullptr;
                array = nullptr;
            }

            TestThreadResources(const TestThreadResources&) = delete;
            TestThreadResources& operator=(const TestThreadResources&) = delete;
        };

        it("returns 1 when param is NULL", []{
            int rc = threadWorker(nullptr);
            AssertThat(rc, Equals(1));
        });

        it("handles null startEvent - quick return", []{
            TestThreadResources resources(true);

            struct TestThread {
                Event* startEvent;
                Event* endEvent;
                Event* stopEvent;
                std::mutex* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            TestThread test{nullptr, resources.endEvent, resources.stopEvent,
                           resources.criticalSection, resources.array, 1, 1};

            Thread* threadPtr = reinterpret_cast<Thread*>(&test);
            int rc = threadWorker(threadPtr);
            AssertThat(rc, Equals(1));
        });

        it("handles null criticalSection - quick return", []{
            TestThreadResources resources(true);

            struct TestThread {
                Event* startEvent;
                Event* endEvent;
                Event* stopEvent;
                std::mutex* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            TestThread test{resources.startEvent, resources.endEvent, resources.stopEvent,
                           nullptr, resources.array, 1, 1};

            Thread* threadPtr = reinterpret_cast<Thread*>(&test);
            int rc = threadWorker(threadPtr);
            AssertThat(rc, Equals(1));
        });

        it("handles invalid arraySize zero - quick return", []{
            TestThreadResources resources(true);

            struct TestThread {
                Event* startEvent;
                Event* endEvent;
                Event* stopEvent;
                std::mutex* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            TestThread test{resources.startEvent, resources.endEvent, resources.stopEvent,
                           resources.criticalSection, resources.array, 0, 1};

            Thread* threadPtr = reinterpret_cast<Thread*>(&test);
            int rc = threadWorker(threadPtr);
            AssertThat(rc, Equals(1));
        });

        it("handles invalid threadNumber zero - quick return", []{
            TestThreadResources resources(true);

            struct TestThread {
                Event* startEvent;
                Event* endEvent;
                Event* stopEvent;
                std::mutex* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            TestThread test{resources.startEvent, resources.endEvent, resources.stopEvent,
                           resources.criticalSection, resources.array, 1, 0};

            Thread* threadPtr = reinterpret_cast<Thread*>(&test);
            int rc = threadWorker(threadPtr);
            AssertThat(rc, Equals(1));
        });

        it("handles null array pointer - quick return", []{
            TestThreadResources resources(false);

            struct TestThread {
                Event* startEvent;
                Event* endEvent;
                Event* stopEvent;
                std::mutex* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            TestThread test{resources.startEvent, resources.endEvent, resources.stopEvent,
                           resources.criticalSection, nullptr, 1, 1};

            Thread* threadPtr = reinterpret_cast<Thread*>(&test);
            int rc = threadWorker(threadPtr);
            AssertThat(rc, Equals(1));
        });

        it("returns 0 with pre-set events", []{
            TestThreadResources resources(true);
            resources.startEvent = new Event(true, true);
            resources.endEvent = new Event(true, false);

            struct TestThread {
                Event* startEvent;
                Event* endEvent;
                Event* stopEvent;
                std::mutex* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            TestThread test{resources.startEvent, resources.endEvent, resources.stopEvent,
                           resources.criticalSection, resources.array, 1, 1};

            Thread* threadPtr = reinterpret_cast<Thread*>(&test);

            std::atomic<bool> threadFinished(false);
            std::thread worker([&]{
                int rc = threadWorker(threadPtr);
                threadFinished.store(true);
            });

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            resources.endEvent->set();

            worker.join();

            AssertThat(threadFinished.load(), Equals(true));
        });
    });
});
