#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "libs.h"

FILE* chooseDeck(int); //sceglie il file del mazzo

int choiceMenu(int minValue, int maxValue); //gestisce i vari menu e le varie scelte
int getAlivePlayersNumber(player*); //rende il numero di giocatori vivi
int whosNext(player *gamePlayers, int currentRound); //restituisce l'ID del giocatore successivo
int chooseVictim(int turn, player *gamePlayers); //sceglie a quale vittima rubare/chiedere una carta
int checkSameCards(char *desc, cardArray *deck, int dim); //controlla quante carte con descrizione identica vi sono
int chooseGiftCard(_Bool isRobbed, int victim,int *deckPData, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4);//sceglie quale carta rubare/regalare
int removeMultipleCards(int deckDim, cardArray *deck, char *desc, int exclude, int nCardsToRemove);//rimuove più carte con descrizione uguale

void transferCard(int posFrom, int posTo, int victim, int *deckPData, cardArray *currentDeck, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4);//trasferisce una carta da un mazzo ad un altro
void acquirePlayers(player*); //acquisisce i nomi dei giocatori
void printCardsOfPlayer(cardArray*, int); //stampa le carte del giocatore scelto
void printMainDeck(card* deck, int dim); //stampa le carte del mazzo principale

_Bool checkCardPresence(cardArray*, int, cardEnum); //controlla se è presente un determinato tipo di carta nel mazzo scelto
_Bool playNope(int currentRound, player *gamePlayers, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4, int *deckPData);//il ciclo delle nope è gestito da questa funzione
_Bool pickedExploding(cardArray *playerDeck, card* deck, int *deckPData, int turn); //gestisce quando viene pescata una Exploding Djanni

card* loadDeck(FILE*, card*, int*); //carica il mazzo dal file al mazzo a nodi
card* shuffleList(card*, int); //mescola una lista fatta di nodi e restituisce il  primo nodo
card* shuffleAssign(card*, cardArray*, cardArray*, cardArray*, cardArray*, int*, int *); //mescola il mazzo principale e assegna le carte ai giocatori
card* insertCardInDeck(cardArray * playerDeck, int position, card* deck);//inserisce una carta nel mazzo giocatore scelto prendendo dal primo nodo del mazzo principale

cardArray* removeCardFromPlayerDeck(int cardPosition, cardArray* playerDeck, int deckDim); //rimuove una determinata carta dal mazzo senza ridimensionarlo

//funzioni del logger
void logger(char logString[], int turn, _Bool printTurn, _Bool newLine); //core del logger
void clearLog(); //pulisce il log del gioco all'avvio del programma
void clearChar(char *string);//pulisce il carattere di log nelle altre funzioni
void logPickedCard(cardArray *deck, int deckPData[], int id, player *gamePlayers, int turn); //logga la carta pescata
void logPlayedCard(cardArray *deck, int id, player *gamePlayers, int turn, int cardPosition, _Bool newLine);//logga la carta giocata
void logRoundPlayerName(char *name, int round, _Bool firstRound);//logga il nome del giocatore di questo turno
void logDeck(card *deck); //salva il mazzo in un file (solo per debug)
void logWinner(char *winnerName, int turn); //logga il vincitore

//funzioni di caricamento e salvataggio
FILE* loadUser(player *gamePlayers, int id, FILE* saveFile); //carica il nome utente e se è vivo
FILE* readDataAfterName(FILE *saveFile, int *deckPData); //carica i dati successivi al nome (numero carte, tipo giocatore)
FILE* loadPlayerDeck(cardArray *deck, FILE* saveFile, int dim);//carica il mazzo del giocatore

card* loadMainDeck(int *deckDim, FILE *saveFile); //carica il mazzo principale

void loadLastInfos(int *currentRound, _Bool *isAttacked, FILE* saveFile); //carica le informazioni finali (turno e attack)

void saveProgress(card *deck, player *gamePlayers, int *deckPData, int currentRound, _Bool isAttacked, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4); //salva la partita
FILE* savePlayer(FILE *saveFile, player *currentPlayer, int id);//salva il giocatore (nome, vivo)
FILE* writePlayerDeck(cardArray *deck, FILE* saveFile, int dim);//salva il mazzo del giocatore
FILE* saveDataAfterName(FILE *saveFile, int *deckPData, int id);//salva numero carte e tipo giocatore
FILE* saveMainDeck(FILE *saveFile, card *deck);//salva il mazzo principale
FILE* saveLastInfos(int currentRound, _Bool haveAttacked, FILE* saveFile);//salva turno e attack
#endif // !FUNCTIONS_H_INCLUDED
