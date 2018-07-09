#ifndef NODEFUNCS_H_INCLUDED
#define NODEFUNCS_H_INCLUDED

#include "libs.h"

int getDeckDim(card*); //calcola la dimensione di un mazzo (a lista concatenata)
card* getLastCard(card*); //rende l'ultimo nodo del mazzo
void printCards(card*);//stampa tutte le carte del mazzo
card* cloneDeck(card*, int, _Bool); //clona un mazzo in uno nuovo
card* removeNode(card*);//elimina un nodo dalla lista
card* movePointer(card* currentNode, int newPosition); //sposta il puntatore di un determinato numero di nodi
void insertNode(card* deck, int type, char* desc);//inserisce un nuovo nodo in coda

#endif // NODEFUNCS_H_INCLUDED
