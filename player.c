#include "pokergame.h"

int player_init(Player * aPlayer){
  if((aPlayer->hand = (Hand *)malloc(sizeof(Hand)))==NULL)
     return -1;
  aPlayer->name = (char*)malloc(sizeof(char)*20);
  aPlayer->money = INITIALMONEY;
  aPlayer->bet =0; //amount the player is currently betting
  aPlayer->fold=0;//0 if the player is in the game, 1 if folded
  hand_init(aPlayer->hand);
  return 0;
}

int player_toString(Player * aPlayer){
  if(aPlayer==NULL)
    return -1;
  printf("%s",aPlayer->name);
  return 0;
}

int player_reset_hand(Player * aPlayer){
  free(aPlayer->hand);
  if((aPlayer->hand = (Hand *)malloc(sizeof(Hand)))==NULL)
     return -1;
  hand_init(aPlayer->hand);
  return 0;
}
void player_destroy(Player * aPlayer){
  free(aPlayer->hand);
  free(aPlayer);
}
