#include "macros.h"

typedef struct score {
    int numCards;
    int sevenCoins;
    int numCoins;
    int primes[NUMSUITS]; /*see macros for suit order*/
    int primeTotal;
    int scopa;
}Score;

void initializeScore(Score *s);
void printScore(Score *s);
int findPrimeVal(int i);
int findPrime(Score s);
void scoreCard(Score *score, Card c);
void scoreDeck(Score *s, Node *head);
void addScore(int p1Field, int p2Field, int *p1, int *p2);