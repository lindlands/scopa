#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "command.h"
#include "list.h"
#include "macros.h"
#include "score.h"

/**
 * @brief Clears terminal and prints line
 * 
 */
void clearAndPrintLine() {
    system("cls");
    printf("\n---------------------------------------\n");
}

/**
 * @brief Prints all cards in given linked list
 * 
 * @param head head of linked list
 */
void printCards(Node *head){
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

/**
 * @brief takes in .txt file and prints it
 * 
 * @param str file name
 */
void printPicture(char str[]){
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

/**
 * @brief Prints "visual" estimation of deck size
 * 
 * @param place location in deck
 */
void deckSize(int place){
    if (place >= 30){
        printf("The deck has a few cards left.");
    }else if (place >= 15){
        
        printf("The deck has about half of the cards left.");
    }else{
        printf("The deck has many cards left.");
    }
    printf("\n\n");
}

/**
 * @brief prints cards of player and table, and the number of cards of the opponent.
 * 
 * @param playerHead head of player's hand
 * @param opTail tail of opponent's hand
 * @param tableHead head of cards on the table
 * @param turn marks player 1 or player 2's turn
 */
void displayCards(Node*playerHead, Node*opTail, Node*tableHead, int turn){
    clearAndPrintLine();
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

/**
 * @brief displays 'screen' that waits for enter to continue
 * 
 * @param text text to display
 */
void buffer(char *text){
    char enterCheck = 'a';
    printf(text);
    while (enterCheck != '\n'){enterCheck = getchar();}
    clearAndPrintLine();
}

/**
 * @brief displays 'screen' on player turn that waits for enter
 * 
 */
void playerBuffer(){
    char * str = "Press ENTER to start turn.\n";
    buffer(str);
}

/**
 * @brief animation' for dealing cards
 * 
 */
void dealText(){
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
    clearAndPrintLine();
    printf("------The cards have been dealt.------\n\n");
}

/**
 * @brief prints text signifying end of game
 * 
 */
void endText(){
    printf("------There are no more cards.------\n\n");
    Sleep(SLEEPL*2);
    clearAndPrintLine();
}

/**
 * @brief asks if the player would like to quit
 * 
 * @return 1 if true, else 0
 */
int quitText(){
    char command[LENGTH] ={'\0'};
    printf("Are you sure you'd like to quit?\n");
    getCommand(command);
    if (compCom(command, "yes") || compCom(command, "y")){
        system("cls");
        return 1;
    }
    return 0;
}

/**
 * @brief displays the rules
 * 
 */
void helpText(){
    help_rules:
    system("cls");
    char command[LENGTH] ={'\0'};
    printf("\n------------------RULES------------------\n");
    printf("Scopa is an Italian game, so it is played with a 40-card deck with cards valuing from 1 to 10. The suits are Cups, Coins, Swords, and Clubs.\n");
    printf("The goal of the game is to collect cards, so by the end of the game, scoring of your cards will grant you the most points.\n\n");
    printf("[Press ENTER to proceed with GAMEPLAY rules or type \"EXIT\".]\n");
    getCommand(command);
    if (compCom(command, "exit")){
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
    printf("However, if there is both a single matching card AND a possible combination of multiple cards, you are forced to capture only the single matching card.\n\n");
    printf("After the card is played, both your card and the captured cards will be removed.\n");
    printf("When both the player and opponent run out of cards, then three cards are dealt to each.\n\n");
    printf("If you clear the board during the round [exculding the last card played of the last round], that is called a SCOPA, which will add a point to your score.\n\n");
    printf("[Press ENTER to proceed with SCORING rules or type \"EXIT\".]\n");
    getCommand(command);
    if (compCom(command, "exit")){
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
    printf("\nWould you like to review any rules?\n");
        getCommand(command);
        if (compCom(command, "no")){
            printf("Okay, we'll get right into it. Good luck! \n\n");
        }else{
            help_loop:
            printf("What would you like to review? [rules/gameplay/scoring/exit]\n");
            getCommand(command);
            if (compCom(command, "rules")){
                goto help_rules;
            }else if (compCom(command, "gameplay")){
                goto help_gameplay;
            }else if (compCom(command, "scoring")){
                goto help_scoring;
            }else if (compCom(command, "exit")){
                return;
            }else{
                goto help_loop;
            }

        }
}

/**
 * @brief displays text when capture isn't valid
 * 
 */
void invalidCaptureText(){
    printf("There is a matching card on the table and therfore cannot capture a combination.");
    Sleep(SLEEPL*4);
}

/**
 * @brief prints starting menu sequence
 * 
 */
void printMenu(){
    char command[LENGTH] = {'\0'};
    while(1){
    printf("\n-----------------SCOPA-----------------\n");
    printf("Welcome!\n");
    printf("What would you like to do? [ start | rules | quit ]\n");
    getCommand(command);
    if (compCom(command, "start")){
        return;
    }else if (compCom(command, "rules")){
        helpText();
    }else if (compCom(command, "quit")){
        if (quitText()){
            exit(0);
        }
    }else{
        printf("Please enter a valid command.\n");
    }
    system("cls");
    }
}
