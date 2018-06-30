// Every length-n integer, n >= 8, n even, is the sum of 3 generalized 
// squares, of lengths n, n-2, and n-4.

#include <iostream>
#include <string> 
#include <vector>
#include <utility>

using namespace std;

struct State {
	int lastN;
	int nextN;
	int lastN4;
	int nextN4;
	int lowerCarry;
	int higherCarry;
	State(int q = 0, int w = 0, int e = 0, int r = 0, int a = 0, int s = 0) {
		lastN = q;
		nextN = w;
		lastN4 = e;
		nextN4 = r;
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
	void addATransitions(State s);
	void addBTransitions(State s);
	void addCTransitions(State s);
	void addDTransitions(State s);
	void addStateTransitions(State s);
	void addInitTransitions();

public:
	AutomatonGenerator(string, int, int, int, int);
	void createStates();
	string getStateName(State s);
	void addTransitions();
	string getInitialStateName();
};

AutomatonGenerator::AutomatonGenerator (string nm, int n0, int nMinusTwo, int nMinusFour, int gc) {
	name = nm + to_string(gc);
	n = n0;
	n2 = nMinusTwo;
	n4 = nMinusFour;
	maxCarry = n + n2 + n4 - 1;
	guessedCarry = gc;
}

string AutomatonGenerator::getStateName(State s) {
	string ret = " " + name;
	ret += "_" + to_string(s.lastN) + "_" + to_string(s.nextN);
	ret += "_" + to_string(s.lastN4) + "_" + to_string(s.nextN4);	

	return ret+"_"+to_string(s.lowerCarry)+"_"+to_string(s.higherCarry)+" ";
}

string AutomatonGenerator::getInitialStateName() {
 return " " + name + "_init ";
}

void AutomatonGenerator::createStates() {
	for(int lastN = 0; lastN <= n; lastN++) {
		for(int nextN = 0; nextN <=n; nextN++) {
			for(int lastN4 = 0; lastN4 <=n4; lastN4++) {
				for(int nextN4 = 0; nextN4 <=n4; nextN4++) {
					for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
						for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
					cout << getStateName(State(lastN,
						nextN,lastN4,
						nextN4,lowerCarry, higherCarry)) <<endl;	
						}
					}
				}
			}
		}
	}
}

void AutomatonGenerator::addStateTransitions(State s) {
	addATransitions(s);
	addBTransitions(s);
	addCTransitions(s);
	addDTransitions(s);
}

void AutomatonGenerator::addDTransitions(State s) {
	if ((s.nextN + s.nextN4 + s.lastN4 + s.lowerCarry) != 0)
		return;
	string sname = getStateName(s);
	int higherBit = (s.lastN + s.higherCarry) % 2;
	int higherCarry = (s.lastN + s.higherCarry) / 2;
	if ((higherBit == 1) && (higherCarry == 0))
		cout << "(" << sname << "d" << higherBit << " acc)\n";
}	

void AutomatonGenerator::addCTransitions(State s) {
	if ((s.nextN4 + s.lastN4 + s.lowerCarry) != 0)
		return;
	string sname = getStateName(s);
	int higherBit = (s.nextN + s.higherCarry) % 2;
	int higherCarry = (s.nextN + s.higherCarry) / 2;
	string destName = getStateName(State(s.lastN, 
			0, 0, 0, 0, higherCarry));
		cout << "(" << sname << "c" << higherBit << destName <<")\n";
}

void AutomatonGenerator::addBTransitions(State s) {
	if (s.nextN4 != 0)
		return;
	string sname = getStateName(s);
	for(int nextN = 0; nextN <=n; nextN++) {
		for (int n2guess = 0; n2guess <= n2; n2guess++) {
			int lowerBit = (nextN + n2guess + s.lastN4 + s.lowerCarry) % 2;
			int lowerCarry = (nextN + n2guess + s.lastN4 + s.lowerCarry) / 2;
			if (lowerCarry != guessedCarry)
				continue;
			int higherBit = (s.nextN + n2guess + s.higherCarry) % 2;
			int higherCarry = (s.nextN + n2guess + s.higherCarry) / 2;
			string destName = getStateName(State(s.lastN, 
					nextN, 0, 0, 0, higherCarry));
				cout << "(" << sname << "b" << higherBit << lowerBit << destName <<")\n";
		}
	}
}

void AutomatonGenerator::addATransitions(State s) {
	string sname = getStateName(s);
	for(int nextN = 0; nextN <=n; nextN++) {
		for (int n2guess = 0; n2guess <= n2; n2guess++) {
			for(int nextN4 = 0; nextN4 <=n4; nextN4++) {
				int lowerBit = (nextN + n2guess + s.nextN4 + s.lowerCarry) % 2;
				int lowerCarry = (nextN + n2guess + s.nextN4 + s.lowerCarry) / 2;
				int higherBit = (s.nextN + n2guess + nextN4 + s.higherCarry) % 2;
				int higherCarry = (s.nextN + n2guess + nextN4 + s.higherCarry) / 2;
				string destName = getStateName(State(s.lastN, 
						nextN, s.lastN4, nextN4, lowerCarry, higherCarry));
					cout << "(" << sname << "a" << higherBit << lowerBit << destName <<")\n";
			}
		}
	}
}

void AutomatonGenerator::addInitTransitions() {
	string sname = getInitialStateName();
	for(int lastN = 0; lastN <= n; lastN++) {
		for(int nextN = 0; nextN <=n; nextN++) {
			for (int n2guess = 0; n2guess <= n2; n2guess++) {
				for(int lastN4 = 0; lastN4 <=n4; lastN4++) {
					for(int nextN4 = 0; nextN4 <=n4; nextN4++) {
						int lowerBit = (nextN + n2guess + lastN4) % 2;
						int lowerCarry = (nextN + n2guess + lastN4) / 2;
						int higherBit = (lastN + n2guess + nextN4 + guessedCarry) % 2;
						int higherCarry = (lastN + n2guess + nextN4 + guessedCarry) / 2;
						string destName = getStateName(State(lastN, 
								nextN, lastN4, nextN4, lowerCarry, higherCarry));
							cout << "(" << sname << "a" << higherBit << lowerBit << destName <<")\n";
					}
				}
			}
		}
	}					
}

void AutomatonGenerator::addTransitions() {
	addInitTransitions();
	for(int lastN = 0; lastN <= n; lastN++) {
		for(int nextN = 0; nextN <=n; nextN++) {
			for(int lastN4 = 0; lastN4 <=n4; lastN4++) {
				for(int nextN4 = 0; nextN4 <=n4; nextN4++) {
					for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
						for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
							addStateTransitions (State(lastN,
								nextN,lastN4,
								nextN4,lowerCarry, higherCarry));
								cout << endl;	
						}
					}
				}
			}
		}
	}
}	

int main() {
	vector <AutomatonGenerator> machines;

	machines.push_back(AutomatonGenerator("Aut", 1,1,1,0));
	machines.push_back(AutomatonGenerator("Aut", 1,1,1,1));
	machines.push_back(AutomatonGenerator("Aut", 1,1,1,2));

	cout << "FiniteAutomaton evenGenSqChecker = (\n";	
	cout << "alphabet = {a00 a01 a10 a11 b00 b01 b10 b11\n c0 c1 d1},\n";

	cout << "states = {\n";
	for(int i =0; i < machines.size(); i++) {
		machines.at(i).createStates();
		cout<<machines.at(i).getInitialStateName()<<endl;
	}
	cout<<endl<<"acc},\n";

	cout << "initialStates = {\n";
	for(int i =0; i < machines.size(); i++)
		cout << machines.at(i).getInitialStateName()<<endl;
	cout << "},\nfinalStates = {acc},\n";

	cout << "transitions = {\n";
	for(int i =0; i < machines.size(); i++)
		machines.at(i).addTransitions();

	cout <<"\n}\n);\n";

	cout << "FiniteAutomaton finalAut = shrinkNwa(evenGenSqChecker);\n";
	cout << "print(numberOfStates(finalAut));\n\n\n\n\n\n\n\n\n\n";
}