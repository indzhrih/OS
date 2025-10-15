#include <bandit/bandit.h>
#include <sstream>
#include <iostream>
#include <mutex>
#include "../Headers/Thread.h"
#include "../Headers/Array.h"
#include "../Headers/Event.h"

using namespace bandit;

go_bandit([]{
    describe("Thread", []{

        it("ctor: invalid args -> invalid_argument", []{
            std::mutex cs;
            Array arr(2);

            AssertThrows(std::invalid_argument,
                         Thread(nullptr, nullptr, nullptr, &cs, &arr, 1, 2));

            Event* ev = new Event(true, false);

            AssertThrows(std::invalid_argument,
                         Thread(ev, ev, ev, &cs, &arr, 1, 0));
            AssertThrows(std::invalid_argument,
                         Thread(ev, ev, ev, &cs, &arr, 0, 2));

            delete ev;
        });

        it("Thread marks element, then signals stopEvent on conflict, after endEvent clears its marks", []{
            Array arr(1);
            Event* startEv = new Event(true, false);
            Event* stopEv = new Event(true, false);
            Event* endEv = new Event(true, false);
            std::mutex cs;

            struct CoutRedirect {
                std::streambuf* old; std::ostringstream oss;
                CoutRedirect(): old(std::cout.rdbuf(oss.rdbuf())) {}
                ~CoutRedirect(){ std::cout.rdbuf(old); }
            } silence;

            Thread thr(startEv, endEv, stopEv, &cs, &arr, 42, 1);

            startEv->set();

            stopEv->wait();

            startEv->reset();

            endEv->set();
            thr.wait();

            AssertThat(arr[0], Equals(0));

            delete startEv; delete stopEv; delete endEv;
        });
    });
});
