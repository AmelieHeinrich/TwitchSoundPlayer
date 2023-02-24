/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 16:04
 */

#include "network.hpp"

#include <sstream>
#include <vector>

namespace tsp
{
    Network::Network(std::string Token, std::string Channel)
    {
        mToken = Token;
        mChannel = Channel;

        WSADATA WsaData;
        if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0) {
            MessageBoxA(nullptr, "Failed to initialise Windows socket system!", "Error", MB_ICONERROR);
        }

        addrinfo Hints = {};
        Hints.ai_family = AF_UNSPEC;
        Hints.ai_socktype = SOCK_STREAM;
        Hints.ai_protocol = IPPROTO_TCP;

        addrinfo *Result;
        if (getaddrinfo("irc.chat.twitch.tv", "6667", &Hints, &Result) != 0) {
            MessageBoxA(nullptr, "getaddrinfo failed!", "Error", MB_ICONERROR);
            WSACleanup();
        }

        mConnectSocket = socket(Result->ai_family, Result->ai_socktype, Result->ai_protocol);
        if (mConnectSocket == INVALID_SOCKET) {
            std::stringstream Stream;
            Stream << "Error at socket(): " << WSAGetLastError();
            MessageBoxA(nullptr, Stream.str().c_str(), "Error", MB_ICONERROR);
            freeaddrinfo(Result);
            WSACleanup();
        }

        if (connect(mConnectSocket, Result->ai_addr, Result->ai_addrlen) == SOCKET_ERROR) {
            closesocket(mConnectSocket);
            mConnectSocket = INVALID_SOCKET;
        }

        freeaddrinfo(Result);

        if (mConnectSocket == INVALID_SOCKET) {
            MessageBoxA(nullptr, "Cannot connect to twitch server!", "Error", MB_ICONERROR);
            WSACleanup();
        }
    }

    Network::~Network()
    {
        closesocket(mConnectSocket);
        WSACleanup();
    }

    void Network::Connect()
    {
        std::string SendToken = "PASS " + mToken + "\r\n";
        std::string SendNickname = "NICK " + mChannel + "\r\n";
        std::string SendChannel = "JOIN #" + mChannel + "\r\n";

        send(mConnectSocket, SendToken.c_str(), SendToken.length(), 0);
        send(mConnectSocket, SendNickname.c_str(), SendNickname.length(), 0);
        send(mConnectSocket, SendChannel.c_str(), SendChannel.length(), 0);
    }

    std::string Network::Receive()
    {
        std::vector<char> Buffer = std::vector<char>(1024);
        int BytesReceived = recv(mConnectSocket, Buffer.data(), 1024, NULL);
        return std::string(Buffer.begin(), Buffer.begin() + BytesReceived);
    }
}
