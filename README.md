# Tic-Tac-Toe AI #
by Neelay Junnarkar

A not very good tic-tac-toe ai built off a program that determines whether a player is sure to gain victory given a specific board state.

## Running the Program ##
	* Source code compilation has only been tested with MinGW 5.3 using C++14
	* Run in format:
		 <exe> <player 1 type> <player 2 type> 
	* Player types are: 
		* human = allow human player to take control of moves of the player
		* ai    = moves are specified by algorithm for the player

## Problems ##
	* AI assumes perfect plays by both players. 
		* If no sure victory, AI plays randomly
	* AI values losing the same as tie-ing.
		* If no sure victory, AI plays randomly and does not bother prolonging game by preventing loss, as opposed to trying to achieve a tie
		* Makes AI seem worse than it is
		   
Original sure-victory algorithm written by Neelay Junnarkar, Ben Cohen-Wang, Tyler Packard.