/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 16:02
 */


#pragma once

#include <string>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

namespace tsp
{
    class Network
    {
    public:
        Network(std::string Token, std::string Channel);
        ~Network();

        std::string Receive();
    private:
        SOCKET mConnectSocket = INVALID_SOCKET;
        std::string mToken;
        std::string mChannel;
    };
};
