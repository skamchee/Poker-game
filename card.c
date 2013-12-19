#include "pokergame.h"

void card_init(Card * aCardPtr, int suit, int rank){
  aCardPtr->suit = suit;
  aCardPtr->rank = rank;
}

int card_toString(Card * aCardPtr){
  if(aCardPtr==NULL)
    return -1;
  switch(aCardPtr->rank){
  case 11:
    printf("J");
    break;
  case 12:
    printf("Q");
    break;
  case 13:
    printf("K");
    break;
  case 14:
    printf("A");
    break;
  default:
    printf("%d",aCardPtr->rank);
    break;
  }
  
  printf(" of ");

  switch(aCardPtr->suit){
  case 1:
    printf("Spades");
    break;
  case 2:
    printf("Hearts");
    break;
  case 3:
    printf("Diamonds");
    break;
  case 4:
    printf("Clubs");
    break;
  default:
    printf("%d",aCardPtr->suit);
    break;
  }
  
  putchar('\n');
  return 0;
}

int card_compare(const void * a, const void * b){
  return ((*(Card**)(a))->rank)-((*(Card**)(b))->rank);
}
void card_destroy(Card * aCardPtr){
  free(aCardPtr);
}

