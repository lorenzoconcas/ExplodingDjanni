#ifndef GRAPHICLIB_H_INCLUDED
#define GRAPHICLIB_H_INCLUDED

#include "libs.h"

void printWelcome(int); //stampo il benvenuto (due tipi)
void printPlayOptions();//stampa le opzioni principali disponibili per l'utente
void clearUI();//pulisce lo schermo
void printMainOptions();//stampa le opzioni principali (le prime mostrate)
void printDifficulty(); //stampo le varie difficoltà
void printSeparator(); //stampa un semplice separatore
void printRules(int type);//stampa le regole
void printInGameOptions();//stampa le opzioni disponibili quando si giocano le carte
void printLoadCustomDeckGuide();//tampa le istruzioni per caricare un mazzo personalizzato
void printAlivePlayers(player*);//stampa i giocatori ancora in gara
void printOtherPlayers(player* gamePlayers, int exclude);//stampa gli altri giocatori
void pause();//mette in attesa il gioco
void printWinner(player *gamePlayers, int turn);//stampa il vincitore
void setRes(int type);//imposta la dimensione della finestra su Windows
#endif // GRAPHICLIB_H_INCLUDED
