// Every length-n number, n even, n >= 18 is the sum of
// either

// A 2 squares of length n-2 and 2 squares of length n-4
// B 3 squares of length n-2 and 1 square of length n-4
// C 1 square of length n and 1 square of length n-4 and 1 square of length n-6
// D 2 squares of length n-2 and 1 square of length n-4 and 1 square of length n-6

#include <iostream>
#include <string> 
#include <vector>
#include <utility>

using namespace std;

struct State {
	int secondLastN;
	int nextHigherN;
	int next2HigherN;
	int nextHigherN2;
	int lastN6;
	int nextLowerN6;
	int lowerCarry;
	int higherCarry;
	State(int q = 0, int w = 0, int e = 0, int r = 0, int t = 0, int y = 0, int a = 0, int s = 0) {
		secondLastN = q;
		nextHigherN = w;
		next2HigherN = e;
		nextHigherN2 = r;
		lastN6 = t;
		nextLowerN6 = y;
		lowerCarry = a;
		higherCarry = s;		
	}
};

class AutomatonGenerator {
private:
	string name;
	int n, n2, n4, n6;
	int maxCarry;
	int guessedCarry;
	void addATransitions();
	void addBTransitions(State s);
	void addCTransitions(State s);
	void addDTransitions(State s);
	void addETransitions(State s);
	void addFTransitions(State s);
	void addGTransitions(State s);
	void addHTransitions(State s);
	void addITransitions(State s);
	void addStateTransitions(State s);

public:
	AutomatonGenerator(string, int, int, int, int, int);
	void createStates();
	string getStateName(State s);
	void addTransitions();
};

// n0 is the number of summands of length n
// nMinusTwo is the number of summands of length n-2, etc...
// gc is the guessed carry we expect to be produced by the lower half

AutomatonGenerator::AutomatonGenerator (string nm, int n0, int nMinusTwo, int nMinusFour, int nMinusSix, int gc) {
	name = nm + to_string(gc);
	n = n0;
	n2 = nMinusTwo;
	n4 = nMinusFour;
	n6 = nMinusSix;
	maxCarry = n + n2 + n4 + n6 - 1;
	guessedCarry = gc;
}

string AutomatonGenerator::getStateName(State s) {
	string ret = " " + name;
	if (n)
		ret += "_" + to_string(s.secondLastN) + "_" + to_string(s.nextHigherN) + "_" + to_string (s.next2HigherN);
	if (n2)
		ret += "_" + to_string(s.nextHigherN2);
	if (n6)
		ret += "_" + to_string(s.lastN6) + "_" + to_string(s.nextLowerN6);	

	return ret+"_"+to_string(s.lowerCarry)+"_"+to_string(s.higherCarry)+" ";
}

void AutomatonGenerator::createStates() {
	for(int secondLastN = 0; secondLastN <= n; secondLastN++) {
		for(int nextHigherN = 0; nextHigherN <=n; nextHigherN++) {
			for(int next2HigherN = 0; next2HigherN <=n; next2HigherN++) {
				for(int nextHigherN2 = 0; nextHigherN2 <=n2; nextHigherN2++) {
					for(int lastN6 = 0; lastN6 <= n6; lastN6++) {
						for(int nextLowerN6 = 0; nextLowerN6 <= n6; nextLowerN6++) {
							for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
								for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
							cout << getStateName(State(secondLastN,
								nextHigherN,next2HigherN,
								nextHigherN2,lastN6,
								nextLowerN6,lowerCarry, higherCarry)) <<endl;	
								}
							}
						}
					}
				}
			}
		}
	}
}	

void AutomatonGenerator::addStateTransitions(State s) {
	addBTransitions(s);
	addCTransitions(s);
	addDTransitions(s);
	addETransitions(s);
	addFTransitions(s);
	addGTransitions(s);
	addHTransitions(s);
	addITransitions(s);
}

void AutomatonGenerator::addITransitions(State s) {
	if ((s.lowerCarry + s.nextLowerN6 + s.lastN6 + s.nextHigherN2 + s.next2HigherN + s.nextHigherN + s.secondLastN) != 0)
		return;
	string sname = getStateName(s);
	int higherBit = (n + s.higherCarry) % 2;
	int higherCarry = (n + s.higherCarry) / 2;
	if ((higherBit != 1) || (higherCarry != 0))
		return;
	cout << "(" << sname << "i" << higherBit <<" acc)\n";
}

void AutomatonGenerator::addHTransitions(State s) {
	if ((s.lowerCarry + s.nextLowerN6 + s.lastN6 + s.nextHigherN2 + s.next2HigherN + s.nextHigherN) != 0)
		return;
	string sname = getStateName(s);
	int higherBit = (s.secondLastN + s.higherCarry) % 2;
	int higherCarry = (s.secondLastN + s.higherCarry) / 2;
	string destName = getStateName(State(0, 
		0, 0, 0, 0, 0, 0, higherCarry));
	cout << "(" << sname << "h" << higherBit << destName <<")\n";
}

void AutomatonGenerator::addGTransitions(State s) {
	if ((s.lowerCarry + s.nextLowerN6 + s.lastN6 + s.nextHigherN2 + s.next2HigherN) != 0)
		return;
	string sname = getStateName(s);
	int higherBit = (s.nextHigherN + n2 + s.higherCarry) % 2;
	int higherCarry = (s.nextHigherN + n2 + s.higherCarry) / 2;
	string destName = getStateName(State(s.secondLastN, 
		0, 0, 0, 0, 0, 0, higherCarry));
	cout << "(" << sname << "g" << higherBit << destName <<")\n";
}

void AutomatonGenerator::addFTransitions(State s) {
	if ((s.lowerCarry + s.nextLowerN6 + s.lastN6) != 0)
		return;
	string sname = getStateName(s);
	int higherBit = (s.nextHigherN + s.nextHigherN2 + s.higherCarry) % 2;
	int higherCarry = (s.nextHigherN + s.nextHigherN2 + s.higherCarry) / 2;
	string destName = getStateName(State(s.secondLastN, 
		s.next2HigherN, 0, 0, 0, 0, 0, higherCarry));
	cout << "(" << sname << "f" << higherBit << destName <<")\n";
}

void AutomatonGenerator::addETransitions(State s) {
	if (s.nextLowerN6 != 0)
		return;
	string sname = getStateName(s);
	for (int lowerN = 0; lowerN <= n; lowerN++) {
		for (int lowerN2 = 0; lowerN2 <= n2; lowerN2++) {
			int lowerBit = (lowerN + lowerN2 + n4 + s.lastN6 + s.lowerCarry) % 2;
			int lowerCarry = (lowerN + lowerN2 + n4 + s.lastN6 + s.lowerCarry) / 2;
			int higherBit = (s.nextHigherN + s.nextHigherN2 + n4 + s.higherCarry) % 2;
			int higherCarry = (s.nextHigherN + s.nextHigherN2 + n4 + s.higherCarry) / 2;
			if (lowerCarry != guessedCarry)
				continue;
			string destName = getStateName(State(s.secondLastN, 
				s.next2HigherN, lowerN, lowerN2, 0, 0, 0, higherCarry));
			cout << "(" << sname << "e" << higherBit << lowerBit << destName <<")\n";
		}		
	}								
}		

void AutomatonGenerator::addDTransitions(State s) {
	if (s.nextLowerN6 != n6)
		return;
	string sname = getStateName(s);
	for (int lowerN = 0; lowerN <= n; lowerN++) {
		for (int lowerN2 = 0; lowerN2 <= n2; lowerN2++) {
			for (int N4 = 0; N4 <= n4; N4++) {
				int lowerBit = (lowerN + lowerN2 + N4 + s.nextLowerN6 + s.lowerCarry) % 2;
				int lowerCarry = (lowerN + lowerN2 + N4 + s.nextLowerN6 + s.lowerCarry) / 2;
				int higherBit = (s.nextHigherN + s.nextHigherN2 + N4 + s.higherCarry) % 2;
				int higherCarry = (s.nextHigherN + s.nextHigherN2 + N4 + s.higherCarry) / 2;
				string destName = getStateName(State(s.secondLastN, 
					s.next2HigherN, lowerN, lowerN2, s.lastN6, 0, lowerCarry, higherCarry));
				cout << "(" << sname << "d" << higherBit << lowerBit << destName <<")\n";
			}		
		}								
	}		
}	

void AutomatonGenerator::addCTransitions(State s) {
	string sname = getStateName(s);
	for (int lowerN = 0; lowerN <= n; lowerN++) {
		for (int lowerN2 = 0; lowerN2 <= n2; lowerN2++) {
			for (int N4 = 0; N4 <= n4; N4++) {
				for (int higherN6 = 0; higherN6 <= n6; higherN6++) {
					int lowerBit = (lowerN + lowerN2 + N4 + s.nextLowerN6 + s.lowerCarry) % 2;
					int lowerCarry = (lowerN + lowerN2 + N4 + s.nextLowerN6 + s.lowerCarry) / 2;
					int higherBit = (s.nextHigherN + s.nextHigherN2 + N4 + higherN6 + s.higherCarry) % 2;
					int higherCarry = (s.nextHigherN + s.nextHigherN2 + N4 + higherN6 + s.higherCarry) / 2;
					string destName = getStateName(State(s.secondLastN, 
						s.next2HigherN, lowerN, lowerN2, s.lastN6, higherN6, lowerCarry, higherCarry));
					cout << "(" << sname << "c" << higherBit << lowerBit << destName <<")\n";
				}		
			}								
		}		
	}		
}		

void AutomatonGenerator::addBTransitions(State s) {
	if ((s.nextHigherN) != 0)
		return;
	string sname = getStateName(s);
	for (int lowerN = 0; lowerN <= n; lowerN++) {
		for (int lowerN2 = 0; lowerN2 <= n2; lowerN2++) {
			for (int N4 = 0; N4 <= n4; N4++) {
				for (int higherN6 = 0; higherN6 <= n6; higherN6++) {
					int lowerBit = (lowerN + lowerN2 + N4 + s.nextLowerN6 + s.lowerCarry) % 2;
					int lowerCarry = (lowerN + lowerN2 + N4 + s.nextLowerN6 + s.lowerCarry) / 2;
					int higherBit = (n + s.nextHigherN2 + N4 + higherN6 + s.higherCarry) % 2;
					int higherCarry = (n + s.nextHigherN2 + N4 + higherN6 + s.higherCarry) / 2;
					string destName = getStateName(State(s.secondLastN, 
						s.next2HigherN, lowerN, lowerN2, s.lastN6, higherN6, lowerCarry, higherCarry));
					cout << "(" << sname << "b" << higherBit << lowerBit << destName <<")\n";
				}		
			}								
		}		
	}		
}	

void AutomatonGenerator::addATransitions() {
	string sname = getStateName(State());
	for (int lowerN = 0; lowerN <= n; lowerN++) {
		for (int higherN = 0; higherN <= n; higherN++) {
			for (int lowerN2 = 0; lowerN2 <= n2; lowerN2++) {
				for (int N4 = 0; N4 <= n4; N4++) {
					for (int lowerN6 = 0; lowerN6 <= n6; lowerN6++) {
						for (int higherN6 = 0; higherN6 <= n6; higherN6++) {
							int lowerBit = (lowerN + lowerN2 + N4 + lowerN6) % 2;
							int lowerCarry = (lowerN + lowerN2 + N4 + lowerN6) / 2;
							int higherBit = (higherN + n2 + N4 + higherN6 + guessedCarry) % 2;
							int higherCarry = (higherN + n2 + N4 + higherN6 + guessedCarry) / 2;
							string destName = getStateName(State(higherN, 
								0, lowerN, lowerN2, lowerN6, higherN6, lowerCarry, higherCarry));
							cout << "(" << sname << "a" << higherBit << lowerBit << destName <<")\n";
						}		
					}								
				}		
			}		
		}	
	}			
}		

void AutomatonGenerator::addTransitions() {
	addATransitions();
	for(int secondLastN = 0; secondLastN <= n; secondLastN++) {
		for(int nextHigherN = 0; nextHigherN <=n; nextHigherN++) {
			for(int next2HigherN = 0; next2HigherN <=n; next2HigherN++) {
				for(int nextHigherN2 = 0; nextHigherN2 <=n2; nextHigherN2++) {
					for(int lastN6 = 0; lastN6 <= n6; lastN6++) {
						for(int nextLowerN6 = 0; nextLowerN6 <= n6; nextLowerN6++) {
							for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
								for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
							addStateTransitions (State(secondLastN,
								nextHigherN,next2HigherN,
								nextHigherN2,lastN6,
								nextLowerN6, lowerCarry, higherCarry));
								cout << endl;	
								}
							}
						}
					}
				}
			}
		}
	}
}	

void addMachines(string nm, int n0, int nMinusTwo, int nMinusFour, int nMinusSix, vector <AutomatonGenerator> &ms) {
	int maxGC = n0 + nMinusTwo + nMinusFour + nMinusSix;
	for (int i = 0; i < maxGC; i++) {
		ms.push_back(AutomatonGenerator(nm, n0, nMinusTwo, nMinusFour, nMinusSix, i));
	}
} 
int main() {
	vector <AutomatonGenerator> machines;

	// A 2 squares of length n-2 and 2 squares of length n-4
	// B 3 squares of length n-2 and 1 square of length n-4
	// C 1 square of length n and 1 square of length n-4 and 1 square of length n-6
	// D 2 squares of length n-2 and 1 square of length n-4 and 1 square of length n-6

	addMachines("A", 0, 2, 2, 0, machines);
	addMachines("B", 0, 3, 1, 0, machines);
	addMachines("C", 1, 0, 1, 1, machines);
	addMachines("D", 0, 2, 1, 1, machines);

	cout << "FiniteAutomaton evenSqChecker = (\n";	
	cout << "alphabet = {a00 a01 a10 a11 b00 b01 b10 b11 c00 c01 c10 c11\n";
	cout << "d00 d01 d10 d11 e00 e01 e10 e11 f00 f01 f10 f11 g0 g1 h0 h1 i1},\n";

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

	cout << "FiniteAutomaton finalAut = shrinkNwa(evenSqChecker);\n";
	cout << "print(numberOfStates(finalAut));\n\n\n\n\n\n\n\n\n\n";
}