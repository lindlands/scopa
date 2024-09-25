#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "list.h"
#include "macros.h"
#include "score.h"

void initializeScore(Score *s){
    /*initalizes all values of a Score to 0*/
    int i;
    s->numCards = 0;
    s->numCoins = 0;
    for (i = 0; i < NUMSUITS; i++){
        s->primes[i] = 0;
    }
    s->scopa = 0;
    s->sevenCoins = 0;
}

void printScore(Score *s){
    /*prints a players score*/
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

int findPrimeVal(int i){
    /* ∆ calculates the correct adjusted prime value of one number. ∆ */
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

int findPrime(Score s){
    /* ∆ calculates the correct adjusted score based on Score's primes[] field. returns score. ∆ */
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

void scoreCard(Score *score, Card c){
    /*compares Card to Score's pertaining value and updates Score if needed*/
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

void scoreDeck(Score *s, Node *head){
    /*Loops through list of cards and scores each card*/
    while(head != NULL){
        scoreCard(s, head->data);
        head = head->next;
    }
}

void addScore(int p1Field, int p2Field, int *p1, int *p2){
    /*adds point to winning player's total*/
    if (p1Field > p2Field){
        (*p1)++;
    }else if (p1Field < p2Field){
        (*p2)++;
    }

}