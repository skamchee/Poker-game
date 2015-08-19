Readme.txt
Poker game

A five-card draw poker game written in C. One human player and 3 AI opponents.
AI programmed using Monte Carlo simulation technique. 

===========================
Build:
===========================
In Terminal:

make clean
make

OR

gcc -Wall -c card.c -I.
gcc -Wall -c deck.c -I.
gcc -Wall -c hand.c -I.
gcc -Wall -c player.c -I.
gcc -Wall -c mcadvisor.c -I.
gcc -Wall -c pokergame.c -I.
gcc -Wall -o pokergame pokergame.o card.o deck.o hand.o player.o mcadvisor.o

===========================
Functionality:
===========================
Run the simulation using the following command line input:
./pokergame 1000
The number 1000 can be varied. It is an argument containing the number of simulations to run per discard option. 
So inputting 1000 will run 32,000 simulations per hand, which takes about 4.6 seconds on a Macbook Air. 
I have found I can get correct Monte Carlo logic even at 200 simulations and the game runs significantly faster (<1 sec response time)

===========================
Rules:
===========================
(A) 2 rounds of betting per set. Bets go into the 'pool'.
(B) Every player must call at least the highest bet in round 1 in order to remain in the game. They don't have to call the highest bet in round 2.
(C) In each round, every player may discard and draw up to 5 cards from the deck.
(D) A player may also choose to fold at any time, during their turn but forfeits any bets they already made.
(E) Player with the highest set of cards at the end of 2 rounds wins all the money in the pool.
(F) If all players except one folds, the remaining player wins the pool, and does not need to show their hand.
(G) The game continues in sets of 2 rounds until all players except one is bankrupt.

===========================
Shortcomings:
===========================

--If a player doesn't have enough money to call a bet, I haven't implemented functionality for the player to go 'all-in'.
Implementing the 'all-in' feature will also need to split the pool of money and create side-bets.
--Also, since rare, I haven't implemented tie breaking. In the event of a tie, the winner will be the first player in order that has the tying hand.
Implementing tie-breaking requires introducing some logic to split the pool winnings.

--If the human player puts in a low bet in round 1, but then bluffs in round 2 by putting in an extraordinarily high bet, all the computer players will likely fold.
In this way, the human player can typically win all the bets the computer players made in round 1. This is because the computer players only bet on a scale linked to the expected value of their cards. The computer bets high when it has good cards, and low when it has bad cards. There isn't any mechanism for the computer to guess if someone is bluffing.
 
Since the betting features are independent of the project grade, I didn't go into these additional features.

===========================
Documentation:
===========================
--deck.c contains the Deck struct. This struct contains 52 consecutive in-memory pointers to Card structs. 
We can iterate through all 52 cards in the deck by iterating over the 52 pointers to Card structs.
The Deck struct has one integer that gives the index of the first pointer (out of 52 pointers) that is at the "top" of the deck. 
To dequeue a card, call dequeue_card, which saves the top card of the deck into one of the provided arguments. 
Then the integer will be incremented to point to the next card in the deck, which becomes the new 'top' card.
To enqueue all the cards back into the deck, we simply assign the integer the index of the first pointer.  

--hand.c contains the Hand struct. This struct contains 5 consecutive in-memory pointers to Card structs. 
We can iterate through all 5 cards in the hand by iterating over the pointers to Card structs.
There are functions to add cards, replace cards, and sort the hand.
This struct also contains a function to get the value of the hand, which is used by the Monte Carlo advisor.

--card.c contains the Card struct. This struct simply contains the data for the suit and rank of the card. 
Also there are some functions to print the card and compare the card to other cards.

--player.c contains a wrapper around the Hand struct. The Player struct contains one pointer to a Hand struct, and some additional information about the player such as their name, money they have, whether the player folded, and what they have bet into the pool in the current round.

--mcadvisor.c contains the logic of the Monte Carlo advisor. Only the first function (called mc) is public. The other two functions are private functions to be used by the mc function.More description on this is in the mcadvisor.c file.

--pokergame.c contains the game loop for the program. 

--pokergame.h contains the function prototypes, #include, and #define for all files


 






