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
#define INPUTLEN 20

#define P1TURN -1
#define P2TURN 1
#define HIDE -1
#define DECKSIZE 40

/*-------------------structs-------------------*/

typedef struct card {
    int value;
    char *suit;
}Card;

typedef struct node {
    Card data;
    int order;
    int flag;
    struct node *next;
}Node;

typedef struct score {
    int numCards;
    int sevenCoins;
    int numCoins;
    int primes[4]; /*Cards are sorted as follows: cups, coins, swords, clubs*/
    int primeTotal;
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
    temp->flag = 0;
    if ((*head) == NULL){
        (*head) = temp;
        (*tail) = temp;
        temp->order = 1;
        
    }else{
        temp->order = (*tail)->order + 1;
        (*tail)->next = temp;
        (*tail) = temp;
    }
}

void resetNums(Node **head){
    /*Resets order value of nodes. Starts from 1*/
    int counter = 1;
    Node*temp = *head;
    while (temp != NULL){
        temp->order = counter;
        counter++;
        temp = temp->next;
    }
}

Card findCardInt(Node **head, Node **tail, int n){
    /* ∆ find any specified card ∆ */
    Node *temp, *previous;
    Card c;
    c.value = 0;
    c.suit = 0;
    temp = *head;
    while (temp != NULL){
        if (temp->order == n){
            return temp->data;
        }
        temp = temp->next;
    }
    return c;
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
    if (temp->order == n){
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
        if (temp->order == n){
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

/*WIP mergesort function for list of cards*/

// void merge(Node *rHead, Node *rTail, Node *lHead, Node *lTail){ //WIP●
//     Node *rPoint, *lPoint;
//     Node *head = NULL;
//     Node *tail = NULL;
//     rPoint = rHead;
//     lPoint = lHead;
//     while (rPoint != NULL && lPoint != NULL){
//         if (rPoint <= lPoint){
//             addCard(&head, &tail, rPoint->data);
//             rPoint = rPoint->next;
//         }else{
//             addCard(&head, &tail, lPoint->data);
//             lPoint = lPoint->next;
//         }
//     }
//     while (rPoint != NULL){
//         addCard(&head, &tail, rPoint->data);
//         rPoint = rPoint->next;
//     }
//     while (lPoint != NULL){
//         addCard(&head, &tail, lPoint->data);
//         lPoint = lPoint->next;
//     }
    
// }

// void mergeSort(Node *head, Node *tail, Node**mergeHead, Node**mergeTail){ //WIP●
//     Node *newTail;
//     Node *newHead;
//     Node *filler;
//     int i;
//     int place = 0;
//     if (head->next == NULL){
//         return;
//     }
//     place = (tail->order - head->order + 1)/2;
//     filler = head;
//     for (int i = 0; i <= place; i++){
//         if (i == place){
//             newHead = filler;
//         }else{
//             if (i == place - 1){
//                 newTail = filler;
//                 filler = filler->next;
//                 newTail->next = NULL;
//             }else{
//                 filler = filler->next;
//             }
            
//         }
//     }
//     mergeSort(head, newTail, mergeHead, mergeTail);
//     mergeSort(newHead, tail, mergeHead, mergeTail);

//     return merge(head, newTail, newHead, tail);


// }

void flagForDeletion(Node **head, int nums[INPUTLEN]){
    /*changes all nodes' flag values to -1 if their order values are in nums[]*/
    Node *filler = *head;
    int i = 0;
    while(filler != NULL){
        while(i <= INPUTLEN && nums[i] != 0){
            if(nums[i] == filler->order){
                filler->flag = -1;
                break;
            }
            i++;
        }
        filler = filler->next;
        i = 0;
    }
}

void deleteFlags(Node **head, Node **tail){
    /*removes all nodes in linked list that have a flag value of -1*/
    Node *filler = (*head);
    Card c;
    while (filler != NULL){
        if (filler->flag == -1){
            c = filler->data;
            filler = filler->next;
            removeCard(head, tail, c);
        }else{
            filler = filler->next;
        }
    }
}

/*-----------------Score funtions-----------------*/

void initializeScore(Score *s){
    int i;
    s->numCards = 0;
    s->numCoins = 0;
    for (int i = 0; i < 4; i++){
        s->primes[i] = 0;
    }
    s->scopa = 0;
    s->sevenCoins = 0;
}

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
    for (int i = 0; i < 4; i++){
        prime += s->primes[i];
    }
    s->primeTotal = prime; //calculates primes for later
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
    }else{ /*face cards*/
        return 10;
    }
}

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

void scoreCard(Score *score, Card c){
    /*compares Card to Score's pertaining value and updates Score if needed*/
    int prime;
    score->numCards++;
    prime = findPrimeVal(c.value);
    if (c.suit == " of Cups"){
        if (score->primes[0] < prime){
            score->primes[0] = prime;
        }
    }else if (c.suit == " of Coins"){
        if (c.value == 7){ //7 of coins!
            score->sevenCoins = 1;
        }
        if (score->primes[1] < prime){
            score->primes[1] = prime;
        }
        score->numCoins++;
    }else if (c.suit == " of Swords"){
        if (score->primes[2] < prime){
            score->primes[2] = prime;
        }
    }else if (c.suit == " of Clubs"){
        if (score->primes[3] < prime){
            score->primes[3] = prime;
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


/*-----------------Card funtions-----------------*/


void printCards(Node *head){
    /*Prints all cards in given linked list*/
    Node* temp = head;
    int place = 0;
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
            place++;
            if (place == 3){
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

char cardToString(Card c, char str[12]){
    /*card formatted to [value of suit] and put in str[]*/
    int i = 0;
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
    for (int i = 0; i < 12; i++){
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
    int cardVal;
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


/*WIP functions to determine if chosen card has a possible match*/

// int playable(Card c, Node* tHead, Node* tTail, Node* matchH, Node* matchR){ //WIP●
//     int temp;
//     if (tHead == NULL){
//         resetNums(&tHead);
//         return 0;
//     }
//     temp = c.value;
//     temp -= tHead->data.value;
//     if (temp > 0 && tHead->next != NULL){ /* if val is greater than 0 and not at end of list: cont. case */
//         // removeCardInt(&tHead, &tTail, tHead->next->order);       


//         tHead->next->order = -1;
//         playable(c, tHead, tTail, matchH, matchR);
//     }else{
//         resetNums(&tHead);
//         playable(c, tHead->next, tTail->next, matchH, matchR);
//     }


//     //----------------------------

//     // j = tTail->order;
//     // focus = tHead;
//     // for (int i = 0; i < j; i++){
//     //     while(focus != NULL){
//     //         temp = temp - tHead->data.value;
//     //         if (temp == 0){ //if reach match
//     //             while(focus != NULL){
//     //                 if (focus->order == -1){
//     //                     addCard(&matchH, &matchR, focus->data);
//     //                 }
//     //             }
                
//     //         }
//     //         if (temp < 0){
//     //             focus->next->flag = -1;
//     //         }
            
//     //         temp = c.value;
//     //         focus = focus->next;
//     //         next = next->next;
//     //     }
        
//     // }
// }


// int checkPlayable(Card c, Node* tHead, Node* tTail, Node** matchH, Node** matchT){ //WIP●
//     int sum;
//     Node* focus = tHead;
//     Node *subFocus;
//     int i;
//     int j;
//     sum = c.value;
//     while (focus != NULL){ /* removes all greater-than cards */
//         if (focus->data.value > sum){
//             i = focus->order;
//             focus = focus->next;
//             removeCardInt(&tHead, &tTail, i);   
//         }else{
//             focus = (focus->next); 
//         }
//     }
//     // /*start of elimination part*/
//     // /*note that this implies a list of greater than one*/
//     // focus = tHead;
//     // subFocus = tHead;
//     // for (int i = 0; i < tTail->order; i++){ //for each focus
//     //     //subFocus = subFocus->next; //this needs to change
//     //     sum = c.value - focus->data.value;
//     //     if (sum == 0){
//     //         addCard(matchH, matchT, focus->data);
//     //         focus = subFocus;
//     //         subFocus = subFocus->next;
//     //     }else{
        
//     //         while (subFocus != NULL){ //go until end of list
//     //             if(subFocus->flag != HIDE){ //ignore hidden nodes
//     //                 sum = sum - subFocus->data.value;
//     //                 if (sum <= 0){
//     //                     if (sum == 0){
//     //                     addCard(matchH, matchT, subFocus->data);
//     //                     }
//     //                     focus->next->flag = HIDE;   
//     //                 }else{
//     //                     subFocus = subFocus->next;
//     //                 } 
//     //             }else{
//     //                 subFocus = subFocus->next;
//     //             }
//     //         }
//     //         focus = subFocus;
//     //     } 
        
//     // }
// }

// int rePlayable(int sum, Node* focus, Node* matchH, Node* matchT){ //WIP●
//     //sum to start is 9
//     int subsum = sum;
//     subsum -= focus->data.value; //subsum == 4
//     if (subsum == 0){
//         return 1; //found match
//     }else{
//         rePlayable(subsum, focus, matchH, matchT);
//     }

// }



/*------------------Command functions------------------*/

void toLower(char* str){
    /*converts string to lowercase*/
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
    /*compares two strings to see if they match*/
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

void parseCommand(char *str, int nums[INPUTLEN]){
    /*takes in string and fills in array with numbers from string*/
    int j = 0;
    int i;
    for (int i = 0; i < strlen(str); i++){
        if (str[i] != ' ' && str[i] != '\n'){
            if (str[i] == 1 && str[i + 1] == 0){
                nums[j] = 10;
                j++;
                i++;
            }else{
                nums[j] = str[i] - 48;
                j++;
            }
        }
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
    for (int i = 0; i < DECKSIZE; i++){
        while (found == 1){ /*can just create array of indexes*/
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
    int i = 0;
    Card c;
    for (int i = 0; i < 3; i++){
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
    /*'amimation' for dealing cards*/
    int i = 0;
    for (int i = 0; i < 3; i++){
        printf("/ ");
        Sleep(SLEEPL/2);
    }
    printf("Dealing Cards ");
    for (int i = 0; i < 3; i++){
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

void action(Score *p1, Node **pHead, Node **pTail, Node *opTail, Node **tHead, Node **tTail, int turn){
    /*the player's turn: displays options and carries out specified action*/
    char cStr[12] = {'\0'};
    int cardPlace = 0;
    Node *posHead = NULL;
    Node *posTail = NULL;
    Node *filler = NULL;
    char command[LENGTH];
    int inputNums[INPUTLEN] = {0};
    int i;
    int prime;
    Card c;
    commandEnter:
    if (*pHead == NULL){
        printf("You have no more cards.");
        Sleep(SLEEPL*2);
        return;
    }
    printf("What would you like to do? [ capture card | place card | check deck | help ]\n");
    getCommand(command);
    if(compCom(command, "place card") == 0){
        printf("Which card? Type either card or position in hand (i.e. 1, 2, 3)\n");
        getCommand(command);
        cardPlace = convertToNum(command);
        if (cardPlace > 0  && cardPlace <= (*pTail)->order){
            //checkPlayable
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

        
    }else if (compCom(command, "capture card") == 0){
        printf("Which card from your hand do you want to play? Type position in hand (i.e. 1, 2, 3).\n"); //put this bit in a function
        getCommand(command);
        cardPlace = convertToNum(command);
        if (cardPlace <= 0  || cardPlace > (*pTail)->order){
            printf("Please enter a valid number or card.\n");
            Sleep(SLEEPL*2);
            goto commandEnter;
        }
        c = findCardInt(pHead, pTail, cardPlace);
        if (c.value == 0){ //shouldn't happen
            printf("There was an error.");
            Sleep(SLEEPL*2);
            goto commandEnter;
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
        parseCommand(command, inputNums);
        if (checkCards(c.value, inputNums, tHead, tTail) == 1){
            for (int i = 0; i < 10; i++){
                if(inputNums[i] == 0){
                    break;
                }
                scoreCard(p1, findCardInt(tHead, tTail, inputNums[i]));
                //removeCardInt(tHead, tTail, inputNums[i]);
            }
            flagForDeletion(tHead, inputNums);
            deleteFlags(tHead, tTail);
            removeCardInt(pHead, pTail, cardPlace);

            if (tHead == NULL){
            printf("SCOPA!\n");
            p1->scopa++;            
        }

        }else{
            printf("Please enter a valid number or card.\n");
            Sleep(SLEEPL*2);
            displayCards(*pHead, opTail, *tHead, turn);
            goto commandEnter;
        }   



    }else if ((compCom(command, "check deck") == 0)){
        deckSize(cardPlace);
        Sleep(SLEEPL*2);
        displayCards(*pHead, opTail, *tHead, turn);
        goto commandEnter;
    }else if ((compCom(command, "help") == 0)){
        helpText();
        displayCards(*pHead, opTail, *tHead, turn);
        goto commandEnter;
    }else{
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
    char cStr[12] = {'\0'};
    Card c;
    c.value = 0;
    c.suit = "a";
    Card cardTable[12] = {c};
    Card deck[DECKSIZE] = {c};
    Card playerHand[12] = {c};
    Card opHand[12] = {c};
    int place = 0;
    int i;
    int state = 0;
    int cardPlace = 0;
    Node table;
    Score p1;
    Score p2;
    int turn = P1TURN;
    int p1Total = 0;
    int p2Total = 0;

    initializeScore(&p1);
    initializeScore(&p2);

    //printPicture(".txt");
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
            if (p1Head == NULL && p2Head == NULL){ //no more cards
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
            displayCards(p1Head, p2Tail, tHead, turn);
            action(&p1, &p1Head, &p1Tail, p2Tail, &tHead, &tTail, turn);
            turn = P2TURN;
        }else{
            printf("\n----PLAYER 2----\n");
            playerBuffer();
            displayCards(p2Head, p1Tail, tHead, turn);
            action(&p2, &p2Head, &p2Tail, p1Tail, &tHead, &tTail, turn);
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

    if (p1.numCards > p2.numCards){
        p1Total++;
    }else if (p1.numCards < p2.numCards){
        p2Total++;
    }
    if (p1.numCoins > p2.numCoins){
        p1Total++;
    }else if (p2.numCards > p1.numCards){
        p2Total++;
    }
    if (p1.scopa > p2.scopa){
        p1Total++;
    }else if (p2.scopa > p1.scopa){
        p2Total++;
    }
    if (p1.sevenCoins > p2.sevenCoins){
        p1Total++;
    }else if (p2.sevenCoins > p1.sevenCoins){
        p2Total++;
    }
    if (p1.primeTotal > p2.primeTotal){
        p1Total++;
    }else if (p2.primeTotal > p1.primeTotal){
        p2Total++;
    }

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


    

}