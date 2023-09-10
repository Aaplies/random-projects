#include <iostream>
#include <vector>
#include <random>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int checkForPairs(const std::vector<int>& hand, const std::vector<int>& hand2, const bool isSameHand) // pass two of the same to check one hand
{
    std::vector<int>::iterator pairs;
    for (int i = 0; i < hand.size(); i++)
    {
        for (int x = 0; x < hand2.size(); x++)
        {
            if (hand[i] == hand2[x] && (i != x || !isSameHand))
            {
                return i;
            }
        }
    }
    return -1; // -1 means no pairs
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

void print_hand(const std::vector<int>& input, const std::string handtype)
{
    std::cout << std::endl;
    std::cout << handtype << " cards are: " << input[0]; // handtype is used when
    for (int i = 1; i < input.size(); i++) //               dev mode where you cn
    { //                                                    see cptrs cards
        std::cout << ", " << input[i];
    }
    std::cout << std::endl;
}

void checkForWin(const std::vector<int>& human_hand, const std::vector<int>& computer_hand, const std::vector<int>& available_cards)
{
    if (human_hand.size() <= 0)
        {
            std::cout << "You win!" << std::endl;
            exit(0);
        }
        else if (computer_hand.size() <= 0)
        {
            std::cout << "Computer wins!" << std::endl;
            exit(0);
        }
        else if (available_cards.size() <= 0)
        {
            std::cout << "Tie!" << std::endl; // don't keep track of discarded
            exit(0); //                          cards so out of cards is a tie
        }
}

int main()
{
    std::random_device random; // use distrib(random) to generate your number
    std::uniform_int_distribution<> distrib(0, 51);
    std::vector<int> human_hand, computer_hand, available_cards, expected_hand; // expected_hand is what it thinks player has
    expected_hand.push_back(-1);
    for (int i = 1; i < 14; i++)
    {
        for (int x = 0; x < 4; x++)
        {
            available_cards.push_back(i); // loops at 13; four per number
        }
    }
    int chosencard; // to know which card to delete of available_cards
    std::vector<int>::iterator delete_at;
    for (int i = 0; i < 5; i++) // change i < 5 to change cards per player
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
    print_hand(human_hand, "Your"); // use posessive with print_hand
    erasePairs(human_hand, human_hand, "You");
    erasePairs(computer_hand, computer_hand, "Computer");
    (checkForPairs(human_hand, human_hand, true) == -1) ? std::cout << "" : std::cout << "failure :(\n"; //       checks each hand for pairs after the pair
    (checkForPairs(computer_hand, computer_hand, true) == -1) ? std::cout << "" : std::cout << "failure :(\n"; // removing process
    int askedcard;
    bool opponenthas = false;
    int shouldaskfor;
    while (true)
    {
        remove_zeroes(expected_hand);
        checkForWin(human_hand, computer_hand, available_cards);
        print_hand(human_hand, "Your");
        // print_hand(expected_hand, "cptr xpctd's");
        // print_hand(computer_hand, "Computer's");
        std::cout << "\nWhat do you ask the computer for?: " << std::endl;
        shouldaskfor = checkForPairs(human_hand, computer_hand, false);
        // (shouldaskfor == -1) ? std::cout << "No cards in common.\n" : std::cout << "Ask for a " << human_hand[shouldaskfor] << std::endl;
        std::cin >> askedcard;
        // askedcard = human_hand[shouldaskfor];
        // std::cout << askedcard << std::endl;
        expected_hand.push_back(askedcard);
        sleep(1);
        opponenthas = false;
        for (int i = 0; i < computer_hand.size(); i++)
        {
            if (computer_hand[i] == askedcard)
            {
                human_hand.push_back(askedcard);
                delete_at = computer_hand.begin() + i;
                computer_hand.erase(delete_at);
                opponenthas = true;
                std::cout << "The computer had a pair.\n";
                sleep(1);
                erasePairs(human_hand, human_hand, "You"); // prevents delayed pairing
                for (int i = 0; i < expected_hand.size(); i++)
                {
                    if (expected_hand[i] == askedcard) // happens if expected_hand is wrong
                    {
                        delete_at = expected_hand.begin() + i; // corrects the deviation
                        expected_hand.erase(delete_at);
                    }
                }
            }
        }
        if (!opponenthas)
        {
            std::cout << "Go Fish." << std::endl;
            chosencard = distrib(random);
            chosencard = chosencard % available_cards.size();
            human_hand.push_back(available_cards[chosencard]);
            sleep(1);
            std::cout << "You caught a " << available_cards[chosencard] << ".\n";
            sleep(1);
            erasePairs(human_hand, human_hand, "You");
            delete_at = available_cards.begin() + chosencard;
            available_cards.erase(delete_at);
        }
        checkForWin(human_hand, computer_hand, available_cards);
        askedcard = (checkForPairs(computer_hand, expected_hand, false) == -1) ? (distrib(random) % computer_hand.size()) : checkForPairs(computer_hand, expected_hand, false);
        // if (no pairs) {set it to rand} else {set it to pairs}
        // prevents computer asking for 0s issue
        // askedcard debug std::cout << "askedcard is " << askedcard << "\n";
        // (checkForPairs(computer_hand, expected_hand, false) == -1) ? std::cout << "Guess\n" : std::cout << "smort\n";
        askedcard = computer_hand[askedcard];
        sleep(1);
        std::cout << "Computer asks for a " << askedcard << std::endl;
        sleep(1);
        opponenthas = false;
        for (int i = 0; i < human_hand.size(); i++)
        {
            if (human_hand[i] == askedcard)
            {
                std::cout << "The computer took your " << askedcard << std::endl;
                computer_hand.push_back(askedcard);
                delete_at = human_hand.begin() + i;
                human_hand.erase(delete_at);
                opponenthas = true;
                sleep(1);
                erasePairs(computer_hand, computer_hand, "Computer");
            }
        }
        if (!opponenthas)
        {
            for (int i = 0; i < expected_hand.size(); i++)
            {
                if (expected_hand[i] == askedcard) // happens if expected_hand is wrong
                {
                    delete_at = expected_hand.begin() + i; // corrects the deviation
                    expected_hand.erase(delete_at);
                }
            }
            std::cout << "Computer went fishing" << std::endl;
            chosencard = distrib(random);
            chosencard = chosencard % available_cards.size();
            computer_hand.push_back(available_cards[chosencard]);
            delete_at = available_cards.begin() + chosencard;
            available_cards.erase(delete_at);
        }
        sleep(1);
        erasePairs(human_hand, human_hand, "You");
        sleep(1);
        erasePairs(computer_hand, computer_hand, "Computer");
        sleep(1);
    }
    return 0;
}
