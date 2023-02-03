#include <iostream>
#include <vector>
#include <random>

int checkForPairs(const std::vector<int>& hand, const std::vector<int>& hand2) // pass two of the same to check one hand
{
    std::vector<int>::iterator pairs;
    for (int i = 0; i < hand.size(); i++)
    {
        for (int x = 0; x < hand2.size(); x++)
        {
            if (hand[i] == hand2[x] && i != x)
            {
                return i;
            }
        }
    }
    return -1;
}

void remove_zeroes(std::vector<int>& input)
{
    std::vector<int>::iterator delete_at;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == 0)
        {
            delete_at = input.begin() + i;
            input.erase(delete_at);
            remove_zeroes(input); // recursively calls the function so that the size is reevaluated
        }
    }
}

void erasePairs(std::vector<int>& hand, std::vector<int>& hand2, std::string handtype)
{
    std::vector<int> handcopy = hand;
    std::vector<int> hand2copy = hand2;
    for (int i = 0; i < handcopy.size(); i++)
    {
        for (int x = 0; x < hand2copy.size(); x++)
        {
            if (i == x)
            {
                continue;
            }
            else if (handcopy[i] == hand2copy[x] && (hand2copy[x] != 0 && handcopy[i] != 0))
            {
                std::cout << handtype << " had a pair of " << hand[i] << "s" << std::endl;
                hand2[x] = hand2[i] = hand[i] = hand[x] = 0;
                hand2copy[x] = hand2copy[i] = handcopy[x] = handcopy[i] = 0; // 0 means pair has been made w/o dsize
            }
        }
    }
    remove_zeroes(hand);
    remove_zeroes(hand2);
}

void print_hand(const std::vector<int>& input)
{
    std::cout << "Your cards are: " << input[0];
    for (int i = 1; i < input.size(); i++)
    {
        std::cout << ", " << input[i];
    }
    std::cout << std::endl;
}

int main()
{
    std::random_device random; // use distrib(random) to generate your number
    std::uniform_int_distribution<> distrib(0, 51);
    std::vector<int> human_hand, computer_hand, available_cards; // expected_hand is what it thinks player has
    for (int i = 1; i < 14; i++)
    {
        for (int x = 0; x < 4; x++)
        {
            available_cards.push_back(i); // loops at 13; four per number
        }
    }
    int chosencard; // to know which card to delete of available_cards
    std::vector<int>::iterator delete_at;
    for (int i = 0; i < 5; i++)
    {
        chosencard = distrib(random);
        chosencard = chosencard % int(available_cards.size()); // deals 1 card to human then one to computer
        delete_at = available_cards.begin() + chosencard;
        human_hand.push_back(available_cards[chosencard]);
        available_cards.erase(delete_at);
        chosencard = distrib(random);
        chosencard = chosencard % int(available_cards.size());
        delete_at = available_cards.begin() + chosencard;
        computer_hand.push_back(available_cards[chosencard]);
        available_cards.erase(delete_at);
    }
    print_hand(human_hand);
    erasePairs(human_hand, human_hand, "You");
    erasePairs(computer_hand, computer_hand, "Computer");
    (checkForPairs(human_hand, human_hand) == -1) ? std::cout << "" : std::cout << "failure :(\n"; //       checks each hand for pairs after the pair
    (checkForPairs(computer_hand, computer_hand) == -1) ? std::cout << "" : std::cout << "failure :(\n"; // removing process
    int askedcard;
    bool opponenthas = false;
    while (true)
    {
        if (human_hand.size() == 0)
        {
            std::cout << "You win!" << std::endl;
            return 0;
        }
        else if (computer_hand.size() == 0)
        {
            std::cout << "Computer wins!" << std::endl;
            return 0;
        }
        else if (available_cards.size() == 0)
        {
            std::cout << "Tie!" << std::endl;
            return 0;
        }
        erasePairs(human_hand, human_hand, "You");
        erasePairs(computer_hand, computer_hand, "Computer");
        print_hand(human_hand);
        std::cout << "What do you ask the computer for?: ";
        std::cin >> askedcard;
        opponenthas = false;
        for (int i = 0; i < computer_hand.size(); i++)
        {
            if (computer_hand[i] == askedcard)
            {
                human_hand.push_back(askedcard);
                delete_at = computer_hand.begin() + i;
                computer_hand.erase(delete_at);
                opponenthas = true;
            }
        }
        if (!opponenthas)
        {
            std::cout << "Go Fish." << std::endl;
            chosencard = distrib(random);
            chosencard = chosencard % available_cards.size();
            human_hand.push_back(available_cards[chosencard]);
            delete_at = available_cards.begin() + chosencard;
            available_cards.erase(delete_at);
        }
        askedcard = distrib(random);
        askedcard = askedcard % computer_hand.size();
        askedcard = (askedcard == 0) ? 1 : askedcard;
        std::cout << "Opponent asks for a " << askedcard << std::endl;
        for (int i = 0; i < human_hand.size(); i++)
        {
            if (human_hand[i] == askedcard)
            {
                computer_hand.push_back(askedcard);
                delete_at = human_hand.begin() + i;
                human_hand.erase(delete_at);
                opponenthas = true;
            }
        }
        if (!opponenthas)
        {
            std::cout << "Opponent went fishing" << std::endl;
            chosencard = distrib(random);
            chosencard = chosencard % available_cards.size();
            computer_hand.push_back(available_cards[chosencard]);
            delete_at = available_cards.begin() + chosencard;
            available_cards.erase(delete_at);
        }
    }
    return 0;
}
