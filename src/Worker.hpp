#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <thread>
#include <iomanip>
#include <syncstream>
#include <mutex>

namespace Core {
    class Worker {
    public:
        Worker();
        ~Worker() = default;
        void assignTask(long long clientId, std::shared_ptr<std::vector<char>> data);
        void stop();
        void run();
    private:
        std::mutex taskMutex;
        std::unordered_map<long long, std::queue<std::shared_ptr<std::vector<char>>>> tasks;
        std::unordered_map<long long, std::unique_ptr<std::mutex>> mutexes;
        bool running = false;

        void process();
        std::string IpToString(int ip);

        static std::mutex logMutex;
        static long long totalBytes;
        static int totalPackets;
    };
}
