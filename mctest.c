#include "pokergame.h"

int main(int argc, char **argv){
  int i=0;


  int sims = atoi(argv[1]);//runs 32*sims simulations

  printf("Welcome to Monte Carlo Poker!\n");
  //set up variables to retrieve info on cards to discard from Monte Carlo sim 
  int *maxdiscards = (int*)malloc(sizeof(int)*HAND_SIZE);;
  int *maxthrown = (int*)malloc(sizeof(int));
  int *maxvalue = (int*)malloc(sizeof(int));
  for(i=0;i<HAND_SIZE;i++){
    maxdiscards[i]=-1;
  }
  *maxthrown=0;
  
    
  //Monte Carlo testing
  
  Deck * testDeck = (Deck*)malloc(sizeof(Deck));
  Hand * testHand = (Hand*)malloc(sizeof(Hand));
  deck_init(testDeck);
  deck_fill(testDeck);
  shuffle(testDeck,2000);
  //  deck_toString(testDeck);
  hand_init(testHand);
  
  swap_card(testDeck,testDeck->topcardindex,find_card(testDeck,1,5));  
  dequeue_card(testDeck,&(testHand->cards[0]));
  swap_card(testDeck,testDeck->topcardindex,find_card(testDeck,2,7));  
  dequeue_card(testDeck,&(testHand->cards[1]));
  swap_card(testDeck,testDeck->topcardindex,find_card(testDeck,3,8));  
  dequeue_card(testDeck,&(testHand->cards[2]));
  swap_card(testDeck,testDeck->topcardindex,find_card(testDeck,4,9));  
  dequeue_card(testDeck,&(testHand->cards[3]));
  swap_card(testDeck,testDeck->topcardindex,find_card(testDeck,1,11));  
  dequeue_card(testDeck,&(testHand->cards[4]));
  printf("testhand\n");
  testHand->cardsHeld=5;
  hand_sort(testHand);
  hand_toString(testHand);
  
    
  //run Monte Carlo simulation

  mc(testHand,maxdiscards,maxthrown,maxvalue,sims);
  printf("Your hand has value: %d\n",hand_value(testHand));
  printf("You have a: %s\n",testHand->class);
  if(*maxthrown==0){
    printf("I think you should keep all your cards.\n");
  }else{
    if(*maxthrown==1)
      printf("I think you should discard %d card:\n",*maxthrown);
    else
      printf("I think you should discard %d cards:\n",*maxthrown);
    for(i=0;i<*maxthrown;i++)
      card_toString(testHand->cards[maxdiscards[i]]);
    
  }

  //printf("\n%d,maxdiscards:%d,%d,%d,%d,%d\n",*maxthrown,maxdiscards[0],maxdiscards[1],maxdiscards[2],maxdiscards[3],maxdiscards[4]);
  //hand_toString(testHand);
  
  //destroy Monte Carlo testing stuff
  deck_destroy(testDeck);
  hand_destroy(testHand);
  
  return 0;
}
