#include <bandit/bandit.h>
#include <windows.h>
#include "../Headers/ThreadHandler.h"
#include "../Headers/Array.h"
#include "TestFixtures.cpp"

using namespace bandit;
using namespace snowhouse;

go_bandit([]{
    describe("threadWorker function", []{
        HANDLE startEv = nullptr;
        HANDLE stopEv = nullptr;
        HANDLE endEv = nullptr;
        CRITICAL_SECTION cs;

        before_each([&]{
            InitializeCriticalSection(&cs);
            startEv = CreateEvent(NULL, TRUE, FALSE, NULL);
            stopEv = CreateEvent(NULL, TRUE, FALSE, NULL);
            endEv = CreateEvent(NULL, TRUE, FALSE, NULL);
        });

        after_each([&]{
            DeleteCriticalSection(&cs);
            if(startEv) CloseHandle(startEv);
            if(stopEv) CloseHandle(stopEv);
            if(endEv) CloseHandle(endEv);
            startEv = stopEv = endEv = nullptr;
        });

        it("returns 1 when param is NULL", []{
            DWORD rc = threadWorker(nullptr);
            AssertThat(rc, Equals<DWORD>(1));
        });

        it("handles null startEvent", [&]{
            struct MockThread {
                HANDLE startEvent;
                HANDLE endEvent;
                HANDLE stopEvent;
                CRITICAL_SECTION* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            MockThread mock;
            mock.startEvent = nullptr;
            mock.endEvent = endEv;
            mock.stopEvent = stopEv;
            mock.criticalSection = &cs;
            mock.array = new Array(1);
            mock.arraySize = 1;
            mock.threadNumber = 1;

            DWORD rc = threadWorker(&mock);
            AssertThat(rc, Equals<DWORD>(1));

            delete mock.array;
        });

        it("handles null criticalSection", [&]{
            struct MockThread {
                HANDLE startEvent;
                HANDLE endEvent;
                HANDLE stopEvent;
                CRITICAL_SECTION* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            MockThread mock;
            mock.startEvent = startEv;
            mock.endEvent = endEv;
            mock.stopEvent = stopEv;
            mock.criticalSection = nullptr;
            mock.array = new Array(1);
            mock.arraySize = 1;
            mock.threadNumber = 1;

            DWORD rc = threadWorker(&mock);
            AssertThat(rc, Equals<DWORD>(1));

            delete mock.array;
        });

        it("handles invalid arraySize zero", [&]{
            struct MockThread {
                HANDLE startEvent;
                HANDLE endEvent;
                HANDLE stopEvent;
                CRITICAL_SECTION* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            MockThread mock;
            mock.startEvent = startEv;
            mock.endEvent = endEv;
            mock.stopEvent = stopEv;
            mock.criticalSection = &cs;
            mock.array = new Array(1);
            mock.arraySize = 0;
            mock.threadNumber = 1;

            DWORD rc = threadWorker(&mock);
            AssertThat(rc, Equals<DWORD>(1));

            delete mock.array;
        });

        it("handles invalid threadNumber zero", [&]{
            struct MockThread {
                HANDLE startEvent;
                HANDLE endEvent;
                HANDLE stopEvent;
                CRITICAL_SECTION* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            MockThread mock;
            mock.startEvent = startEv;
            mock.endEvent = endEv;
            mock.stopEvent = stopEv;
            mock.criticalSection = &cs;
            mock.array = new Array(1);
            mock.arraySize = 1;
            mock.threadNumber = 0;

            DWORD rc = threadWorker(&mock);
            AssertThat(rc, Equals<DWORD>(1));

            delete mock.array;
        });

        it("handles null array pointer", [&]{
            struct MockThread {
                HANDLE startEvent;
                HANDLE endEvent;
                HANDLE stopEvent;
                CRITICAL_SECTION* criticalSection;
                Array* array;
                int arraySize;
                int threadNumber;
            };

            MockThread mock;
            mock.startEvent = startEv;
            mock.endEvent = endEv;
            mock.stopEvent = stopEv;
            mock.criticalSection = &cs;
            mock.array = nullptr;
            mock.arraySize = 1;
            mock.threadNumber = 1;

            DWORD rc = threadWorker(&mock);
            AssertThat(rc, Equals<DWORD>(1));
        });
    });
});
