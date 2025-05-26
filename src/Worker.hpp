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
        ~Worker();
        void assignTask(long long clientId, std::shared_ptr<std::vector<char>> data);
        void stop();
        void run();
    private:
        std::mutex taskMutex;
        std::unordered_map<long long, std::pair<std::unique_ptr<std::mutex>, std::queue<std::shared_ptr<std::vector<char>>>>> tasks;
        bool running = false;

        void process();
        void flush();
        std::string IpToString(int ip);

        static std::mutex logMutex;
        static long long totalBytes;
        static int totalPackets;
    };
}
