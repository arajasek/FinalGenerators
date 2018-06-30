// "every n-bit number, n even, n >= 6 is the sum of at most 2 powers of 2 
// and either
//         at most one binary square of length n and at most one
//          binary square of length n-4
//         OR
//         at most one binary square of length n-2 and at most one
//          binary square of length n-4"

#include <iostream>
#include <string> 
#include <vector>
#include <utility>

using namespace std;

enum {Q_TYPE, R_TYPE, S_TYPE};

struct State {
	int type;
	int nextHigherN;
	int lastN4;
	int nextLowerN4;
	int lowerCarry;
	int higherCarry;
	State(int t = Q_TYPE, int q = 0, int w = 0, int e = 0, int a = 0, int s = 0) {
		type = t; 
		nextHigherN = q;
		lastN4 = w;
		nextLowerN4 = e;
		lowerCarry = a;
		higherCarry = s;		
	}
};

class AutomatonGenerator {
private:
	string name;
	int n, n2, n4;
	int maxCarry;
	int guessedCarry;
	void addATransitions();
	void addBTransitions(State s);
	void addCTransitions(State s);
	void addDTransitions(State s);
	void addETransitions(State s);
	void addFTransitions(State s);
	void addStateTransitions(State s);

public:
	AutomatonGenerator(string, int, int, int, int);
	void createStates();
	string getStateName(State s);
	void addTransitions();
};

AutomatonGenerator::AutomatonGenerator (string nm, int n0, int nMinusTwo, int nMinusFour, int gc) {
	name = nm + to_string(gc);
	n = n0;
	n2 = nMinusTwo;
	n4 = nMinusFour;
	maxCarry = n + n2 + n4 + 1;
	guessedCarry = gc;
}

string AutomatonGenerator::getStateName(State s) {
	string ret = " " + name;
	switch (s.type) {
		case Q_TYPE:
		ret += "_Q";
		break;
		case R_TYPE:
		ret += "_R";
		break;
		case S_TYPE:
		ret += "_S";
		break;
		default: 
		ret += "???" + to_string(s.type);
		break;
	}
	if (n)
		ret += "_" + to_string(s.nextHigherN);
	if (n4)
		ret += "_" + to_string(s.lastN4) + "_" + to_string(s.nextLowerN4);

	return ret+"_"+to_string(s.lowerCarry)+"_"+to_string(s.higherCarry)+" ";
}

void AutomatonGenerator::createStates() {
	for(int nextHigherN = 0; nextHigherN <=n; nextHigherN++) {
		for(int lastN4 = 0; lastN4 <=n4; lastN4++) {
			for(int nextLowerN4 = 0; nextLowerN4 <=n4; nextLowerN4++) {
				for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
					for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
						cout << getStateName(State(Q_TYPE, nextHigherN, lastN4, nextLowerN4, lowerCarry, higherCarry)) <<endl;	
						cout << getStateName(State(R_TYPE, nextHigherN, lastN4, nextLowerN4, lowerCarry, higherCarry)) <<endl;	
						cout << getStateName(State(S_TYPE, nextHigherN, lastN4, nextLowerN4, lowerCarry, higherCarry)) <<endl;	
					}
				}
			}
		}
	}
}

void AutomatonGenerator::addATransitions() {
	int HigherBit, HigherCarry, LowerBit, LowerCarry;
	string DestName;
	string sname = getStateName(State());
	for(int lowerNGuess = 0; lowerNGuess <=n; lowerNGuess++) {
		for(int N2Guess = 0; N2Guess <=n2; N2Guess++) {
			for(int lowerN4Guess = 0; lowerN4Guess <=n4; lowerN4Guess++) {
				for(int higherN4Guess = 0; higherN4Guess <=n4; higherN4Guess++) {
					HigherBit = (n + N2Guess + higherN4Guess + guessedCarry) % 2;
					HigherCarry = (n + N2Guess + higherN4Guess + guessedCarry) / 2;
					LowerBit = (lowerNGuess + N2Guess + lowerN4Guess) % 2;
					LowerCarry = (lowerNGuess + N2Guess + lowerN4Guess) / 2;
					DestName = getStateName(State(Q_TYPE, lowerNGuess, lowerN4Guess, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

					HigherBit = (n + N2Guess + higherN4Guess + guessedCarry + 1) % 2;
					HigherCarry = (n + N2Guess + higherN4Guess + guessedCarry + 1) / 2;
					LowerBit = (lowerNGuess + N2Guess + lowerN4Guess) % 2;
					LowerCarry = (lowerNGuess + N2Guess + lowerN4Guess) / 2;
					DestName = getStateName(State(R_TYPE, lowerNGuess, lowerN4Guess, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

					HigherBit = (n + N2Guess + higherN4Guess + guessedCarry) % 2;
					HigherCarry = (n + N2Guess + higherN4Guess + guessedCarry) / 2;
					LowerBit = (lowerNGuess + N2Guess + lowerN4Guess + 1) % 2;
					LowerCarry = (lowerNGuess + N2Guess + lowerN4Guess + 1) / 2;
					DestName = getStateName(State(R_TYPE, lowerNGuess, lowerN4Guess, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

					HigherBit = (n + N2Guess + higherN4Guess + guessedCarry + 2) % 2;
					HigherCarry = (n + N2Guess + higherN4Guess + guessedCarry + 2) / 2;
					LowerBit = (lowerNGuess + N2Guess + lowerN4Guess) % 2;
					LowerCarry = (lowerNGuess + N2Guess + lowerN4Guess) / 2;
					DestName = getStateName(State(S_TYPE, lowerNGuess, lowerN4Guess, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

					HigherBit = (n + N2Guess + higherN4Guess + guessedCarry) % 2;
					HigherCarry = (n + N2Guess + higherN4Guess + guessedCarry) / 2;
					LowerBit = (lowerNGuess + N2Guess + lowerN4Guess + 2) % 2;
					LowerCarry = (lowerNGuess + N2Guess + lowerN4Guess + 2) / 2;
					DestName = getStateName(State(S_TYPE, lowerNGuess, lowerN4Guess, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

					HigherBit = (n + N2Guess + higherN4Guess + guessedCarry + 1) % 2;
					HigherCarry = (n + N2Guess + higherN4Guess + guessedCarry + 1) / 2;
					LowerBit = (lowerNGuess + N2Guess + lowerN4Guess + 1) % 2;
					LowerCarry = (lowerNGuess + N2Guess + lowerN4Guess + 1) / 2;
					DestName = getStateName(State(S_TYPE, lowerNGuess, lowerN4Guess, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";
				}
			}
		}
	}
}

void AutomatonGenerator::addFTransitions(State s) {
	if ((s.nextLowerN4 + s.lastN4 + s.lowerCarry + s.nextHigherN) != 0)
		return;
	int HigherBit, HigherCarry;
	string DestName;
	string sname = getStateName(s);

	HigherBit = (n + s.higherCarry) % 2;
	HigherCarry = (n + s.higherCarry) / 2;
	if ((HigherBit == 1) && (HigherCarry == 0))
		cout << "(" << sname << "f" << HigherBit << " acc" <<")\n";

	if (s.type != S_TYPE) {
		HigherBit = (n + s.higherCarry + 1) % 2;
		HigherCarry = (n + s.higherCarry + 1) / 2;
		if ((HigherBit == 1) && (HigherCarry == 0))
			cout << "(" << sname << "f" << HigherBit << " acc" <<")\n";										
	}
}

void AutomatonGenerator::addETransitions(State s) {
	if ((s.nextLowerN4 != 0) || (s.lastN4 != 0) || (s.lowerCarry != guessedCarry))
		return;
	int HigherBit, HigherCarry;
	string DestName;
	string sname = getStateName(s);

	HigherBit = (s.nextHigherN + s.higherCarry) % 2;
	HigherCarry = (s.nextHigherN + s.higherCarry) / 2;
	DestName = getStateName(State(s.type, 0, 0, 0, 0, HigherCarry));
	cout << "(" << sname << "e" << HigherBit << DestName <<")\n";

	if (s.type != S_TYPE) {
		HigherBit = (s.nextHigherN + s.higherCarry + 1) % 2;
		HigherCarry = (s.nextHigherN + s.higherCarry + 1) / 2;
		DestName = getStateName(State(s.type + 1, 0, 0, 0, 0, HigherCarry));
		cout << "(" << sname << "e" << HigherBit << DestName <<")\n";										
	}

	if (s.type == Q_TYPE) {
		HigherBit = (s.nextHigherN + s.higherCarry + 2) % 2;
		HigherCarry = (s.nextHigherN + s.higherCarry + 2) / 2;
		DestName = getStateName(State(s.type + 2, 0, 0, 0, 0, HigherCarry));
		cout << "(" << sname << "e" << HigherBit << DestName <<")\n";									
	}
}

void AutomatonGenerator::addDTransitions(State s) {
	if (s.nextLowerN4 != 0)
		return;
	int HigherBit, HigherCarry, LowerBit, LowerCarry;
	string DestName;
	string sname = getStateName(s);
	int N2Guess = n2;
	for(int lowerNGuess = 0; lowerNGuess <=n; lowerNGuess++) {
		HigherBit = (s.nextHigherN + N2Guess + s.higherCarry) % 2;
		HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry) / 2;
		LowerBit = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry) % 2;
		LowerCarry = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry) / 2;
		DestName = getStateName(State(s.type, lowerNGuess, 0, 0, LowerCarry, HigherCarry));
		cout << "(" << sname << "d" << HigherBit << LowerBit << DestName <<")\n";

		if (s.type != S_TYPE) {
			HigherBit = (s.nextHigherN + N2Guess + s.higherCarry + 1) % 2;
			HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry + 1) / 2;
			LowerBit = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry) % 2;
			LowerCarry = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry) / 2;
			DestName = getStateName(State(s.type + 1, lowerNGuess, 0, 0, LowerCarry, HigherCarry));
			cout << "(" << sname << "d" << HigherBit << LowerBit << DestName <<")\n";

			HigherBit = (s.nextHigherN + N2Guess + s.higherCarry) % 2;
			HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry) / 2;
			LowerBit = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry + 1) % 2;
			LowerCarry = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry + 1) / 2;
			DestName = getStateName(State(s.type + 1, lowerNGuess, 0, 0, LowerCarry, HigherCarry));
			cout << "(" << sname << "d" << HigherBit << LowerBit << DestName <<")\n";										
		}

		if (s.type == Q_TYPE) {
			HigherBit = (s.nextHigherN + N2Guess + s.higherCarry + 2) % 2;
			HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry + 2) / 2;
			LowerBit = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry) % 2;
			LowerCarry = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry) / 2;
			DestName = getStateName(State(s.type + 2, lowerNGuess, 0, 0, LowerCarry, HigherCarry));
			cout << "(" << sname << "d" << HigherBit << LowerBit << DestName <<")\n";

			HigherBit = (s.nextHigherN + N2Guess + s.higherCarry) % 2;
			HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry) / 2;
			LowerBit = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry + 2) % 2;
			LowerCarry = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry + 2) / 2;
			DestName = getStateName(State(s.type + 2, lowerNGuess, 0, 0, LowerCarry, HigherCarry));
			cout << "(" << sname << "d" << HigherBit << LowerBit << DestName <<")\n";	

			HigherBit = (s.nextHigherN + N2Guess + s.higherCarry + 1) % 2;
			HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry + 1) / 2;
			LowerBit = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry + 1) % 2;
			LowerCarry = (lowerNGuess + N2Guess + s.lastN4 + s.lowerCarry + 1) / 2;
			DestName = getStateName(State(s.type + 2, lowerNGuess, 0, 0, LowerCarry, HigherCarry));
			cout << "(" << sname << "d" << HigherBit << LowerBit << DestName <<")\n";									
		}
	}
}

void AutomatonGenerator::addCTransitions(State s) {
	if (s.nextLowerN4 != n4)
		return;
	int HigherBit, HigherCarry, LowerBit, LowerCarry;
	string DestName;
	string sname = getStateName(s);
	for(int lowerNGuess = 0; lowerNGuess <=n; lowerNGuess++) {
		for(int N2Guess = 0; N2Guess <=n2; N2Guess++) {
			HigherBit = (s.nextHigherN + N2Guess + s.higherCarry) % 2;
			HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry) / 2;
			LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) % 2;
			LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) / 2;
			DestName = getStateName(State(s.type, lowerNGuess, s.lastN4, 0, LowerCarry, HigherCarry));
			cout << "(" << sname << "c" << HigherBit << LowerBit << DestName <<")\n";

			if (s.type != S_TYPE) {
				HigherBit = (s.nextHigherN + N2Guess + s.higherCarry + 1) % 2;
				HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry + 1) / 2;
				LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) % 2;
				LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) / 2;
				DestName = getStateName(State(s.type + 1, lowerNGuess, s.lastN4, 0, LowerCarry, HigherCarry));
				cout << "(" << sname << "c" << HigherBit << LowerBit << DestName <<")\n";

				HigherBit = (s.nextHigherN + N2Guess + s.higherCarry) % 2;
				HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry) / 2;
				LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) % 2;
				LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) / 2;
				DestName = getStateName(State(s.type + 1, lowerNGuess, s.lastN4, 0, LowerCarry, HigherCarry));
				cout << "(" << sname << "c" << HigherBit << LowerBit << DestName <<")\n";										
			}

			if (s.type == Q_TYPE) {
				HigherBit = (s.nextHigherN + N2Guess + s.higherCarry + 2) % 2;
				HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry + 2) / 2;
				LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) % 2;
				LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) / 2;
				DestName = getStateName(State(s.type + 2, lowerNGuess, s.lastN4, 0, LowerCarry, HigherCarry));
				cout << "(" << sname << "c" << HigherBit << LowerBit << DestName <<")\n";

				HigherBit = (s.nextHigherN + N2Guess + s.higherCarry) % 2;
				HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry) / 2;
				LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 2) % 2;
				LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 2) / 2;
				DestName = getStateName(State(s.type + 2, lowerNGuess, s.lastN4, 0, LowerCarry, HigherCarry));
				cout << "(" << sname << "c" << HigherBit << LowerBit << DestName <<")\n";	

				HigherBit = (s.nextHigherN + N2Guess + s.higherCarry + 1) % 2;
				HigherCarry = (s.nextHigherN + N2Guess + s.higherCarry + 1) / 2;
				LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) % 2;
				LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) / 2;
				DestName = getStateName(State(s.type + 2, lowerNGuess, s.lastN4, 0, LowerCarry, HigherCarry));
				cout << "(" << sname << "c" << HigherBit << LowerBit << DestName <<")\n";									
			}
		}
	}
}

void AutomatonGenerator::addBTransitions(State s) {
	int HigherBit, HigherCarry, LowerBit, LowerCarry;
	string DestName;
	string sname = getStateName(s);
	for(int lowerNGuess = 0; lowerNGuess <=n; lowerNGuess++) {
		for(int N2Guess = 0; N2Guess <=n2; N2Guess++) {
			for(int higherN4Guess = 0; higherN4Guess <=n4; higherN4Guess++) {
				HigherBit = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry) % 2;
				HigherCarry = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry) / 2;
				LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) % 2;
				LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) / 2;
				DestName = getStateName(State(s.type, lowerNGuess, s.lastN4, higherN4Guess, LowerCarry, HigherCarry));
				cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";

				if (s.type != S_TYPE) {
					HigherBit = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry + 1) % 2;
					HigherCarry = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry + 1) / 2;
					LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) % 2;
					LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) / 2;
					DestName = getStateName(State(s.type + 1, lowerNGuess, s.lastN4, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";

					HigherBit = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry) % 2;
					HigherCarry = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry) / 2;
					LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) % 2;
					LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) / 2;
					DestName = getStateName(State(s.type + 1, lowerNGuess, s.lastN4, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";										
				}

				if (s.type == Q_TYPE) {
					HigherBit = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry + 2) % 2;
					HigherCarry = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry + 2) / 2;
					LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) % 2;
					LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry) / 2;
					DestName = getStateName(State(s.type + 2, lowerNGuess, s.lastN4, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";

					HigherBit = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry) % 2;
					HigherCarry = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry) / 2;
					LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 2) % 2;
					LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 2) / 2;
					DestName = getStateName(State(s.type + 2, lowerNGuess, s.lastN4, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";	

					HigherBit = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry + 1) % 2;
					HigherCarry = (s.nextHigherN + N2Guess + higherN4Guess + s.higherCarry + 1) / 2;
					LowerBit = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) % 2;
					LowerCarry = (lowerNGuess + N2Guess + s.nextLowerN4 + s.lowerCarry + 1) / 2;
					DestName = getStateName(State(s.type + 2, lowerNGuess, s.lastN4, higherN4Guess, LowerCarry, HigherCarry));
					cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";									
				}
			}
		}
	}
}

void AutomatonGenerator::addStateTransitions(State s) {
	addBTransitions(s);
	if (s.nextLowerN4 == n4)
		addCTransitions(s);
	if (s.nextLowerN4 == 0)
		addDTransitions(s);
	if (((s.nextLowerN4 + s.lastN4) == 0) && (s.lowerCarry == guessedCarry))
		addETransitions(s);
	addFTransitions(s);
}

void AutomatonGenerator::addTransitions() {
	addATransitions();
	for(int nextHigherN = 0; nextHigherN <=n; nextHigherN++) {
		for(int lastN4 = 0; lastN4 <=n4; lastN4++) {
			for(int nextLowerN4 = 0; nextLowerN4 <=n4; nextLowerN4++) {
				for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
					for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {
						addStateTransitions(State(Q_TYPE, nextHigherN, lastN4, nextLowerN4, lowerCarry, higherCarry));
						addStateTransitions(State(R_TYPE, nextHigherN, lastN4, nextLowerN4, lowerCarry, higherCarry));
						addStateTransitions(State(S_TYPE, nextHigherN, lastN4, nextLowerN4, lowerCarry, higherCarry));
					}
				}
			}
		}
	}
}

void addMachines(string nm, int n0, int nMinusTwo, int nMinusFour, vector <AutomatonGenerator> &ms) {
	int maxGC = n0 + nMinusTwo + nMinusFour + 1;
	for (int i = 0; i <= maxGC; i++) {
		ms.push_back(AutomatonGenerator(nm, n0, nMinusTwo, nMinusFour, i));
	}
}

int main() {
	vector <AutomatonGenerator> machines;

	addMachines("Z", 0, 0, 0, machines);
	addMachines("A", 1, 0, 0, machines);
	addMachines("B", 0, 0, 1, machines);
	addMachines("C", 1, 0, 1, machines);
	addMachines("D", 0, 1, 0, machines);
	addMachines("E", 0, 0, 1, machines);
	addMachines("F", 0, 1, 1, machines);

	cout << "FiniteAutomaton evenSqPowChecker = (\n";	
	cout << "alphabet = {a00 a01 a10 a11 b00 b01 b10 b11\n";
	cout << "c00 c01 c10 c11 d00 d01 d10 d11 e0 e1 f1},\n";

	cout << "states = {\n";
	for(int i =0; i < machines.size(); i++)
		machines.at(i).createStates();
	cout<<"acc},\n";

	cout << "initialStates = {\n";
	for(int i =0; i < machines.size(); i++)
		cout << machines.at(i).getStateName(State())<<endl;
	cout << "},\nfinalStates = {acc},\n";

	cout << "transitions = {\n";
	for(int i =0; i < machines.size(); i++)
		machines.at(i).addTransitions();

	cout <<"\n}\n);\n";

	cout << "FiniteAutomaton finalAut = shrinkNwa(evenSqPowChecker);\n";
	cout << "print(numberOfStates(finalAut));\n\n\n\n\n\n\n\n\n\n";
}