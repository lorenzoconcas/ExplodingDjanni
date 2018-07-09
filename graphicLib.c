#include "libs.h"

void clearUI() {
#if DEBUG_RELEASE == false
#ifdef _WIN32
    system("cls");
#else
    system("clear")e
#endif // _WIN32
#endif

}

void printWelcome(int mode) {
    clearUI();
    switch (mode) {
    case EXPANDED: {
        setRes(0);

        printf("=============================================================\n");
        printf("||                                                         ||\n");
        printf("||              Exploding Djanni - Il Gioco                ||\n");
        printf("||                                                         ||\n");
        printf("=============================================================\n");

        break;
    }

    case MINI: {
        setRes(1);
        printf("ExplodingDjanni");
    }
    }

}

void printSeparator() {
    printf("============================================================");
}

void printMainOptions() {
    printf("\nScegli una delle seguenti azioni : ");
    printf("\n(1) = Nuova partita\n    Inizia una nuova partita e regala del cibo a Djanni\n");
    printf("\n(2) = Carica Partita\n    Consente di caricare una partita in corso\n");
    printf("\n(3) = Informazioni\n    Mostra le regole del gioco\n");
    printf("\n(4) = Esci\n    Uscita, prova a digitare 4");
    printf("\nScelta:\\> ");
}

void printDifficulty() {
    printf("\nScegli fra 3 difficolta'\nDigita uno dei seguenti numeri\n");
    printf("\n(1) = Facile\n\tAdatto a chi usa il gioco per la prima volta\n");
    printf("\n(2) = Media\n\tQuando il gioco si fa duro, Djanni inizia a miagolare\n");
    printf("\n(3) = Difficile\n\tSembra sia piu' facile laurearsi che vincere questo gioco\n");
    printf("\n(4) = Personalizzato\n\tCarica un mazzo creato da te\n");
    printf("\nScelta:\\> ");

}

void printPlayOptions() {
    printf("\n");
    printf("\nOpzioni");
    printf("\n(1) = Gioca Carta/e\n");
    printf("\n(2) = Esci dal gioco\n       Chiude il gioco (perdendo tutti i progressi non salvati)\n");
    printf("\n(3) = Info Gioco\n       Mostra le regole del gioco e altre informazioni\n");
    printf("Scelta:\\> ");
}



void printRules(int type) {
    FILE *fp = fopen("help1.txt", "r");
    char a[128];
    clearUI();
    printWelcome(type);
    setRes(2);
    while (fgets(a, 128, fp) != NULL)
        printf("%s", a);
    printf("\nPremi (1) Per visualizzare le informazioni sulle carte, altrimenti (2) per uscire\nScelta :\\> ");
    if (choiceMenu(1, 2) == 1) {
        clearUI();
        printWelcome(MINI);
        setRes(3);
        fclose(fp);
        fp = fopen("help2.txt", "r");
        while (fgets(a, 128, fp) != NULL)
            printf("%s", a);
        pause();
    }
    setRes(type);
    printWelcome(type);
}

void printInGameOptions() {
    printf("\nOpzioni : ");
    printf("\n(1) Gioca la carta");
    printf("\n(2) Passa alla Carta Successiva");
    printf("\n(3) Pesca e chiudi il turno");
    printf("\n(4) Salva partita");
    printf("\nScelta:\\> ");
}

void printLoadCustomDeckGuide() {
    printf("\nInserisci il nome del file del mazzo (max %d caratteri), l'estensione e' aggiunta automaticamente (.txt)", FILENAME_MAX_DIM);
    printf("\nSintassi:");
#if _WIN32
    printf("\n\tSe il mazzo si trova nella cartella dell'eseguibile digitare solo il nome del file");
    printf("\n\tSe il mazzo si trova in una diversa posizione specificare l'intero percorso");
    printf("\n\tEs : C:\\Cartella\\nomefile");
#else
    printf("\nUtenti macOS :");
    printf("\nPosizionare il mazzo nella carta /users/nomeutente/ e inserire il nome del file");
    printf("\nUtenti Linux");
    printf("\nNon lo so");
#endif
    printf("\nNome:\\> ");
}

void printAlivePlayers(player* gamePlayers) {
    int i = 0;

    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (gamePlayers[i].alive == true) {
            printf("\n(%d) %s, (Giocatore N (%d))", i + 1, gamePlayers[i].name, i + 1);
        }
    }
    printf("\n");
}


void printOtherPlayers(player* gamePlayers, int exclude) {
    int i = 0;
    for (i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (i != exclude) { //se il non è il giocatore da escludere procedi a controllare se sia vivo
            if(gamePlayers[i].alive == true) //se il giocatore è vivo stampa il suo nome
                printf("\n(%d) %s, (Giocatore N (%d))", i + 1, gamePlayers[i].name, i + 1);
        }
    }
    printf("\n");
}

void pause() {
    char anyKey;
    printf("\nPremi INVIO per continuare...");
    scanf("%c", &anyKey);
}

void setRes(int type) {
#if _WIN32
#if autoRes == true
    switch (type) {
    case 0:
        system("mode con cols=62 lines=34");
        break;
    case 1:
        system("mode con cols=121 lines=34");
        break;
    case 2:
        system("mode con cols=125 lines=34");
        break;
    case 3:
        system("mode con cols=173 lines=34");
        break;
    case 4:
        system("mode con cols=53 lines=43");
        break;
    }
#endif
#endif
}

void printWinner(player *gamePlayers, int turn) {
    int i;
    clearUI();
    setRes(4);
    {
        printf("                   .............");
        printf("\n            oooOO88#######88##8#Oooo:o.");
        printf("\n      O8Oo  OOoooO8#############Oo::oO. :OOO.");
        printf("\n     O8: :ooOOoooO##############Oo::o8oo:..O8.");
        printf("\n    .8o     o8oooO##############Oo::OO     :8:");
        printf("\n    :8o     :8oooO#############8Oo::Oo     .8o");
        printf("\n    :8o      8OooO#############8Oo:o8:     :8o");
        printf("\n    .88      8OooO8############8o::o#      O8.");
        printf("\n     o8:     o8ooO8############Oo::OO     .8O");
        printf("\n     .8O     .8ooO8############Oo::8:     O8.");
        printf("\n      :8o     OOooO8##########8Oo:o8     :8o");
        printf("\n       :8o    :8ooO8##########Oo::Oo    :8o");
        printf("\n        .8O.   8OoOO##########Oo:o8    o8:");
        printf("\n         .OO:  .8OoO8########8O::Oo  .OO.");
        printf("\n           :OO. O8oO8########8o:oO  o8o");
        printf("\n             oO: 8OOO8######8Oo:O..OO.");
        printf("\n              .oOo8OO88#####8o:oooO.");
        printf("\n                .O88O88####8OooOO:");
        printf("\n                  :88O8####8ooOo");
        printf("\n                   :8O8####8oOo");
        printf("\n                   ..88####88.:");
        printf("\n                     :88###8o");
        printf("\n                      O8##8O");
        printf("\n                      :8#8O:");
        printf("\n                      oO##oo");
        printf("\n                      :O88o:");
        printf("\n                      :O88o:");
        printf("\n                      OO##oo");
        printf("\n                     OO8##8oO");
        printf("\n                    .O8###8Oo:");
        printf("\n                    OO8####8oo");
        printf("\n                  .oO8#####8o:o.");
        printf("\n                  oO88#####8Oo:O");
        printf("\n                 :OO88#####8Oo:Oo");
        printf("\n                 OOO8######8Oo::8.");
        printf("\n                oOO88######8Ooooo8");
        printf("\n                OOO88######8OOo:oO");
        printf("\n                .::::ooooo:o::....");
    }
    for (i = 0; i < 4; i++) {
        if (gamePlayers[i].alive == true) {
            printf("\nComplimenti %s, hai vinto la partita!", gamePlayers[i].name);
            logWinner(gamePlayers[i].name, turn);
        }
    }

}
