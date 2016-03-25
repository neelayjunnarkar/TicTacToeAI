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

enum class GameType {
	FAIL          = 0b000,
	AI_AI         = 0b001,
	PLAYER_AI     = 0b010,
	PLAYER_PLAYER = 0b100
};

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

bool in_bounds(int i) {
	if (i >= 0 && i < 3)
		return true;
	return false;
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
	int x, y;
	do {
		x = rand() % 3;
		y = rand() % 3;
	} while (!in_bounds(x) || !in_bounds(y) || board[y][x] != '.');
	board[y][x] = ai;
}

void human_move(Board &board, char human) {
	std::cout << "Turn of " << human << std::endl;
	int x, y;
	do {
		std::cin >> x >> y;
	} while (board[y][x] != '.');
	board[y][x] = human;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Please run program in format: <exe> <p1 type> <p2 type>" << std::endl;
		std::cerr << "Options for types are: \'ai\' or \'human\'" << std::endl;
		return -1;
	}
	std::cout << argv[1] << " " << argv[2] << std::endl;
	GameType game_type(GameType::FAIL);
	if ((argv[1] == std::string("ai") && argv[2] == std::string("human") || (argv[1] == std::string("human") && argv[2] == std::string("ai")))) {
		game_type = GameType::PLAYER_AI;
	} else if (argv[1] == std::string("human") && argv[2] == std::string("human")) {
		game_type = GameType::PLAYER_PLAYER;
	} else if (argv[1] == std::string("ai") && argv[1] == std::string("ai")) {
		game_type = GameType::AI_AI;
	}
	if (game_type == GameType::FAIL) {
		std::cerr << "Oops! Something went wrong; perhaps with the player type arguments you passed in. Please try again" << std::endl;
		return -3;
	}
	srand(time(NULL));
	char p2 = 'O';
	char p1 = 'X';
	
	Board board {{'.','.','.'},{'.','.','.'},{'.','.','.'}};
	std::cout << to_string(board) << std::endl;
	
	/** GameType AI vs AI **/
	if (game_type == GameType::AI_AI) {
		while(winner(board) == 'F' && !is_filled(board)) {
			std::cout << to_string(board) << std::endl;
			ai_move(board, p2);
			if (winner(board) != 'F') {
				break;
			}
			if (is_filled(board)) {
				break;
			}
			std::cout << to_string(board) << std::endl;
			ai_move(board, p1);
		}
		std::cout << to_string(board) << std::endl;
		if (winner(board) == 'F') {
			std::cout << "No Winner!" << std::endl;	
		} else if (winner(board) == p1) {
			std::cout << p1 << " Wins!" << std::endl;
		} else if (winner(board) == p2) {
			std::cout << p2 << " Wins!" << std::endl;
		}
	}
	
	/** GameType HUMAN vs AI **/
	if (game_type == GameType::PLAYER_AI) {
		bool p1_start = rand() % 2;
		
		if (p1_start) {
			std::cout << "Your Turn!" << std::endl;
			human_move(board, p1);
		}
		
		while(winner(board) == 'F') {
			if (is_filled(board)) {
				break;
			}
			std::cout << to_string(board) << std::endl;
			ai_move(board, p2);
			if (winner(board) != 'F') {
				break;
			}
			std::cout << to_string(board) << std::endl;
			human_move(board, p1);
		}
		std::cout << to_string(board) << std::endl;
		if (winner(board) == 'F') {
			std::cout << "No Winner!" << std::endl;	
		} else if (winner(board) == p1) {
			std::cout << p1 << " Wins!" << std::endl;
		} else if (winner(board) == p2) {
			std::cout << p2 << " Wins!" << std::endl;
		}
	}
	
	/** GameType HUMAN vs HUMAN **/
	if (game_type == GameType::PLAYER_PLAYER) {
		bool p1_start = rand() % 2;
		
		if (p1_start) {
			human_move(board, p1);
		}
		
		while(winner(board) == 'F') {
			if (is_filled(board)) {
				break;
			}
			std::cout << to_string(board) << std::endl;
			human_move(board, p2);
			if (winner(board) != 'F') {
				break;
			}
			std::cout << to_string(board) << std::endl;
			human_move(board, p1);
		}
		std::cout << to_string(board) << std::endl;
		if (winner(board) == 'F') {
			std::cout << "No Winner!" << std::endl;	
		} else if (winner(board) == p1) {
			std::cout << "p1 Player Wins!" << std::endl;
		} else if (winner(board) == p2) {
			std::cout << "p2 Wins!" << std::endl;
		}
	}
	return 0;
}