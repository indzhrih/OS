#include <bandit/bandit.h>
#include "../Headers/Event.h"
#include <thread>
#include <chrono>

using namespace bandit;
using namespace snowhouse;

go_bandit([]{
    describe("Event", []{
        describe("constructor", []{
            it("creates manual reset event with initial state false", []{
                Event ev(true, false);
                AssertThat(ev.is_set(), Equals(false));
            });

            it("creates manual reset event with initial state true", []{
                Event ev(true, true);
                AssertThat(ev.is_set(), Equals(true));
            });

            it("creates auto reset event with initial state false", []{
                Event ev(false, false);
                AssertThat(ev.is_set(), Equals(false));
            });

            it("creates auto reset event with initial state true", []{
                Event ev(false, true);
                AssertThat(ev.is_set(), Equals(true));
            });
        });

        describe("set method", []{
            it("sets manual reset event to signaled", []{
                Event ev(true, false);
                ev.set();
                AssertThat(ev.is_set(), Equals(true));
            });

            it("sets auto reset event to signaled", []{
                Event ev(false, false);
                ev.set();
                AssertThat(ev.is_set(), Equals(true));
            });

            it("sets event multiple times", []{
                Event ev(true, false);
                ev.set();
                ev.set();
                ev.set();
                AssertThat(ev.is_set(), Equals(true));
            });
        });

        describe("reset method", []{
            it("resets manual reset event", []{
                Event ev(true, true);
                ev.reset();
                AssertThat(ev.is_set(), Equals(false));
            });

            it("resets auto reset event", []{
                Event ev(false, true);
                ev.reset();
                AssertThat(ev.is_set(), Equals(false));
            });

            it("resets event multiple times", []{
                Event ev(true, true);
                ev.reset();
                ev.reset();
                ev.reset();
                AssertThat(ev.is_set(), Equals(false));
            });
        });

        describe("is_set method", []{
            it("returns true for set manual reset event", []{
                Event ev(true, true);
                AssertThat(ev.is_set(), Equals(true));
            });

            it("returns false for reset manual reset event", []{
                Event ev(true, false);
                AssertThat(ev.is_set(), Equals(false));
            });

            it("returns true for set auto reset event", []{
                Event ev(false, true);
                AssertThat(ev.is_set(), Equals(true));
            });

            it("returns false for reset auto reset event", []{
                Event ev(false, false);
                AssertThat(ev.is_set(), Equals(false));
            });
        });

        describe("wait method", []{
            it("returns immediately for already set manual reset event", []{
                Event ev(true, true);
                ev.wait();
                AssertThat(ev.is_set(), Equals(true));
            });

            it("returns immediately for already set auto reset event", []{
                Event ev(false, true);
                ev.wait();
                AssertThat(ev.is_set(), Equals(false));
            });

            it("waits for manual reset event to be set", []{
                Event ev(true, false);
                bool threadFinished = false;

                std::thread t([&]{
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    ev.set();
                    threadFinished = true;
                });

                ev.wait();
                AssertThat(threadFinished, Equals(true));
                AssertThat(ev.is_set(), Equals(true));

                t.join();
            });

            it("waits for auto reset event to be set and resets it", []{
                Event ev(false, false);
                bool threadFinished = false;

                std::thread t([&]{
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    ev.set();
                    threadFinished = true;
                });

                ev.wait();
                AssertThat(threadFinished, Equals(true));
                AssertThat(ev.is_set(), Equals(false));

                t.join();
            });
        });

        describe("manual reset behavior", []{
            it("manual reset event stays signaled after wait", []{
                Event ev(true, false);
                ev.set();
                ev.wait();
                AssertThat(ev.is_set(), Equals(true));
                ev.wait();
                AssertThat(ev.is_set(), Equals(true));
            });

            it("manual reset event requires explicit reset", []{
                Event ev(true, false);
                ev.set();
                ev.wait();
                ev.wait();
                ev.wait();
                AssertThat(ev.is_set(), Equals(true));
                ev.reset();
                AssertThat(ev.is_set(), Equals(false));
            });
        });

        describe("auto reset behavior", []{
            it("auto reset event resets after wait", []{
                Event ev(false, false);
                ev.set();
                ev.wait();
                AssertThat(ev.is_set(), Equals(false));
            });

            it("auto reset event resets automatically", []{
                Event ev(false, false);
                ev.set();
                ev.wait();
                ev.set();
                ev.wait();
                AssertThat(ev.is_set(), Equals(false));
            });
        });
    });
});
