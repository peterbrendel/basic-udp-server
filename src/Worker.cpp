#include <Worker.hpp>
#include <iostream>
#include <thread>
#include <iomanip>
#include <syncstream>

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
            tasks[clientId] = std::queue<std::shared_ptr<std::vector<char>>>();
            mutexes[clientId] = std::make_unique<std::mutex>();
        }

        // Lock the mutex for this clientId and push data into the queue
        std::unique_lock<std::mutex> queueLock(*mutexes[clientId]);
        tasks[clientId].emplace(std::move(data));

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
        for (auto& [clientId, taskQueue] : tasks) {
            // Process the tasks in the queue
            std::unique_lock<std::mutex> lock(*mutexes[clientId]);
            auto data = taskQueue.front();
            taskQueue.pop();

            auto threadId = std::this_thread::get_id();
            auto clock = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            int ip = clientId & (2 << (sizeof(short) * 8 - 1));
            short port = clientId >> sizeof(short) * 8;

            std::unique_lock<std::mutex> logLock(logMutex);
            std::cout << threadId << "\t| ";
            std::cout << std::put_time(std::localtime(&clock), "%Y-%m-%d %H:%M:%S") << " | ";
            std::cout << IpToString(ip) << ":" << port << " | ";
            std::cout << "payload size: " << data->size() << " bytes\t| ";
            std::cout << "total bytes: " << totalBytes << "\t| ";
            std::cout << "total packets: " << totalPackets << "\t| ";
            std::cout << std::endl;
            totalBytes += data->size();
            totalPackets++;
            logLock.unlock();

            std::unique_lock<std::mutex> taskLock(taskMutex);
            if (taskQueue.empty()) {
                tasks.erase(clientId);
                mutexes.erase(clientId);
            }
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
