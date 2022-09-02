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
    Node* rear;
    int size;
}List;


/*---------------------Linked list---------------------*/

void createList(){
    Card c;
    List* list = (List*)malloc(sizeof(List));
}

void addCard(Node **head, Node **rear, Card c){
    /* ∆ Adds specified card to the end of llist. ∆ */
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = c;
    temp->next = 0;
    if ((*head) == NULL){
        (*head) = temp;
        (*rear) = temp;
        temp->num = 1;
        temp->flag = 0;
    }else{
        temp->num = (*rear)->num + 1;
        (*rear)->next = temp;
        (*rear) = temp;
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

int removeCard(Node **head, Node **rear, Card c){
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
                *rear = previous;
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

Card removeCardInt(Node **head, Node **rear, int n){
    /* ∆ Removes any specified card ∆ */
    /* ◎ EDGECASES: end of list removed, any in middle, first removed */
    Node *temp, *previous;
    Card c;
    temp = *head;
    previous = *head;
    /*first node*/
    if (temp->num == n){
        *head = (*head)->next;
        c = temp->data;
        free(temp);
        resetNums(head);
        return c;
    }
    /*not first node, loop through*/
    temp = temp->next;
    while (temp != NULL){
        if (temp->num == n){
            if (temp->next == NULL){
                *rear = previous;
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

int playable(Card c, Node* tHead, Node* tRear, Node* matchH, Node* matchR){
    int temp;
    if (tHead == NULL){
        resetNums(&tHead);
        return 0;
    }
    temp = c.value;
    temp -= tHead->data.value;
    if (temp > 0 && tHead->next != NULL){ /* if val is greater than 0 and not at end of list: cont. case */
        // removeCardInt(&tHead, &tRear, tHead->next->num);       


        tHead->next->num = -1;
        playable(c, tHead, tRear, matchH, matchR);
    }else{
        resetNums(&tHead);
        playable(c, tHead->next, tRear->next, matchH, matchR);
    }
}

int checkPlayable(Card c, Node* tHead, Node* tRear, Node* matchH, Node* matchR){
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
            removeCardInt(&tHead, &tRear, i);   
        }else{
            focus = (focus->next); 
        }
    }
    j = tRear->num;
    focus = tHead;
    for (int i = 0; i < j; i++){
        while(focus != NULL){
            temp = temp - tHead->data.value;
            if (temp == 0){ //if reach match
                while(focus != NULL){
                    if (focus->num == -1){
                        addCard(matchH, matchR, focus->data);
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

void dealCards(Card* deck, Node**head1, Node**rear1, Node**head2, Node**rear2, int* place){
    int i = 0;
    Card c;
    for (int i = 0; i < 3; i++){
        c = deck[*place];
        addCard(head1, rear1, c);
        *place = (*place)+1;
        c = deck[*place];
        addCard(head2, rear2, c);
        *place = (*place)+1;
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



/*------------------------------------------------------*/



int main(void){

    /* -- LISTS -- */
    // List* pHand = (List*)malloc(sizeof(List) * 2);
    Node *pHead = NULL;
    Node *pRear = NULL;
    Node *opHead = NULL;
    Node *opRear = NULL;
    Node *tHead = NULL;
    Node *tRear = NULL;
    Node *posHead = NULL;
    Node *posRear = NULL;
   


    // rear = 0;
    // Card b;
    // b.suit = "of Coins";
    // b.value = 7;
    // addCard(&head, &rear, b;
    // b.suit = "of Swords";
    // b.value = 10;
    // addCard(&head, &rear, b);
    // removeCard(&head, &rear, b);


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
    //initializeDeck(deck);
    //dealCards(deck, &pHead, &pRear, &opHead, &opRear, &place);
    //printCards(pHead);

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
        addCard(&tHead, &tRear, deck[place]);
        place = place+1;
    } 
    dealCards(deck, &pHead, &pRear, &opHead, &opRear, &place);
    printf("------The cards have been dealt.------\n\n");
    while (state == 0){
        printf("The cards in your hand are are: ");
        printCards(pHead);
        printf("The cards on the table are:     ");
        printCards(tHead);
        printf("Your opponent has ");
        printf("%d", opRear->num);
        printf(" cards.\n\n");
        commandEnter:
        printf("What would you like to do? [ play card | place card | help ]\n");
        getCommand(command);
        if(compCom(command, "place card") == 0){
            printf("Which card? Type either card or position in hand (i.e. 1, 2, 3)\n");
            getCommand(command);
            cardToString(&playerHand[0], cStr);
            cardPlace = convertToNum(command);
            if (cardPlace > 0  && cardPlace <= pRear->num){
                checkPlayable(pHead->data, tHead, tRear, posHead, posRear); /* thead/ trear are filler for now*/
                c = removeCardInt(&pHead, &pRear, cardPlace);
                addCard(&tHead, &tRear, c);
            }else{
                printf("Please enter a valid number or card.\n");
                Sleep(SLEEPL);
            }

            // if (compCom(command, cStr) == 0){
            //     /*remove the card from the linked list !!!! End marker? >>needs traversal so hmm. probably fine?*/
            //     /*need to be able to remove from middle on linked list*/
            //     /*put it on the end of the table cards (end marker!!)*/
            //     c = removeCardInt(&pHead, &pRear, 1);
            //     addCard(&tHead, &tRear, c);

            // }
            

        }else if (compCom(command, "play card") == 0){
            /*1) select card from list*/
            /*remove from linked list*/
            /*remove from hand ^^use same function as above? as picking card and removing from hand. extra flavor goes after*/
            /*Have to have New structure of "won" cards, saved for later.*/
            /*Probably can go through elaborate checks to find:*/
            /*Seven of coins, highest card count, highest number of coins, save the highest number card of each suit (primes), scopas*/
           
           /*Create funtion that can check if card is playable. can loop through each card for above funct.*/ 

        }else if ((compCom(command, "help") == 0)){
            helpText();
        }else{
            goto commandEnter;
        }
        system("cls");
        printf("\n---------------------------------------\n");
    }

    

}