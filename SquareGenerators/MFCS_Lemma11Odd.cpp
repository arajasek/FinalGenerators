// Every length-n integer, n >= 8, n even, is the sum of 3 generalized 
// squares, of lengths n, n-2, and n-4.

#include <iostream>
#include <string> 
#include <vector>
#include <utility>

using namespace std;

struct State {
	int nextN1p;
	int lastN3;
	int nextN3;
	int lowerCarry;
	int higherCarry;
	State(int w = 0, int e = 0, int r = 0, int a = 0, int s = 0) {
		nextN1p = w;
		lastN3 = e;
		nextN3 = r;
		lowerCarry = a;
		higherCarry = s;		
	}
};

class AutomatonGenerator {
private:
	string name;
	int n1p, n1m, n3;
	int maxCarry;
	int guessedCarry;
	void addATransitions(State s);
	void addBTransitions(State s);
	void addCTransitions(State s);
	void addStateTransitions(State s);
	void addInitTransitions();

public:
	AutomatonGenerator(string, int, int, int, int);
	void createStates();
	string getStateName(State s);
	void addTransitions();
	string getInitialStateName();
};

AutomatonGenerator::AutomatonGenerator (string nm, int n1plus, int nMinusOne, int nMinusThree, int gc) {
	name = nm + to_string(gc);
	n1p = n1plus;
	n1m = nMinusOne;
	n3 = nMinusThree;
	maxCarry = n1p + n1m + n3 - 1;
	guessedCarry = gc;
}

string AutomatonGenerator::getStateName(State s) {
	string ret = " " + name;
	ret += "_" + to_string(s.nextN1p);
	ret += "_" + to_string(s.lastN3) + "_" + to_string(s.nextN3);	

	return ret+"_"+to_string(s.lowerCarry)+"_"+to_string(s.higherCarry)+" ";
}

string AutomatonGenerator::getInitialStateName() {
 return " " + name + "_init ";
}

void AutomatonGenerator::createStates() {
	for(int nextN1p = 0; nextN1p <=n1p; nextN1p++) {
		for(int lastN3 = 0; lastN3 <=n3; lastN3++) {
			for(int nextN3 = 0; nextN3 <=n3; nextN3++) {
				for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
					for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
				cout << getStateName(State(nextN1p,lastN3,
					nextN3,lowerCarry, higherCarry)) <<endl;	
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
}

void AutomatonGenerator::addCTransitions(State s) {
	if ((s.nextN3 + s.lastN3 + s.lowerCarry) != 0)
		return;
	string sname = getStateName(s);
	int higherBit = (s.nextN1p + s.higherCarry) % 2;
	int higherCarry = (s.nextN1p + s.higherCarry) / 2;
	if ((higherBit == 1) && (higherCarry == 0))
		cout << "(" << sname << "c" << higherBit << " acc)\n";
}

void AutomatonGenerator::addBTransitions(State s) {
	if (s.nextN3 != 0)
		return;
	string sname = getStateName(s);
	for(int nextN1p = 0; nextN1p <=n1p; nextN1p++) {
		for(int n1mguess = 0; n1mguess <= n1m; n1mguess++) {
			int lowerBit = (nextN1p + n1mguess + s.lastN3 + s.lowerCarry) % 2;
			int lowerCarry = (nextN1p + n1mguess + s.lastN3 + s.lowerCarry) / 2;
			if (lowerCarry != guessedCarry)
				continue;
			int higherBit = (s.nextN1p + n1mguess + s.higherCarry) % 2;
			int higherCarry = (s.nextN1p + n1mguess + s.higherCarry) / 2;
			string destName = getStateName(State(nextN1p, 0, 0, 0, higherCarry));
				cout << "(" << sname << "b" << higherBit << lowerBit << destName <<")\n";
		}
	}
}

void AutomatonGenerator::addATransitions(State s) {
	string sname = getStateName(s);
	for(int nextN1p = 0; nextN1p <=n1p; nextN1p++) {
		for(int n1mguess = 0; n1mguess <= n1m; n1mguess++) {
			for(int nextN3 = 0; nextN3 <=n3; nextN3++) {
				int lowerBit = (nextN1p + n1mguess + s.nextN3 + s.lowerCarry) % 2;
				int lowerCarry = (nextN1p + n1mguess + s.nextN3 + s.lowerCarry) / 2;
				int higherBit = (s.nextN1p + n1mguess + nextN3 + s.higherCarry) % 2;
				int higherCarry = (s.nextN1p + n1mguess + nextN3 + s.higherCarry) / 2;
				string destName = getStateName(State(nextN1p, s.lastN3, nextN3, lowerCarry, higherCarry));
					cout << "(" << sname << "a" << higherBit << lowerBit << destName <<")\n";
			}
		}
	}
}

void AutomatonGenerator::addInitTransitions() {
	string sname = getInitialStateName();
	for(int nextN1p = 0; nextN1p <=n1p; nextN1p++) {
		for(int n1mguess = 0; n1mguess <= n1m; n1mguess++) {
			for(int lastN3 = 0; lastN3 <=n3; lastN3++) {
				for(int nextN3 = 0; nextN3 <=n3; nextN3++) {
					int lowerBit = (nextN1p + n1mguess + lastN3) % 2;
					int lowerCarry = (nextN1p + n1mguess + lastN3) / 2;
					int higherBit = (n1mguess + nextN3 + guessedCarry) % 2;
					int higherCarry = (n1mguess + nextN3 + guessedCarry) / 2;
					string destName = getStateName(State(nextN1p, lastN3, nextN3, lowerCarry, higherCarry));
						cout << "(" << sname << "a" << higherBit << lowerBit << destName <<")\n";
				}
			}
		}
	}
}

void AutomatonGenerator::addTransitions() {
	addInitTransitions();
	for(int nextN1p = 0; nextN1p <=n1p; nextN1p++) {
		for(int lastN3 = 0; lastN3 <=n3; lastN3++) {
			for(int nextN3 = 0; nextN3 <=n3; nextN3++) {
				for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
					for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
						addStateTransitions (State(nextN1p,
							lastN3, nextN3,lowerCarry, higherCarry));
							cout << endl;	
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

	cout << "FiniteAutomaton oddGenSqChecker = (\n";	
	cout << "alphabet = {a00 a01 a10 a11 b00 b01 b10 b11\n c1},\n";

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

	cout << "FiniteAutomaton finalAut = shrinkNwa(oddGenSqChecker);\n";
	cout << "print(numberOfStates(finalAut));\n\n\n\n\n\n\n\n\n\n";
}