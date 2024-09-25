#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>

#include "list.h"
#include "macros.h"
#include "score.h"

/**
 * @brief initalizes all values of a Score to 0
 * 
 * @param s Score
 */
void initializeScore(Score *s){
    int i;
    s->numCards = 0;
    s->numCoins = 0;
    for (i = 0; i < NUMSUITS; i++){
        s->primes[i] = 0;
    }
    s->scopa = 0;
    s->sevenCoins = 0;
}

/**
 * @brief Prints a player's score
 * 
 * @param s Score
 */
void printScore(Score *s){
    int prime = 0;
    int i;
    printf("Number of cards captured: ");
    printf("%d", s->numCards);
    printf("\n");
    Sleep(SLEEPL);
    printf("Number of Coins captured: ");
    printf("%d", s->numCoins);
    printf("\n");
    Sleep(SLEEPL);
    printf("Number of Scopas: ");
    printf("%d", s->scopa);
    printf("\n");
    Sleep(SLEEPL);
    printf("Prime: ");
    for (i = 0; i < NUMSUITS; i++){
        prime += s->primes[i];
    }
    s->primeTotal = prime; /*calculates primes for later*/
    printf("%d", prime);
    printf("\n");
    Sleep(SLEEPL);
    if (s->sevenCoins == 1){
        printf("You captured the 7 of Coins.\n");
    }

}

/**
 * @brief Calculates the prime value of a number
 * 
 * @param i card number value
 * @return int: prime value
 */
int findPrimeVal(int i){
    if (i == 7){
        return 21;
    }else if (i == 6){
        return 18;
    }else if (i == 1){
        return 16;
    }else if (i == 5){
        return 15;
    }else if (i == 4){
        return 14;
    }else if (i == 3){
        return 13;
    }else if (i == 2){
        return 12;
    }else{ 
        /*face cards*/
        return 10;
    }
}

/**
 * @brief Calculates the correct adjusted score based on Score's primes[] field.
 * 
 * @param s Score
 * @return int: total score value
 */
int findPrime(Score s){
    int i;
    int total = 0;
    for (i = 0; i < NUMSUITS; i++){
        if (s.primes[i] == 7){
            total += 21;
        }else if (s.primes[i] == 6){
            total += 18;
        }else if (s.primes[i] == 1){
            total += 16;
        }else if (s.primes[i] == 5){
            total += 15;
        }else if (s.primes[i] == 4){
            total += 14;
        }else if (s.primes[i] == 3){
            total += 13;
        }else if (s.primes[i] == 2){
            total += 12;
        }else{ 
            /*face cards*/
            total += 10;
        }
    }
    return total;
}

/**
 * @brief Updates the Score with the given card
 * 
 * @param score Score
 * @param c Card
 */
void scoreCard(Score *score, Card c){
    int prime;
    score->numCards++;
    prime = findPrimeVal(c.value);
    if (c.suit == " of Cups"){
        if (score->primes[CUPS] < prime){
            score->primes[CUPS] = prime;
        }
    }else if (c.suit == " of Coins"){
        if (c.value == 7){ 
            score->sevenCoins = 1;
        }
        if (score->primes[COINS] < prime){
            score->primes[COINS] = prime;
        }
        score->numCoins++;
    }else if (c.suit == " of Swords"){
        if (score->primes[SWORDS] < prime){
            score->primes[SWORDS] = prime;
        }
    }else if (c.suit == " of Clubs"){
        if (score->primes[CLUBS] < prime){
            score->primes[CLUBS] = prime;
        }
    }
}

/**
 * @brief Loops through list of cards and scores each card
 * 
 * @param s Score
 * @param head head of linked list
 */
void scoreDeck(Score *s, Node *head){
    while(head != NULL){
        scoreCard(s, head->data);
        head = head->next;
    }
}

/**
 * @brief Adds point to winning player's total
 * 
 * @param p1Field field value from player 1's score
 * @param p2Field field value from player 2's score
 * @param p1Total int representing total score for p1
 * @param p2Total int representing total score for p1
 */
void addScore(int p1Field, int p2Field, int *p1Total, int *p2Total){
    /*adds point to winning player's total*/
    if (p1Field > p2Field){
        (*p1Total)++;
    }else if (p1Field < p2Field){
        (*p2Total)++;
    }
}

/**
 * @brief Calculates total scores of players
 * 
 * @param p1 player 1's Score
 * @param p2 player 2's Score
 * @param p1Total int representing total score for p1
 * @param p2Total int representing total score for p2
 */
void calculateScores(Score *p1, Score *p2, int *p1Total, int *p2Total) {
	addScore(p1->numCards, p2->numCards, p1Total, p2Total);
    addScore(p1->numCoins, p2->numCoins, p1Total, p2Total);
    addScore(p1->scopa, p2->scopa, p1Total, p2Total);
    addScore(p1->sevenCoins, p2->sevenCoins, p1Total, p2Total);
    addScore(p1->primeTotal, p2->primeTotal, p1Total, p2Total);
}

/**
 * @brief Calculates and displays both players' scores
 * 
 * @param p1 
 * @param p2 
 */
void displayScores(Score *p1, Score *p2){
    int p1Total = 0;
    int p2Total = 0;
    printf("--SCORES--\n\n");
    printf("PLAYER 1: \n");
    printScore(p1);
    Sleep(SLEEPL*2);
    printf("\n\n");
    printf("PLAYER 2: \n");
    printScore(p2);
    Sleep(SLEEPL*2);
    printf("\n");

	calculateScores(p1, p2, &p1Total, &p2Total);

    printf("PLAYER 1 total: ");
    printf("%d", p1Total);
    printf("\n");

    printf("PLAYER 2 total: ");
    printf("%d", p2Total);
    printf("\n");

    if (p1Total > p2Total){
        printf("\nPLAYER 1 wins!");
    }else if (p2Total > p1Total){
        printf("\nPLAYER 2 wins!");
    }else{
        printf("\nPLAYER 1 and PLAYER 2 are tied!");
    }
}