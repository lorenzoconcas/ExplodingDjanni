#include "libs.h"

//serve a stampare il tipo carta
char cardType[][17] = { "EXPLODING DJANNI","MEOOOW", "SHUFFLE", "NOPE","SEE THE FUTURE", "ATTACK", "SKIP", "FAVOR", "DJANNI CARD" };

FILE* chooseDeck(int choice) {
    //variabili
    FILE *deckFile = NULL; //file del mazzo
    char fileName[FILENAME_MAX_DIM]; //nome del file personalizzato
    char extension[] = ".txt"; //estensione del file

    switch (choice) {
    case EASY: {
        deckFile = fopen("explodingDjanniEasy.txt", "r");
        logger("E' stato scelto il mazzo HEASY", 0, true, true);
        break;
    }
    case MEDIUM: {
        deckFile = fopen("explodingDjanniMedium.txt", "r");
        logger("E' stato scelto il mazzo MEDIUM", 0, true, true);
        break;
    }
    case HARD: {
        deckFile = fopen("explodingDjanniHard.txt", "r");
        logger("E' stato scelto il mazzo HARD", 0, true, true);
        break;
    }
    case CUSTOM: {
        //carica un mazzo personalizzato
        printLoadCustomDeckGuide();
        scanf("%s", fileName);
        getchar();
        strcat(fileName, extension);
        deckFile = fopen(fileName, "r");
        if (deckFile != NULL)
            printf("\nMazzo accettato");
        logger("E' stato scelto un mazzo personalizzato", 0, true, true);
        break;
    }
    }
    //se il mazzo non viene trovato
    if (deckFile == NULL) {
        printf("\nMazzo non trovato");
        logger("Il mazzo non è stato trovato", 0, true, true);
        exit(-1);
    }

    return deckFile;
}

int choiceMenu(int minValue, int maxValue) {
    //questa funzione serve a gestire le scelte dell'utente
    int choice;
    do {
        scanf("%d", &choice);
        getchar();
        if (choice < minValue || choice > maxValue)
            printf("\nScelta non valida, riprova\nScelta:\\> ");
    } while (choice < minValue || choice > maxValue);
    return choice;
}
int getAlivePlayersNumber(player* gamePlayers) {
    //questa funzione rende il numero di giocatori ancora vivi
    int i, alive = 0;
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (gamePlayers[i].alive == true)
            alive++;
    }
    return alive;
}
int whosNext(player *gamePlayers, int currentRound) {
    /*
    questa funzione permette di sapere qual'è il prossimo giocatore,
    nel caso il valore sia superiore ai giocatori supportati, resetto il contatore
    */
    do {
        currentRound++;
        switch (currentRound) {
        case 4: {
            //serve a resettare il ciclo dei giocatori
            currentRound = 0;
            break;
        }
        default: {
            break;
        }
        }
    } while (gamePlayers[currentRound].alive == false);
    return currentRound;
}
int chooseVictim(int turn, player *gamePlayers) {
    /*
    Questa funzione viene usata dalla favor e dalle Djanni Card,
    per scegliere la vittima a cui rubare o farsi dare una carta
    */
    int victim;
    printf("\nScegli uno dei giocatori che ti regalera' una carta a sua scelta");
    printOtherPlayers(gamePlayers, turn);
    do {
        printf("\nScelta:\\> ");
        scanf("%d", &victim);
        getchar();
        victim--;
        if (victim == turn || victim < 0 || victim > 4)
            printf("\nScelta non valida, Riprova :\\>");
    } while (victim == turn || victim < 0 || victim > 4);

    return victim;

}
int checkSameCards(char *desc, cardArray *deck, int dim) {
    //usato dalla Djanni Card per verificare se ci siano carte identiche nella descrizione
    int sameCards = 0;
    int i;
    for (i = 0; i < dim; i++) {
        if (strcmp(deck[i].desc, desc) == 0)
            sameCards++;
    }
    return sameCards;
}
int chooseGiftCard(_Bool isRobbed, int victim, int *deckPData, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4) {
    /*
    Serve a scegliere quale carta regalare/rubare
    Scegliamo prima il mazzo da cui rubare la carta e poi quale carta prendere
    */
    int giftCard;
    cardArray *victimDeck = NULL;

    if (isRobbed == false) {
        pause();
        printf("\nScegli quale carta donare al tuo avversario\n");
    }

    //per risparmiare righe di codice utilizziamo uno switch
    switch (victim) {
    case 0: {
        victimDeck = deckP1;
        break;
    }
    case 1: {
        victimDeck = deckP2;
        break;
    }
    case 2: {
        victimDeck = deckP3;
        break;
    }
    case 3: {
        victimDeck = deckP4;
        break;
    }
    }

    //mostro le carte del giocatore
    printCardsOfPlayer(victimDeck, deckPData[victim]);

    printf("\nScelta:\\> ");
    do {
        scanf("%d", &giftCard);
        getchar();
        giftCard--;
        if (giftCard < 0 || giftCard >  deckPData[victim] - 1)
            printf("\nScelta non valida, riprova\nScelta:\\> ");
    } while (giftCard < 0 || giftCard >  deckPData[victim] - 1);

    if (isRobbed == false) {
        printf("\nRendi il comando");
        pause();
    }
    return giftCard;
}
int removeMultipleCards(int deckDim, cardArray *deck, char *desc, int exclude, int nCardsToRemove) {
    /*
    Usata dalla Djanni Card quando ne vengono giocate tre uguali, per rimuovere le due carte extra
    (la prima viene sostituita da quella che viene rubata)
    Viene prima copiato il mazzo principale escluse le carte da togliere in un mazzo ausiliario
    poi vengono ricopiate da quest'ultimo a quello principale
    Il mazzo principale viene ridimensionato nel main
    */

    int i,k, j, nElements = 0;
    int *positions = NULL;
    cardArray *tmpDeck = NULL;
    for (i = 0; i < deckDim; i++) {
        if (strcmp(deck[i].desc, desc) == 0)
            nElements++;
    }
    positions = (int*)malloc(nElements * sizeof(int));
    for (i = 0, k = 0; i < deckDim; i++) {
        if (strcmp(deck[i].desc, desc) == 0) {
            positions[k] = i;
            k++;

        }
    }
    tmpDeck = (cardArray*)malloc((deckDim - nCardsToRemove) * sizeof(cardArray));

    for (i = 0, k = 0, j = 0; i < deckDim; i++) {
        if (i == positions[k]) {

            k++;
        } else {
            tmpDeck[j].type = deck[i].type;
            strcpy(tmpDeck[j].desc, deck[i].desc);
            j++;
        }
    }

    for (i = 0; i < (deckDim - nCardsToRemove); i++) {
        deck[i].type = tmpDeck[i].type;
        strcpy(deck[i].desc, tmpDeck[i].desc);
    }

    return (deckDim - nCardsToRemove);


}

void acquirePlayers(player* gamePlayers) {
    //acquisisco i nomi dei giocatori, e setto il valore alive a true
    int i;
    char tmpChar[LOGGER_CHAR_DIM];

    printWelcome(0);
    printf("\nInserisci i nomi dei giocatori ");
    logger("Ecco i nomi dei giocatori : ", 0, true, true);
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        printf("\nNome Giocatore (%d):\\> ", i + 1); //non vogliamo confondere l'utente con
        scanf("%s", gamePlayers[i].name);
        getchar();
        clearChar(tmpChar);
        strcat(tmpChar, "Nome : ");
        strcat(tmpChar, gamePlayers[i].name);
        logger(tmpChar, 0, false, true);
        gamePlayers[i].alive = true;
    }
}
void printCardsOfPlayer(cardArray *playerDeck, int dim) {
    /*
    Stampa le carte presenti nel mazzo passato alla funzione, per un totale pari a quello passato
    */
    int k = 0;
    printSeparator();
    printSeparator();
    for (k = 0; k < dim; k++) {
        printf("\nCarta N(%d) : [Tipo : %s][Descrizione : %s]", k + 1, cardType[playerDeck[k].type], playerDeck[k].desc);
    }
    printf("\n");
    printf("\n");
    printSeparator();
    printSeparator();
}
void printMainDeck(card* deck, int dim) {
    int i;
    printSeparator();
    printSeparator();
    for (i = 0; i < dim; i++) {
        printf("\nCarta N(%d) : [Tipo : %s][Descrizione : %s]", i + 1, cardType[deck->type], deck->desc);
        deck = deck->next;
    }
    printf("\n");
    printSeparator();
    printSeparator();
}
void transferCard(int posFrom, int posTo, int victim, int *deckPData, cardArray *currentDeck, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4) {
    //trasferisco una carta dal mazzo vittima (scelto tramite switch) al mazzo corrente, dalla posizione iniziale (posFrom) alla posizine di arrivo (posTo)
    cardArray *victimDeck;
    switch (victim) {
    case 0: {
        victimDeck = deckP1;
        break;
    }
    case 1: {
        victimDeck = deckP2;
        break;
    }
    case 2: {
        victimDeck = deckP3;
        break;
    }
    case 3: {
        victimDeck = deckP4;
        break;
    }
    }

    currentDeck[posTo].type = victimDeck[posFrom].type;
    strcpy(currentDeck[posTo].desc, victimDeck[posFrom].desc);
    //rimuovo la carta dal mazzo vittima
    removeCardFromPlayerDeck(posFrom, victimDeck, deckPData[victim]);
}

_Bool checkCardPresence(cardArray* deck, int dim, cardEnum type) {
    //controllo se una carta è presente verificandone il tipo, appena viene trovata il ciclo viene interroto
    int i = 0;
    _Bool isPresent = false;
    for (i = 0; i < dim; i++) {
        //tmp = deck[i].type;
        if (deck[i].type == type) {
            isPresent = true;
            break;
        }
    }
    return isPresent;
}
_Bool playNope(int currentRound, player *gamePlayers, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4, int *deckPData) {
    int result = 0;
    int nopeInDecks[] = { 0,0,0,0 };
    int nopePlayedByPlayers[] = { 0,0,0,0 };
    int i, id = 0, aux;
    int prevRound = 0;
    int minPlayerWithNope = 0;
    _Bool aNopeIsPlayed;
    _Bool isNopped;
    _Bool canPlay[NUMBER_OF_PLAYERS] = { true, true, true, true };
    _Bool minCondition = true;
    //controllo quante nope vi sono nei mazzi
    {
        id = 0;
        if (gamePlayers[id].alive == true) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP1[i].type == NOPE)
                    nopeInDecks[id]++;
            }
#if DEBUG_RELEASE == true
            printf("\n(%s)nopeP1 %d", gamePlayers[id].name, nopeInDecks[id]);
#endif // true


        }
    }
    {
        id = 1;
        if (gamePlayers[id].alive == true) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP2[i].type == NOPE)
                    nopeInDecks[id]++;
            }
#if DEBUG_RELEASE == true
            printf("\n(%s)nopeP2 %d", gamePlayers[id].name, nopeInDecks[id]);
#endif
        }
    }
    {
        id = 2;
        if (gamePlayers[id].alive == true) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP3[i].type == NOPE)
                    nopeInDecks[id]++;
            }
#if DEBUG_RELEASE == true
            printf("\n(%s)nopeP3 %d", gamePlayers[id].name, nopeInDecks[id]);
#endif
        }
    }
    {
        id = 3;
        if (gamePlayers[id].alive == true) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP4[i].type == NOPE)
                    nopeInDecks[id]++;
            }
#if DEBUG_RELEASE == true
            printf("\n(%s)nopeP4 %d", gamePlayers[id].name, nopeInDecks[id]);
#endif
        }
    }


    //controllo quanti hanno nope
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (nopeInDecks[i] < 1)
            canPlay[i] = false;
        else if (nopeInDecks[i] >0)
            canPlay[i] = true;
    }
    //controllo se un solo giocatore ha le nope chiedo direttamente a lui se la vuole giocare altrimenti ciclo chi possiede le nope
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (canPlay[i] == true)
            minPlayerWithNope++;
    }
    if (minPlayerWithNope == 1) {
        id = 0;
        for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
            if (canPlay[i] == true) {
                id = i;
            }
        }
        clearUI();
        printf("\n%s vuoi giocare una nope?\n(1) Gioca\n(2) Non intervenire\nScelta:\\> ", gamePlayers[id].name);
        if (choiceMenu(1, 2) == 1) {
            nopePlayedByPlayers[id]++;
            nopeInDecks[id]--;
        }
    } else {
        clearUI();
        //parto dal giocatore corrente
        id = currentRound;
        do {
            //calcolo quello successivo, se
            id = whosNext(gamePlayers, id);
            if (id == prevRound)
                minCondition = false;
            //se ha nope, controllo se ha ancora carte sufficenti altrimenti setto false e non verà più incluso nel ciclo
            if (canPlay[id] == true) {
                if (nopeInDecks[id] > 0) {
                    printf("\nE' stata giocata una carta,%s vuoi annullarla?\n (1) Usa una Nope\n (2) Non intervenire\nScelta:\\> ", gamePlayers[id].name);
                    if (choiceMenu(1, 2) == 1) {
                        nopeInDecks[id]--;
                        nopePlayedByPlayers[id]++;
                    }
                    //lo stesso se rinuncia a giocare la nope, non potrà più farlo fino alla prossima occasione
                    else {
                        canPlay[id] = false;
                    }
                } else {
                    canPlay[id] = false;
                }
            }
            //controllo se vi sono ancora giocatori in grado di giocare nope
            minCondition = false;
            aNopeIsPlayed = 0;
            for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
                if (canPlay[i] == true)
                    aNopeIsPlayed++;
            }
            if (aNopeIsPlayed > 0) {
                minCondition = true;
            } else
                minCondition = false;

            prevRound = id;
        } while (minCondition == true);
    }

    //rimuove le nope utilizzate
    {
        id = 0;
        aux = 0;
        if (nopePlayedByPlayers[id] > 0) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP1[i].type == NOPE && aux < nopePlayedByPlayers[id]) {
                    deckP1 = removeCardFromPlayerDeck(i, deckP1, deckPData[id]);
                    aux++;
                }
            }
        }
        id = 1;
        aux = 0;
        if (nopePlayedByPlayers[id] > 0) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP2[i].type == NOPE&& aux < nopePlayedByPlayers[id]) {
                    deckP2 = removeCardFromPlayerDeck(i, deckP2, deckPData[id]);
                    aux++;
                }
            }
        }
        id = 2;
        aux = 0;
        if (nopePlayedByPlayers[id] > 0) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP3[i].type == NOPE && aux < nopePlayedByPlayers[id]) {
                    deckP3 = removeCardFromPlayerDeck(i, deckP3, deckPData[id]);
                    aux++;
                }
            }
        }
        id = 3;
        aux = 0;
        if (nopePlayedByPlayers[id] > 0) {
            for (i = 0; i < deckPData[id]; i++) {
                if (deckP4[i].type == NOPE && aux < nopePlayedByPlayers[id]) {
                    deckP4 = removeCardFromPlayerDeck(i, deckP4, deckPData[id]);
                    aux++;
                }
            }
        }
    }
    result = 0;
    //calcolo le nope giocate e aggiorno le informazioni dei mazzi (le carte sono gia state tolte)
    for (i = 0; i < 4; i++) {
        result = result + nopePlayedByPlayers[i];
        deckPData[i] = deckPData[i] - nopePlayedByPlayers[i]; //tolgo le nope giocate da ogni giocatore alla dimensione del mazzo
    }
    if (result == 0) {
        isNopped = false;
    } else if (result % 2 == 0)
        isNopped = false;
    else {
        isNopped = true;
        printf("\nNon hai alcun potere qui (contro la Nope)");
    }
    clearUI();
    return isNopped;
}
_Bool pickedExploding(cardArray *playerDeck,card* deck, int *deckPData, int turn) {
    /*
    Controllo che sia presente una carta di tipo Meoow, in caso affermativo
    chiedo all'utente se desidera utilizzarla per salvarsi
    Se decide di salvarsi copio riposiziono la carta exploding djanni nel mazzo principale e la cancello dal mazzo del giocatore
    la carta exploding djanni si trova sicuramente alla fin edel mazzo perchè è appena stata pescata, quindi copio quella
    in seguito cerco la prima carta meoow e la rimuovo
    Se il giocatore decide di non salvarsi procedo a eliminare il mazzo del giocatore e restituisco false, utilizzato poi dalla funzione chiamante per
    deallocare la variabile del mazzo e impostare gli ultimi dati (vedi funzione chiamante)
    */

    int i = 0;
    if (checkCardPresence(playerDeck, deckPData[turn], MEOOOW) == true) {
        printf("\nHai pescato una Exploding Djanni, ma puoi salvarti usando una Meoow\nVuoi Giocarla?\n(1) Gioca\n(2) Non giocare (cosi' perderai la partita)\nScelta:\\> ");
        if (choiceMenu(1, 2) == 1) {// il giocatore sceglie di salvarsi
            //rimetto la carta E_D nel mazzo principale e la tolgo dal mazzo del giocatore rimettendola nel mazzo principale
            insertNode(deck, playerDeck[deckPData[turn] - 1].type, playerDeck[deckPData[turn] - 1].desc);
            removeCardFromPlayerDeck(deckPData[turn] - 1, playerDeck, deckPData[turn]);
            deckPData[turn]--;
            //rimuovo la carta meoow dal mazzo
            for (i = 0; i < deckPData[turn]; i++) {
                if (playerDeck[i].type == MEOOOW)
                    break;
            }
            removeCardFromPlayerDeck(i, playerDeck, deckPData[turn]);
            deckPData[turn]--;
            return true;
        } else {
            for (i = 0; i < deckPData[turn]; i++)
                removeCardFromPlayerDeck(0, playerDeck, deckPData[turn]);
            return false;
        }
    } else {
        for (i = 0; i < deckPData[turn]; i++)
            removeCardFromPlayerDeck(0, playerDeck, deckPData[turn]);
        return false;
    }
}

card* loadDeck(FILE* deckFile, card* deck, int* deckData) {
    //variabili
    card *head = NULL;
    int i;
    int tmp = 0; //verrà utilizzato per evitare il warning del cast da int a enum

    //leggo la prima riga del mazzo, contenente le dimensioni del mazzo
    {
        fscanf(deckFile, "%d", &deckData[nExplodingCards]);
        fscanf(deckFile, "%d", &deckData[nMeoowCards]);
        fscanf(deckFile, "%d", &deckData[nOtherCards]);
    }
    //segno la somma delle carte (aka dimensione del mazzo)
    deckData[nAllCards] = deckData[nExplodingCards] + deckData[nMeoowCards] + deckData[nOtherCards];
    //deck = (card*)malloc(sizeof(card)); //alloco il primo nodo del mazzo
    head = deck;
    for (i = 0; i < deckData[nAllCards]; i++) {
        fscanf(deckFile, "%d", &tmp); //legge il tipo di carta
        deck->type = tmp;
        fscanf(deckFile, " "); //utilizzato per catturare lo spazio fra il tipo di carta e la descrizione
        fscanf(deckFile, "%[^\n]s", deck->desc); //legge la descrizione della carta sino al carattere \0 di termine stringa
        deck->next = (card*)malloc(sizeof(card)); //alloco il prossimo nodo che conterrà la prossima carta
        deck = deck->next; //sposto il puntatore al prossimo nodo
    }
    deck->next = NULL;
    return head;
}
card* shuffleAssign(card* deck, cardArray* deckP1, cardArray* deckP2, cardArray* deckP3, cardArray* deckP4, int *deckData, int *deckPData) {
    //variabili
    card *meoowDeck = NULL, *explodingDeck = NULL, *otherDeck = NULL, *newDeck = NULL;
    card *auxMainDeck = NULL, *aux = NULL;
    int i;

    auxMainDeck = deck;

    explodingDeck = cloneDeck(deck, deckData[nExplodingCards], true);
    //sposto il puntatore ausiliario per poter copiare le carte meoow
    for (i = 0; i < deckData[nExplodingCards]; i++) {
        auxMainDeck = auxMainDeck->next;
    }
    meoowDeck = cloneDeck(auxMainDeck, deckData[nMeoowCards], true);
    //sposto il puntatore ausiliario per poter copiare le carte normali
    for (i = 0; i < deckData[nMeoowCards]; i++) {
        auxMainDeck = auxMainDeck->next;
    }
    otherDeck = cloneDeck(auxMainDeck, deckData[nOtherCards], true);
    //mescolo le carte normali
    otherDeck = shuffleList(otherDeck, deckData[nOtherCards]);
    //assegno le carte ai giocatori
    /*mazzo 1*/
    {
        for (i = 0; i < NORMAL_ASSIGNED_PERDECK; i++) {
            deckP1[i].type = otherDeck->type;
            strcpy(deckP1[i].desc, otherDeck->desc);
            otherDeck = removeNode(otherDeck);
        }
        //assegno la carta meoow
        deckP1[4].type = meoowDeck->type;
        strcpy(deckP1[4].desc, meoowDeck->desc);
        meoowDeck = removeNode(meoowDeck);
    }
    /*mazzo 2*/
    {
        for (i = 0; i < NORMAL_ASSIGNED_PERDECK; i++) {
            deckP2[i].type = otherDeck->type;
            strcpy(deckP2[i].desc, otherDeck->desc);
            otherDeck = removeNode(otherDeck);
        }
        //assegno la carta meoow
        deckP2[4].type = meoowDeck->type;
        strcpy(deckP2[4].desc, meoowDeck->desc);
        meoowDeck = removeNode(meoowDeck);
    }
    /*mazzo 3*/
    {
        for (i = 0; i < NORMAL_ASSIGNED_PERDECK; i++) {
            deckP3[i].type = otherDeck->type;
            strcpy(deckP3[i].desc, otherDeck->desc);
            otherDeck = removeNode(otherDeck);
        }
        //assegno la carta meoow
        deckP3[4].type = meoowDeck->type;
        strcpy(deckP3[4].desc, meoowDeck->desc);
        meoowDeck = removeNode(meoowDeck);
    }
    /*mazzo 4*/
    {
        for (i = 0; i < NORMAL_ASSIGNED_PERDECK; i++) {
            deckP4[i].type = otherDeck->type;
            strcpy(deckP4[i].desc, otherDeck->desc);
            otherDeck = removeNode(otherDeck);
        }
        //assegno la carta meoow
        deckP4[4].type = meoowDeck->type;
        strcpy(deckP4[4].desc, meoowDeck->desc);
        meoowDeck = removeNode(meoowDeck);
    }
    //inserisco la dimensione dei mazzi dei giocatori
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        deckPData[i] = INITIAL_DECK_DIMENSION;
    }
    //cancello il mazzo originale
    for (i = 0; i < nAllCards; i++) {
        deck = removeNode(deck);
    }
    //aggiorno i valori dimensioni carte del mazzo principale
    deckData[nOtherCards] = deckData[nOtherCards] - NORMAL_ASSIGNED;
    deckData[nMeoowCards] = deckData[nMeoowCards] - MEOOW_ASSIGNED;
    deckData[nAllCards] = deckData[nAllCards] - NORMAL_ASSIGNED - MEOOW_ASSIGNED;
    //creo un nuovo mazzo, nel mentre elimino i mazzi ausiliari
    newDeck = (card*)malloc(sizeof(card));
    aux = newDeck;
    while (otherDeck->next != NULL) {
        newDeck->type = otherDeck->type;
        strcpy(newDeck->desc, otherDeck->desc);
        newDeck->next = (card*)malloc(sizeof(card));
        newDeck = newDeck->next;
        otherDeck = removeNode(otherDeck);
    }
    while (meoowDeck->next != NULL) {
        newDeck->type = meoowDeck->type;
        strcpy(newDeck->desc, meoowDeck->desc);
        newDeck->next = (card*)malloc(sizeof(card));
        newDeck = newDeck->next;
        meoowDeck = removeNode(meoowDeck);
    }
    while (explodingDeck->next != NULL) {
        newDeck->type = explodingDeck->type;
        strcpy(newDeck->desc, explodingDeck->desc);
        newDeck->next = (card*)malloc(sizeof(card));
        newDeck = newDeck->next;
        explodingDeck = removeNode(explodingDeck);
    }
    newDeck->next = NULL; //chiudo il mazzo con un NULL
    //mescolo per l'ultima volta il mazzo
    newDeck = aux;
    newDeck = shuffleList(newDeck, deckData[nAllCards]);

    //restituisco il nuovo mazzo
    return newDeck;
}
card* shuffleList(card* oldList, int listDim) {
    int i = 0, k = 0; //contatori
    int randomNumber;
    int *randomArray = NULL;
    card* newList = NULL;
    card* nodeReset = NULL;
    card* aux = NULL;
    /*creo una lista di numeri non ripetuti*/
    {
        randomArray = (int*)malloc(listDim * sizeof(int));
        i = 0;
        while (i < listDim) {
            randomNumber = rand() % (listDim);
            for (k = 0; k < i; k++) {
                if (randomArray[k] == randomNumber)
                    break;
            }
            if (k == i)
                randomArray[i++] = randomNumber;
        }
    }
    //alloco il primo nodo della nuova lista
    newList = (card*)malloc(sizeof(card));
    //copio i puntatori per ripristinarli in seguito
    aux = newList;
    nodeReset = oldList;
    //inizio a creare una lista mescolata secondo l'array precedentemente generato
    for (i = 0; i < listDim; i++) {
        //scorro fino al nodo corretto
        for (k = 0; k < randomArray[i]; k++) {
            oldList = oldList->next;
        }
        //copio le informazioni
        newList->type = oldList->type;
        strcpy(newList->desc, oldList->desc);
        //alloco il nodo
        newList->next = (card*)malloc(sizeof(card));
        //sposto il puntatore della nuova lista al nodo successivo
        newList = newList->next;
        //riporto il puntatore del mazzo all'origine
        oldList = nodeReset;
    }
    //chiudo il mazzo facendo puntare a NULL
    newList->next = NULL;
    //riporto il puntatore al'inizio della lista
    newList = aux;
    //elimino la lista originale
    for (i = 0; i < listDim; i++) {
        oldList = removeNode(oldList);
    }
    //restituisco la lista mescolata
    return newList;
}
card* insertCardInDeck(cardArray * playerDeck, int position, card* deck) {
    //copio la nuova carta
    playerDeck[position].type = deck->type;//inserisco il tipo di carta (int)
    strcpy(playerDeck[position].desc, deck->desc);//inserisco la descrizione della carta (string)
    //stampo la carta pescata
    printf("\nHai pescato una %s, dice : %s", cardType[playerDeck[position].type], playerDeck[position].desc);
    return deck;
}

cardArray* removeCardFromPlayerDeck(int cardPosition, cardArray* playerDeck, int deckDim) {
    /*
    questa funzione si occupa di spostare le carte successiva a quella da rimuovere
    di una posizione verso l'alto (eliminando di fatto quella che vogliamo togliere)
    il mazzo *NON* viene ridimensionato
    */
    int i = 0;
    for (i = cardPosition; i < deckDim; i++) {
        playerDeck[i].type = playerDeck[i + 1].type;
        strcpy(playerDeck[i].desc, playerDeck[i + 1].desc);
    }
    return playerDeck;
}

//funzioni del logger
void logger(char logString[], int turn, _Bool printTurn, _Bool newLine) {
    /*
    Questa funzione registra uno storico della partita, segnando il turno e il relativo commento
    solitamente viene chiamata da altre funzioni dedicate
    Il vecchio log viene cancellato dalla funzione clearLog() all'apertura del programma
    poichè questa funzione non cancella il file ogni volta che viene chiamata
    */
    FILE *logFile = NULL;
    logFile = fopen("Log del gioco.txt", "r");
    if (logFile == NULL)
        logFile = fopen("Log del gioco.txt", "w+");
    else
        logFile = fopen("Log del gioco.txt", "a+");

    if (printTurn == true)
        fprintf(logFile, "Turno %d ", turn);
    fprintf(logFile, logString);
    if(newLine == true)
        fprintf(logFile, "\n");

    fclose(logFile);
}
void clearLog() {
    FILE *logFile = NULL;
    logFile = fopen("Log del gioco.txt", "r");
    if (logFile != NULL) {
        fclose(logFile);
        logFile = fopen("Log del gioco.txt", "w");
        fprintf(logFile, " ");
        fclose(logFile);
    }
}
void clearChar(char *string) {
    strcpy(string, "");
}
void logPickedCard(cardArray *deck, int deckPData[], int id, player *gamePlayers, int turn) {
    char tmpChar[LOGGER_CHAR_DIM];
    clearChar(tmpChar);
    strcat(tmpChar, gamePlayers[id].name);
    strcat(tmpChar, " ha pescato : ");
    strcat(tmpChar, cardType[deck[deckPData[id] - 1].type]);
    strcat(tmpChar, " ");
    strcat(tmpChar, deck[deckPData[id] - 1].desc);
    logger(tmpChar, turn, true, true);
}
void logPlayedCard(cardArray *deck, int id, player *gamePlayers, int turn, int cardPosition, _Bool newLine) {
    char tmpChar[LOGGER_CHAR_DIM];
    clearChar(tmpChar);
    strcat(tmpChar, gamePlayers[id].name);
    strcat(tmpChar, " ha giocato una  ");
    strcat(tmpChar, cardType[deck[cardPosition].type]);
    strcat(tmpChar, " che dice ");
    strcat(tmpChar, deck[cardPosition].desc);
    logger(tmpChar, turn, true, newLine);
}
void logRoundPlayerName(char *name, int round, _Bool firstRound) {
    char tmpChar[LOGGER_CHAR_DIM];
    clearChar(tmpChar);
    if(firstRound == true)
        strcat(tmpChar, "Il gioco inizia da : ");
    else
        strcat(tmpChar, "Turno di  : ");
    strcat(tmpChar, name);
    logger(tmpChar, round, true, true);
}
void logDeck(card *deck) {
#if DEBUG_RELEASE == true
    FILE *fp = NULL;
    fp = fopen("mazzomescolato.txt", "w");
    while (deck->next != NULL) {
        fprintf(fp, cardType[deck->type]);
        fprintf(fp, " ");
        fprintf(fp, deck->desc);
        fprintf(fp, "\n");
        deck = deck->next;
    }
    fclose(fp);
#endif
}
void logWinner(char *winnerName, int turn) {
    char tmpChar[LOGGER_CHAR_DIM];
    strcpy(tmpChar, winnerName);
    strcpy(tmpChar, " ha vinto la partita");
    logger(tmpChar, turn, true, true);

}
//caricamento
FILE* loadUser(player *gamePlayers, int id, FILE* saveFile) {
    //permette di caricare le informazioni del giocatore dal file binario
    player tmp[1];
    fread(tmp, sizeof(player), 1, saveFile);
    gamePlayers[id].alive = tmp->alive;
    strcpy(gamePlayers[id].name, tmp->name);
    return saveFile;
}
FILE* readDataAfterName(FILE *saveFile, int *deckPData) {
    //questa informazione legge il numero di carte del giocatore e il tipo
    //(che viene ignorato perchè questa versione non supporta giocatori di tipo computer)
    dataAfterName tmp[1];
    fread(tmp, sizeof(dataAfterName), 1, saveFile);
    *deckPData = tmp->cardLeftInPlayerDeck;
    return saveFile;
}
FILE* loadPlayerDeck(cardArray *deck, FILE* saveFile, int dim) {
    //carico il mazzo del giocatore
    fread(deck, sizeof(cardArray), dim, saveFile);
    return saveFile;
}
card* loadMainDeck(int *deckDim, FILE *saveFile) {
    //leggo la dimensione del mazzo
    //in seguito alloco un array dinamico della dimensione del mazzo
    //lo leggo dal file binario e poi lo copio in una lista concatenata
    cardArray *tmpDeck = NULL;
    card *head = NULL, *tmpNode = NULL;
    int i, dim[1];

    fread(dim, sizeof(int), 1, saveFile);
    tmpDeck = (cardArray*)malloc(dim[0] * sizeof(cardArray));
    deckDim[0] = dim[0];

    tmpNode = (card*)malloc(sizeof(card));
    head = tmpNode;
    fread(tmpDeck, sizeof(cardArray), dim[0], saveFile);

    for (i = 0; i < dim[0]; i++) {

        tmpNode->type = tmpDeck[i].type;
        strcpy(tmpNode->desc, tmpDeck[i].desc);

        tmpNode->next = (card*)malloc(sizeof(card));
        tmpNode = tmpNode->next;
    }

    return head;
}
void loadLastInfos(int *currentRound, _Bool *isAttacked, FILE* saveFile) {
    //leggo il turno e se il giocatore ha
    dataAtTheEnd tmpData[1];
    fread(tmpData, sizeof(dataAtTheEnd), 1, saveFile);
    *currentRound = tmpData[0].turn;
    *isAttacked = tmpData[0].haveAttacked;
}

//salvataggio
void saveProgress(card* deck, player *gamePlayers, int *deckPData, int currentRound, _Bool isAttacked, cardArray *deckP1, cardArray *deckP2, cardArray *deckP3, cardArray *deckP4) {
    /*
    Viene acquisito il nome del file e si controlla se esso sia gia presente, in caso lo sia viene chiesto se si vuole sovrascrivere
    altrimenti chiedo un nuovo nome.
    Dopodiche vengono chiamate le funzioni presenti più in avanti per salvare i vari dati
    */
    char fileName[FILENAME_MAX_DIM];
    char ext[] = ".sav";
    int choice;
    int id;
    FILE *saveFile = NULL;
    _Bool keepCycling = false;

    printf("\nInserisci nome del file di salvataggio (opzionalmente anche la posizione):\\> ");
    //gestisco la presenza o meno del file
    do {
        scanf("%s", fileName);
        getchar();
        strcat(fileName, ext); //inserisco l'estensione nel nome del file
        printf("\nControllo...");
        saveFile = fopen(fileName, "rb");
        if (saveFile != NULL) {
            //se il file esiste già chiediamo all'utente cosa intende fare
            printf("\nIl file esiste gia', vuoi sovrascriverlo?");
            printf("\n\t(1) Sovrascrivi\n\t(2) Scegli un nome diverso\nScelta:\\>");
            choice = choiceMenu(1, 2);
            if (choice == 1) {
                //se sceglie di sovrascrivere riapriamo il file in modalità scrittura
                fclose(saveFile);
                saveFile = fopen(fileName, "wb");
                keepCycling = false;
            } else {
                printf("\nInserisci un nuovo nome :\\> ");
                keepCycling = true;
            }
        } else
            saveFile = fopen(fileName, "wb");
    } while (keepCycling == true);

    //salvo i giocatori
    {
        id = 0;
        saveFile = savePlayer(saveFile, gamePlayers, id);
        if (gamePlayers[id].alive == true) {
            saveFile = saveDataAfterName(saveFile, deckPData, id);
            saveFile = writePlayerDeck(deckP1, saveFile, deckPData[id]);
        }
        id = 1;
        saveFile = savePlayer(saveFile, gamePlayers, id);
        if (gamePlayers[id].alive == true) {
            saveFile = saveDataAfterName(saveFile, deckPData, id);
            saveFile = writePlayerDeck(deckP2, saveFile, deckPData[id]);
        }
        id = 2;
        saveFile = savePlayer(saveFile, gamePlayers, id);
        if (gamePlayers[id].alive == true) {
            saveFile = saveDataAfterName(saveFile, deckPData, id);
            saveFile = writePlayerDeck(deckP3, saveFile, deckPData[id]);
        }
        id = 3;
        saveFile = savePlayer(saveFile, gamePlayers, id);
        if (gamePlayers[id].alive == true) {
            saveFile = saveDataAfterName(saveFile, deckPData, id);
            saveFile = writePlayerDeck(deckP4, saveFile, deckPData[id]);
        }
    }
    //salvo il mazzo
    saveFile = saveMainDeck(saveFile, deck);
    saveFile = saveLastInfos(currentRound, isAttacked, saveFile);
    //chiudo il file
    fclose(saveFile);
    printf("\nFile salvato");
    logger("Partita salvata", 0, false, true);
}
FILE* savePlayer(FILE *saveFile, player *currentPlayer, int id) {
    player tmp[1];
    strcpy(tmp[0].name, currentPlayer[id].name);
    tmp[0].alive = currentPlayer[id].alive;
    fwrite(tmp, sizeof(player), 1, saveFile);
    return saveFile;
}
FILE* writePlayerDeck(cardArray *deck, FILE* saveFile, int dim) {
    fwrite(deck, sizeof(cardArray), dim, saveFile);
    return saveFile;
}
FILE* saveDataAfterName(FILE *saveFile, int *deckPData, int id) {
    //questa informazione legge il numero di carte del giocatore
    dataAfterName tmp[1];
    tmp[0].cardLeftInPlayerDeck = deckPData[id];
    tmp[0].playerType = uman;
    fwrite(tmp, sizeof(dataAfterName), 1, saveFile);
    return saveFile;
}
FILE* saveMainDeck(FILE *saveFile,card *deck) {
    int i = 0, tmpInt[1];
    cardArray *tmp = NULL;
    card* tmpNode = deck;
    while (tmpNode->next != NULL) {
        i++;
        tmpNode = tmpNode->next;
    }
    tmpInt[0] = i;
    fwrite(tmpInt, sizeof(int), 1, saveFile);
    tmp = (cardArray*)malloc(tmpInt[0] * sizeof(cardArray));
    for (i = 0; i < tmpInt[0]; i++) {
        tmp[i].type = deck->type;
        strcpy(tmp[i].desc,deck->desc);
        deck = deck->next;
    }
    fwrite(tmp, sizeof(cardArray), tmpInt[0], saveFile);
    return saveFile;
}
FILE* saveLastInfos(int currentRound, _Bool haveAttacked, FILE* saveFile) {
    dataAtTheEnd tmpData[1];
    tmpData[0].haveAttacked = haveAttacked;
    tmpData[0].turn = currentRound;
    fwrite(tmpData, sizeof(dataAtTheEnd), 1, saveFile);
    return saveFile;
}
