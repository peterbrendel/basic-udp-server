#include <Server.hpp>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace Core {
    Server::Server(short port, short workerCount) : port(port)
    {
        for (short i = 0; i < workerCount; ++i) {
            workers.emplace_back(std::make_unique<Worker>());
        }
    }

    Server::~Server()
    {
        std::cout << "Attempting to stop server gracefully" << std::endl;
        for (auto& worker : workers) {
            worker->stop();
        }
    }

    void Server::run()
    {
        std::cout << "Server is running..." << std::endl;

        std::vector<std::thread> threads;
        for (auto& worker : workers) {
            threads.emplace_back([&worker]() {
                worker->run();
            });
        }

        listen([&workers = this->workers](long long clientId, const std::array<char, 1024>& buffer, size_t size) {
            // copy the buffer to a heap-allocated vector
            std::vector<char> data(buffer.begin(), buffer.begin() + size);

            // find the worker responsible for this client
            short workerIndex = clientId % workers.size();

            // provide ownership of the data to the worker
            workers[workerIndex]->assignTask(clientId, std::make_shared<std::vector<char>>(std::move(data)));

            // buffer is destroyed but the data is still available in the worker
        });

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

    }

    void Server::listen(std::function<void(long long clientId, const std::array<char, 1024>& buffer, size_t size)> dispatch)
    {
        int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Failed to bind socket" << std::endl;
            close(serverSocket);
            return;
        }

        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);
        std::array<char, 1024> buffer;

        while (true) {
            ssize_t received = recvfrom(serverSocket, buffer.data(), buffer.size() - 1, 0, (sockaddr*)&clientAddr, &clientAddrLen);
            if (received < 0) {
                std::cerr << "Failed to receive data" << std::endl;
                continue;
            }

            buffer[received] = '\0';

#ifdef DEBUG_BUILD
            char clientIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIp, sizeof(clientIp));
            std::cout << "Received data from client" << std::endl;
            std::cout << "Client address: " << clientIp << ":" << clientPort << std::endl;
            std::cout << "Data: " << buffer << std::endl;
#endif
            int clientIp = ntohl(clientAddr.sin_addr.s_addr);
            short clientPort = ntohs(clientAddr.sin_port);

            long long clientId = (clientIp << sizeof(short) * 8) | clientPort;

            dispatch(clientId, buffer, received);
        }

        close(serverSocket);
    }
}
