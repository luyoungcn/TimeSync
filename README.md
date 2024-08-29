# TimeSync

一个小工具，用于将 PC 端的时间同步到嵌入式板卡。

适用于 Ubuntu 操作系统。

使用方法：

`./TimeSync`

日志打印最终时间同步的结果。



```bash
./TimeSync
2024-08-16 16:29:36
timestamp: 1723796976304927
Received timestamp: 1723796976304956
时间同步成功，差值：29 微秒
```


# 嵌入式板卡端代码参考

```cpp
zmq::message_t request;
requestSocket_.recv(request, zmq::recv_flags::none);

std::string requestData(static_cast<char*>(request.data()), request.size());

if (requestData.substr(0, strlen("req_timesync")) == "req_timesync") {
    const size_t string_length = strlen("req_timesync");
    const size_t timestamp_offset = string_length + 1;

    if (requestData.size() >= timestamp_offset + sizeof(uint64_t)) {
        uint64_t timestamp;
        memcpy(&timestamp, &requestData[timestamp_offset], sizeof(uint64_t));

        spdlog::info("Received request timestamp: {}", timestamp);
        struct timespec new_time;
        new_time.tv_sec = static_cast<time_t>(timestamp / 1000000);
        new_time.tv_nsec = (timestamp % 1000000) * 1000;

        int result = clock_settime(CLOCK_REALTIME, &new_time);
        if (result == -1) {
            spdlog::error("Failed to set system time");
        }
    } else {
        std::cerr << "Received message is too short to contain a timestamp." << std::endl;
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

    std::string errorMessage = "invalid_request";
   
    zmq::message_t response(errorMessage.size());
    memcpy(response.data(), errorMessage.c_str(), errorMessage.size());
    requestSocket_.send(response, zmq::send_flags::none);
}
```
