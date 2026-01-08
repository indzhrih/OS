#include <bandit/bandit.h>
#include "../Headers/Thread.h"
#include "TestFixtures.cpp"
#include <thread>

using namespace bandit;
using namespace snowhouse;

go_bandit([]{
    describe("Thread", []{
        describe("constructor", []{
            Event* startEvent = nullptr;
            Event* stopEvent = nullptr;
            Event* endEvent = nullptr;
            std::mutex* cs = nullptr;
            Array* arr = nullptr;

            before_each([&]{
                startEvent = new Event(false, false);
                stopEvent = new Event(true, false);
                endEvent = new Event(true, false);
                cs = new std::mutex();
                arr = new Array(5);
            });

            after_each([&]{
                delete startEvent;
                delete stopEvent;
                delete endEvent;
                delete cs;
                delete arr;
                startEvent = stopEvent = endEvent = nullptr;
                cs = nullptr;
                arr = nullptr;
            });

            it("creates thread with valid parameters", [&]{
                CoutRedirect silence;
                Thread thr(startEvent, endEvent, stopEvent, cs, arr, 1, 5);

                startEvent->set();
                endEvent->set();
                thr.wait();
            });

            it("throws on null startEvent", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(nullptr, endEvent, stopEvent, cs, arr, 1, 2));
            });

            it("throws on null endEvent", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, nullptr, stopEvent, cs, arr, 1, 2));
            });

            it("throws on null stopEvent", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, endEvent, nullptr, cs, arr, 1, 2));
            });

            it("throws on null criticalSection", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, endEvent, stopEvent, nullptr, arr, 1, 2));
            });

            it("throws on null array", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, endEvent, stopEvent, cs, nullptr, 1, 2));
            });

            it("throws on zero threadNumber", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, endEvent, stopEvent, cs, arr, 0, 2));
            });

            it("throws on negative threadNumber", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, endEvent, stopEvent, cs, arr, -1, 2));
            });

            it("throws on zero arraySize", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, endEvent, stopEvent, cs, arr, 1, 0));
            });

            it("throws on negative arraySize", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEvent, endEvent, stopEvent, cs, arr, 1, -2));
            });
        });

        describe("wait method", []{
            Event* startEvent = nullptr;
            Event* stopEvent = nullptr;
            Event* endEvent = nullptr;
            std::mutex* cs = nullptr;
            Array* arr = nullptr;

            before_each([&]{
                startEvent = new Event(false, false);
                stopEvent = new Event(true, false);
                endEvent = new Event(true, false);
                cs = new std::mutex();
                arr = new Array(1);
            });

            after_each([&]{
                delete startEvent;
                delete stopEvent;
                delete endEvent;
                delete cs;
                delete arr;
                startEvent = stopEvent = endEvent = nullptr;
                cs = nullptr;
                arr = nullptr;
            });

            it("returns when thread completes with endEvent set", [&]{
                CoutRedirect silence;
                Thread thr(startEvent, endEvent, stopEvent, cs, arr, 1, 1);

                startEvent->set();
                endEvent->set();
                thr.wait();
            });

            it("handles already terminated thread", [&]{
                CoutRedirect silence;
                Thread thr(startEvent, endEvent, stopEvent, cs, arr, 1, 1);

                startEvent->set();
                endEvent->set();
                thr.wait();
            });
        });

        describe("destructor", []{
            Event* startEvent = nullptr;
            Event* stopEvent = nullptr;
            Event* endEvent = nullptr;
            std::mutex* cs = nullptr;
            Array* arr = nullptr;

            before_each([&]{
                startEvent = new Event(false, false);
                stopEvent = new Event(true, false);
                endEvent = new Event(true, false);
                cs = new std::mutex();
                arr = new Array(1);
            });

            after_each([&]{
                delete startEvent;
                delete stopEvent;
                delete endEvent;
                delete cs;
                delete arr;
                startEvent = stopEvent = endEvent = nullptr;
                cs = nullptr;
                arr = nullptr;
            });

            it("joins thread in destructor", [&]{
                CoutRedirect silence;
                {
                    Thread thr(startEvent, endEvent, stopEvent, cs, arr, 1, 1);
                    startEvent->set();
                    endEvent->set();
                }
            });
        });
    });
});
