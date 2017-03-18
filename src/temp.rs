#[derive(PartialEq, Clone, Copy)]
enum Symbol {
	O,
	X,
	Blank
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


struct Game<'a> {
	state: [[Symbol; 3]; 3],
	p1:    &'a Fn([[Symbol; 3]; 3]) -> [[Symbol; 3]; 3], 
	p2:    &'a Fn([[Symbol; 3]; 3]) -> [[Symbol; 3]; 3]
}

impl<'a> Game<'a> {
	fn new(p1: &'a Fn([[Symbol; 3]; 3]) -> [[Symbol; 3]; 3], p2: &'a Fn([[Symbol; 3]; 3]) -> [[Symbol; 3]; 3]) -> Game<'a> {
		Game { state: [[Symbol::Blank, Symbol::Blank, Symbol::Blank],
						[Symbol::Blank, Symbol::Blank, Symbol::Blank],
						[Symbol::Blank, Symbol::Blank, Symbol::Blank]],
				p1: p1,
				p2: p2 }
	}

	fn winner(&self) -> Option<Symbol> {
		
		// Horizontal Victory
		if self.state[0].iter().all(|ref x| **x == self.state[0][0]) {
			return Some(self.state[0][0]);
		} else if self.state[1].iter().all(|ref x| **x == self.state[1][0]) {
			return Some(self.state[1][0]);
		} else if self.state[2].iter().all(|ref x| **x == self.state[2][0]) {
			return Some(self.state[2][0]);
		}

		// Vertical Victory
		if self.state.iter().all(|ref arr| (*arr)[0] == self.state[0][0]) {
			return Some(self.state[0][0]);
		} else if self.state.iter().all(|ref arr| (*arr)[1] == self.state[0][1]) {
			return Some(self.state[0][1]);
		} else if self.state.iter().all(|ref arr| (*arr)[2] == self.state[0][2]) {
			return Some(self.state[0][2]);
		}

		None
	}
}

impl<'a> Iterator for Game<'a> {
	type Item = [[Symbol; 3]; 3];
	fn next(&mut self) -> Option<[[Symbol; 3]; 3]> {
		None
	}
}

fn p1(state: [[Symbol; 3]; 3]) -> [[Symbol; 3]; 3] { state }
fn p2(state: [[Symbol; 3]; 3]) -> [[Symbol; 3]; 3] { state }

fn main() {
	let game = Game::new(|x| x, |x| x);	
}
