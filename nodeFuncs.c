#include "libs.h"
int getDeckDim(card* firstNode) {
    //questa funzione calcola la dimensione di una lista
    int dim = 0;
    while (firstNode->next != NULL) {
        firstNode = firstNode->next;
        dim++;
    }
    return dim;
}
card* getLastCard(card* firstNode) {
    while (firstNode->next != NULL) {
        firstNode = firstNode->next;
    }
    return firstNode;
}
void printCards(card* firstNode) {
    char cardType[][17] = { "EXPLODING DJANNI","MEOOOW", "SHUFFLE", "NOPE","SEE THE FUTURE", "ATTACK", "SKIP", "FAVOR", "DJANNI CARD" };
    int i = 1;
    if(firstNode != NULL)
        while (firstNode->next != NULL) {
#if DEBUG_RELEASE == true
            printf("\n%d %p TIPO %s - DESC %s", i, firstNode, cardType[firstNode->type], firstNode->desc);
#else
            printf("\n%d TIPO %s - DESC %s", i, cardType[firstNode->type], firstNode->desc);
#endif
            firstNode = firstNode->next;
            i++;
        }
}
card* cloneDeck(card* node, int dim, _Bool returnHead) {
    //questa funzione permette di creare un nuovo mazzo partendo da uno gia esistente
    //assicurati che dim sia maggiore di 0
    //se si stanno copiando più mazzi in uno, quindi returnHead == false
    //ricordarsi di fare un backup della testa
    int i;
    card* newDeck = NULL, *headNode;
    //alloco il primo nodo
    newDeck = (card*)malloc(sizeof(card));
    //copio il primo nodo
    headNode = newDeck;
    //copio le carte
    for (i = 0; i < dim; i++) {
        newDeck->type = node->type;
        strcpy(newDeck->desc, node->desc);
        node = node->next;
        newDeck->next = (card*)malloc(sizeof(card));
        newDeck = newDeck->next;
    }
    //
    //restituisco il nodo
    if (returnHead == true) {
        newDeck->next = NULL;
        return headNode;
    } else {

        return newDeck;
    }
}
card* removeNode(card* currentNode) {
    //questa funzione si occupa di eliminare il nodo corrente e rende il successivo, utile per togliere la prima carta del mazzo
    card* aux = NULL;
    if (currentNode->next == NULL) {
        free(currentNode);
        currentNode = NULL;
        return NULL;
    } else {
        aux = currentNode->next;
        free(currentNode);
        currentNode = NULL;
        return aux;
    }
}
card* movePointer(card* currentNode, int newPosition) {
    int i;
    for (i = 0; i < newPosition; i++) {
        if (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
    }
    return currentNode;
}
void insertNode(card* deck, int type, char* desc) {
    while (deck->next != NULL) {
        deck = deck->next;
    }
    //alloco spazio per una nuova carta
    deck->next = (card*)malloc(sizeof(card));
    //passo al nodo successivo
    deck = deck->next;
    //copio le informazioni della carta
    deck->type = type;
    strcpy(deck->desc, desc);
    //rendo null il prossimo nodo
    deck->next = NULL;
}
