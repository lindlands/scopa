#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <Windows.h>
#include <ctype.h>
#include <time.h>

#include "command.h"
#include "list.h"
#include "macros.h"

#define SUITL 10
#define SLEEPL 350

#define P1TURN -1
#define P2TURN 1
#define DECKSIZE 40
#define HANDSIZE 3

#define COINS 0
#define CUPS 1
#define SWORDS 2
#define CLUBS 3

#define NUMSUITS 4

/*-------------------structs-------------------*/

typedef struct score {
    int numCards;
    int sevenCoins;
    int numCoins;
    int primes[NUMSUITS]; /*see macros for suit order*/
    int primeTotal;
    int scopa;
}Score;

/*-----------------Score functions-----------------*/

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


/*-----------------Card functions-----------------*/


void printCards(Node *head){
    /*Prints all cards in given linked list*/
    Node* temp = head;
    int place = 0;
    if (head == NULL){
        printf("There are no cards.\n");
    }else{
        while(temp->next != NULL){
            printf("[");
            printf("%d", (*temp).data.value);
            printf((*temp).data.suit);
            printf("]");
            printf(", ");
            temp = (*temp).next;
            place++;
            if (place == 3){
                /*to make format*/
                printf("\n                            ");
                place = 0;
            }
        }
        printf("[");
        printf("%d", (*temp).data.value);
        printf((*temp).data.suit);
        printf("]");
        printf("\n");
    }
}

void cardToString(Card c, char str[12]){
    /*card formatted to [value of suit] and put in str[]*/
    int i;
    int place = 0;
    memset(str,0,strlen(str));
    if (c.value == 10){
        str[place] = '1';
        str[place + 1] = '0';
        place++;
    }else{
        str[place] = c.value + 48;
    }
    place++;
    for (i = 0; i < 12; i++){
        if (c.suit[i] == '\000'){
            break;
        }
        str[place] = c.suit[i];
        place++;
    }
}

int checkCards(int val, int nums[INPUTLEN], Node **head, Node **tail){
    /*verifies that list adds up to chosen value*/
    int i;
    Card c;
    int counter = 0;
    while(nums[i] != 0 && i <= INPUTLEN){
        c = findCardInt(head, tail, nums[i]);
        counter += c.value;
        i++;
    }
    if (counter == val){
        return 1;
    }
    return 0;
}

int checkPlayable(int sum, Node* tHead, Node* tTail, Node** matchH, Node** matchT){
    /*appends combinations that add up to sum to matchH/T*/
    Node* focus;
    Node *oldMatchT;
    int i;
    int j;
    focus = tHead;
    oldMatchT = *matchT;
    focus = tHead;
    while (focus != NULL){
        /*focus until end of the list*/
        if (focus->flag != FLAG_HIDE){
            if ((sum - focus->data.value) == 0){
                addCard(matchH, matchT, focus->data);
            }
            if ((sum - focus->data.value) > 0){
                checkPlayable(sum - focus->data.value, focus->next, tTail, matchH, matchT);
                if (oldMatchT != *matchT){
                    /*if new cards added to match, then checkPlayable was successful*/
                    addCard(matchH, matchT, focus->data);
                    oldMatchT = *matchT;
                }
            }
        }
        focus = focus->next;
    }
}


/*-----------------Game initialization-----------------*/

void printPicture(char str[]){
    /*takes in .txt file (extension included in str) and prints it*/
    FILE *txtfile;
    char c;
    txtfile = fopen(str, "r");
    if (txtfile == NULL){
        printf("There was an error.");
    }
    c = fgetc(txtfile); 
    while (c != EOF) { 
        printf ("%c", c); 
        c = fgetc(txtfile); 
    } 
    fclose(txtfile);
}

void initializeDeck(Card* deck){
    /*adds randomized cards to deck only if deck is fully initialized*/
    int i;
    int counter = 0;
    int num;
    int found = 1;
    Card pile[DECKSIZE];
    for (i = 1; i <= 10; i++){
        pile[counter].value = i;
        pile[counter].suit = " of Cups";
        counter++;
        pile[counter].value = i;
        pile[counter].suit = " of Coins";
        counter++;
        pile[counter].value = i;
        pile[counter].suit = " of Swords";
        counter++;
        pile[counter].value = i;
        pile[counter].suit = " of Clubs";
        counter++;
    }
    counter = 0;
    for (i = 0; i < DECKSIZE; i++){
        srand(time(NULL));
        while (found == 1){
            num = (rand() %(DECKSIZE));
            if (deck[num].value == 0){
                deck[num].value = pile[i].value;
                deck[num].suit = pile[i].suit;
                found = 0;
            }
        }
        counter++;
        found = 1;
    }
}

void deckSize(int place){
    /*takes in location int deck (int) and prints "visual" estimation of deck size*/
    if (place >= 30){
        printf("The deck has a few cards left.");
    }else if (place >= 15){
        
        printf("The deck has about half of the cards left.");
    }else{
        printf("The deck has many cards left.");
    }
    printf("\n\n");
}

void dealCards(Card* deck, Node**head1, Node**Tail1, Node**head2, Node**Tail2, int* place){
    /*adds three cards to both players' hands*/
    int i;
    Card c;
    for (i = 0; i < HANDSIZE; i++){
        if (*place >= 39){
            break;
        }
        c = deck[*place];
        addCard(head1, Tail1, c);
        *place = (*place)+1;
        c = deck[*place];
        addCard(head2, Tail2, c);
        *place = (*place)+1;
    }
}

void displayCards(Node*playerHead, Node*opTail, Node*tableHead, int turn){
    /*prints cards of player and table, and the number of cards of the opponent.*/
    system("cls");
    printf("\n---------------------------------------\n");
    if (turn == P1TURN){
        printf("----PLAYER 1----\n");
    }else{
        printf("----PLAYER 2----\n");
    }
    printf("The cards in your hand are: ");
    printCards(playerHead);
    printf("The cards on the table are: ");
    printCards(tableHead);
    printf("Your opponent has ");
    if (opTail == NULL){
        printf("0 cards.\n\n");
    }else{
        printf("%d", opTail->order);
        if (opTail->order == 1){
            printf(" card.\n\n");
        }else{
            printf(" cards.\n\n");
        }
    }
}

void buffer(char *c){
    /*displays 'screen' that waits for \n to continue*/
    char enterCheck = 'a';
    printf(c);
    while (enterCheck != '\n'){enterCheck = getchar();}
    system("cls");
    printf("\n---------------------------------------\n");
}

void playerBuffer(){
    /*displays 'screen' that waits for \n to continue*/
    char * str = "Press ENTER to start turn.\n";
    buffer(str);
}

void dealText(){
    /*'animation' for dealing cards*/
    int i;
    for (i = 0; i < 3; i++){
        printf("/ ");
        Sleep(SLEEPL/2);
    }
    printf("Dealing Cards ");
    for (i = 0; i < 3; i++){
        printf("/ ");
        Sleep(SLEEPL/2);
    }
    Sleep(SLEEPL);
    system("cls");
    printf("\n---------------------------------------\n");
}

void helpText(){
    /*displays the rules*/
    help_rules:
    system("cls");
    char command[LENGTH] ={'\0'};
    printf("\n------------------RULES------------------\n");
    printf("Scopa is an Italian game, so it is played with a 40-card deck with cards valuing from 1 to 10. The suits are Cups, Coins, Swords, and Clubs.\n");
    printf("The goal of the game is to collect cards, so by the end of the game, scoring of your cards will grant you the most points.\n\n");
    printf("[Press ENTER to proceed with GAMEPLAY rules or type \"EXIT\" to start playing.]\n");
    getCommand(command);
    if (compCom(command, "exit") == 0){
        return;
    }
    help_gameplay:
    system("cls");
    printf("\n-----------------GAMEPLAY-----------------\n");
    printf("Each player will receive 3 cards, and there are 4 face-up cards played on the table.\n");
    printf("You and the opponent will take turns either placing a card on the table or playing a card.\n\n");
    printf("Placing a card on the table simply adds it to the rest of the cards. A card that can capture a card on the table cannot be played.\n\n");
    printf("Capturing cards involves playing a card from your hand.\n");
    printf("You can capture a card it either matching the value of the card you play, or the sum of several cards adds up to your played card.\n\n");
    printf("After the card is played, both your card and the captured cards will be removed.\n");
    printf("When both the player and opponent run out of cards, then three cards are dealt to each.\n\n");
    printf("If you clear the board during the round [exculding the last card played of the last round], that is called a SCOPA, which will add a point to your score.\n\n");
    printf("[Press ENTER to proceed with SCORING rules or type \"EXIT\" to start playing.]\n");
    getCommand(command);
    if (compCom(command, "exit") == 0){
        return;
    }
    help_scoring:
    system("cls");
    printf("\n-----------------SCORING-----------------\n");
    printf("At the end of the game, all of the captured cards are evaluated.\n\n");
    printf("Points can be gained for:\n");
    printf("- Capturing the Seven of Coins\n");
    printf("- Capturing the greatest number of cards\n");
    printf("- Capturing the greatest number of Coins\n");
    printf("- Having the highest \"Prime\".\n");
    printf("---  A Prime is the total of adding the \"best\" card in each suit. The ranking is as follows:\n");
    printf("---  [7]: 21pts. [6], 18pts. [1], 16pts. [5], 15pts. [4], 14pts. [3], 13pts. [2], 12 pts. [8/9/10], 10 pts.\n");
    printf("\nKeep this in mind when capturing cards!\n");
    printf("\nAre you ready to play?\n");
        getCommand(command);
        if (compCom(command, "yes") == 0){
            printf("Okay, we'll get right into it. Good luck! \n\n");
        }else{
            help_loop:
            printf("What would you like to review? [rules/gameplay/scoring/exit]\n");
            getCommand(command);
            if (compCom(command, "rules") == 0){
                goto help_rules;
            }else if (compCom(command, "gameplay") == 0){
                goto help_gameplay;
            }else if (compCom(command, "scoring") == 0){
                goto help_scoring;
            }else if (compCom(command, "exit") == 0){
                return;
            }else{
                goto help_loop;
            }

        }
}

int captureCard(Score *pScore, Node **pHead, Node **pTail, Node **tHead, Node **tTail){
    /*player inputs card from hand and card(s) from table they want to capture*/
    char cStr[12] = {'\0'};
    int cardPlace = 0;
    Node *posHead;
    Node *posTail;
    Node *filler = NULL;
    char command[LENGTH];
    int inputNums[INPUTLEN] = {0};
    int i;
    Card c;
    if (*tHead == NULL){
            printf("There are no cards to capture.\n");
            Sleep(SLEEPL*2);
            return 0;
        }
        printf("Which card from your hand do you want to play? Type position in hand (i.e. 1, 2, 3).\n"); //put this bit in a function
        getCommand(command);
        cardPlace = convertToNum(command);
        if (cardPlace <= 0  || cardPlace > (*pTail)->order){
            printf("Please enter a valid number or card.\n");
            Sleep(SLEEPL*2);
            return 0;
        }
        c = findCardInt(pHead, pTail, cardPlace);
        if (c.value == 0){ 
            /*shouldn't happen*/
            printf("There was an error.");
            Sleep(SLEEPL*2);
            return 0;
        }

        system("cls");
        printf("\n---------------------------------------\n");
        c = findCardInt(pHead, pTail, cardPlace);
        cardToString(c, cStr);
        printf("Your card: ");
        printf("[");
        printf(cStr);
        printf("]\n\n");
        filler = *tHead;
        while (filler != NULL){
            printf("-");
            printf("%d", filler->order);
            printf("- ");
            cardToString(filler->data, cStr);
            printf("[");
            printf(cStr);
            printf("]  \n");
            filler = filler->next;
        }
        printf("\n");

        printf("Which cards on the table? Type position on table separated by a space (e.g. 3 1 5).\n");
        getCommand(command);
        if (parseCommand(command, inputNums) && checkCards(c.value, inputNums, tHead, tTail) == 1){
            for (i = 0; i < 10; i++){ 
                if(inputNums[i] == 0){
                    break;
                }
                scoreCard(pScore, findCardInt(tHead, tTail, inputNums[i]));
            }
            flagForDeletion(tHead, inputNums);
            deleteFlags(tHead, tTail);
            removeCardInt(pHead, pTail, cardPlace);

            if ((*tHead) == NULL){
            system("cls");
            printf("\n\n----SCOPA!----\n");
            pScore->scopa++;
            Sleep(SLEEPL*2);     
            return 1; 
        }

        }else{
            printf("Please enter a valid number or card.\n");
            Sleep(SLEEPL*2);
            return 0;
        }   

}

int placeCard(Node **pHead, Node **pTail, Node **tHead, Node **tTail){
    /*player inputs card from hand, card is added to table if no other moves*/
    int cardPlace = 0;
    Node *posHead;
    Node *posTail;
    char command[LENGTH];
    Card c;
    printf("Which card? Type either card or position in hand (i.e. 1, 2, 3)\n");
    getCommand(command);
    cardPlace = convertToNum(command);
    if (cardPlace > 0  && cardPlace <= (*pTail)->order){
        /*checks if card can be played*/
        posHead = NULL;
        posTail = NULL;
        c = findCardInt(pHead, pTail, cardPlace);
        checkPlayable(c.value, *tHead, *tTail, &posHead, &posTail);
        resetFlags(tHead);
        if (posHead != NULL){
            printf("A card can only be placed on the table when there are no cards to capture.\n");
            Sleep(SLEEPL*4);
            return 0;

        }else{
            c = removeCardInt(pHead, pTail, cardPlace);
            addCard(tHead, tTail, c);
        }
        return 1;
    }else{
        printf("Please enter a valid number or card.\n");
        Sleep(SLEEPL*2);
        return 0;
        
    }
}

void action(Score *p1, Node **pHead, Node **pTail, Node *opTail, Node **tHead, Node **tTail, int turn, int place){
    /*the player's turn: displays options and carries out specified action*/
    char cStr[12] = {'\0'};
    int cardPlace = 0;
    Node *posHead;
    Node *posTail;
    Node *filler = NULL;
    char command[LENGTH];
    int inputNums[INPUTLEN] = {0};
    int i;
    Card c;
    commandEnter:
    displayCards(*pHead, opTail, *tHead, turn);
    if (*pHead == NULL){
        printf("You have no more cards.");
        Sleep(SLEEPL*2);
        return;
    }
    printf("What would you like to do? [ capture card | place card | sort cards | check deck | help ]\n");
    getCommand(command);
    if(compCom(command, "place card") == 0){
        if (!placeCard(pHead, pTail, tHead, tTail)){
            goto commandEnter;
        }
    }else if (compCom(command, "capture card") == 0){
        if (!captureCard(p1, pHead, pTail, tHead, tTail)){
            goto commandEnter;
        }
    }else if ((compCom(command, "sort cards") == 0)){
        insertionSort(pHead, pTail);
        goto commandEnter;
    }else if ((compCom(command, "check deck") == 0)){
        deckSize(place);
        Sleep(SLEEPL*2);
        goto commandEnter;
    }else if ((compCom(command, "help") == 0)){
        helpText();
        goto commandEnter;
    }else{
        printf("Please enter a valid command.\n");
        Sleep(SLEEPL*2);
        goto commandEnter;
    }
}


/*------------------------------------------------------*/



int main(void){
    /* -- LISTS -- */
    Node *p1Head = NULL;
    Node *p1Tail = NULL;
    Node *p2Head = NULL;
    Node *p2Tail = NULL;
    Node *tHead = NULL;
    Node *tTail = NULL;
    Node *posHead = NULL;
    Node *posTail = NULL;
   
    char command[LENGTH] = {'\0'};
    Card c;
    c.value = 0;
    c.suit = "a";
    Card deck[DECKSIZE] = {c};
    int place = 0;
    int i;
    int state = 0;
    Node table;
    Score p1;
    Score p2;
    int turn = P1TURN;
    int p1Total = 0;
    int p2Total = 0;

    initializeScore(&p1);
    initializeScore(&p2);

    /*printPicture(".txt");*/ /*optional/unused to print header text from file*/
    printf("\n-----------------SCOPA-----------------\n");
    printf("Welcome!\n");
    beginning:
    // printf("Would you like to play single player or two player?\n");
    // getCommand(command);
    // if (compCom(command, "single") == 0 || compCom(command, "single player") == 0){
    //     Sleep(SLEEPL); 
    // }else if (compCom(command, "no") == 0){
    //     helpText();
    // }else{
    //     printf("Please enter either yes or no. ");
    //     goto beginning;
    // }
    printf("Do you know how to play scopa?\n");
    getCommand(command);
    if (compCom(command, "yes") == 0){
        printf("Okay, we'll get right into it. Good luck! \n\n");
        Sleep(SLEEPL); 
    }else if (compCom(command, "no") == 0){
        helpText();
    }else{
        printf("Please enter either yes or no. ");
        goto beginning;
    }
    system("cls");
    printf("\n---------------------------------------\n");

    initializeDeck(deck);    
    for (i = 0; i <= HANDSIZE; i++){
        addCard(&tHead, &tTail, deck[place]);
        place = place+1;
    } 

    while (state == 0){ /*-------------------------------------------------------------*/
        
        if(p1Head == NULL && p2Head == NULL){
            dealCards(deck, &p1Head, &p1Tail, &p2Head, &p2Tail, &place);
            if (p1Head == NULL && p2Head == NULL){ 
                /*no more cards*/
                if (turn == P1TURN){
                    scoreDeck(&p2, tHead);
                }else{
                    scoreDeck(&p1, tHead);
                }
                state = 1;
            }
            dealText();
            printf("------The cards have been dealt.------\n\n");
        }

        if(turn == P1TURN){
            printf("\n----PLAYER 1----\n");
            playerBuffer();
            action(&p1, &p1Head, &p1Tail, p2Tail, &tHead, &tTail, turn, place);
            turn = P2TURN;
        }else{
            printf("\n----PLAYER 2----\n");
            playerBuffer();
            action(&p2, &p2Head, &p2Tail, p1Tail, &tHead, &tTail, turn, place);
            turn = P1TURN;
        }

        system("cls");
        printf("\n---------------------------------------\n");
    }

    printf("--SCORES--\n\n");
    printf("PLAYER 1: \n");
    printScore(&p1);
    Sleep(SLEEPL*2);
    printf("\n\n");
    printf("PLAYER 2: \n");
    printScore(&p2);
    Sleep(SLEEPL*2);
    printf("\n");

    addScore(p1.numCards, p2.numCards, &p1Total, &p2Total);
    addScore(p1.numCoins, p2.numCoins, &p1Total, &p2Total);
    addScore(p1.scopa, p2.scopa, &p1Total, &p2Total);
    addScore(p1.sevenCoins, p2.sevenCoins, &p1Total, &p2Total);
    addScore(p1.primeTotal, p2.primeTotal, &p1Total, &p2Total);

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
    printf("\n\n");
    buffer("Press ENTER to exit.");

    removeList(&p1Head, &p1Tail);
    removeList(&p2Head, &p2Tail);
    removeList(&tHead, &tTail);
    removeList(&posHead, &posTail);

    return 0;    

}