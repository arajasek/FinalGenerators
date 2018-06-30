// "every n-bit number, n odd, n >= 5, is the sum of at most 2 powers of 2
// and either
//         at most 2 binary squares of length n-1
//         OR
//         at most one binary square of length n-1 and at most one
//         binary square of length n-3"

#include <iostream>
#include <string> 
#include <vector>
#include <utility>

using namespace std;

enum {Q_TYPE, R_TYPE, S_TYPE};

struct State {
	int type;
	int nextHigherN1;
	int lowerCarry;
	int higherCarry;
	State(int t = Q_TYPE, int q = 0, int a = 0, int s = 0) {
		type = t; 
		nextHigherN1 = q;
		lowerCarry = a;
		higherCarry = s;		
	}
};

class AutomatonGenerator {
private:
	string name;
	int n1, n3;
	int maxCarry;
	int guessedCarry;
	void addATransitions(State s);
	void addBTransitions(State s);
	void addCTransitions(State s);
	void addDTransitions(State s);
	void addETransitions(State s);
	void addStateTransitions(State s);
	string getStateName(State s);

public:
	AutomatonGenerator(string, int, int, int);
	void createStates();
	void addTransitions();
	string getInitState();
};

AutomatonGenerator::AutomatonGenerator (string nm, int nMinusOne, int nMinusThree, int gc) {
	name = nm + to_string(gc);
	n1 = nMinusOne;
	n3 = nMinusThree;
	maxCarry = n1 + n3 + 1;
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
	if (n1)
		ret += "_" + to_string(s.nextHigherN1);

	return ret+"_"+to_string(s.lowerCarry)+"_"+to_string(s.higherCarry)+" ";
}

string AutomatonGenerator::getInitState() {
	return getStateName(State(Q_TYPE, n1, 0, guessedCarry));
}

void AutomatonGenerator::createStates() {
	for(int nextHigherN1 = 0; nextHigherN1 <=n1; nextHigherN1++) {
		for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
			for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {	
				cout << getStateName(State(Q_TYPE, nextHigherN1, lowerCarry, higherCarry)) <<endl;	
				cout << getStateName(State(R_TYPE, nextHigherN1, lowerCarry, higherCarry)) <<endl;	
				cout << getStateName(State(S_TYPE, nextHigherN1, lowerCarry, higherCarry)) <<endl;	
			}
		}
	}
}

void AutomatonGenerator::addETransitions(State s) {
	if ((s.lowerCarry != 0) || (s.nextHigherN1 != 0))
		return;
	int HigherBit, HigherCarry;
	string sname = getStateName(s);

	HigherBit = s.higherCarry % 2;
	HigherCarry = s.higherCarry / 2;
	if ((HigherBit == 1) && (HigherCarry == 0))
		cout << "(" << sname << "e1 acc )\n";

	if (s.type != S_TYPE) {
		HigherBit = (s.higherCarry + 1) % 2;
		HigherCarry = (s.higherCarry + 1) / 2;
		if ((HigherBit == 1) && (HigherCarry == 0))
			cout << "(" << sname << "e1 acc )\n";	
	}
}

void AutomatonGenerator::addDTransitions(State s) {
	if ((s.lowerCarry != 0) || (s.nextHigherN1 != n1))
		return;
	int HigherBit, HigherCarry;
	string DestName;
	string sname = getStateName(s);

	HigherBit = (s.nextHigherN1 + s.higherCarry) % 2;
	HigherCarry = (s.nextHigherN1 + s.higherCarry) / 2;
	DestName = getStateName(State(s.type, 0, 0, HigherCarry));
	cout << "(" << sname << "d" << HigherBit << DestName <<")\n";

	if (s.type != S_TYPE) {
		HigherBit = (s.nextHigherN1 + s.higherCarry + 1) % 2;
		HigherCarry = (s.nextHigherN1 + s.higherCarry + 1) / 2;
		DestName = getStateName(State(s.type, 0, 0, HigherCarry));
		cout << "(" << sname << "d" << HigherBit << DestName <<")\n";									
	}

	if (s.type == Q_TYPE) {
		HigherBit = (s.nextHigherN1 + s.higherCarry + 2) % 2;
		HigherCarry = (s.nextHigherN1 + s.higherCarry + 2) / 2;
		DestName = getStateName(State(s.type, 0, 0, HigherCarry));
		cout << "(" << sname << "d" << HigherBit << DestName <<")\n";				
	}
}

void AutomatonGenerator::addCTransitions(State s) {
	if (s.lowerCarry != guessedCarry)
		return;
	int HigherBit, HigherCarry;
	string DestName;
	string sname = getStateName(s);

	HigherBit = (s.nextHigherN1 + s.higherCarry) % 2;
	HigherCarry = (s.nextHigherN1 + s.higherCarry) / 2;
	DestName = getStateName(State(s.type, n1, 0, HigherCarry));
	cout << "(" << sname << "c" << HigherBit << DestName <<")\n";

	if (s.type != S_TYPE) {
		HigherBit = (s.nextHigherN1 + s.higherCarry + 1) % 2;
		HigherCarry = (s.nextHigherN1 + s.higherCarry + 1) / 2;
		DestName = getStateName(State(s.type, n1, 0, HigherCarry));
		cout << "(" << sname << "c" << HigherBit << DestName <<")\n";									
	}

	if (s.type == Q_TYPE) {
		HigherBit = (s.nextHigherN1 + s.higherCarry + 2) % 2;
		HigherCarry = (s.nextHigherN1 + s.higherCarry + 2) / 2;
		DestName = getStateName(State(s.type, n1, 0, HigherCarry));
		cout << "(" << sname << "c" << HigherBit << DestName <<")\n";				
	}
}

void AutomatonGenerator::addBTransitions(State s) {
	int HigherBit, HigherCarry, LowerBit, LowerCarry;
	string DestName;
	string sname = getStateName(s);
	int N3Guess = n3;
	for(int lowerN1Guess = 0; lowerN1Guess <=n1; lowerN1Guess++) {
		HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry) % 2;
		HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry) / 2;
		LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry) % 2;
		LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry) / 2;
		DestName = getStateName(State(s.type, lowerN1Guess, LowerCarry, HigherCarry));
		cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";

		if (s.type != S_TYPE) {
			HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) % 2;
			HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) / 2;
			LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry) % 2;
			LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry) / 2;
			DestName = getStateName(State(s.type + 1, lowerN1Guess, LowerCarry, HigherCarry));
			cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";

			HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry) % 2;
			HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry) / 2;
			LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry + 1) % 2;
			LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry + 1) / 2;
			DestName = getStateName(State(s.type + 1, lowerN1Guess, LowerCarry, HigherCarry));
			cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";										
		}

		if (s.type == Q_TYPE) {
			HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry + 2) % 2;
			HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry + 2) / 2;
			LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry) % 2;
			LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry) / 2;
			DestName = getStateName(State(s.type + 2, lowerN1Guess, LowerCarry, HigherCarry));
			cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";

			HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry) % 2;
			HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry) / 2;
			LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry + 2) % 2;
			LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry + 2) / 2;
			DestName = getStateName(State(s.type + 2, lowerN1Guess, LowerCarry, HigherCarry));
			cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";	

			HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) % 2;
			HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) / 2;
			LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry + 1) % 2;
			LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry + 1) / 2;
			DestName = getStateName(State(s.type + 2, lowerN1Guess, LowerCarry, HigherCarry));
			cout << "(" << sname << "b" << HigherBit << LowerBit << DestName <<")\n";									
		}
	}
}

void AutomatonGenerator::addATransitions(State s) {
	int HigherBit, HigherCarry, LowerBit, LowerCarry;
	string DestName;
	string sname = getStateName(s);
	for(int lowerN1Guess = 0; lowerN1Guess <=n1; lowerN1Guess++) {
		for(int N3Guess = 0; N3Guess <=n3; N3Guess++) {
			HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry) % 2;
			HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry) / 2;
			LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry) % 2;
			LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry) / 2;
			DestName = getStateName(State(s.type, lowerN1Guess, LowerCarry, HigherCarry));
			cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

			if (s.type != S_TYPE) {
				HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) % 2;
				HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) / 2;
				LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry) % 2;
				LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry) / 2;
				DestName = getStateName(State(s.type + 1, lowerN1Guess, LowerCarry, HigherCarry));
				cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

				HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry) % 2;
				HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry) / 2;
				LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry + 1) % 2;
				LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry + 1) / 2;
				DestName = getStateName(State(s.type + 1, lowerN1Guess, LowerCarry, HigherCarry));
				cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";										
			}

			if (s.type == Q_TYPE) {
				HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry + 2) % 2;
				HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry + 2) / 2;
				LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry) % 2;
				LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry) / 2;
				DestName = getStateName(State(s.type + 2, lowerN1Guess, LowerCarry, HigherCarry));
				cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";

				HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry) % 2;
				HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry) / 2;
				LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry + 2) % 2;
				LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry + 2) / 2;
				DestName = getStateName(State(s.type + 2, lowerN1Guess, LowerCarry, HigherCarry));
				cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";	

				HigherBit = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) % 2;
				HigherCarry = (s.nextHigherN1 + N3Guess + s.higherCarry + 1) / 2;
				LowerBit = (lowerN1Guess + N3Guess + s.lowerCarry + 1) % 2;
				LowerCarry = (lowerN1Guess + N3Guess + s.lowerCarry + 1) / 2;
				DestName = getStateName(State(s.type + 2, lowerN1Guess, LowerCarry, HigherCarry));
				cout << "(" << sname << "a" << HigherBit << LowerBit << DestName <<")\n";									
			}
		}
	}
}

void AutomatonGenerator::addStateTransitions(State s) {
	addATransitions(s);
	addBTransitions(s);
	addCTransitions(s);
	addDTransitions(s);
	addETransitions(s);
}

void AutomatonGenerator::addTransitions() {
	for(int nextHigherN1 = 0; nextHigherN1 <=n1; nextHigherN1++) {
		for(int lowerCarry = 0; lowerCarry <=maxCarry; lowerCarry++) {
			for(int higherCarry = 0; higherCarry <=maxCarry; higherCarry++) {
				addStateTransitions(State(Q_TYPE, nextHigherN1, lowerCarry, higherCarry));
				addStateTransitions(State(R_TYPE, nextHigherN1, lowerCarry, higherCarry));
				addStateTransitions(State(S_TYPE, nextHigherN1, lowerCarry, higherCarry));
			}
		}
	}
}

void addMachines(string nm, int n1, int n3, vector <AutomatonGenerator> &ms) {
	int maxGC = n1 + n3 + 1;
	for (int i = 0; i <= maxGC; i++) {
		ms.push_back(AutomatonGenerator(nm, n1, n3, i));
	}
}

int main() {
	vector <AutomatonGenerator> machines;

	addMachines("Z", 0, 0, machines);
	addMachines("A", 1, 0, machines);
	addMachines("B", 2, 0, machines);
	addMachines("C", 0, 1, machines);
	addMachines("D", 1, 1, machines);

	cout << "FiniteAutomaton oddSqPowChecker = (\n";	
	cout << "alphabet = {a00 a01 a10 a11 b00 b01 b10 b11 c0 c1 d0 d1 e1},\n";

	cout << "states = {\n";
	for(int i =0; i < machines.size(); i++)
		machines.at(i).createStates();
	cout<<"acc},\n";

	cout << "initialStates = {\n";
	for(int i =0; i < machines.size(); i++)
		cout << machines.at(i).getInitState()<<endl;
	cout << "},\nfinalStates = {acc},\n";

	cout << "transitions = {\n";
	for(int i =0; i < machines.size(); i++)
		machines.at(i).addTransitions();

	cout <<"\n}\n);\n";

	cout << "FiniteAutomaton finalAut = shrinkNwa(oddSqPowChecker);\n";
	cout << "print(numberOfStates(finalAut));\n\n\n\n\n\n\n\n\n\n";
}



