#include "pokergame.h"

int mc(Hand * origHand, int*maxdiscards, int*maxthrown, int *maxvalue, int sims){
  /*Use the mc function to get information from the Monte Carlo advisor on the hand struct supplied at Hand * orighand 
    Read the maxdiscards, maxthrown and maxvalue variables to figure what to discard or amount to bet
  
    Hand * origHand = The hand struct supplied to mc needs to be sorted. Use the hand_sort function in hand.c 

    int *maxdiscards holds the indexes of cards that should be discarded. The indices correspond to the card array stored in the Hand struct
    int *maxthrown = number of cards the advisor recommends discarding
    int *maxvalue = highest expected value of the hand. Can be used for betting logic

    int sims = The number of simulations to run per discard option
    Note. there are 32 discard options, so the number of simulations actually run is value in int sims * 32*/
  int i=0;
  int start =0;
  int level =0;
  int thrown =0;
  int *discards = (int*)malloc(sizeof(int)*HAND_SIZE);
  for(i=0;i<HAND_SIZE;i++)
    discards[i]=-1;
  
  for(thrown=0;thrown<HAND_SIZE+1;thrown++){
    mc_card_combinations(origHand,discards,start,thrown,level,sims,maxvalue,maxdiscards,maxthrown);
    for(i=0;i<HAND_SIZE;i++)
      discards[i]=-1;
  }
  return 0;
}

int mc_card_combinations(Hand * origHand, int* discards, int start, int thrown, int level, int sims, int *maxvalue, int*maxdiscards, int*maxthrown){
  /*This is a private function of mcadvisor.c, used by the function mc
    This function produces the appropriate 32 combinations out of 6 discard options and runs mc_expectedvalue once on each of 32 combinations
    Usage:
    Run mc_card_combinations function six times -> once for each discard option(5 choose 5, 5 choose 4, 5 choose 3, 5 choose 2, 5 choose 1, 5 choose 0)
  */

  int i=0;
  int value;
  /*level = number of recursion levels. at each level we discard a card. 
    so when recursion levels = number of cards we want to discard, we have discarded an appropriate number of cards*/
  if(level==thrown){
    //printf("\n%d,%d,%d,%d,%d",discards[0],discards[1],discards[2],discards[3],discards[4]);
    value = mc_expectedvalue(origHand, discards, thrown, sims);
    if(value>*maxvalue){
      *maxvalue = value;
      *maxthrown = thrown;
      for(i=0;i<HAND_SIZE;i++)
	maxdiscards[i]=discards[i];
    }
    return 0;
  }
  for(i=start;i<HAND_SIZE;i++){
    discards[level]=i;
    mc_card_combinations(origHand,discards,i+1,thrown,level+1,sims,maxvalue,maxdiscards,maxthrown);
  }
  return 0;
} 

int mc_expectedvalue(Hand * origHand, int *discards, int thrown, int sims){
  /*This is a private function of mcadvisor.c, used by the function mc_card_combinations
    This function runs the actual Monte Carlo simulation and produces the expected value of a hand

    Usage:
    Run this function mc_expectedvalue 32 times as follows:
    Once for 5 choose 5
    5 times for 5 choose 4
    10 times for 5 choose 3
    10 times for 5 choose 2
    5 times for 5 choose 1
    Once for 5 choose 0
  */
  int values=13*CLASS*RANKS+1;//+1 to make space in the occurrences array for corner case 13*8+13 = 117 royal flush
  int i=0;
  int j=0;
  int mcnum=0;
  // the occurrences array is used as a frequency distribution, with the expected value as a 'hashkey' and frequency as the 'hashvalue' 
  int *occurrences = (int*)malloc(sizeof(int)*(values));
  Deck * simDeck = (Deck*)malloc(sizeof(Deck));
  Hand * simHand = (Hand*)malloc(sizeof(Hand));
  
  for(i=0;i<values;i++){
    occurrences[i]=0;
  }
  deck_init(simDeck);
  deck_fill(simDeck);
  
  hand_init(simHand);
  hand_copy(simHand,origHand);
     
  for(i=0;i<sims;i++){
    //on each sim, recreate the simdeck and simhands
     enqueue_deck(simDeck);
     shuffle(simDeck,1500);
     hand_copy(simHand,origHand);

    //remove from the deck the cards that are already in our hand
    for(j=0;j<HAND_SIZE;j++){
      remove_card(simDeck,simHand->cards[j]);
    }

    //throw out and replace the cards that the player wants to discard
    for(j=0;j<thrown;j++){
      dequeue_card(simDeck,&(simHand->cards[discards[j]]));
    }
  
    occurrences[hand_value(simHand)] +=1;
  }  

  for(i=1;i<values;i++){
    mcnum +=i*occurrences[i];
    //if(occurrences[i]!=0)
    //printf("\nvalue[%d]: occurs:%d",i,occurrences[i]);
  }

  free(occurrences);
  free(simDeck);
  free(simHand);
  //printf("\nExpected Value: %d\n",mcnum/sims);
  return mcnum/sims;
}

