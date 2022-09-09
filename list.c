#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>

typedef struct card{}Card;

typedef struct node {
    Card data;
    struct node *next;
}Node;


/*---------------------Linked list---------------------*/

void createList(){
    Card c;
    Node* head = (Node*)malloc(sizeof(Node)); /*casts to node pointer as initially returns void pointer♡♡♡*/
    head->data = c;

}

void addCard(Node **head, Node **rear, Card c){
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = c;
    if ((*head) == NULL){
        (*head) = temp;
        (*rear) = temp;
        (*head)->next = 0;
    }else{
        (*rear)->next = temp;
        (*rear) = temp;
    }
}

int removeCard(Node **head, Card c){
    Node **temp, **previous;
    temp = head;
    previous = head;
    /*first node*/
    if ((*temp)->data.suit == c.suit && (*temp)->data.value == c.value){
        *head = (*head)->next;
        return 0;
    }
    /*not first node, loop through*/
    *temp = (*temp)->next;
    while (temp != NULL){
        if ((*temp)->data.suit == c.suit && (*temp)->data.value == c.value){
            (*previous)->next = (*temp)->next;
            return 0;
        }
        previous = temp;
        *temp = (*temp)->next;
    }
    return 1;


}

void dealCards(Card* deck, Card hand1[3], Card hand2[3], int* place){
    int i = 0;
    for (int i = 0; i < 3; i++){
        hand1[i] = deck[*place];
        *place = (*place)+1;
        hand2[i] = deck[*place];
        *place = (*place)+1;
    }
}

void addCard(Node **head, Node **rear, Card c){
    /* ∆ Adds specified card to the end of llist. ∆ */
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = c;
    if ((*head) == NULL){
        (*head) = temp;
        (*rear) = temp;
        (*head)->next = 0;
    }else{
        (*rear)->next = temp;
        (*rear) = temp;
    }
}

void printCards(Card *hand, int max){
    for(int i = 0; i < max; i++){
        if(hand[i].value !=0){
            if (i > 0){
                printf(", ");
            }
            printf("[");
            printf("%d", hand[i].value);
            printf(hand[i].suit);
            printf("]");
        }
    }
    printf("\n");
}

int countCards(Card *hand, int max){
    int i;
    int counter;
    for (int i = 0; i < max; i++){
        if(hand[i].value == 0){
            break;
        }
        counter++;
    }
    return counter;
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
            return 0;
        }
        previous = temp;
        temp = temp->next;
    }
    return 1;


}