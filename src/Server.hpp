#include <Worker.hpp>
#include <vector>
#include <functional>

namespace Core {
    class Server
    {
    public:
        Server(short port, short workerCount);
        ~Server();
        void run();
        void stop();
    private:
        short port;
        bool running = false;
        std::vector<std::unique_ptr<Worker>> workers;
        std::vector<std::thread> threads;

        void listen(std::function<void(long long clientId, const std::array<char, 1024>& buffer, size_t size)> dispatch);
    };
}
