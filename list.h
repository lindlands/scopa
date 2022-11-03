#include "macros.h"

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

typedef struct list {
    Node* head;
    Node* tail;
    int size;
}List;


void createList();
void addCard(Node **head, Node **tail, Card c);
void resetNums(Node **head);
Card findCardInt(Node **head, Node **tail, int n);
Card removeCardInt(Node **head, Node **tail, int n);
void removeList(Node **head, Node **tail);
void flagForDeletion(Node **head, int nums[INPUTLEN]);
void deleteFlags(Node **head, Node **tail);
