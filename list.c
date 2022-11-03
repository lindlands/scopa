#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "macros.h"


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

void removeList(Node **head, Node **tail){
    while ((*head) != NULL){
        removeCardInt(head, tail, 1);
    }
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