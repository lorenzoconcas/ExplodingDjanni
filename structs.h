#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#define INITIAL_DECK_DIMENSION 5
#define DEBUG_RELEASE false
#define autoRes true
#define MIN_RAND 0
#define NORMAL_ASSIGNED 16
#define MEOOW_ASSIGNED 4
#define NORMAL_ASSIGNED_PERDECK 4
#define NUMBER_OF_PLAYERS 4
#define NAME_OF_PLAYERS_DIM 24
#define CARD_DESCRIPTION_DIM 128
#define FILENAME_MAX_DIM 4096
#define MAGIC_NUMBER 3
#define LOGGER_CHAR_DIM 512
//valori accettati dai menu usati chiamando choiceMenu()

#define MAIN_OPTIONS_MAX 4
#define MAIN_OPTIONS_MIN 1
#define PLAY_OPTIONS_MAX 4
#define PLAY_OPTIONS_MIN 1
#define DECKFILE_OPTION_MAX 4
#define DECKFILE_OPTION_MIN 1


typedef enum { EXPLODING_DJANNI, MEOOOW, SHUFFLE, NOPE, SEE_THE_FUTURE, ATTACK, SKIP, FAVOR, DJANNI_CARD } cardEnum;
typedef enum { DEBUG_LOG, GAMEPLAY_LOG } logType;
typedef enum { nAllCards, nExplodingCards, nMeoowCards, nOtherCards } deckInfoType;
typedef enum { NONE, EASY, MEDIUM, HARD, CUSTOM} deckDifficulty;
typedef enum { EXPANDED, MINI} welcomeType;

typedef struct cardNode {
    cardEnum type;
    char desc[CARD_DESCRIPTION_DIM];
    struct cardNode *next;
} card;

typedef struct {
    char desc[CARD_DESCRIPTION_DIM];
    int type;
} cardArray;

typedef struct {
    char name[NAME_OF_PLAYERS_DIM];
    _Bool alive;
} player;

//strutture utili alla lettura/scrittura del file di salvataggio

typedef enum { computer, uman } playerKind;

typedef struct {
    int cardLeftInPlayerDeck;
    playerKind playerType;
} dataAfterName;

typedef struct {
    int turn;
    _Bool haveAttacked;
} dataAtTheEnd;

#endif // !STRUCTS_H_INCLUDED
