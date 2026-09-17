#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <atomic>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class NetworkManager {
public:
	using DataCallback = std::function<void(const std::vector<uint8_t>&)>;

	NetworkManager();
	~NetworkManager();

	bool Connect(const char* ip, int port, bool is_server = false);
	void Disconnect();
	void Send(const void* data, size_t size);
	void SetRecieveDataCallback(DataCallback callback);

private:
	void StartThreads();
	void SendLoop();
	void RecvLoop();
	void SendInternal(const uint8_t* data, size_t size);
	SOCKET GetCurrentSocket() const;

	std::atomic<bool> is_running;
	SOCKET sock;
	SOCKET client_sock;
	std::thread send_thread;
	std::thread recv_thread;

	std::mutex send_mutex;
	std::queue<std::vector<uint8_t>> send_queue;

	DataCallback callback;
};
