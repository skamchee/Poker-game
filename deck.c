#include"pokergame.h"

#define cards aDeckPtr->cards

int deck_init(Deck * aDeckPtr){
  /*initialize a deck.
mallocs DECK_SIZE number of pointers, all consecutive in memory, each pointing to an individual card struct. The card structs themselves may or may not be consecutive in memory, but we don't care. We will traverse the consecutive set of DECK_SIZE number of pointers to manipulate the card structs in stead. 
 */
  int card=FIRSTCARD;
  if((cards =(Card **)malloc(sizeof(Card*)*DECK_SIZE))==NULL)
    return -1;  
  /*malloc empty cards*/
  for(card=FIRSTCARD;card<DECK_SIZE;card++){
    if((cards[card]=(Card*)malloc(sizeof(Card)))==NULL)
	return -1;
  }

  /*set the top of the deck*/
  aDeckPtr->topcardindex = FIRSTCARD;
  return 0;
}

//fill up the deck with cards that have value (suits and ranks)
void deck_fill(Deck * aDeckPtr){
  int rank;
  int suit;
  int card=FIRSTCARD;
  for(rank=2;rank<15;rank++){
    for(suit=1;suit<5;suit++){
      card_init(cards[card],suit,rank);
      card += 1;
    }
  }
}

//print everything in the deck, starting at the topcard
void deck_toString(Deck * aDeckPtr){
  int i;
  for(i=aDeckPtr->topcardindex;i<DECK_SIZE;i++)
    card_toString(cards[i]);
}

//take the top card off the deck
int dequeue_card(Deck * aDeckPtr, Card ** data){
  if(aDeckPtr->topcardindex==DECK_SIZE)
    return -1;/*dequeue fails because there are no more cards*/
  *data = cards[aDeckPtr->topcardindex];
  aDeckPtr->topcardindex++;
  return 0;
}

//take a specific card out of the deck
int remove_card(Deck *aDeckPtr, Card * card){
  int index = find_card(aDeckPtr,card->suit,card->rank);
  swap_card(aDeckPtr,index,aDeckPtr->topcardindex);
  aDeckPtr->topcardindex++;
  return 0;
}

//return the index of the card, and -1 if not found
int find_card(Deck * aDeckPtr, int suit, int rank){
  int i=0;
  for(i=aDeckPtr->topcardindex;i<DECK_SIZE;i++){
    if(cards[i]->rank == rank && cards[i]->suit == suit)
      return i;
  }
  return -1;
}

/*Swap pointers to two cards structs*/
int swap_card(Deck * aDeckPtr, int s, int t){
  Card ** tmp=(Card **)malloc(sizeof(Card*));
  if(tmp==NULL)
    return -1;
  *tmp = cards[t];
  cards[t] = cards[s];
  cards[s]=*tmp;
  free(tmp);
  return 0;
}

//enqueue all cards back onto the deck
void enqueue_deck(Deck * aDeckPtr){
  aDeckPtr->topcardindex = FIRSTCARD;
}

//randomize the cards in the deck. only works on a full deck. not to be used on an deck with cards already dealt
void shuffle(Deck * aDeckPtr, int shuffles){
  int a;
  int b;
  int i;
  //srand(time(NULL));
  a =rand()%DECK_SIZE;
  //srand(time(NULL)+a);
  b =rand()%DECK_SIZE;
  for(i=0;i<shuffles;i++){
    swap_card(aDeckPtr,a,b);
    //srand(a+i+b);
    a=rand()%DECK_SIZE;
    //srand(b-i+a);
    b=rand()%DECK_SIZE;
  }
}

//destroy the cards underlying the deck and the deck itself
void deck_destroy(Deck * aDeckPtr){
  int card;
  for(card=FIRSTCARD;card<DECK_SIZE;card++){
    card_destroy(cards[card]);
  }
  free(cards);
  free(aDeckPtr);
}
