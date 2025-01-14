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
#include "score.h"
#include "view.h"

#define SUITL 10

#define P2TURN 1
#define DECKSIZE 40
#define HANDSIZE 3
#define CARD_STR_LEN 12

/**
 * @brief Converts card to a string in format of "[value] of [suit]"
 * 
 * @param c Card
 * @param str converted string
 */
void cardToString(Card c, char str[CARD_STR_LEN]){
    int i;
    int place = 0;
    memset(str,0,strlen(str));
    if (c.value == 10){
        str[place] = '1';
        str[place + 1] = '0';
        place++;
    }else{
        str[place] = c.value + CHAR_TO_NUM;
    }
    place++;
    for (i = 0; i < CARD_STR_LEN; i++){
        if (c.suit[i] == '\000'){
            break;
        }
        str[place] = c.suit[i];
        place++;
    }
}

/**
 * @brief Verifies that values of cards in linked list add to given sum
 * 
 * @param expectedSum int
 * @param nums int array of order values of cards
 * @param head head of linked list
 * @param tail tail of linked list
 * @return int: 1 if sum is correct
 */
int verifySumOfCards(int expectedSum, int nums[INPUTLEN], Node **head, Node **tail){
    int i;
    Card c;
    int counter = 0;
    while(nums[i] != 0 && i <= INPUTLEN){
        c = findCardInt(head, tail, nums[i]);
        counter += c.value;
        i++;
    }
    if (counter == expectedSum){
        return 1;
    }
    return 0;
}

/**
 * @brief Appends all possible card combinations that add up
 * to the given sum to linked list
 * 
 * @param sum int
 * @param tHead head of linked list of all cards
 * @param tTail tail of linked list of all cards
 * @param matchH head of linked list with possible card combinations
 * @param matchT tail of linked list with possible card combinations
 */
void checkPlayable(int sum, Node* tHead, Node* tTail, Node** matchH, Node** matchT){
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

/**
 * @brief Finds if there is a single card in the linked list with
 * the same value as the given card.
 * 
 * @param c Card
 * @param tHead head of linked list
 * @param tTail tail of linked list
 * @return int: 0 if true
 */
int isValidCapture(Card c, Node **tHead, Node **tTail){
    Node *posHead;
    Node *posTail;
    Node *focus;
    int hasSingleMatch = 1;
    posHead = NULL;
    posTail = NULL;
    checkPlayable(c.value, *tHead, *tTail, &posHead, &posTail);
    focus = posHead;
    while (focus != NULL){
        if (posHead->data.value == c.value){
            hasSingleMatch = 0;
        }
        focus = focus->next;
    }
    resetFlags(tHead);
    return hasSingleMatch;
}


/*-----------------Game initialization-----------------*/

/**
 * @brief adds randomized cards to deck only if deck is fully initialized
 * 
 * @param deck deck to be added to
 */
void initializeDeck(Card* deck){
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

/**
 * @brief adds default number of cards to both players' hands
 * 
 * @param deck deck of cards in play
 * @param p1Head head of player one's hand
 * @param p1Tail tail of player one's hand
 * @param p2Head head of player two's hand
 * @param p2Tail tail of player two's hand
 * @param place location in deck
 */
void dealCards(Card* deck, Node**p1Head, Node**p1Tail, Node**p2Head, Node**p2Tail, int* place){
    int i;
    Card c;
    for (i = 0; i < HANDSIZE; i++){
        if (*place >= 39){
            break;
        }
        c = deck[*place];
        addCard(p1Head, p1Tail, c);
        *place = (*place)+1;
        c = deck[*place];
        addCard(p2Head, p2Tail, c);
        *place = (*place)+1;
    }
}

int lengthOfInput(int input[INPUTLEN]){
    int counter = 0;
    for (int i = 0; i < INPUTLEN; i++){
        if (input[i] != 0) {
            counter++;
        }else{
            return counter;
        }
    }
}

/**
 * @brief player inputs card from hand and card(s) from table they want to capture
 * 
 * @param pScore player's score
 * @param pHead head of player's hand
 * @param pTail tail of player's hand
 * @param tHead head of cards on table
 * @param tTail tail of cards on table
 * @return 1 if successful, 0 if cannot capture 
 */
int captureCard(Score *pScore, Node **pHead, Node **pTail, Node **tHead, Node **tTail){
    char cStr[CARD_STR_LEN] = {'\0'};
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

        clearAndPrintLine();
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
        if (parseCommand(command, inputNums) && verifySumOfCards(c.value, inputNums, tHead, tTail)){
            if (lengthOfInput(inputNums) > 1 && !isValidCapture(c, tHead, tTail)){
                invalidCaptureText();
                return 0;
            }
            for (i = 0; i < 10; i++){ 
                if(inputNums[i] == 0){
                    break;
                }
                scoreCard(pScore, findCardInt(tHead, tTail, inputNums[i]));
            }
            flagForDeletion(tHead, inputNums);
            deleteFlags(tHead, tTail);
            c = removeCardInt(pHead, pTail, cardPlace);
            scoreCard(pScore, c);

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

/**
 * @brief player inputs card from hand, card is added to table if no other moves
 * 
 * @param pHead head of player's hand
 * @param pTail tail of player's hand
 * @param tHead head of cards on table
 * @param tTail tail of cards on table
 * @return 1 if successful, 0 if cannot place 
 */
int placeCard(Node **pHead, Node **pTail, Node **tHead, Node **tTail){
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

/**
 * @brief the player's turn: displays options and carries out specified action. 
 * 
 * @param pScore player's score
 * @param pHead head of player's hand
 * @param pTail tail of player's hand
 * @param opTail tail of opponent's hand
 * @param tHead head of cards on table
 * @param tTail tail of cards on table
 * @param turn marks player 1 or player 2's turn
 * @param place location in deck
 * @return Returns 1 if player quits 
 */
int action(Score *pScore, Node **pHead, Node **pTail, Node *opTail, Node **tHead, Node **tTail, int turn, int place){
    char cStr[CARD_STR_LEN] = {'\0'};
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
        return 0;
    }
    printf("What would you like to do? [ capture card | place card | sort cards | check deck | help ]\n");
    getCommand(command);
    if(compCom(command, "place card")){
        if (!placeCard(pHead, pTail, tHead, tTail)){
            goto commandEnter;
        }
    }else if (compCom(command, "capture card")){
        if (!captureCard(pScore, pHead, pTail, tHead, tTail)){
            goto commandEnter;
        }
    }else if (compCom(command, "sort cards")){
        insertionSort(pHead, pTail);
        goto commandEnter;
    }else if (compCom(command, "check deck")){
        deckSize(place);
        Sleep(SLEEPL*2);
        goto commandEnter;
    }else if (compCom(command, "help")){
        helpText();
        goto commandEnter;
    }else if (compCom(command, "exit") || compCom(command, "quit") || compCom(command, "q")){
        if (quitText()){
            return 1;
        }
        Sleep(SLEEPL*2);
        goto commandEnter;
    }else{
        printf("Please enter a valid command.\n");
        Sleep(SLEEPL*2);
        goto commandEnter;
    }
    return 0;
}

/*------------------------------------------------------*/



int main(void){
    /* -- LISTS -- */
    menu: ;

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

    initializeScore(&p1);
    initializeScore(&p2);

    /*printPicture(".txt");*/ /*optional/unused to print header text from file*/
    printMenu();
    clearAndPrintLine();

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
            }else{
                dealText();
            }
        }
        if (state == 0){
            if(turn == P1TURN){
                printf("\n----PLAYER 1----\n");
                playerBuffer();
                if (action(&p1, &p1Head, &p1Tail, p2Tail, &tHead, &tTail, turn, place)){
                    goto menu;
                }
                turn = P2TURN;
            }else{
                printf("\n----PLAYER 2----\n");
                playerBuffer();
                if (action(&p2, &p2Head, &p2Tail, p1Tail, &tHead, &tTail, turn, place)){
                    goto menu;
                }
                turn = P1TURN;
            }
        }

        clearAndPrintLine();
    }
    
    endText();
    displayScores(&p1, &p2);
    printf("\n\n");
    buffer("Press ENTER to exit.");

    removeList(&p1Head, &p1Tail);
    removeList(&p2Head, &p2Tail);
    removeList(&tHead, &tTail);
    removeList(&posHead, &posTail);

    return 0;    

}