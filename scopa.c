#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <Windows.h>
#include <ctype.h>

#define LENGTH 64
#define SUITL 10
#define SLEEPL 350

#define P1TURN -1
#define P2TURN 1

/*-------------------structs-------------------*/

typedef struct card {
    int value;
    char *suit;
}Card;

typedef struct node {
    Card data;
    int num;
    int flag;
    struct node *next;
}Node;

typedef struct score {
    int numCards;
    int sevenCoins;
    int numCoins;
    int primes[4]; /*Cards are sorted as follows: cups, coins, swords, clubs*/
    int scopa;

}Score;

typedef struct list {
    Node* head;
    Node* tail;
    int size;
}List;


/*---------------------Linked list---------------------*/

void createList(){
    Card c;
    List* list = (List*)malloc(sizeof(List));
}

void addCard(Node **head, Node **tail, Card c){
    /* ∆ Adds specified card to the end of llist. ∆ */
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = c;
    temp->next = 0;
    if ((*head) == NULL){
        (*head) = temp;
        (*tail) = temp;
        temp->num = 1;
        temp->flag = 0;
    }else{
        temp->num = (*tail)->num + 1;
        (*tail)->next = temp;
        (*tail) = temp;
    }
}

void resetNums(Node **head){
    int counter = 1;
    Node*temp = *head;
    while (temp != NULL){
        temp->num = counter;
        counter++;
        temp = temp->next;
    }
}

int removeCard(Node **head, Node **tail, Card c){
    /* ∆ Removes any specified card ∆ */
    /* ◎ EDGECASES: end of list removed, any in middle, first removed */
    Node *temp, *previous;
    temp = *head;
    previous = *head;
    /*first node*/
    if (temp->data.suit == c.suit && temp->data.value == c.value){
        *head = (*head)->next;
        free(temp);
        resetNums(head);
        return 0;
    }
    /*not first node, loop through*/
    temp = temp->next;
    while (temp != NULL){
        if (temp->data.suit == c.suit && temp->data.value == c.value){
            if (temp->next == NULL){
                *tail = previous;
            }
            previous->next = temp->next;
            free(temp);
            resetNums(head);
            return 0;
        }
        previous = temp;
        temp = temp->next;
    }
    return 1;
}

Card removeCardInt(Node **head, Node **tail, int n){
    /* ∆ Removes any specified card ∆ */
    /* ◎ EDGECASES: end of list removed, any in middle, first removed */
    Node *temp, *previous;
    Card c;
    temp = *head;
    previous = *head;
    /*first node*/
    if (temp->num == n){
        if ((*head) == (*tail)){
            *head = (*head)->next;
            c = temp->data;
            free(temp);
            *tail = NULL;
            *head = NULL;
        }else{
            *head = (*head)->next;
            c = temp->data;
            free(temp);
            resetNums(head);
        }
        return c;
    }
    /*not first node, loop through*/
    temp = temp->next;
    while (temp != NULL){
        if (temp->num == n){
            if (temp->next == NULL){
                *tail = previous;
            }
            previous->next = temp->next;
            c = temp->data;
            free(temp);
            resetNums(head);
            return c;
        }
        previous = temp;
        temp = temp->next;
    }
    return c;
}


/*-----------------Card funtions-----------------*/

int findPrime(Score s){
    /* ∆ calculates the correct adjusted score based on Score's primes[] field. returns score. ∆ */
    int i;
    int total = 0;
    for (int i = 0; i < 4; i++){
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
        }else{ /*face cards*/
            total += 10;
        }
    }
    return total;
}

void printCards(Node *head){
    Node* temp = head;
    if (head == NULL){
        printf("You have no cards.\n");
    }else{
        while(temp->next != NULL){
            printf("[");
            printf("%d", (*temp).data.value);
            printf((*temp).data.suit);
            printf("]");
            printf(", ");
            temp = (*temp).next;
        }
        printf("[");
        printf("%d", (*temp).data.value);
        printf((*temp).data.suit);
        printf("]");
        printf("\n");
    }
}

char* cardToString(Card* c, char s[12]){
    int i = 0;
    int place = 0;
    if (c->value == 10){
        s[place] = '1';
        s[place + 1] = '0';
        place++;
    }else{
        s[place] = c->value + 48;
    }
    place++;
    for (int i = 0; i < 12; i++){
        if (c->suit[i] == '\000'){
            break;
        }
        s[place] = c->suit[i];
        place++;
    }
}

int playable(Card c, Node* tHead, Node* tTail, Node* matchH, Node* matchR){
    int temp;
    if (tHead == NULL){
        resetNums(&tHead);
        return 0;
    }
    temp = c.value;
    temp -= tHead->data.value;
    if (temp > 0 && tHead->next != NULL){ /* if val is greater than 0 and not at end of list: cont. case */
        // removeCardInt(&tHead, &tTail, tHead->next->num);       


        tHead->next->num = -1;
        playable(c, tHead, tTail, matchH, matchR);
    }else{
        resetNums(&tHead);
        playable(c, tHead->next, tTail->next, matchH, matchR);
    }
}

int checkPlayable(Card c, Node* tHead, Node* tTail, Node* matchH, Node* matchR){
    int temp;
    Node* focus = tHead;
    Node *next;
    int i;
    int j;
    temp = c.value;
    while (focus != NULL){ /* removes all greater-than cards */
        if (focus->data.value > temp){
            i = focus->num;
            focus = focus->next;
            removeCardInt(&tHead, &tTail, i);   
        }else{
            focus = (focus->next); 
        }
    }
    j = tTail->num;
    focus = tHead;
    for (int i = 0; i < j; i++){
        while(focus != NULL){
            temp = temp - tHead->data.value;
            if (temp == 0){ //if reach match
                while(focus != NULL){
                    if (focus->num == -1){
                        addCard(&matchH, &matchR, focus->data);
                    }
                }
                
            }
            if (temp < 0){
                focus->next->flag = -1;
            }
            
            temp = c.value;
            focus = focus->next;
            next = next->next;
        }
        
    }


   
}



/*------------------Command functions------------------*/

void toLower(char* str){
    int i;
    int j = strlen(str);
    for(int i = 0; i < j; i++){
        if (str[i] == '\000'){
            break;
        }
        str[i] = tolower(str[i]);
    }
}

int getCommand(char *arr){
    fgets(arr, LENGTH, stdin);
    toLower(arr);
    printf("\n");
}

int compCom(char *input, char *match){
    int result;
    int len;
    //toLower(match);
    len = strlen(match);
    if(strlen(input) < len){
        return 1;
    }
    result = strncmp(input,match,len);
    if(result == 0){
        if ( !result && input[len] && (input[len] != '\n' || input[len+1]) ){
            return -1;
        }
        return result;
    }
}

int convertToNum(char* command){
    /* takes in string and returns int if possible. else, returns -1. */
    int i;
    for (int i = 0; i < strlen(command) - 1; i++){
        if(!isdigit(command[i])){
            return -1;
        }
    }
    return atoi(command);
}



/*-----------------Game initialization-----------------*/

void initializeDeck(Card* deck){
    int i;
    int counter = 0;
    int num;
    int found = 1;
    Card pile[40];
    for (int i = 1; i <= 10; i++){
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
    for (int i = 0; i < 40; i++){
        while (found == 1){ /*can just create array of indexes*/
            num = (rand() %(40));
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

void dealCards(Card* deck, Node**head1, Node**Tail1, Node**head2, Node**Tail2, int* place){
    int i = 0;
    Card c;
    for (int i = 0; i < 3; i++){
        c = deck[*place];
        addCard(head1, Tail1, c);
        *place = (*place)+1;
        c = deck[*place];
        addCard(head2, Tail2, c);
        *place = (*place)+1;
    }
}

void displayCards(Node*playerHead, Node*opTail, Node*tableHead){
    printf("The cards in your hand are: ");
    printCards(playerHead);
    printf("The cards on the table are: ");
    printCards(tableHead);
    printf("Your opponent has ");
    if (opTail == NULL){
        printf("0 cards.\n\n");
    }else{
        printf("%d", opTail->num);
        if (opTail->num == 1){
            printf(" card.\n\n");
        }else{
            printf(" cards.\n\n");
        }
    }
}

void helpText(){
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
    printf("Placing a card on the table simply adds it to the rest of the cards. You can only do this when none of your cards can be played.\n\n");
    printf("Playing a card will capture cards on the table.\n");
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
            printf("What would you like to review? [rules/gameplay/scoring]\n");
            getCommand(command);
            if (compCom(command, "rules") == 0){
                goto help_rules;
            }else if (compCom(command, "gameplay") == 0){
                goto help_gameplay;
            }else if (compCom(command, "scoring") == 0){
                goto help_scoring;
            }else{
                goto help_loop;
            }

        }
}

void action(Score *p1, Node **pHead, Node **pTail, Node **tHead, Node **tTail){
    char cStr[12] = {'\0'};
    int cardPlace = 0;
    Node *posHead = NULL;
    Node *posTail = NULL;
    char command[LENGTH];
    Card c;
    commandEnter:
    if (*pHead == NULL){
        printf("You have no more cards.");
        Sleep(SLEEPL*4);
        return;
    }
    printf("What would you like to do? [ capture card | place card | help ]\n");
    getCommand(command);
    if(compCom(command, "place card") == 0){
        printf("Which card? Type either card or position in hand (i.e. 1, 2, 3)\n");
        getCommand(command);
        cardPlace = convertToNum(command);
        if (cardPlace > 0  && cardPlace <= (*pTail)->num){
            //checkPlayable(p1Head->data, tHead, tTail, posHead, posTail); /* thead/ tTail are filler for now*/
            //addCard(&posHead, &posTail, c);
            if (posHead != NULL){
                printf("A card can only be placed on the table when there are no cards to capture.\n");
                Sleep(SLEEPL*2);

            }else{
                c = removeCardInt(pHead, pTail, cardPlace);
                addCard(tHead, tTail, c);
            }
        }else{
            printf("Please enter a valid number or card.\n");
            Sleep(SLEEPL*2);
            goto commandEnter;
            
        }

        // if (compCom(command, cStr) == 0){
        //     /*remove the card from the linked list !!!! End marker? >>needs traversal so hmm. probably fine?*/
        //     /*need to be able to remove from middle on linked list*/
        //     /*put it on the end of the table cards (end marker!!)*/
        //     c = removeCardInt(&p1Head, &p1Tail, 1);
        //     addCard(&tHead, &tTail, c);

        // }
        

    }else if (compCom(command, "capture card") == 0){
        /*1) select card from list*/
        /*remove from linked list*/
        /*remove from hand ^^use same function as above? as picking card and removing from hand. extra flavor goes after*/
        /*Have to have New structure of "won" cards, saved for later.*/
        /*Probably can go through elaborate checks to find:*/
        /*Seven of coins, highest card count, highest number of coins, save the highest number card of each suit (primes), scopas*/
        
        /*Create funtion that can check if card is playable. can loop through each card for above funct.*/ 
        checkPlayable((*pHead)->data, *tHead, *tTail, posHead, posTail);
        if (posHead == NULL){
            printf("There are no cards that can be captured with your hand.\n");
            Sleep(SLEEPL*2);
        }
        if (tHead == NULL){
            printf("SCOPA!\n");
            p1->scopa++;
            
            
        }

    }else if ((compCom(command, "help") == 0)){
        helpText();
    }
}





/*------------------------------------------------------*/



int main(void){

    /* -- LISTS -- */
    // List* pHand = (List*)malloc(sizeof(List) * 2);
    Node *p1Head = NULL;
    Node *p1Tail = NULL;
    Node *p2Head = NULL;
    Node *p2Tail = NULL;
    Node *tHead = NULL;
    Node *tTail = NULL;
    Node *posHead = NULL;
    Node *posTail = NULL;
   


    // tail = 0;
    // Card b;
    // b.suit = "of Coins";
    // b.value = 7;
    // addCard(&head, &tail, b;
    // b.suit = "of Swords";
    // b.value = 10;
    // addCard(&head, &tail, b);
    // removeCard(&head, &tail, b);


    char command[LENGTH] = {'\0'};
    char cStr[12] = {'\0'};
    Card c;
    c.value = 0;
    c.suit = "a";
    Card cardTable[12] = {c};
    Card deck[40] = {c};
    Card playerHand[12] = {c};
    Card opHand[12] = {c};
    int place = 0;
    int i;
    int state = 0;
    int cardPlace = 0;;
    Node table;
    Score p1;
    Score p2;
    int turn = P1TURN;
    //initializeDeck(deck);
    //dealCards(deck, &p1Head, &p1Tail, &p2Head, &p2Tail, &place);
    //printCards(p1Head);

    printf("\n-----------------SCOPA-----------------\n");
    printf("Welcome!\n");
    beginning:
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
    for (int i = 0; i < 4; i++){
        addCard(&tHead, &tTail, deck[place]);
        place = place+1;
    } 

    while (state == 0){ /*-------------------------------------------------------------*/

        if(p1Head == NULL && p2Head == NULL){
            dealCards(deck, &p1Head, &p1Tail, &p2Head, &p2Tail, &place);
            printf("------The cards have been dealt.------\n\n");
        }
 
        if(turn == P1TURN){
            printf("----PLAYER 1----\n");
            displayCards(p1Head, p2Tail, tHead);
            action(&p1, &p1Head, &p1Tail, &tHead, &tTail);
            turn = P2TURN;
        }else{
            printf("----PLAYER 2----\n");
            displayCards(p2Head, p1Tail, tHead);
            action(&p2, &p2Head, &p2Tail, &tHead, &tTail);
            turn = P1TURN;
        }

        system("cls");
        printf("\n---------------------------------------\n");
    }

    

}