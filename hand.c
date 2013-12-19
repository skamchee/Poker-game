#include "pokergame.h"

/*classes range from 0-8*/
enum {HIGHCARD,PAIR,TWOPAIR,THREEK,STRAIGHT,FLUSH,FULLHOUSE,FOURK,SFLUSH} classes;

int hand_init(Hand * aHandPtr){
  int i=0;
  if((aHandPtr->cards = (Card **)malloc(sizeof(Card*)*HAND_SIZE))==NULL)
    return -1;
  //malloc empty cards
  for(i=0;i<HAND_SIZE;i++){
    if((aHandPtr->cards[i]=(Card*)malloc(sizeof(Card)))==NULL)
      return -1;
  }
  aHandPtr->cardsHeld=0;

  //hand_value must be run in order for these to have anything meaningful
  aHandPtr->value=0;
  aHandPtr->class = (char*)malloc(sizeof(char)*20);
  strcpy(aHandPtr->class,"123456789012345678");
  return 0;
}

int add_card(Hand * aHandPtr, Card * card){
  if(aHandPtr->cardsHeld==HAND_SIZE)//hand is full
    return -1;
  aHandPtr->cards[aHandPtr->cardsHeld]=card;
  aHandPtr->cardsHeld++;
  return 0;
}

int replace_card(Hand * t, int index_t, Card * s){
  if(index_t>HAND_SIZE || t == NULL || s ==NULL)
    return -1;
  t->cards[index_t]=s;
  return 0;
}

/*print the cards in a hand*/
int hand_toString(Hand * aHandPtr){
  int i;
  if(aHandPtr==NULL)
    return -1;
  for(i=0;i<aHandPtr->cardsHeld;i++)
    card_toString(aHandPtr->cards[i]);
  return 0;
}

/*print the cards in a hand like an ordered list*/
int hand_toString_ordered(Hand * aHandPtr){
  int i;
  if(aHandPtr==NULL)
    return -1;
  for(i=0;i<aHandPtr->cardsHeld;i++){
    printf("[%d] ",i+1);
    card_toString(aHandPtr->cards[i]);
  }
  return 0;
}

int hand_copy(Hand * t, Hand * s){
  int i=0;
  int x=0;
  if(t==NULL || s==NULL)
    return -1;
  for(i=0;i<HAND_SIZE;i++){
    if((x = replace_card(t,i,s->cards[i]))==-1)
      return -1;
  }
  t->cardsHeld = s->cardsHeld;
  return 0;
}

void hand_destroy(Hand * aHandPtr){
  free(aHandPtr->cards);
  free(aHandPtr);
}

/*make sure to run hand_sort before printing or using the monte carlo advisor on a player's hand:
  1. The Monte Carlo advisor returns the indexes of cards that you should discard. Sorting after running the Monte Carlo advisor would make the hand not correspond to the advisors results anymore*/

void hand_sort(Hand * aHandPtr){
  qsort(aHandPtr->cards,aHandPtr->cardsHeld,sizeof(Card*),card_compare);
}

/*Returns the value of the hand where value =13*class+rank. Note: the hand does not have to be sorted for this function */
int hand_value(Hand * aHandPtr){
  int i=0;
  int suits[5] = {0};//suits go from 1-4 in the card struct, so make array of size 5 with indices from 0-4
  int ranks[15] = {0};//ranks go from 2 to 14 in the card struct, so make array of size 15 with indices from 0-14
  int orderedcards=0;
  int straight=0;
  int flush=0;
  int threekind=0;
  int pair=0;

  int threek_highcard=0;
  int pair_highcard=0;
  int highcard=0;

  for(i=0;i<HAND_SIZE;i++){
    suits[aHandPtr->cards[i]->suit] +=1;
    ranks[aHandPtr->cards[i]->rank] +=1;
  }
  //find the highcard
  for(i=2;i<15;i++){
    if(ranks[i]>0 && i>highcard)
      highcard=i;
  }
  //look for a flush
  for(i=1;i<5;i++){
    if(suits[i]==5)
      flush=1;
  }
  //look for a straight
  for(i=2;i<14;i++){
    if(ranks[i]==1 && ranks[i+1]==1)
      orderedcards +=1;
    if(orderedcards==4){
      straight =1;
      break;
    }
  }
  
  if(flush==1 && straight==1){
    strcpy(aHandPtr->class,"Straight Flush");
    aHandPtr->value = 13*SFLUSH+highcard-1;//-1 because card->rank is on scale 2-14, but monte carlo uses ranks 1-13
    return aHandPtr->value;//straight flush
  }

  //look for four of a kind, three of a kind and pairs
  for(i=2;i<15;i++){
    if(ranks[i]==4){
      strcpy(aHandPtr->class,"Four of a Kind");
      aHandPtr->value = 13*FOURK+i-1;
      return aHandPtr->value;
    }   
    if(ranks[i]==3){
      threekind +=1;
      threek_highcard = i;
    }
    if(ranks[i]==2){
      pair +=1;
      if(i>pair_highcard)
	pair_highcard=i;
    }
  }

  if(threekind == 1 && pair == 1){
    strcpy(aHandPtr->class,"Full House");
    aHandPtr->value = 13*FULLHOUSE+threek_highcard-1;
    return aHandPtr->value;
  }
  else if(flush==1){
    strcpy(aHandPtr->class,"Flush");
    aHandPtr->value = 13*FLUSH+highcard-1;
    return aHandPtr->value;
  }
  else if(straight==1){
    strcpy(aHandPtr->class,"Straight");
    aHandPtr->value = 13*STRAIGHT+highcard-1;
    return aHandPtr->value;
  }
  else if(threekind==1){
    strcpy(aHandPtr->class,"Three of a Kind");
    aHandPtr->value = 13*THREEK+threek_highcard-1;
    return aHandPtr->value;
  }
  else if(pair==2){
    strcpy(aHandPtr->class,"Two Pair");
    aHandPtr->value =  13*TWOPAIR+pair_highcard-1;
    return aHandPtr->value;
  }
  else if(pair==1){
    strcpy(aHandPtr->class,"Pair");
    aHandPtr->value = 13*PAIR+pair_highcard-1;
    return aHandPtr->value;
  }
  strcpy(aHandPtr->class,"High Card");
  aHandPtr->value = 13*HIGHCARD+highcard-1;
  return aHandPtr->value;
}
