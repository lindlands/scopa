#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "macros.h"


void addCard(Node **head, Node **tail, Card c){
    /* ∆ Adds specified card to the end of llist. ∆ */
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = c;
    temp->next = 0;
    temp->flag = FLAG_NONE;
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

void resetFlags(Node **head){
    /*Resets the flag value of nodes to FLAG_NONE*/
    Node *temp = *head;
    while (temp != NULL){
        temp->flag = FLAG_NONE;
        temp = temp->next;
    }
}

Card findCardInt(Node **head, Node **tail, int n){
    /* ∆ find any specified card ∆ */
    Node *temp;
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

int compareCard(Card c1, Card c2){
    /*returns 1 if first card is greater, 0 if second*/
    if (c1.value > c2.value){
        return 1;
    }else{
		/*suit doesn't matter*/
		return 0;
	}
}

void insertionSort(Node **head, Node **tail){
    Node *filler = *head;
	Node *prev = *head;
    while(filler != NULL){
        Node *start = *head;
		Node *sprev = NULL;
        while (start != filler){
            if (compareCard(filler->data, start->data)){
				/*card goes here*/
				prev->next = filler->next;
				if (filler->next == NULL){
					*tail = prev;
				}
				filler->next = start;
				if (sprev != NULL){
					sprev->next = filler;
				}else{
					*head = filler;
				}
				break;
            }else{
				sprev = start;
                start = start->next;
            }
        }
		prev = filler;
		filler = filler->next;
    }
	resetNums(head);
}


void flagForDeletion(Node **head, int nums[INPUTLEN]){
    /*changes all nodes' flag values to FLAG_DELETE if their order values are in nums[]*/
    Node *filler = *head;
    int i = 0;
    while(filler != NULL){
        while(i <= INPUTLEN && nums[i] != 0){
            if(nums[i] == filler->order){
                filler->flag = FLAG_DELETE;
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
        if (filler->flag == FLAG_DELETE){
            c = filler->data;
            filler = filler->next;
            removeCard(head, tail, c);
        }else{
            filler = filler->next;
        }
    }
}