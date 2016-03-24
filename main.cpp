#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <algorithm>

using Move = std::tuple<int, int>;
using Board = char[3][3];

void copy(const Board &src, Board &dst) {
	for (int y = 0; y < 3; ++y)
		for (int x = 0; x < 3; ++x)
			dst[y][x] = src[y][x];
}

std::string to_string(const Board &board) {
	std::ostringstream os;
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			os << board[y][x];
		}
		os << std::endl;
	}
	return os.str();
}

char other(char player) {
	if (player == 'O')
		return 'X';
	else if (player == 'X')
		return 'O';
	return 'F';
}

bool is_filled(const Board &board) {
	for (int y = 0; y < 3; ++y) 
		for (int x = 0; x < 3; ++x) 
			if (board[y][x] == '.')
				return false;
	return true;
}

char winner(const Board &board) {
	char winner = 'F';
	for (int y = 0; y < 3; ++y) {
		if ((board[y][0] == board[y][1] && board[y][1] == board[y][2]))
			if (board[y][0] == 'O' || board[y][0] == 'X')
				winner = board[y][0];
	}
	for (int x = 0; x < 3; ++x) {
		if ((board[0][x] == board[1][x] && board[1][x] == board[2][x]))
			if (board[0][x] == 'O' || board[0][x] == 'X')
				winner = board[0][x];
	}
	if ((board[0][0] == board[1][1] && board[1][1] == board[2][2]))
		if (board[0][0] == 'O' || board[0][0] == 'X')
			winner = board[0][0];
	if ((board[0][2] == board[1][1] && board[1][1] == board[2][0]))
		if (board[0][2] == 'O' || board[0][2] == 'X')
			winner = board[0][2];
	return winner;
}

bool ai_move_backend(const Board &board, char player, char original) {
	char win_player = winner(board);
	if (win_player == original) {
		return true;
	} else if (win_player == other(original)) {	
		return false;
	} else if (win_player == 'F' && is_filled(board)) {
		return false;
	}
	
	bool win = false;
	if (player == original)
		win = false;
	else if (player == other(original))
		win = true;
		
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			if (board[y][x] == '.') {
				Board clone;
				copy(board, clone);
				clone[y][x] = player;
				if (player == original) {
					win |= ai_move_backend(clone, other(player), original);
				} else if (player == other(original)) {
					win &= ai_move_backend(clone, other(player), original);
				}
			}
		}
	}
	return win;
}

bool is_win(const Board &board, const Move &move, char player, char original) {
	Board clone;
	copy(board, clone);
	int x, y;
	std::tie(x, y) = move;
	clone[y][x] = player;
	return ai_move_backend(clone, other(player), original);
}

void ai_move(Board &board, char ai) {
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			if (board[y][x] == '.' && is_win(board, std::tie(x, y), ai, ai)) {
				board[y][x] = ai;
				return;
			}
		}
	}
}

void human_move(Board &board, char human) {
	int x, y;
	do {
		std::cin >> x >> y;
	} while (board[y][x] != '.');
	board[y][x] = human;
}

int main() {	
	char ai = 'O';
	char human = 'X';
	
	Board board {{'.','.','.'},{'.','.','.'},{'.','.','.'}};
	std::cout << to_string(board) << std::endl;
	
	while(winner(board) == 'F') {
		if (is_filled(board)) {
			break;
		}
		std::cout << to_string(board) << std::endl;
		ai_move(board, ai);
		if (winner(board) != 'F') {
			break;
		}
		std::cout << to_string(board) << std::endl;
		ai_move(board, human);
	}
	
	// srand(time(NULL));
	// bool human_start = rand() % 2;
	
	// if (human_start) {
	// 	std::cout << "Your Turn!" << std::endl;
	// 	human_move(board, human);
	// }
	
	// while(winner(board) == 'F') {
	// 	if (is_filled(board)) {
	// 		break;
	// 	}
	// 	std::cout << to_string(board) << std::endl;
	// 	ai_move(board, ai);
	// 	if (winner(board) != 'F') {
	// 		break;
	// 	}
	// 	std::cout << to_string(board) << std::endl;
	// 	human_move(board, human);
	// }
	// std::cout << to_string(board) << std::endl;
	// if (winner(board) == 'F') {
	// 	std::cout << "No Winner!" << std::endl;	
	// } else if (winner(board) == human) {
	// 	std::cout << "Human Player Wins!" << std::endl;
	// } else if (winner(board) == ai) {
	// 	std::cout << "AI Wins!" << std::endl;
	// }
	return 0;
}