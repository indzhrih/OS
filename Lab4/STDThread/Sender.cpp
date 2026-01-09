#include <iostream>
#include <thread>
#include "Headers/SharedState.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Not enough args!";
        return 1;
    }

    SharedState state(argv[1]);

    std::thread inputThread(&SharedState::runInputLoop, &state);
    std::thread writerThread(&SharedState::runWriterLoop, &state);

    inputThread.join();
    state.requestStop();
    writerThread.join();

    return 0;
}
