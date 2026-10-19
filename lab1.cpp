// ENSC 251 - Lab Assignment 1
// Simple playing card management system: Card and Deck classes.

#include <iostream>
using namespace std;

// ============================================================
// Card class
// ============================================================
// Suit mapping: 0 = Clubs, 1 = Diamonds, 2 = Hearts, 3 = Spades
// Rank mapping: 1 = Ace, 2-10 = number cards, 11 = Jack, 12 = Queen, 13 = King
class Card
{
public:
    // 1.1 Constructors
    Card();                       // default: Ace of Clubs
    Card(int newRank, int newSuit);

    // 1.1 Getters
    int getRank() const;
    int getSuit() const;

    // 1.1 Setters
    void setRank(int newRank);
    void setSuit(int newSuit);
    void setCard(int newRank, int newSuit);

    // 1.2 True if Jack, Queen, or King
    bool isFaceCard() const;

    // 1.3 True if rank is 1-13 and suit is 0-3; prints an error otherwise
    bool isValid() const;

    // 1.4 Print rank, suit, and face-card status
    void output(ostream& outs) const;

    // 1.5 Print readable names
    void printSuitName(ostream& outs) const;
    void printRankName(ostream& outs) const;

    // 3. Friend function: same rank and same suit
    friend bool equal(const Card& c1, const Card& c2);

private:
    int rank;
    int suit;
};

// ---------- Card implementation ----------

Card::Card() : rank(1), suit(0)
{
}

Card::Card(int newRank, int newSuit) : rank(newRank), suit(newSuit)
{
}

int Card::getRank() const
{
    return rank;
}

int Card::getSuit() const
{
    return suit;
}

void Card::setRank(int newRank)
{
    rank = newRank;
}

void Card::setSuit(int newSuit)
{
    suit = newSuit;
}

void Card::setCard(int newRank, int newSuit)
{
    rank = newRank;
    suit = newSuit;
}

bool Card::isFaceCard() const
{
    return rank >= 11 && rank <= 13;
}

bool Card::isValid() const
{
    bool valid = true;
    if (rank < 1 || rank > 13)
    {
        cout << "Error: invalid rank " << rank
             << " (must be between 1 and 13)." << endl;
        valid = false;
    }
    if (suit < 0 || suit > 3)
    {
        cout << "Error: invalid suit " << suit
             << " (must be between 0 and 3)." << endl;
        valid = false;
    }
    return valid;
}

void Card::printSuitName(ostream& outs) const
{
    switch (suit)
    {
    case 0: outs << "Clubs";    break;
    case 1: outs << "Diamonds"; break;
    case 2: outs << "Hearts";   break;
    case 3: outs << "Spades";   break;
    default: outs << "Unknown Suit"; break;
    }
}

void Card::printRankName(ostream& outs) const
{
    switch (rank)
    {
    case 1:  outs << "Ace";   break;
    case 11: outs << "Jack";  break;
    case 12: outs << "Queen"; break;
    case 13: outs << "King";  break;
    default:
        if (rank >= 2 && rank <= 10)
            outs << rank;
        else
            outs << "Unknown Rank";
        break;
    }
}

void Card::output(ostream& outs) const
{
    printRankName(outs);
    outs << " of ";
    printSuitName(outs);
    outs << " (rank " << rank << ", suit " << suit << ")";
    if (isFaceCard())
        outs << " - Face card";
    else
        outs << " - Not a face card";
}

bool equal(const Card& c1, const Card& c2)
{
    return c1.rank == c2.rank && c1.suit == c2.suit;
}

// ============================================================
// Deck class
// ============================================================
// cards[] has a fixed size of 52. Only cards[0 .. numCards-1] are in the
// active deck. The top card is cards[numCards - 1].
const int DECK_SIZE = 52;

class Deck
{
public:
    // 2.1 Constructors
    Deck();                 // empty deck
    Deck(bool full);        // full = true -> all 52 cards, false -> empty

    // 2.1 Getters
    int getNumCards() const;
    Card getCard(int index) const;

    // 2.1 Setters
    void setNumCards(int n);
    bool setCard(int index, Card c);

    // Fill the deck with all 52 cards in order
    void fillDeck();

    // 2.2 Remove and return the top card
    Card drawTopCard();

    // 2.3 Add a card to the top of the deck if there is space
    bool addCard(Card c);

    // 2.4 Print the active portion of the deck
    void printDeck(ostream& outs) const;

    // 2.5 True if the deck has no cards
    bool isEmpty() const;

    // Helper: true if the deck already contains a card equal to c
    bool contains(const Card& c) const;

private:
    Card cards[DECK_SIZE];
    int numCards;
};

// ---------- Deck implementation ----------

Deck::Deck() : numCards(0)
{
}

Deck::Deck(bool full) : numCards(0)
{
    if (full)
        fillDeck();
}

int Deck::getNumCards() const
{
    return numCards;
}

Card Deck::getCard(int index) const
{
    if (index < 0 || index >= numCards)
    {
        cout << "Error: index " << index << " is outside the active deck." << endl;
        return Card(0, -1);   // invalid card signals an error
    }
    return cards[index];
}

void Deck::setNumCards(int n)
{
    if (n < 0 || n > DECK_SIZE)
    {
        cout << "Error: number of cards must be between 0 and "
             << DECK_SIZE << "." << endl;
        return;
    }
    numCards = n;
}

bool Deck::setCard(int index, Card c)
{
    if (index < 0 || index >= numCards)
    {
        cout << "Error: index " << index << " is outside the active deck." << endl;
        return false;
    }
    if (!c.isValid())
        return false;
    // Avoid duplicates (ignore the card currently at this index)
    for (int i = 0; i < numCards; i++)
    {
        if (i != index && equal(cards[i], c))
        {
            cout << "Error: duplicate card not allowed." << endl;
            return false;
        }
    }
    cards[index] = c;
    return true;
}

void Deck::fillDeck()
{
    numCards = 0;
    for (int s = 0; s <= 3; s++)
        for (int r = 1; r <= 13; r++)
            cards[numCards++] = Card(r, s);
}

Card Deck::drawTopCard()
{
    if (isEmpty())
    {
        cout << "Error: cannot draw a card, the deck is empty." << endl;
        return Card(0, -1);   // invalid card signals an error
    }
    numCards--;
    return cards[numCards];
}

bool Deck::addCard(Card c)
{
    if (numCards >= DECK_SIZE)
    {
        cout << "Error: deck is full, cannot add card." << endl;
        return false;
    }
    if (!c.isValid())
        return false;
    if (contains(c))
    {
        cout << "Error: duplicate card not allowed (";
        c.output(cout);
        cout << ")." << endl;
        return false;
    }
    cards[numCards] = c;
    numCards++;
    return true;
}

void Deck::printDeck(ostream& outs) const
{
    outs << "Deck contains " << numCards << " card(s):" << endl;
    if (isEmpty())
    {
        outs << "  (empty)" << endl;
        return;
    }
    for (int i = numCards - 1; i >= 0; i--)
    {
        outs << "  " << (numCards - i) << ". ";
        cards[i].output(outs);
        if (i == numCards - 1)
            outs << "   <-- top";
        outs << endl;
    }
}

bool Deck::isEmpty() const
{
    return numCards == 0;
}

bool Deck::contains(const Card& c) const
{
    for (int i = 0; i < numCards; i++)
        if (equal(cards[i], c))
            return true;
    return false;
}

// ============================================================
// Test helpers
// ============================================================
int testsRun = 0;
int testsPassed = 0;

void check(bool condition, const char* description)
{
    testsRun++;
    if (condition)
    {
        testsPassed++;
        cout << "[PASS] " << description << endl;
    }
    else
    {
        cout << "[FAIL] " << description << endl;
    }
}

void section(const char* title)
{
    cout << endl << "========== " << title << " ==========" << endl;
}

// ============================================================
// 4. Main: test every feature
// ============================================================
int main()
{
    // ---------- Card: constructors, getters, setters ----------
    section("Card constructors, getters, setters");
    Card defaultCard;
    check(defaultCard.getRank() == 1 && defaultCard.getSuit() == 0,
          "Default constructor gives Ace of Clubs");

    Card queenHearts(12, 2);
    check(queenHearts.getRank() == 12 && queenHearts.getSuit() == 2,
          "Card(12, 2) is Queen of Hearts");

    Card temp;
    temp.setRank(7);
    temp.setSuit(3);
    check(temp.getRank() == 7 && temp.getSuit() == 3,
          "setRank(7) and setSuit(3) work");
    temp.setCard(10, 1);
    check(temp.getRank() == 10 && temp.getSuit() == 1,
          "setCard(10, 1) works");

    // ---------- Card: isFaceCard ----------
    section("Card::isFaceCard");
    check(Card(11, 0).isFaceCard(), "Jack is a face card");
    check(Card(12, 1).isFaceCard(), "Queen is a face card");
    check(Card(13, 2).isFaceCard(), "King is a face card");
    check(!Card(1, 3).isFaceCard(), "Ace is not a face card");
    check(!Card(10, 3).isFaceCard(), "10 is not a face card");

    // ---------- Card: isValid ----------
    section("Card::isValid");
    check(Card(1, 0).isValid(), "Ace of Clubs is valid (lower bounds)");
    check(Card(13, 3).isValid(), "King of Spades is valid (upper bounds)");
    cout << "Expect error messages for the next 4 checks:" << endl;
    check(!Card(0, 2).isValid(), "Rank 0 is invalid");
    check(!Card(14, 2).isValid(), "Rank 14 is invalid");
    check(!Card(5, -1).isValid(), "Suit -1 is invalid");
    check(!Card(5, 4).isValid(), "Suit 4 is invalid");

    // ---------- Card: output and name helpers ----------
    section("Card::output, printRankName, printSuitName");
    Card samples[] = { Card(1, 0), Card(7, 1), Card(10, 2),
                       Card(11, 3), Card(12, 2), Card(13, 0) };
    for (int i = 0; i < 6; i++)
    {
        cout << "  ";
        samples[i].output(cout);
        cout << endl;
    }
    cout << "  printRankName(King) -> ";
    Card(13, 3).printRankName(cout);
    cout << ", printSuitName(Diamonds) -> ";
    Card(13, 1).printSuitName(cout);
    cout << endl;

    // ---------- Friend function equal ----------
    section("Friend function equal()");
    check(equal(Card(5, 2), Card(5, 2)), "5 of Hearts equals 5 of Hearts");
    check(!equal(Card(5, 2), Card(6, 2)), "Different rank -> not equal");
    check(!equal(Card(5, 2), Card(5, 3)), "Different suit -> not equal");

    // ---------- Deck: constructors ----------
    section("Deck constructors and isEmpty");
    Deck emptyDeck;
    check(emptyDeck.isEmpty() && emptyDeck.getNumCards() == 0,
          "Default Deck() is empty");
    Deck emptyDeck2(false);
    check(emptyDeck2.isEmpty(), "Deck(false) is empty");
    Deck fullDeck(true);
    check(fullDeck.getNumCards() == 52 && !fullDeck.isEmpty(),
          "Deck(true) has 52 cards");

    // Full deck has no duplicates and every card is valid
    bool allValid = true, noDuplicates = true;
    for (int i = 0; i < fullDeck.getNumCards(); i++)
    {
        Card ci = fullDeck.getCard(i);
        if (ci.getRank() < 1 || ci.getRank() > 13 ||
            ci.getSuit() < 0 || ci.getSuit() > 3)
            allValid = false;
        for (int j = i + 1; j < fullDeck.getNumCards(); j++)
            if (equal(ci, fullDeck.getCard(j)))
                noDuplicates = false;
    }
    check(allValid, "All 52 cards in full deck are valid");
    check(noDuplicates, "Full deck has no duplicate cards");

    // ---------- Deck: printDeck ----------
    section("Deck::printDeck (full deck)");
    fullDeck.printDeck(cout);

    // ---------- Deck: drawTopCard ----------
    section("Deck::drawTopCard");
    Card top = fullDeck.drawTopCard();
    cout << "Drew: ";
    top.output(cout);
    cout << endl;
    check(equal(top, Card(13, 3)), "Top card of fresh full deck is King of Spades");
    check(fullDeck.getNumCards() == 51, "Deck size dropped to 51");
    check(!fullDeck.contains(top), "Drawn card is no longer in the deck");

    Card second = fullDeck.drawTopCard();
    check(equal(second, Card(12, 3)) && fullDeck.getNumCards() == 50,
          "Next draw is Queen of Spades, size 50");

    cout << "Expect an error message for drawing from an empty deck:" << endl;
    Card fromEmpty = emptyDeck.drawTopCard();
    check(emptyDeck.getNumCards() == 0, "Drawing from empty deck leaves size at 0");
    check(fromEmpty.getRank() == 0, "Drawing from empty deck returns an invalid card");

    // ---------- Deck: addCard ----------
    section("Deck::addCard");
    check(fullDeck.addCard(top), "Put King of Spades back into the deck");
    check(fullDeck.getNumCards() == 51, "Deck size back to 51");

    cout << "Expect a duplicate error:" << endl;
    check(!fullDeck.addCard(Card(1, 0)), "Cannot add Ace of Clubs again (duplicate)");

    cout << "Expect an invalid-card error:" << endl;
    check(!emptyDeck.addCard(Card(20, 1)), "Cannot add an invalid card");

    check(fullDeck.addCard(second) && fullDeck.getNumCards() == 52,
          "Add Queen of Spades back -> 52 cards");
    cout << "Expect a deck-full error:" << endl;
    Deck fullDeck2(true);
    check(!fullDeck2.addCard(Card(2, 2)), "Cannot add to a full deck");

    // Build a small hand in the empty deck
    emptyDeck.addCard(Card(1, 2));
    emptyDeck.addCard(Card(11, 0));
    emptyDeck.addCard(Card(7, 1));
    check(emptyDeck.getNumCards() == 3 && !emptyDeck.isEmpty(),
          "Added 3 cards to the empty deck");
    emptyDeck.printDeck(cout);

    // ---------- Deck: getters and setters ----------
    section("Deck getters and setters");
    check(equal(emptyDeck.getCard(1), Card(11, 0)), "getCard(1) is Jack of Clubs");
    check(emptyDeck.setCard(1, Card(9, 3)), "setCard(1, 9 of Spades) succeeds");
    check(equal(emptyDeck.getCard(1), Card(9, 3)), "getCard(1) is now 9 of Spades");
    cout << "Expect a duplicate error:" << endl;
    check(!emptyDeck.setCard(0, Card(7, 1)), "setCard rejects a duplicate");
    cout << "Expect an index error:" << endl;
    check(!emptyDeck.setCard(5, Card(2, 2)), "setCard rejects an out-of-range index");

    emptyDeck.setNumCards(2);
    check(emptyDeck.getNumCards() == 2, "setNumCards(2) shrinks the active deck");
    cout << "Expect a range error:" << endl;
    emptyDeck.setNumCards(60);
    check(emptyDeck.getNumCards() == 2, "setNumCards(60) is rejected");

    // ---------- Draw an entire deck ----------
    section("Draw every card from a full deck");
    Deck drawAll(true);
    int drawn = 0;
    while (!drawAll.isEmpty())
    {
        drawAll.drawTopCard();
        drawn++;
    }
    check(drawn == 52 && drawAll.isEmpty(), "Drew exactly 52 cards, deck now empty");
    drawAll.printDeck(cout);

    // ---------- Summary ----------
    section("Summary");
    cout << testsPassed << " / " << testsRun << " tests passed." << endl;

    return 0;
}
