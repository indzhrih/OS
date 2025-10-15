#include <bandit/bandit.h>
#include "../Headers/ThreadHandler.h"

using namespace bandit;

go_bandit([]{
    describe("threadWorker", []{
        it("Returns error (1), if param == NULL", []{
            int rc = threadWorker(nullptr);
            AssertThat(rc, Equals<int>(1));
        });
    });
});
