/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 24/02/2023 15:25
 */

#include <iostream>
#include <regex>

#include "network.hpp"

int main()
{
    tsp::Network* Network = new tsp::Network("oauth:ewtwfhl2uwrebp2imqdpremhc86lww", "amelie_dev");

    (void)Network->Receive();
    (void)Network->Receive();

    while (true) {
        std::string Reply = Network->Receive();

        if (Reply.length() > 0) {
            std::regex Research("!(.+)@.+ PRIVMSG #([^\\s]+) :(.*)");
            std::smatch Match;
            std::regex_search(Reply, Match, Research);

            std::cout << Match[2] << ": " << Match[3] << std::endl;
        }
    }
    
    delete Network;
    return (0);
}
