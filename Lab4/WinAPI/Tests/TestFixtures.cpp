#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>

struct SilenceCerr {
    std::streambuf* old{};
    std::ostringstream sink;
    SilenceCerr()  { old = std::cerr.rdbuf(sink.rdbuf()); }
    ~SilenceCerr() { std::cerr.rdbuf(old); }
};

namespace TestFixtures {
    static DWORD run_process_and_wait(const std::string& cmdline) {
        STARTUPINFOA si{};
        PROCESS_INFORMATION pi{};
        si.cb = sizeof(si);
        std::string cmd = cmdline;

        BOOL ok = CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
        if (!ok) return 0xFFFFFFFF;

        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD code = 0;
        GetExitCodeProcess(pi.hProcess, &code);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return code;
    }

    static void prepare_ring_file(const std::string& fname, int capacity) {
        FILE* f = std::fopen(fname.c_str(), "wb");
        int head = 0, tail = 0;
        std::fwrite(&capacity, sizeof(int), 1, f);
        std::fwrite(&head, sizeof(int), 1, f);
        std::fwrite(&tail, sizeof(int), 1, f);
        std::fclose(f);
    }

    static void cleanup_file(const std::string& fname) {
        std::remove(fname.c_str());
    }
}
