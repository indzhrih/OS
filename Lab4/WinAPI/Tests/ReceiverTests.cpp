#include "doctest/doctest.h"
#include <windows.h>

TEST_CASE("Receiver-style CreateEvent manual reset succeeds") {
    HANDLE e = CreateEventA(NULL, TRUE, FALSE, "ReadyEvent_Test");
    CHECK(e != NULL);
    if (e) CloseHandle(e);
}

TEST_CASE("Receiver-style message semaphore increments and decrements") {
    HANDLE s = CreateSemaphoreA(NULL, 0, 2, "MessageSemaphore_Test");
    ReleaseSemaphore(s, 1, NULL);
    DWORD w = WaitForSingleObject(s, 0);
    CHECK(w == WAIT_OBJECT_0);
    if (s) CloseHandle(s);
}

TEST_CASE("Receiver-style free slots semaphore blocks after zero") {
    HANDLE fs = CreateSemaphoreA(NULL, 1, 1, "FreeSlotsSemaphore_Test");
    DWORD w1 = WaitForSingleObject(fs, 0);
    (void)w1;
    DWORD w2 = WaitForSingleObject(fs, 0);
    CHECK(w2 == WAIT_TIMEOUT);
    if (fs) CloseHandle(fs);
}
