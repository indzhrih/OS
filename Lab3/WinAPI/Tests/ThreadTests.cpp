#include <bandit/bandit.h>
#include <windows.h>
#include "../Headers/Thread.h"
#include "../Headers/Array.h"
#include "TestFixtures.cpp"

using namespace bandit;
using namespace snowhouse;

go_bandit([]{
    describe("Thread", []{
        describe("constructor", []{
            HANDLE startEv = nullptr;
            HANDLE stopEv = nullptr;
            HANDLE endEv = nullptr;
            CRITICAL_SECTION cs;
            Array* arr2 = nullptr;
            Array* arr5 = nullptr;

            before_each([&]{
                InitializeCriticalSection(&cs);
                arr2 = new Array(2);
                arr5 = new Array(5);
                startEv = CreateEvent(NULL, TRUE, FALSE, NULL);
                stopEv = CreateEvent(NULL, TRUE, FALSE, NULL);
                endEv = CreateEvent(NULL, TRUE, FALSE, NULL);
            });

            after_each([&]{
                DeleteCriticalSection(&cs);
                delete arr2;
                delete arr5;
                arr2 = arr5 = nullptr;
                if(startEv) CloseHandle(startEv);
                if(stopEv) CloseHandle(stopEv);
                if(endEv) CloseHandle(endEv);
                startEv = stopEv = endEv = nullptr;
            });

            it("creates thread with valid parameters", [&]{
                Thread thr(startEv, endEv, stopEv, &cs, arr5, 1, 5);
            });

            it("throws on null startEvent", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(nullptr, endEv, stopEv, &cs, arr2, 1, 2));
            });

            it("throws on null endEvent", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, nullptr, stopEv, &cs, arr2, 1, 2));
            });

            it("throws on null stopEvent", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, endEv, nullptr, &cs, arr2, 1, 2));
            });

            it("throws on null criticalSection", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, endEv, stopEv, nullptr, arr2, 1, 2));
            });

            it("throws on null array", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, endEv, stopEv, &cs, nullptr, 1, 2));
            });

            it("throws on zero threadNumber", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, endEv, stopEv, &cs, arr2, 0, 2));
            });

            it("throws on negative threadNumber", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, endEv, stopEv, &cs, arr2, -1, 2));
            });

            it("throws on zero arraySize", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, endEv, stopEv, &cs, arr2, 1, 0));
            });

            it("throws on negative arraySize", [&]{
                AssertThrows(std::invalid_argument,
                             Thread(startEv, endEv, stopEv, &cs, arr2, 1, -2));
            });
        });

        describe("wait method", []{
            HANDLE startEv = nullptr;
            HANDLE stopEv = nullptr;
            HANDLE endEv = nullptr;
            CRITICAL_SECTION cs;
            Array* arr1 = nullptr;

            before_each([&]{
                InitializeCriticalSection(&cs);
                arr1 = new Array(1);
                startEv = CreateEvent(NULL, TRUE, FALSE, NULL);
                stopEv = CreateEvent(NULL, TRUE, FALSE, NULL);
                endEv = CreateEvent(NULL, TRUE, FALSE, NULL);
            });

            after_each([&]{
                DeleteCriticalSection(&cs);
                delete arr1;
                arr1 = nullptr;
                if(startEv) CloseHandle(startEv);
                if(stopEv) CloseHandle(stopEv);
                if(endEv) CloseHandle(endEv);
                startEv = stopEv = endEv = nullptr;
            });

            it("returns when thread completes", [&]{
                CoutRedirect silence;
                Thread thr(startEv, endEv, stopEv, &cs, arr1, 1, 1);
                SetEvent(startEv);
                Sleep(100);
                SetEvent(endEv);
                thr.wait();
            });

            it("handles already terminated thread", [&]{
                CoutRedirect silence;
                Thread thr(startEv, endEv, stopEv, &cs, arr1, 1, 1);
                SetEvent(startEv);
                Sleep(100);
                SetEvent(endEv);
                thr.wait();
            });
        });

        describe("destructor", []{
            HANDLE startEv = nullptr;
            HANDLE stopEv = nullptr;
            HANDLE endEv = nullptr;
            CRITICAL_SECTION cs;
            Array* arr1 = nullptr;

            before_each([&]{
                InitializeCriticalSection(&cs);
                arr1 = new Array(1);
                startEv = CreateEvent(NULL, TRUE, FALSE, NULL);
                stopEv = CreateEvent(NULL, TRUE, FALSE, NULL);
                endEv = CreateEvent(NULL, TRUE, FALSE, NULL);
            });

            after_each([&]{
                DeleteCriticalSection(&cs);
                delete arr1;
                arr1 = nullptr;
                if(startEv) CloseHandle(startEv);
                if(stopEv) CloseHandle(stopEv);
                if(endEv) CloseHandle(endEv);
                startEv = stopEv = endEv = nullptr;
            });

            it("closes thread handle", [&]{
                CoutRedirect silence;
                {
                    Thread thr(startEv, endEv, stopEv, &cs, arr1, 1, 1);
                }
            });
        });
    });
});
