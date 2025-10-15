#include <bandit/bandit.h>
#include "../Headers/ThreadHandler.h"

using namespace bandit;

go_bandit([]{
    describe("threadWorker", []{
        it("Returns error (1), if param == NULL", []{
            DWORD rc = threadWorker(nullptr);
            AssertThat(rc, Equals<DWORD>(1));
        });
    });
});