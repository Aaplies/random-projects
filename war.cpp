#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>
#include <string>

void checkForWin(const std::vector<int>& player1, const std::vector<int>& player2)
{
    if (player1.size() == 0 || player2.size() == 0)
    {
        (player1.size() == 0) ? std::cout << "Player 2 wins!\n" : std::cout << "Player 1 wins!\n";
        exit(0);
    }
}

void war(std::vector<int>& player1, std::vector<int>& player2, std::pair<int, int> active_pair, std::vector<int>::iterator& delete_at)
{
    const int war_wager = 10;
    std::cout << "WAR!!" << std::endl;
    std::vector<int> p1wager;
    std::vector<int> p2wager;
    std::cout << "  p1 p2" << std::endl;
    for (int i = 0; i < war_wager; i++)
    {
        p1wager.push_back(player1[i]);
        std::cout << 1 + i << ". " << player1[i];
        p2wager.push_back(player2[i]);
        std::cout << " " << player2[i] << std::endl;
        delete_at = player2.begin();
        player2.erase(delete_at);
        delete_at = player1.begin();
        player1.erase(delete_at);
        checkForWin(player1, player2);
        sleep(1);
    }
    sleep(1);
    if (p1wager[war_wager - 1] > p2wager[war_wager - 1])
    {
        for (int i = 0; i < p1wager.size(); i++)
        {
            player1.push_back(p1wager[i]);
            player1.push_back(p2wager[i]);
        }
        player1.push_back(active_pair.first);
        player1.push_back(active_pair.second);
        std::cout << "P1 WON THE WAR!!\n" << std::endl;
    }
    else if (p1wager[war_wager - 1] < p2wager[war_wager - 1])
    {
        for (int i = 0; i < p2wager.size(); i++)
        {
            player2.push_back(p1wager[i]);
            player2.push_back(p2wager[i]);
        }
        player2.push_back(active_pair.first);
        player2.push_back(active_pair.second);
        std::cout << "P2 WON THE WAR!!\n" << std::endl;
    }
    else
    {
        for (int i = 0; i < p1wager.size(); i++)
        {
            player2.push_back(p1wager[i]);
            player1.push_back(p2wager[i]);
        }
        player2.push_back(active_pair.first);
        player1.push_back(active_pair.second);
        
        std::cout << "THE WAR WAS A TIE\n" << std::endl;
    }
}

int main()
{
    std::random_device random;
    std::uniform_int_distribution<> distrib(0, 51);
    std::pair<int, int> active_pair;
    std::vector<int> available_cards, player1, player2;
    std::vector<int>::iterator delete_at;
    for (int i = 2; i < 15; i++)
    {
        for (int x = 0; x < 4; x++)
        {
            available_cards.push_back(i);
        }
    }
    int chosencard;
    std::string go;
    for (int i = 0; i < 24; i++)
    {
        chosencard = distrib(random);
        chosencard = chosencard % available_cards.size();
        player1.push_back(available_cards[chosencard]);
        delete_at = available_cards.begin() + chosencard;
        available_cards.erase(delete_at);
        chosencard = distrib(random);
        chosencard = chosencard % available_cards.size();
        player2.push_back(available_cards[chosencard]);
        delete_at = available_cards.begin() + chosencard;
        available_cards.erase(delete_at);
    }
    std::cout << "Press enter to put down your card.\n";
    while (true)
    {
        active_pair.first = 0;
        active_pair.second = 0;
        checkForWin(player1, player2);
        std::cout << "P1(" << player1.size() << "):";
        getline(std::cin, go);
        std::cout << "P1 put down a";
        (player1[0] == 8 || player1[0] == 11) ? std::cout << "n " << player1[0] << "\n" << std::endl : std::cout << " " << player1[0] << "\n" << std::endl;
        active_pair.first = player1[0];
        delete_at = player1.begin();
        player1.erase(delete_at);
        std::cout << "P2(" << player2.size() << "):";
        getline(std::cin, go);
        std::cout << "P2 put down a";
        (player2[0] == 8 || player2[0] == 11) ? std::cout << "n " << player2[0] << "\n" << std::endl : std::cout << " " <<  player2[0] << "\n" << std::endl;
        active_pair.second = player2[0];
        delete_at = player2.begin();
        player2.erase(delete_at);
        sleep(1);
        checkForWin(player1, player2);
        if (active_pair.first > active_pair.second)
        {
            player1.push_back(active_pair.first);
            player1.push_back(active_pair.second);
            std::cout << "P1 won the battle!\n" << std::endl;
            active_pair.first = 0;
            active_pair.second = 0;
        }
        else if (active_pair.first < active_pair.second)
        {
            player2.push_back(active_pair.first);
            player2.push_back(active_pair.second);
            std::cout << "P2 won the battle!\n" << std::endl;
            active_pair.first = 0;
            active_pair.second = 0;
        }
        else if (active_pair.first == active_pair.second)
        {
            war(player1, player2, active_pair, delete_at);
        }
        else
        {
            std::cout << "there was an error :/" << std::endl;
            std::cout << "p1 " << active_pair.first << "\np2 " << active_pair.second << std::endl;
            return 1;
        }
        active_pair.first = 0;
        active_pair.second = 0;
    }
    return 0;
}
