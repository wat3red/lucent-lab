#include <oxorany/oxorany.h>
#include "network_manager.h"
#include "logger.h"

std::condition_variable send_cv;

NetworkManager::NetworkManager() : is_running(false), sock(-1), client_sock(-1) {
    // LOGD("NetworkManager initialized");
}

NetworkManager::~NetworkManager() {
    // LOGD("NetworkManager destructor called");
    Disconnect();
}

bool NetworkManager::Connect(const char* ip, int port, bool is_server) {
    LOGD("Connect called with IP: %s, port: %d, is_server: %d", ip, port, is_server);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        LOGE("Failed to create socket: %s", strerror(errno));
        return false;
    }

    int flag = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    // int priority = 6; // SO_PRIORITY для Linux
    // setsockopt(sock, SOL_SOCKET, SO_PRIORITY, &priority, sizeof(priority));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    LOGD("Created socket with descriptor: %d", sock);

    if (is_server) {
        LOGI("Operating in server mode");

        if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
            LOGE("Bind failed: %s", strerror(errno));
            return false;
        }
        LOGD("Socket bound successfully");

        if (listen(sock, 1) == -1) {
            LOGE("Listen failed: %s", strerror(errno));
            return false;
        }
        LOGD("Socket listening for connections");

        LOGD("Waiting for client to connect...");
        client_sock = accept(sock, nullptr, nullptr);

        setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
        // setsockopt(client_sock, SOL_SOCKET, SO_PRIORITY, &priority, sizeof(priority));

        if (client_sock == -1) {
            LOGE("Accept failed: %s", strerror(errno));
            return false;
        }


        LOGD("Client connected with socket: %d", client_sock);
    }
    else {
        LOGI("Operating in client mode");
        while (true) {
            if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
                sleep(1);
            }
            else {
                LOGI("Connected successfully to server");
                break;
            }
        }
    }

    StartThreads();
    return true;
}

void NetworkManager::Disconnect() {
    LOGI("Disconnect called");
    is_running = false;

    if (send_thread.joinable()) {
        LOGI("Joining send thread");
        send_thread.join();
    }

    if (recv_thread.joinable()) {
        LOGI("Joining receive thread");
        recv_thread.join();
    }

    if (sock != -1) {
        LOGI("Closing main socket: %d", sock);
        close(sock);
    }

    if (client_sock != -1) {
        LOGI("Closing client socket: %d", client_sock);
        close(client_sock);
    }

    LOGI("Disconnect complete");
}

// In NetworkManager.cpp
void NetworkManager::Send(const void* data, size_t size) {
    // Prevent sending oversized packets that can't be represented by uint32_t
    if (size > UINT32_MAX) {
        LOGE("Packet size too large to send.");
        return;
    }

    // Create a new vector for the full packet (header + payload)
    std::vector<uint8_t> packet;
    packet.reserve(sizeof(uint32_t) + size);

    // 1. Create the header: a 4-byte size prefix in network byte order
    uint32_t net_size = htonl(static_cast<uint32_t>(size));

    // 2. Add header and payload to the packet
    packet.insert(packet.end(), (uint8_t*)&net_size, (uint8_t*)&net_size + sizeof(uint32_t));
    packet.insert(packet.end(), static_cast<const uint8_t*>(data), static_cast<const uint8_t*>(data) + size);

    // 3. Queue the complete packet for sending
    {
        std::lock_guard<std::mutex> lock(send_mutex);
        send_queue.push(std::move(packet));
    }
    send_cv.notify_one();
}

void NetworkManager::SetRecieveDataCallback(DataCallback callback) {
    this->callback = callback;
}

void NetworkManager::StartThreads() {
    LOGI("Starting network threads");
    is_running = true;

    send_thread = std::thread(&NetworkManager::SendLoop, this);
    recv_thread = std::thread(&NetworkManager::RecvLoop, this);
    LOGI("Network threads started");
}

void NetworkManager::SendLoop() {
    LOGI("SendLoop thread started");
    int packet_count = 0;

    while (is_running) {
        std::vector<uint8_t> packet;

        {
            std::unique_lock<std::mutex> lock(send_mutex);
            send_cv.wait(lock, [this] {
                return !send_queue.empty() || !is_running;
                });

            if (!is_running && send_queue.empty())
                break;

            packet = std::move(send_queue.front());
            send_queue.pop();
        }

        if (!packet.empty()) {
            // LOGD("SendLoop processing packet #", ++packet_count, " size: ", packet.size());
            SendInternal(packet.data(), packet.size());
        }
    }

    LOGD("SendLoop thread exiting");
}

void NetworkManager::RecvLoop() {
    LOGD("RecvLoop thread started");

    std::vector<uint8_t> buffer(4096);
    int packet_count = 0;

    while (is_running) {
        int current_socket = GetCurrentSocket();
        // LOGD("Waiting to receive data on socket: ", current_socket);

        int received = recv(current_socket, buffer.data(), buffer.size(), 0);

        if (received < 0) {
            LOGE("Receive error: %s", strerror(errno));
            break;
        }

        if (received == 0) {
            // LOGD("Connection closed by peer");
            break;
        }

        // LOGD("Received packet #", ++packet_count, " size: ", received, " bytes");

        if (received > 0 && callback) {
            std::vector<uint8_t> data(buffer.begin(), buffer.begin() + received);
            // LOGD("Invoking data callback with ", received, " bytes");
            callback(data);
        }
    }
    LOGD("RecvLoop thread exiting");
}

void NetworkManager::SendInternal(const uint8_t* data, size_t size) {
    // LOGD("SendInternal called with ", size, " bytes");
    int current_socket = GetCurrentSocket();

    size_t total_sent = 0;
    while (total_sent < size && is_running) {
        int sent = send(current_socket, data + total_sent, size - total_sent, 0);

        if (sent <= 0) {
            LOGE("Send error: %s", strerror(errno));
            break;
        }

        total_sent += sent;
        // LOGD("Sent ", sent, " bytes, total: ", total_sent, "/", size);
    }
}

int NetworkManager::GetCurrentSocket() const {
    int current_socket = client_sock != -1 ? client_sock : sock;
    return current_socket;
}