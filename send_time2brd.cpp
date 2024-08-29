#include "send_time2brd.h"
#include "zmq.hpp"
#include <iostream>

#define REQ_ADDR "tcp://172.16.1.10:4866"

void send_time2brd(uint64_t timestamp)
{
    uint64_t resp_timestamp;

    zmq::context_t context;
    zmq::socket_t senderSocket(context, ZMQ_REQ);

    senderSocket.connect(REQ_ADDR);

    const char* requestString = "req_radar_timesync";

    size_t stringLength = strlen(requestString);
    size_t messageLength = stringLength + 1 + sizeof(uint64_t); // 1 for null-terminator

    zmq::message_t message(messageLength);

    memcpy(message.data(), requestString, stringLength);
    static_cast<char*>(message.data())[stringLength] = '\0';

    memcpy(static_cast<char*>(message.data()) + stringLength + 1, &timestamp, sizeof(uint64_t));

    senderSocket.send(message, zmq::send_flags::none);

    zmq::message_t reply;
    senderSocket.recv(reply, zmq::recv_flags::none);

    if (reply.size() >= sizeof(uint64_t)) {
        std::memcpy(&resp_timestamp, reply.data(), sizeof(uint64_t));
        std::cout << "Received timestamp: " << resp_timestamp << std::endl;
    } else {
        std::cerr << "Received message is too short to contain a uint64_t." << std::endl;
    }

    uint64_t time_diff = std::abs(static_cast<int64_t>(resp_timestamp - timestamp));

    if (time_diff < 10000) {
        std::cout << "\033[32m";
        std::cout << "时间同步成功，差值：" << time_diff << " 微秒" << std::endl;
        std::cout << "\033[0m";
    } else {
        std::cerr << "\033[31m";
        std::cerr << "时间同步失败，差值：" << time_diff << " 微秒，大于 10000 微秒，请重新执行时间同步." << std::endl;
        std::cerr << "\033[0m";
    }

}
