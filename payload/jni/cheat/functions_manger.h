#pragma once

class FunctionsManager {
    std::vector<Function*> functions;
    NetworkManager* networkManager;
public:
    void SetNetworkManager(NetworkManager* newNetworkManager) {
        networkManager = newNetworkManager;
    }

    std::vector<Function*> GetFunctions() {
        return functions;
    }

    void AddFunction(Function* func) {
        functions.push_back(func);
    }

    void InitFunctions() {
        for (const auto& function : functions) {
            // LOGD("function: %d", function->GetId());
            function->Init();
        }
    }

    void HandleIncomingData(const uint8_t* data, size_t size) {
        while (size >= sizeof(FunctionPacket)) {
            const FunctionPacket* packet = reinterpret_cast<const FunctionPacket*>(data);

            const size_t totalSize = sizeof(FunctionPacket) + packet->dataSize;

            if (size < totalSize) break;

            for (auto* func : functions) {
                if (func->GetId() == packet->functionId) {
                    func->ApplySettings(data + sizeof(FunctionPacket));
                    break;
                }
            }

            data += totalSize;
            size -= totalSize;
        }
    }
    std::vector<uint8_t> batched_data_buffer;
    std::vector<uint8_t> temp_data_buffer;
    void SendAllData() {
        batched_data_buffer.clear();
        batched_data_buffer.reserve(4096);

        auto loop_start_time = std::chrono::high_resolution_clock::now();

        for (auto* func : functions) {
            // auto func_start_time = std::chrono::high_resolution_clock::now();

            temp_data_buffer.clear();
            func->GetData(temp_data_buffer); // The function we are timing

            // auto func_end_time = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double, std::milli> func_time = func_end_time - func_start_time;

            // Log the time for each specific function
            // You'll need a way to identify the function, e.g., by its ID or a name.
            // LOGD("GetData for functionId %d took: %f ms", func->GetId(), func_time.count());

            if (temp_data_buffer.empty()) {
                continue;
            }

            // ... rest of the code to insert header and data ...
            FunctionPacket header;
            header.functionId = func->GetId();
            header.type = PACKET_DATA;
            header.dataSize = temp_data_buffer.size();
            batched_data_buffer.insert(batched_data_buffer.end(), (uint8_t*)&header, (uint8_t*)&header + sizeof(FunctionPacket));
            batched_data_buffer.insert(batched_data_buffer.end(), temp_data_buffer.begin(), temp_data_buffer.end());
        }

        if (!batched_data_buffer.empty()) {
            networkManager->Send(batched_data_buffer.data(), batched_data_buffer.size());
        }
    }
};