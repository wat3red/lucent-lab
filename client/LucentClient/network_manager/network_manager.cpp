#include "network_manager.h"

#include <iostream>
#include <oxorany/oxorany_include.h>

NetworkManager::NetworkManager() : is_running(false), sock(INVALID_SOCKET), client_sock(INVALID_SOCKET) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

NetworkManager::~NetworkManager() {
	Disconnect();
	WSACleanup();
}

bool NetworkManager::Connect(const char* ip, int port, bool is_server) {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) return false;

	char flag = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)); 

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);

	if (is_server) {
		std::cout << oxorany("Server was started") << std::endl;
		bind(sock, (sockaddr*)&addr, sizeof(addr));
		listen(sock, 1);
		client_sock = accept(sock, nullptr, nullptr);

		setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)); 

		std::cout << oxorany("client_sock: ") << client_sock << std::endl;
		if (client_sock == INVALID_SOCKET) return false;
	}
	else {
		if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) return false;
	}

	StartThreads();
	return true;
}

void NetworkManager::Disconnect() {
	is_running = false;
	if (send_thread.joinable()) send_thread.join();
	if (recv_thread.joinable()) recv_thread.join();

	closesocket(sock);
	if (client_sock != INVALID_SOCKET) closesocket(client_sock);
}

std::condition_variable send_cv;

void NetworkManager::Send(const void* data, size_t size) {
	{
		std::lock_guard<std::mutex> lock(send_mutex);
		send_queue.emplace(static_cast<const uint8_t*>(data), static_cast<const uint8_t*>(data) + size);
	}
	send_cv.notify_one();
}

void NetworkManager::SetRecieveDataCallback(DataCallback callback) {
	this->callback = callback;
}

void NetworkManager::StartThreads() {
	is_running = true;
	send_thread = std::thread(&NetworkManager::SendLoop, this);
	recv_thread = std::thread(&NetworkManager::RecvLoop, this);
}

void NetworkManager::SendLoop() {
	while (is_running) {
		std::unique_lock<std::mutex> lock(send_mutex);
		send_cv.wait(lock, [&] { return !send_queue.empty() || !is_running; });

		if (!is_running) break;
		std::vector<uint8_t> packet = std::move(send_queue.front());
		send_queue.pop();
		lock.unlock();

		SendInternal(packet.data(), packet.size());
	}
}

// In NetworkManager.cpp
void NetworkManager::RecvLoop() {
	std::vector<uint8_t> buffer; // Persistent buffer to store incoming data
	uint32_t packet_size = 0;

	char temp_buf[4096];
	int current_socket = GetCurrentSocket();

	while (is_running) {
		int received = recv(current_socket, temp_buf, sizeof(temp_buf), 0);

		if (received <= 0) {
			is_running = false; // Signal other threads to stop
			break;
		}

		// Add newly received data to our persistent buffer
		buffer.insert(buffer.end(), temp_buf, temp_buf + received);

		// Process all complete packets that might be in the buffer
		while (true) {
			// 1. If we don't know the size of the next packet, try to read it
			if (packet_size == 0) {
				if (buffer.size() >= sizeof(uint32_t)) {
					// Read the 4-byte size header
					memcpy(&packet_size, buffer.data(), sizeof(uint32_t));
					packet_size = ntohl(packet_size); // Convert from network to host byte order

					// Remove the size header from the buffer
					buffer.erase(buffer.begin(), buffer.begin() + sizeof(uint32_t));
				}
				else {
					// Not enough data to read the size header, wait for more
					break;
				}
			}

			// 2. If we know the packet size, check if we have received the full packet
			if (packet_size > 0 && buffer.size() >= packet_size) {
				// We have a complete packet
				std::vector<uint8_t> complete_packet(buffer.begin(), buffer.begin() + packet_size);

				if (callback) {
					callback(complete_packet);
				}

				// Remove the processed packet from the buffer
				buffer.erase(buffer.begin(), buffer.begin() + packet_size);

				// Reset to read the next packet's header
				packet_size = 0;
			}
			else {
				// Not enough data for a full packet, wait for more
				break;
			}
		}
	}
	send_cv.notify_all(); // Wake up sender thread to let it exit cleanly
}
//
//void NetworkManager::RecvLoop() {
//	std::vector<uint8_t> buffer(4096);
//	while (is_running) {
//		int received = recv(GetCurrentSocket(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);
//
//		if (received > 0 && callback) {
//			std::vector<uint8_t> data(buffer.begin(), buffer.begin() + received);
//			callback(data);
//		}
//	}
//}

void NetworkManager::SendInternal(const uint8_t* data, size_t size) {
	size_t total_sent = 0;
	while (total_sent < size && is_running) {
		int sent = send(GetCurrentSocket(), reinterpret_cast<const char*>(data + total_sent), size - total_sent, 0);
		if (sent <= 0) break;
		total_sent += sent;
	}
}

SOCKET NetworkManager::GetCurrentSocket() const {
	return client_sock != INVALID_SOCKET ? client_sock : sock;
}