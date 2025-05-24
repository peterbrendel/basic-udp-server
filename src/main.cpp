#include <iostream>
#include <ArgParser.hpp>
#include <Server.hpp>

int main(int argc, char* argv[]) {
    Core::Server* server;
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

    server->run();
    delete server;

    return 0;
}
