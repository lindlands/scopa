#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "macros.h"

/**
 * @brief Adds card to the end of linked list
 * 
 * @param head head of linked list
 * @param tail tail of linked list
 * @param card card to be added 
 */
void addCard(Node **head, Node **tail, Card card){
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = card;
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

/**
 * @brief Resets order value of nodes. Starts from 1.
 * 
 * @param head head of linked list
 */
void resetNums(Node **head){
    int counter = 1;
    Node*temp = *head;
    while (temp != NULL){
        temp->order = counter;
        counter++;
        temp = temp->next;
    }
}

/**
 * @brief Resets the flag value of nodes to FLAG_NONE
 * 
 * @param head head of linked list
 */
void resetFlags(Node **head){
    Node *temp = *head;
    while (temp != NULL){
        temp->flag = FLAG_NONE;
        temp = temp->next;
    }
}

/**
 * @brief Find card in list by integer
 * 
 * @param head head of linked list
 * @param tail tail of linked list
 * @param n integer value of card to find
 * @return Card that matches the n integer value
 */
Card findCardInt(Node **head, Node **tail, int n){
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

/**
 * @brief Removes given card from the linked list
 * 
 * @param head head of linked list
 * @param tail tail of linked list
 * @param card card to be removed
 * @return int: 0 if successful
 */
int removeCard(Node **head, Node **tail, Card card){
    Node *temp, *previous;
    temp = *head;
    previous = *head;
    /*first node*/
    if (temp->data.suit == card.suit && temp->data.value == card.value){
        *head = (*head)->next;
        free(temp);
        resetNums(head);
        return 0;
    }
    /*not first node, loop through*/
    temp = temp->next;
    while (temp != NULL){
        if (temp->data.suit == card.suit && temp->data.value == card.value){
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

/**
 * @brief Removes and returns card from linked list that matches
 * the given place in the list
 * 
 * @param head head of linked list 
 * @param tail tail of linked list
 * @param n place in the list 
 * @return Card that matches the given numbers
 */
Card removeCardInt(Node **head, Node **tail, int n){
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

/**
 * @brief Removes all cards from the linked list
 * 
 * @param head head of linked list
 * @param tail tail of linked list
 */
void removeList(Node **head, Node **tail){
    while ((*head) != NULL){
        removeCardInt(head, tail, 1);
    }
}

/**
 * @brief Compares the numeric values of two cards
 * 
 * @param c1 card
 * @param c2 card
 * @return int: returns 1 if first card is greater
 */
int compareCards(Card c1, Card c2){
    if (c1.value > c2.value){
        return 1;
    }else{
		/*suit doesn't matter*/
		return 0;
	}
}

/**
 * @brief Sorts linked list by card value
 * 
 * @param head head of linked list
 * @param tail tail of linked list
 */
void insertionSort(Node **head, Node **tail){
    Node *filler = *head;
	Node *prev = *head;
    while(filler != NULL){
        Node *start = *head;
		Node *sprev = NULL;
        while (start != filler){
            if (compareCards(filler->data, start->data)){
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

/**
 * @brief changes all nodes' flag values to FLAG_DELETE if
 * their order values are in nums[]
 * 
 * @param head head of linked list
 * @param nums array containing order values of cards to remove
 */
void flagForDeletion(Node **head, int nums[INPUTLEN]){
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

/**
 * @brief Removes all nodes in linked list that have a flag
 * value of FLAG_DELETE
 * 
 * @param head head of linked list
 * @param tail tail of linked list
 */
void deleteFlags(Node **head, Node **tail){
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