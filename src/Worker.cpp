#include <Worker.hpp>
#include <iostream>

namespace Core {
    std::mutex Worker::logMutex;
    long long Worker::totalBytes;
    int Worker::totalPackets;

    Worker::Worker()
    {
        running = true;
    }

    void Worker::assignTask(long long clientId, std::shared_ptr<std::vector<char>> data)
    {
        if (running == false) {
            return;
        }

        std::unique_lock<std::mutex> taskLock(taskMutex);
        // If new client create a new queue and mutex for this clientId
        if (tasks.find(clientId) == tasks.end()) {
            tasks[clientId] = { std::make_unique<std::mutex>(), std::queue<std::shared_ptr<std::vector<char>>>() };
        }

        // Lock the mutex for this clientId and push data into the queue
        std::unique_lock<std::mutex> queueLock(*tasks[clientId].first);
        tasks[clientId].second.emplace(std::move(data));

        // Unlock the mutex automatically when going out of scope
    }

    void Worker::stop()
    {
        std::cout << "Gracefully stopping worker..." << std::endl;
        running = false;
        process();

        std::cout << "Worker stopped." << std::endl;
    }

    void Worker::run() {
        std::unique_lock<std::mutex> lock(logMutex);
        std::cout << "Worker " << std::this_thread::get_id() << " started" << std::endl;
        lock.unlock();

        while (running) {
            process();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void Worker::process() {
        for (auto it = tasks.begin(); it != tasks.end(); ) {
            auto& [clientId, task] = *it;
            auto& [mutex, taskQueue] = task;

            std::unique_lock<std::mutex> queueLock(*mutex);

            if (taskQueue.empty()) {
                std::unique_lock<std::mutex> taskLock(taskMutex);
                it = tasks.erase(it);
                continue;
            }

            auto data = taskQueue.front();
            taskQueue.pop();
            queueLock.unlock();

            auto threadId = std::this_thread::get_id();
            auto clock = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            int ip = clientId >> 16;
            unsigned short port = clientId & 0xFFFF;

            std::unique_lock<std::mutex> logLock(logMutex);
            std::cout << std::right << std::setw(15) << threadId << " | ";
            std::cout << std::put_time(std::localtime(&clock), "%Y-%m-%d %H:%M:%S") << " | ";
            std::cout << std::right << std::setw(15) << IpToString(ip) << ":" << std::left << std::setw(5) << port << " | ";
            std::cout << "incoming " << std::right << std::setw(4) << data->size() << " bytes | ";
            std::cout << "total bytes: " << std::left << std::setw(12) << totalBytes << " | ";
            std::cout << "total packets: " << std::left << std::setw(12) << totalPackets << '\n';
            std::cout << std::flush;
            totalBytes += data->size();
            totalPackets++;
        }
    }

    std::string Worker::IpToString(int ip) {
        std::string ipStr;

        for (int i = 0; i < 4; ++i) {
            ipStr += std::to_string((ip >> (i * 8)) & 0xFF);
            if (i < 3) {
                ipStr += ".";
            }
        }

        return ipStr;
    }
}
