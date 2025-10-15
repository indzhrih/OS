#include <bandit/bandit.h>
#include <windows.h>
#include "../Headers/Thread.h"
#include "../Headers/Array.h"

using namespace bandit;

go_bandit([]{
    describe("Thread", []{

        it("ctor: invalid args -> invalid_argument", []{
            CRITICAL_SECTION cs;
            InitializeCriticalSection(&cs);
            Array arr(2);

            AssertThrows(std::invalid_argument,
                         Thread(nullptr, nullptr, nullptr, &cs, &arr, 1, 2));

            HANDLE ev = CreateEvent(NULL, TRUE, FALSE, NULL);
            AssertThat(ev != NULL, Equals(true));

            AssertThrows(std::invalid_argument,
                         Thread(ev, ev, ev, &cs, &arr, 1, 0));
            AssertThrows(std::invalid_argument,
                         Thread(ev, ev, ev, &cs, &arr, 0, 2));

            CloseHandle(ev);
            DeleteCriticalSection(&cs);
        });

        it("Thread marks element, and then signals stopEvent when conflict and nulls endEvent", []{
            Array arr(1);
            HANDLE startEv = CreateEvent(NULL, TRUE, FALSE, NULL);
            HANDLE stopEv  = CreateEvent(NULL, TRUE, FALSE, NULL);
            HANDLE endEv   = CreateEvent(NULL, TRUE, FALSE, NULL);
            CRITICAL_SECTION cs; InitializeCriticalSection(&cs);

            struct CoutRedirect { std::streambuf* old; std::ostringstream oss;
                CoutRedirect(): old(std::cout.rdbuf(oss.rdbuf())) {}
                ~CoutRedirect(){ std::cout.rdbuf(old); }
            } silence;

            Thread thr(startEv, endEv, stopEv, &cs, &arr, 42, 1);

            SetEvent(startEv);
            auto wr = WaitForSingleObject(stopEv, 2000);
            AssertThat(wr, Equals((DWORD)WAIT_OBJECT_0));

            ResetEvent(startEv);

            SetEvent(endEv);
            thr.wait();
            AssertThat(arr[0], Equals(0));

            CloseHandle(startEv); CloseHandle(stopEv); CloseHandle(endEv);
            DeleteCriticalSection(&cs);
        });
    });
});