#include<stdio.h>
#include<stdlib.h>
#include<time.h> 
#include<string.h>
#include<unistd.h> /*needed for sleep()*/

#define HAND_SIZE 5       /*number of consecutive in-memory pointers to pointers of Card structs in a Hand struct*/
#define DECK_SIZE 52      /*number of consecutive in-memory pointers to pointers of Card structs in a Deck struct*/
#define FIRSTCARD 0       /*index of the first card in a Deck struct*/
#define NUMPLAYERS 4      /*number of players in the game*/
#define HUMANPLAYER 0     /*holds the index in the players array that determines which is the human player*/
#define CLASS 8           /*number of classes: pair, two pair, three of a kind, straight, flush, full house, four of a kind, straight flush*/
#define RANKS 13          /*number of different cards (2 thru A)*/
#define COMBINATIONS 32   /*total possible different options of discarding a full hand of 5 cards. Monte Carlo simulation is run on all 32 options.*/
#define CARDVALUES 117    /*total number of possible values of a hand in a linear scale (117 = 13 ranks * 8 classes + 13 ranks)*/
#define FOLDED 1          /*a variable that means the player folded. intended usage is to set the "fold" variable in an instance of the Player struct to FOLDED*/ 
#define INITIALMONEY 100  /*starting amount of money for all players*/

/*defines a card. A card has one of four suits and 13 ranks (2-A)*/
typedef struct card{
  int suit;
  int rank;
}Card;

/*defines a hand. A hand holds HAND_SIZE number consecutive in-memory pointers to pointers to Card structs*/
typedef struct hand{
  Card **cards;  /*HAND_SIZE number of pointers pointed to by Card **cards.They are consecutive in memory, so we can iterate over them with array notation or pointer arithmetic.*/
  int cardsHeld; 
  int value;     /*value of the hand as calculated using linear scale (13*class+rank). Must run the function hand_value(Hand * hand) before this variable holds anything meanningful*/
  char *class;   /*string of the type of hand (Pair, Two Pair, etc). Must run the function hand_value(Hand *hand) on an instance of the hand struct first.*/
}Hand;

typedef struct deck{
  Card **cards;      /*DECK_SIZE number of pointers to pointed to by Card **cards. Consecutive in-memory, so we can iterate over them using array notation or pointer arithmetic.*/
  Card *topcard;     /*card at the top of the deck*/
  int topcardindex;
}Deck;

typedef struct player{
  Hand *hand;
  char *name;
  int money;         /*money the players has in their purse*/
  int bet;           /*holds the bet the player has put in during a round. reset at the end of every set*/
  int fold;          /* 0=didn't fold, 1=player folded*/
}Player;


//function prototypes for the Card struct
int card_toString(Card * aCardPtr);
void card_init(Card * aCardPtr, int suit, int rank);
void card_destroy(Card * aCardPtr);
int card_compare(const void * a, const void * b);

//function prototypes for the Deck struct
void deck_toString(Deck * aDeckPtr);
int dequeue_card(Deck * aDeckPtr, Card ** data);
int swap_card(Deck * aDeckPtr, int s, int t);
int deck_init(Deck * aDeckPtr);
void deck_fill(Deck * aDeckPtr);
void shuffle(Deck * aDeckPtr, int shuffles);
void deck_destroy(Deck * aDeckPtr);
int replace_from(Deck * aDeckPtr, Hand * aHandPtr, int * discards, int thrown);
void enqueue_deck(Deck * aDeckPtr);
int remove_card(Deck * aDeckPtr, Card * card);
int find_card(Deck * aDeckPtr, int suit, int rank);

//function prototypes for the Hand struct
int hand_init(Hand * aHandPtr);
int add_card(Hand * aHandPtr, Card * card);
int replace_card(Hand * aHandPtr, int index, Card *s);
int hand_toString(Hand * aHandPtr);
int hand_toString_ordered(Hand * aHandPtr);
void hand_destroy(Hand * aHandPtr);
void hand_sort(Hand * aHandPtr);
int hand_value(Hand * aHandPtr);
int hand_copy(Hand * t, Hand * s);

//function prototypes for the Player struct
int player_init(Player * aPlayer);
int player_toString(Player * aPlayer);
int player_reset_hand(Player * aPlayer);
void player_destroy(Player * aPlayer);

//function prototypes for the Monte Carlo advisor
int mc(Hand * origHand, int * maxdiscards, int * maxthrown, int * maxvalue, int sims);
int mc_card_combinations(Hand * origHand, int * discards, int start, int thrown, int level, int sims, int * maxvalue, int * maxdiscards, int * maxthrown);
int mc_expectedvalue(Hand * origHand, int * discards, int thrown, int draws);

//helper function prototypes for the main game loop (pokergame.c)
int check_input(int adiscard, int * maxthrown, int * maxdiscards);
int get_max_incremental_bet(Player * player, int expectedvalue, int maxcumulbet);
int get_incremental_bet(Player * player, int currentbet);

