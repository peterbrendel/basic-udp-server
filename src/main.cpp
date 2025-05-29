#include <iostream>
#include <ArgParser.hpp>
#include <Server.hpp>
#include <signal.h>

#define unused(x) (void)(x)

int main(int argc, char* argv[]) {
    static Core::Server* server;
    {
        std::vector<std::string> args(argv, argv + argc);
        ArgParser argParser;

        for (int i = 1; i < argc-1; i += 2) {
            if (i + 1 < argc) {
                argParser.parse(args[i], args[i + 1]);
            }
        }
        server = new Core::Server(argParser.port, argParser.workers);
    }

    struct sigaction sa;
    sa.sa_handler = [](int signum) {
        unused(signum);
        std::cout << std::endl;
        if (server) {
            server->stop();
        }
    };

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    server->run();

    if (server) {
        delete server;
        server = nullptr;
    }

    std::cout << "Server stopped gracefully." << std::endl;
    std::cout << "Exiting..." << std::endl;

    return 0;
}
