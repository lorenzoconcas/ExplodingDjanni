#include "libs.h"

int main() {
    srand((unsigned int)time(NULL)); //inizializzo il seed della funzione rand

    /*variabili*/
    char cardType[][17] = { "EXPLODING DJANNI","MEOOOW", "SHUFFLE", "NOPE","SEE THE FUTURE", "ATTACK", "SKIP", "FAVOR", "DJANNI CARD" };
    FILE *deckFile = NULL; //puntatore a file (del mazzo)

    card *deck = NULL; //primo nodo del mazzo

    cardArray *deckP1 = NULL, *deckP2 = NULL, *deckP3 = NULL, *deckP4 = NULL; //mazzi dei giocatori
    cardArray* deckPlayer = NULL; //mazzo del giocatore corrente

    player gamePlayers[NUMBER_OF_PLAYERS]; //array contenente nomi dei giocatori e se sono vivi o meno

    int deckPData[4]; //dimensioni dei mazzi dei giocatori
    int deckData[4]; //dimensioni del mazzo principale
    //  [0] dimensione totale
    //  [1] numero exploding
    //  [2] numero meoow
    //  [3] numero altro tipo
    int currentRound = 0; //turno del giocatore
    int currentCard = 0;
    int choice = 0; //serve a gestire la scelta dell'utente
    int i, tmp;
    int globalTurn = 0;
    int sameDjannis = 0;
    int victim;
    int robbedCard;
    int attackedRound = 0;

    _Bool firstLoop = true; //stabilisco se è la prima volta che si entra nel ciclo di gioco
    _Bool keepPlaying = true;
    _Bool removeCard = true; //consente la rimozione della carta dal mazzo, solitamente è sempre rimossa tranne in alcuni casi
    _Bool isAttacked = false;
    _Bool isNopped;
    _Bool skipped = false;

    char anyKey;

    //variabili utilizzate per cricamento/salvataggio file
    FILE *saveFile = NULL; //file di salvataggio
    char filePosition[FILENAME_MAX_DIM]; //posizione file di salvataggio
    char ext[] = ".sav"; //estensone file di salvataggio

//menu principale
    clearLog(); //pulisco il log del gioco (nome file : "Log del gioco.txt"
    printWelcome(EXPANDED); //stampo il benvenuto
    logger("Exploding Djanni", 0, false, true);
    do {
        printMainOptions(); //stampo le opzioni del gioco
        choice = choiceMenu(MAIN_OPTIONS_MIN, MAIN_OPTIONS_MAX);
        switch (choice) {
        //caso nuova partita
        case 1: {
            /*alloco i mazzi dei giocatori*/
            deckP1 = (cardArray*)malloc(INITIAL_DECK_DIMENSION * sizeof(cardArray));
            deckP2 = (cardArray*)malloc(INITIAL_DECK_DIMENSION * sizeof(cardArray));
            deckP3 = (cardArray*)malloc(INITIAL_DECK_DIMENSION * sizeof(cardArray));
            deckP4 = (cardArray*)malloc(INITIAL_DECK_DIMENSION * sizeof(cardArray));

            clearUI();
            printWelcome(EXPANDED);
            //chiedo al giocatore quale mazzo voglia utilizzare
            printDifficulty();
            deckFile = chooseDeck(choiceMenu(DECKFILE_OPTION_MIN, DECKFILE_OPTION_MAX)); //chiamo la funzione seleziona mazzo

            /*alla funzione seguente passo il file del mazzo, il primo nodo di quello che sarà il mazzo
            e il puntatore ad intero che conterrà le informazioni sulla grandezza del mazzo*/
            deck = (card*)malloc(sizeof(card));
            deck = loadDeck(deckFile, deck, deckData); //carico il mazzo selezionato in una lista concatenata
            acquirePlayers(gamePlayers); //acquisisco i nomi dei giocatori
            deck = shuffleAssign(deck, deckP1, deckP2, deckP3, deckP4, deckData, deckPData); //mescolo il mazzo e assegno le carte ai giocatori

            logger("Il mazzo e' stato mescolato", 0, true, true);

            currentRound = rand() % (NUMBER_OF_PLAYERS); //estraggo a sorte il primo giocatore

            logRoundPlayerName(gamePlayers[currentRound].name, globalTurn, true);

            printf("\nIl gioco inizia da %s, premi il tasto INVIO per iniziare", gamePlayers[currentRound].name);
            scanf("%c", &anyKey);

            logger("Il gioco prende il via!", 0, true, true);
            break;
        }
        //caso caricamento
        case 2: {
            printf("\nInserisci nome del salvataggio (senza estensione, max 4096 caratteri)");
            printf("\nNome file :\\> ");
            scanf("%s", filePosition);
            getchar();
            strcat(filePosition, ext);
            saveFile = fopen(filePosition, "rb");
            if (saveFile == NULL) {
                printf("\nQualcosa e' andato storto, controlla che il file sia presente o che tu abbia abbastanza memoria");
                pause();
                return 0;
            } else {
                printf("\nCarico il file...");
                /*giocatore 1*/ {
                    i = 0;
                    saveFile = loadUser(gamePlayers, i, saveFile);
                    if (gamePlayers[i].alive == true) {
                        saveFile = readDataAfterName(saveFile, &deckPData[i]);
                        deckP1 = (cardArray*)malloc(deckPData[i] * sizeof(cardArray));
                        saveFile = loadPlayerDeck(deckP1, saveFile, deckPData[i]);

                    }
                }
                /*giocatore 2*/ {
                    i = 1;
                    saveFile = loadUser(gamePlayers, i, saveFile);
                    if (gamePlayers[i].alive == true) {
                        saveFile = readDataAfterName(saveFile, &deckPData[i]);
                        deckP2 = (cardArray*)malloc(deckPData[i] * sizeof(cardArray));
                        saveFile = loadPlayerDeck(deckP2, saveFile, deckPData[i]);

                    }
                }
                /*giocatore 3*/ {
                    i = 2;
                    saveFile = loadUser(gamePlayers, i, saveFile);
                    if (gamePlayers[i].alive == true) {
                        saveFile = readDataAfterName(saveFile, &deckPData[i]);
                        deckP3 = (cardArray*)malloc(deckPData[i] * sizeof(cardArray));
                        saveFile = loadPlayerDeck(deckP3, saveFile, deckPData[i]);

                    }
                }
                /*giocatore 4*/ {
                    i = 3;
                    saveFile = loadUser(gamePlayers, i, saveFile);
                    if (gamePlayers[i].alive == true) {
                        saveFile = readDataAfterName(saveFile, &deckPData[i]);
                        deckP4 = (cardArray*)malloc(deckPData[i] * sizeof(cardArray));
                        saveFile = loadPlayerDeck(deckP4, saveFile, deckPData[i]);
                    }
                }

                deck = loadMainDeck(&deckData, saveFile);
                loadLastInfos(&currentRound, &isAttacked, saveFile);
                printf("\nSalvataggio caricato con successo!");
                logger("La partita viene caricata dal file : ", globalTurn, false, false);
                logger(filePosition, globalTurn, false, true);
                pause();
            }
            break;
        }
        case 3: {
            printRules(0);
            choice = 5;
            break;
        }
        case 4: {
            logger("Il gioco viene chiuso", 0, false, true);
            printf("\nHai deciso di uscire");
            return 0;
        }
        }
    } while (choice < 1 || choice > 4);
#if DEBUG_RELEASE == true
    logDeck(deck);
#endif
    //sezione gioco
    //questo ciclo viene ripetuto finchè ci sono almeno due giocatori vivi
    do {
        //incremento il turno e salvo nel log il nome del giocatore
        globalTurn++;
        keepPlaying = true;
        //calcolo il turno successivo, se è il primo turno non calcolo il turno in quanto già estratto prima
        //controllo se il giocatore è sotto attacco
        if (firstLoop == true)
            firstLoop = false;
        else {
            if (isAttacked == false)
                currentRound = whosNext(gamePlayers, currentRound);
            else //isAttacked == true
                if (attackedRound == 0) {
                    attackedRound++;
                    currentRound = whosNext(gamePlayers, currentRound);
                } else {
                    attackedRound = 0;
                    isAttacked = false;
                }

        }
        //questo switch viene utilizzato per scegliere su quale deck si devono fare le operazioni (migliorare il commento)
        switch (currentRound) {
        case 0: {
            deckPlayer = deckP1;
            break;
        }
        case 1: {
            deckPlayer = deckP2;
            break;
        }
        case 2: {
            deckPlayer = deckP3;
            break;
        }
        case 3: {
            deckPlayer = deckP4;
            break;
        }
        }
        //Pulisco lo schermo e stampo il benvenuto in versione ridotta in seguito stampo quale giocatore ha il turno
        clearUI();
        printWelcome(MINI);
        printf("\nTurno di %s", gamePlayers[currentRound].name);
        logRoundPlayerName(gamePlayers[currentRound].name, globalTurn, false);
        //mostro le opzioni al giocatore
        printPlayOptions();
        choice = choiceMenu(1, 3);
        switch (choice) {
        //il giocatore decide di giocare le carte
        case 1: { //il giocatore scegle di giocare le carte
            currentCard = 0;  //impostiamo il contatore a 0 cosi iniziamo dalla prima carta
            //le carte vengonono giocate qui
            do {
                if (choice == 1) {
                    printf("\nEcco le tue carte\n");
                    printCardsOfPlayer(deckPlayer, deckPData[currentRound]);
                }
                printf("\nCarta corrente : (%d) %s %s", currentCard + 1, cardType[deckPlayer[currentCard].type], deckPlayer[currentCard].desc);
                printInGameOptions();
                choice = choiceMenu(1, 4);
                //gioca la carta
                if (choice == 1) {
                    //se la carta è di tipo exploding o meoow la nope non può essere giocata quindi controlliamo prima quei due casi
                    switch (deckPlayer[currentCard].type) {
                    case EXPLODING_DJANNI: {
                        //la carta non è giocabile,
                        //è un caso che non capiterà mai perchè la carta viene gestita al momento in cui viene pescata
                        currentCard++; //passiamo alla carta successiva
                        break;
                    }
                    case MEOOOW: {
                        //non permettiamo di giocare una carta meoow senza che sia stata prima pescata una explodingDjanni
                        //quindi passiamo alla carta successiva
                        printf("\nQuesta carta non puo' essere giocata senza prima pescare una Exploding Djanni");
                        currentCard++;
                        removeCard = false;
                        break;
                    }
                    case NOPE: {
                        printf("\nVuoi giocare la carta? Non avra' alcun effetto utile");
                        printf("\n\t(1) Gioca\n\t(2) Prossima Carta\nScelta:\\>");
                        choice = choiceMenu(1, 2);
                        //se la carta verrà giocata (choice == 1) verrà semplicemente tolta dal mazzo come con le altre
                        if (choice == 2) {
                            currentCard++;
                            removeCard = false;
                        } else
                            removeCard = true;
                        break;
                    }
                    //queste carte sono annullabili dalla nope
                    //per ogni carta seguente viene prima controllato se qualcuno vuole giocare una nope
                    case SHUFFLE: {
                        isNopped = playNope(currentRound, gamePlayers, deckP1, deckP2, deckP3, deckP4, &deckPData);
                        //mescolo le carte dal mazzo (usando la stessa funzione usata per mescolare il mazzo all'inizio)
                        if (isNopped == false) {
                            deck = shuffleList(deck, deckData[nAllCards]);
                            logPlayedCard(deck, currentRound, gamePlayers, globalTurn, currentCard, true);
                        } else {
                            logPlayedCard(deckPlayer, currentRound, gamePlayers, globalTurn, currentCard, false);
                            logger(", ma viene noppato", globalTurn, false, true);
                        }
                        break;
                    }
                    case SEE_THE_FUTURE: {
                        isNopped = playNope(currentRound, gamePlayers, deckP1, deckP2, deckP3, deckP4, &deckPData);
                        //stampo le prime tre carte del mazzo
                        if (isNopped == false) {
                            logPlayedCard(deckPlayer, currentRound, gamePlayers, globalTurn, currentCard, true);
                            printf("\nEcco le prime tre carte del mazzo\n");
                            printMainDeck(deck, MAGIC_NUMBER);
                            pause();
                        } else {
                            logPlayedCard(deckPlayer, currentRound, gamePlayers, globalTurn, currentCard, false);
                            logger(", ma viene noppato", globalTurn, false, true);
                        }
                        break;
                    }
                    case SKIP: {
                        //se si ha il via libera, setto skipped a true ( e la carta non verrà pescata alla fine del turno)
                        isNopped = playNope(currentRound, gamePlayers, deckP1, deckP2, deckP3, deckP4, &deckPData);
                        if (isNopped == false)
                            skipped = true;
                        else {
                            logPlayedCard(deckPlayer, currentRound, gamePlayers, globalTurn, currentCard, false);
                            logger(", ma viene noppato", globalTurn, false, true);
                        }
                        break;
                    }
                    case DJANNI_CARD: {
                        /*controllo che vi siano abbastanza Djanni Identiche
                        in caso favorevole controllo che nessuno voglia giocare una nope
                        se è libero di giocare le carte djanni,
                        se ha due djanni uguali faccio scegliere una vittima al giocatore,
                        se sono tre scelgo sia la vittima sia quale carta rubare,
                        se sono più di tre ne considero soloo 3
                        */
                        sameDjannis = checkSameCards(deckPlayer[currentCard].desc, deckPlayer, deckPData[currentRound]);
                        if (sameDjannis > 1) {
                            if (sameDjannis > 3)
                                sameDjannis = 3;
                            isNopped = playNope(currentRound, &gamePlayers, deckP1, deckP2, deckP3, deckP4, &deckPData);
                            if (isNopped == false) {
                                robbedCard = 0;
                                victim = chooseVictim(currentRound, gamePlayers);
                                if (sameDjannis == 2) {
                                    robbedCard = rand() % (deckPData[victim]);
                                } else if (sameDjannis == 3) {
                                    robbedCard = chooseGiftCard(true, victim, deckPData, deckP1, deckP2, deckP3, deckP4);
                                }
                                tmp = removeMultipleCards(deckPData[currentRound], deckPlayer, deckPlayer[currentCard].desc, currentCard, sameDjannis - 1);
                                deckPData[currentRound] = deckPData[currentRound] - sameDjannis +1;
                                transferCard(robbedCard, tmp-1, victim, deckPData, deckPlayer, deckP1, deckP2, deckP3, deckP4);
                                deckPData[victim]--;
                                removeCard = false;

                            } else {
                                logPlayedCard(deck, currentRound, gamePlayers, globalTurn, currentCard, false);
                                logger(", ma viene noppato", globalTurn, false, true);
                            }
                        } else {
                            printf("\nNon hai abbastanza carte identiche (devono essere almeno 2)\nVuoi giocarla?\n(1) Gioca\n(2) Cambia carta\nScelta :\\> ");
                            if (choiceMenu(1, 2) == 2) {
                                currentCard++;
                                removeCard = false;
                            }
                        }
                        break;
                    }
                    case ATTACK: {
                        isNopped = playNope(currentRound, gamePlayers, deckP1, deckP2, deckP3, deckP4, deckPData);
                        //setto il valore isAttacked su true cosi il prossimo giocatore dovrà fare due turni
                        if (isNopped == false) {
                            logPlayedCard(deckPlayer, currentRound, gamePlayers, globalTurn, currentCard, true);
                            if (attackedRound > 0)
                                attackedRound = 0;
                            isAttacked = true;
                        } else {
                            logPlayedCard(deckPlayer, currentRound, gamePlayers, globalTurn, currentCard, false);
                            logger(", ma viene noppato", globalTurn, false, true);
                        }
                        break;
                    }
                    case FAVOR: {
                        /*faccio scegliere al giocatore da chi farsi donare una carta
                        poi faccio scegliere alla vittima quale carta regalare*/
                        robbedCard = 0;
                        isNopped = playNope(currentRound, gamePlayers, deckP1, deckP2, deckP3, deckP4, deckPData);
                        if (isNopped == false) {
                            printf("\nLa carta regalata prendera' il posto di questa (posizione %d)", currentCard + 1);
                            victim = chooseVictim(currentRound, gamePlayers); //vittima da cui prendere la carta
                            clearUI();
                            printf("\nPassa il controllo a %s", gamePlayers[victim].name);
                            robbedCard = chooseGiftCard(false, victim, deckPData, deckP1, deckP2, deckP3, deckP4); //è la posizione della carta da trasferire
                            transferCard(robbedCard, currentCard, victim, deckPData, deckPlayer, deckP1, deckP2, deckP3, deckP4); //trasferisco la carta
                            clearUI();
                            deckPData[victim]--;
                            printf("%s ti ha regalato una carta %s che dice %s", gamePlayers[victim].name, cardType[deckPlayer[currentCard].type], deckPlayer[currentCard].desc);
                            removeCard = false;
                        }
                        break;
                    }
                    }
                    /*Tolgo la carta dal mazzo*/ {
                        if (removeCard == true) {
                            deckPlayer = removeCardFromPlayerDeck(currentCard, deckPlayer, deckPData[currentRound]); //la carta viene rimossa ma il mazzo non viene ridotto di dimensione
                            //agiamo direttamente sul mazzo originale per evitare errori
                            //ridimensiono il mazzo
                            deckPData[currentRound]--;
                            switch (currentRound) {
                            case 0: {
                                deckP1 = (cardArray*)realloc(deckP1, deckPData[currentRound] * sizeof(cardArray));
                                break;
                            }
                            case 1: {
                                deckP2 = (cardArray*)realloc(deckP2, deckPData[currentRound] * sizeof(cardArray));
                                break;
                            }
                            case 2: {
                                deckP3 = (cardArray*)realloc(deckP3, deckPData[currentRound] * sizeof(cardArray));
                                break;
                            }
                            case 3: {
                                deckP4 = (cardArray*)realloc(deckP4, deckPData[currentRound] * sizeof(cardArray));
                                break;
                            }
                            }
                            currentCard = 0;
                        } else
                            removeCard = true;
                    }
                    clearUI();
                }
                //usato per scorrere le carte
                else if (choice == 2) {
                    if (currentCard < deckPData[currentRound] - 1) { //passiamo alla carta successiva
                        currentCard++;
                    } else //se ci troviamo nell'ultima carta torniamo alla prima
                        currentCard = 0;
                }
                //chiudo il turno
                else if (choice == 3) {
                    keepPlaying = false;
                    break;
                }
                //salvo la partita
                else if (choice == 4) {
                    saveProgress(deck, gamePlayers, deckPData, currentRound, isAttacked, deckP1, deckP2, deckP3, deckP4);
                }

            } while (deckPData[currentRound] > 0 && keepPlaying == true);
            /*Ora che il turno è finito procedo a pescare una carta*/
            if (skipped == false) {
                deckPData[currentRound]++; //riduco la dimensione del mazzo
                //realloco il mazzo
                switch (currentRound) {
                case 0: {
                    deckP1 = (cardArray*)realloc(deckP1, deckPData[currentRound] * sizeof(cardArray));
                    deckPlayer = deckP1;
                    break;
                }
                case 1: {
                    deckP2 = (cardArray*)realloc(deckP2, deckPData[currentRound] * sizeof(cardArray));
                    deckPlayer = deckP2;
                    break;
                }
                case 2: {
                    deckP3 = (cardArray*)realloc(deckP3, deckPData[currentRound] * sizeof(cardArray));
                    deckPlayer = deckP3;
                    break;
                }
                case 3: {
                    deckP4 = (cardArray*)realloc(deckP4, deckPData[currentRound] * sizeof(cardArray));
                    deckPlayer = deckP4;
                    break;
                }
                }
                //inserisco la carta
                insertCardInDeck(deckPlayer, deckPData[currentRound] - 1, deck);
                //loggo quale carta ho preso
                logPickedCard(deckPlayer, deckPData, currentRound, gamePlayers, globalTurn);
                //rimuovo la carta dal mazzo principale
                deck = removeNode(deck);
                deckData[nAllCards]--;
            } else {
                skipped = false;
                logger("Il giocatore non pesca perche' ha usato una SKIP", globalTurn, true, true);
            }
            //controllo che la carta pescata non sia una exploding djanni
            if (deckPlayer[deckPData[currentRound] - 1].type == EXPLODING_DJANNI) {
                if (pickedExploding(deckPlayer, deck, &deckPData, currentRound) == false) {
                    printf("\n%s e' morto", gamePlayers[currentRound].name);
                    free(deckPlayer);
                    gamePlayers[currentRound].alive = false;
                } else {
                    printf("\nTi sei salvato");
                    logger("Il giocatore si salva con una meoow", globalTurn, true, true);
                    deckData[nAllCards]++; //incremento la dimensione del mazzo principale
                    logDeck(deck);
                    printCards(deck);
                }
            }

            printf("\nIl tuo turno e' finito");
            logger("Il turno e' finito", globalTurn, true, true);
            pause();
            break;
        }
        //il giocatore sceglie di chiudere il gioco
        case 2: {
            logger("Il gioco viene chiuso", globalTurn, false, true);
            return 0;
        }
        //mostra le regole
        case 3: {
            printRules(1);
            break;
        }
        }

    } while (getAlivePlayersNumber(gamePlayers) > 1);
    //stampo il giocatore e lo salvo nel logger
    printWinner(gamePlayers, globalTurn);
    printf("\nGioco finito");
    logger("Il gioco e' finito", globalTurn, true, true);
    pause();
    return 0;
}
