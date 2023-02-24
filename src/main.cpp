/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 15:25
 */

#include <iostream>
#include <regex>

#include "network.hpp"
#include "audio.hpp"

int main()
{
    tsp::AudioContext* Context = new tsp::AudioContext();

    tsp::Network* Network = new tsp::Network();
    Network->SetToken("oauth:ewtwfhl2uwrebp2imqdpremhc86lww");
    Network->SetChannel("amelie_dev");

    Network->Connect();
    (void)Network->Receive();
    (void)Network->Receive();

    while (true) {
        std::string Reply = Network->Receive();

        if (Reply.length() > 0) {
            std::regex Research("!(.+)@.+ PRIVMSG #([^\\s]+) :(.*)");
            std::smatch Match;
            std::regex_search(Reply, Match, Research);

            std::cout << Match[1] << ": " << Match[3] << std::endl;
        }

        Context->Update();
    }
    
    delete Network;
    delete Context;
    return (0);
}
