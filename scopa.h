#ifndef SC_HEADER
#define SC_HEADER

typedef struct node{}Node;
typedef struct card{}Card;
void createList();
void addCard(Node **list, Card c);
void removeCard(Node **list, Card c);

#endif