/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 15:25
 */

#include <iostream>
#include <regex>
#include <memory>
#include <thread>

#include "network.hpp"
#include "audio.hpp"
#include "renderer.hpp"

void UpdateChat(const std::shared_ptr<tsp::Network>& Network)
{
    std::string Reply = Network->Receive();

    if (Reply.length() > 0) {
        std::regex Research("!(.+)@.+ PRIVMSG #([^\\s]+) :(.*)");
        std::smatch Match;
        std::regex_search(Reply, Match, Research);

        std::cout << Match[1] << ": " << Match[3] << std::endl;
    }
}

int main()
{
    std::unique_ptr<tsp::AudioContext> Audio = std::make_unique<tsp::AudioContext>();
    std::unique_ptr<tsp::Renderer> Renderer = std::make_unique<tsp::Renderer>();
    std::shared_ptr<tsp::Network> Network = std::make_shared<tsp::Network>();

    Network->SetToken("oauth:ewtwfhl2uwrebp2imqdpremhc86lww");
    Network->SetChannel("amelie_dev");
    Network->Connect();

    std::thread NetworkThread(UpdateChat, Network);

    while (Renderer->IsOpen()) {
        Renderer->Update();
        Audio->Update();
    }

    NetworkThread.join();
    return (0);
}
