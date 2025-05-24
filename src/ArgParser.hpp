#include <string>
#include <unordered_map>
#include <functional>

#define DEFAULT_PORT 2152
#define DEFAULT_WORKERS 1

class ArgParser {
    public:
        ArgParser();
        void parse(std::string flag, std::string value);

        short port = DEFAULT_PORT;
        short workers = DEFAULT_WORKERS;
    private:
        std::unordered_map<std::string, std::function<void(std::string)>> flagHandlers;
};
