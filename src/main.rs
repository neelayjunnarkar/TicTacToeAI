extern crate rand;

#[derive(PartialEq, Clone, Copy)]
enum Symbol {
	O,
	X,
	Blank
}

#[derive(PartialEq)]
enum Winner {
	Tie,
	Player(Symbol),
	NoWinner
}

impl std::fmt::Display for Symbol {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		write!(f, "{}", match *self {
			Symbol::O => "O",
			Symbol::X => "X",
			Symbol::Blank => " "
		})
	}
}

struct Game {
	state:		[[Symbol; 3]; 3],
	p1:			Box<Fn([[Symbol; 3]; 3], Symbol) -> [[Symbol; 3]; 3]>, 
	p2:			Box<Fn([[Symbol; 3]; 3], Symbol) -> [[Symbol; 3]; 3]>,
	p1_symbol:	Symbol,
	p2_symbol:	Symbol,
	turn:		Symbol,
	winner:		Winner
}

impl Game {
	fn new(p1: Box<Fn([[Symbol; 3]; 3], Symbol) -> [[Symbol; 3]; 3]>,
		p2: Box<Fn([[Symbol; 3]; 3], Symbol) -> [[Symbol; 3]; 3]>,
		p1_symbol: Symbol,
		p2_symbol: Symbol) -> Game {
		Game {  state: [[Symbol::Blank; 3]; 3],
				p1: p1,
				p2: p2,
				p1_symbol: p1_symbol,
				p2_symbol: p2_symbol,
				turn: p1_symbol,
				winner: Winner::NoWinner}
	}

	fn is_won(state: [[Symbol; 3]; 3]) -> Winner {
		
		// Horizontal Victory
		if state[0].iter().all(|ref x| **x != Symbol::Blank && **x == state[0][0]) {
			return Winner::Player(state[0][0]);
		} else if state[1].iter().all(|ref x| **x != Symbol::Blank && **x == state[1][0]) {
			return Winner::Player(state[1][0]);
		} else if state[2].iter().all(|ref x|  **x != Symbol::Blank && **x == state[2][0]) {
			return Winner::Player(state[2][0]);
		}

		// Vertical Victory
		if state.iter().all(|ref arr| (*arr)[0] != Symbol::Blank && (*arr)[0] == state[0][0]) {
			return Winner::Player(state[0][0]);
		} else if state.iter().all(|ref arr| (*arr)[1] != Symbol::Blank && (*arr)[1] == state[0][1]) {
			return Winner::Player(state[0][1]);
		} else if state.iter().all(|ref arr| (*arr)[2] != Symbol::Blank && (*arr)[2] == state[0][2]) {
			return Winner::Player(state[0][2]);
		}

		// Diagonal Victory
		if (0..3).all(|i| state[i][i] != Symbol::Blank && state[i][i] == state[0][0]) {
			return Winner::Player(state[0][0]);
		} else if (0..3).all(|i| state[2-i][i] != Symbol::Blank && state[2-i][i] == state[2][0]) {
			return Winner::Player(state[2][0]);
		}

		// Tie - all cells are filled, and should have failed victory tests to reach here
		if state.iter().all(|ref arr| arr.iter().all(|ref x| **x != Symbol::Blank)) {
			return Winner::Tie;
		}

		Winner::NoWinner
	}

	// Non-static version
	fn winner(&self) -> Winner {
		Game::is_won(self.state)
	}
}

impl<'a> Iterator for &'a mut Game {
	type Item = [[Symbol; 3]; 3];
	fn next(&mut self) -> Option<[[Symbol; 3]; 3]> {
		if self.turn == self.p1_symbol {
			self.state = (self.p1)(self.state, self.p1_symbol);
			self.turn = self.p2_symbol;
		} else if self.turn == self.p2_symbol {
			self.state = (self.p2)(self.state, self.p2_symbol);
			self.turn = self.p1_symbol;
		}
		
		self.winner = self.winner();
		match self.winner {
			Winner::Tie | Winner::Player(_) => None,
			Winner::NoWinner => Some(self.state)
		}
	}
}

fn print_board(state: [[Symbol; 3]; 3]) {
	for y in 0..3 {
		for x in 0..3 {
			print!("{}, ", state[y][x]);
		}
		println!("");
	}
}

fn player_move(mut state: [[Symbol; 3]; 3], symbol: Symbol) -> [[Symbol; 3]; 3] { 
	println!("Player {} Move", symbol);
	let mut input = String::new();
	loop {
		std::io::stdin().read_line(&mut input).expect("failed input");
		let coords = input.trim().split(' ').map(|x|x.parse::<usize>()).collect::<Vec<_>>();
		input.clear();
		if coords.len() != 2 {
			println!("Please enter input in format: <x> <y>");
			println!("There should be 2 nonnegative integers");
			continue;
		}
		if let (&Ok(x), &Ok(y)) = (&coords[0], &coords[1]) {
			if !(x <= 2 && y <= 2) {
				println!("Input coordinates must be either 0, 1, or 2");
				continue;
			}
			if state[y][x] != Symbol::Blank {
				println!("Cell is not blank. Please choose a blank cell");
				continue;
			}
			state[y][x] = symbol;
			return state;
		}
	}
}

fn ai_move_backend(mut state: [[Symbol; 3]; 3], symbol: Symbol, move_pos: (usize, usize)) -> Result<f32, String> {
	let (x, y) = move_pos;
	state[y][x] = symbol;

	match Game::is_won(state) {
		Winner::Player(winner_symbol) => {
			if winner_symbol == symbol {
				Ok(1.0)
			} else {
				Ok(0.0)
			}
		}
		Winner::Tie  => {
			Ok(0.5)
		}
		Winner::NoWinner => {
			let value = (0..9).filter(|i: &usize| state[i / 3][i % 3] == Symbol::Blank)
				.map(|i| ai_move_backend(
					state,
					match symbol {
						Symbol::O => Symbol::X,
						Symbol::X => Symbol::O,
						Symbol::Blank => Symbol::Blank
					},
					(i % 3, i /3 )))
				.max_by(|a, b| { 
					if a < b {
						std::cmp::Ordering::Less
					} else if a > b {
						std::cmp::Ordering::Greater
					} else {
						std::cmp::Ordering::Equal
					}
				});
			if let Some(Ok(val)) = value {
				Ok(1.0 - val)
			} else {
				Err("Some error".to_string())
			}
		}
	}
}

fn ai_move(mut state: [[Symbol; 3]; 3], symbol: Symbol) -> [[Symbol; 3]; 3] {
	println!("AI {} Move", symbol);
	if let Some((i, _)) = (0..9).filter(|i: &usize| state[i / 3][i % 3] == Symbol::Blank)
		.map(|i| (i, ai_move_backend(state, symbol, (i % 3, i / 3) )))
		.max_by(|a, b| {
			let &(_, ref a_val) = a;
			let &(_, ref b_val) = b;
			if a_val < b_val {
				std::cmp::Ordering::Less
			} else if a_val > b_val {
				std::cmp::Ordering::Greater
			} else {
				std::cmp::Ordering::Equal
			}
		}) { 
		state[i / 3][i % 3] = symbol;
	}
	state
}

fn main() {
	let mut game = if rand::random::<bool>() { 
			Game::new(Box::new(ai_move), Box::new(player_move), Symbol::O, Symbol::X) 
		} else {
			Game::new(Box::new(player_move), Box::new(ai_move), Symbol::O, Symbol::X)
		};
	for state in &mut game {
		print_board(state);	
		println!("");
	}
	print_board(game.state);
	println!("");
	match game.winner {
		Winner::Tie => println!("Tie"),
		Winner::Player(symbol) => {
			if symbol == game.p1_symbol {
				println!("P1, {}, Won!", game.p1_symbol);
			} else if symbol == game.p2_symbol {
				println!("P2, {}, Won!", game.p2_symbol);
			} else {
				println!("potential error, but {} won", symbol);
			}
		},
		Winner::NoWinner => println!("No winner?!?!?!?")
	}
}
