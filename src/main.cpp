/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 15:25
 */

#include <iostream>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <sstream>

int main()
{
    WSADATA WsaData;
    if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0) {
        MessageBoxA(nullptr, "Failed to initialise Windows socket system!", "Error", MB_ICONERROR);
        return (-1);
    }

    addrinfo Hints = {};
    Hints.ai_family = AF_UNSPEC;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_protocol = IPPROTO_TCP;

    addrinfo *Result;
    if (getaddrinfo("irc.chat.twitch.tv", "6667", &Hints, &Result) != 0) {
        MessageBoxA(nullptr, "getaddrinfo failed!", "Error", MB_ICONERROR);
        WSACleanup();
        return (-1);
    }

    SOCKET ConnectSocket = INVALID_SOCKET;
    ConnectSocket = socket(Result->ai_family, Result->ai_socktype, Result->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        std::stringstream Stream;
        Stream << "Error at socket(): " << WSAGetLastError();
        MessageBoxA(nullptr, Stream.str().c_str(), "Error", MB_ICONERROR);
        freeaddrinfo(Result);
        WSACleanup();
        return (-1);
    }

    if (connect(ConnectSocket, Result->ai_addr, Result->ai_addrlen) == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(Result);

    if (ConnectSocket == INVALID_SOCKET) {
        MessageBoxA(nullptr, "Cannot connect to twitch server!", "Error", MB_ICONERROR);
        WSACleanup();
        return (-1);
    }

    std::string Token = "PASS oauth:ewtwfhl2uwrebp2imqdpremhc86lww\r\n";
    std::string Nickname = "NICK amelie_dev\r\n";
    std::string Channel = "JOIN #amelie_dev\r\n";

    send(ConnectSocket, Token.c_str(), Token.length(), 0);
    send(ConnectSocket, Nickname.c_str(), Nickname.length(), 0);
    send(ConnectSocket, Channel.c_str(), Channel.length(), 0);

    while (true) {
        constexpr int BufferLength = 100000;
        char Buffer[BufferLength] = {};

        int BytesReceived = recv(ConnectSocket, Buffer, BufferLength, NULL);
        Buffer[BytesReceived] = '\0';

        std::cout << Buffer << std::endl;
    }

    WSACleanup();
    return (0);
}
