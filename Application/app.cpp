#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

enum Color { RED, YELLOW, GREEN, BLUE, NONE };
enum Value { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, DRAW_TWO, REVERSE, SKIP, WILD, WILD_DRAW_FOUR };

struct Card {
    Color color;
    Value value;

    string toString() const {
        string colors[] = { "Red", "Yellow", "Green", "Blue", "None" };
        string values[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Draw Two", "Reverse", "Skip", "Wild", "Wild Draw Four" };
        return colors[color] + " " + values[value];
    }
};

class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        for (int color = RED; color <= BLUE; ++color) {
            for (int value = ZERO; value <= SKIP; ++value) {
                cards.push_back({ static_cast<Color>(color), static_cast<Value>(value) });
                if (value != ZERO) {
                    cards.push_back({ static_cast<Color>(color), static_cast<Value>(value) });
                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            cards.push_back({ NONE, WILD });
            cards.push_back({ NONE, WILD_DRAW_FOUR });
        }
        shuffle();
    }

    void shuffle() {
        srand(time(0));
        random_shuffle(cards.begin(), cards.end());
    }

    Card draw() {
        if (cards.empty()) {
            throw runtime_error("Deck is empty!");
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    bool isEmpty() const {
        return cards.empty();
    }

    void addToBottom(const Card &card) {
        cards.insert(cards.begin(), card);
    }
};

class Player {
public:
    vector<Card> hand;
    string name;

    Player(const string &name) : name(name) {}

    void drawCard(Deck &deck) {
        try {
            Card drawnCard = deck.draw();
            hand.push_back(drawnCard);
        } catch (const runtime_error &e) {
            cerr << "Exception: " << e.what() << endl;
        }
    }

    void showHand() const {
        cout << name << "'s hand:" << endl;
        for (int i = 0; i < hand.size(); ++i) {
            cout << i + 1 << ": " << hand[i].toString() << endl;
        }
    }
};

class Game {
private:
    Deck deck;
    vector<Player> players;
    int currentPlayer;
    bool reverseDirection;

public:
    Game(const vector<string> &playerNames) : currentPlayer(0), reverseDirection(false) {
        for (const auto &name : playerNames) {
            players.push_back(Player(name));
        }
        startGame();
    }

    void startGame() {
        for (auto &player : players) {
            for (int i = 0; i < 7; ++i) {
                player.drawCard(deck);
            }
        }

        Card startingCard = deck.draw();
        while (startingCard.value == WILD || startingCard.value == WILD_DRAW_FOUR || startingCard.value == REVERSE || startingCard.value == SKIP) {
            deck.addToBottom(startingCard);
            startingCard = deck.draw();
        }

        cout << "Starting card: " << startingCard.toString() << endl;
        playRound(startingCard);
    }

    void playRound(Card topCard) {
        while (true) {
            Player &player = players[currentPlayer];
            player.showHand();

            cout << "Current top card: " << topCard.toString() << endl;
            cout << player.name << ", choose a card to play (0 to draw): ";
            int choice;
            cin >> choice;

            if (choice == 0) {
                player.drawCard(deck);
                Card drawnCard = player.hand.back();
                cout << player.name << " drew a card: " << drawnCard.toString() << endl;
                continue;
            }

            try {
                Card playedCard = player.hand.at(choice - 1);
                if (isValidMove(playedCard, topCard)) {
                    player.hand.erase(player.hand.begin() + choice - 1);
                    topCard = playedCard;
                    applyCardEffect(playedCard, topCard);
                    if (player.hand.empty()) {
                        cout << player.name << " wins!" << endl;
                        break;
                    }
                } else {
                    cout << "Invalid move. Try again." << endl;
                    continue; // Stay on the same player's turn
                }
            } catch (const out_of_range &e) {
                cerr << "Exception: Invalid card choice. Try again." << endl;
                continue; // Stay on the same player's turn
            }

            nextPlayer();
        }
    }

    bool isValidMove(const Card &playedCard, const Card &topCard) {
        return playedCard.color == topCard.color || playedCard.value == topCard.value ||
               playedCard.color == NONE ||
               (playedCard.value == REVERSE && playedCard.color == topCard.color) ||
               (playedCard.value == SKIP && playedCard.color == topCard.color) ||
               (playedCard.value == SKIP && playedCard.value == ZERO); // Allow playing Skip (00) card
    }

    void applyCardEffect(const Card &playedCard, Card &topCard) {
        switch (playedCard.value) {
            case DRAW_TWO:
                nextPlayer();
                players[currentPlayer].drawCard(deck);
                players[currentPlayer].drawCard(deck);
                break;
            case REVERSE:
                reverseDirection = !reverseDirection;
                break;
            case SKIP:
                nextPlayer();
                nextPlayer(); // Skip the next player's turn
                break;
            case WILD:
                changeColor(topCard);
                break;
            case WILD_DRAW_FOUR:
                nextPlayer();
                players[currentPlayer].drawCard(deck);
                players[currentPlayer].drawCard(deck);
                players[currentPlayer].drawCard(deck);
                players[currentPlayer].drawCard(deck);
                changeColor(topCard);
                break;
            default:
                break;
        }
    }

    void changeColor(Card &topCard) {
        cout << "Choose a color (0: Red, 1: Yellow, 2: Green, 3: Blue): ";
        int colorChoice;
        cin >> colorChoice;
        topCard.color = static_cast<Color>(colorChoice);
    }

    void nextPlayer() {
        if (reverseDirection) {
            currentPlayer = (currentPlayer - 1 + players.size()) % players.size();
        } else {
            currentPlayer = (currentPlayer + 1) % players.size();
        }
    }
};

int main() {
    int numPlayers;
    cout << "Enter the number of players: ";
    cin >> numPlayers;

    vector<string> playerNames;
    for (int i = 0; i < numPlayers; ++i) {
        string name;
        cout << "Enter the name of player " << i + 1 << ": ";
        cin >> name;
        playerNames.push_back(name);
    }

    Game game(playerNames);
    return 0;
}
