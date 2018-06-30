/*

To understand how the automata work, or see the results proven, consult
section 6 of this paper:

http://drops.dagstuhl.de/opus/volltexte/2018/8497/pdf/LIPIcs-STACS-2018-54.pdf

This program generates automata for the following 4 cases

(a) one each exactly of palindromes of length n, n-1, n-2

(b) one each exactly of palindromes of length n, n-2, n-3

(c) one each exactly of palindromes of length n-1, n-2, n-3

(d) one palindrome of length n-1 and one of n-2 .

The base can be provided to these machines as a parameter. 
It thus generators our machines for both bases 3 and 4.
*/

#include <iostream>
#include <string> 

using namespace std;

class AutomatonGenerator {
public:
	string name;
	int maxCarry;
	int base;
	void createStates();
	void createInitialStates();
	void createFinalStates();
	string getStateName(int, int, int, int, int);
	void addBtransitions();
	void addAtransitions();
	void addCtransitions();
	void addDtransitions();
	void addEtransitions();
	void addTransitions();
}

class AutomatonGeneratorCaseA : public AutomatonGenerator{
public:
	AutomatonGeneratorCaseA(int);
};

AutomatonGeneratorCaseA::AutomatonGeneratorCaseA(int b) {
	maxCarry = 2;
	base = b;
}

//Q States are 5-tuples (c1, c2, x, y, z)
// c1, expected higher carry
// c2, produced lower carry
// x, the next lower guess of the n-word
// y, the two-away lower guess of the n-word
// z, the next lower guess of the n-1-word

string AutomatonGeneratorCaseA::getStateName(int c1, int c2, int x, int y, int z) {
	return ("A_"+to_string(c1)+"_"+to_string(c2)+"_"+to_string(x)+to_string(y)+to_string(z));
}

void AutomatonGeneratorCaseA::addEtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int y = 0; y < base; y++) {
					for (int z = 0; z < base; z++) {
						string originState = getStateName(c1, c2, x, y, z);
						for (int k = 0; k < base; k++) {
							int bit = (x + c2 + k + z) % base;
							int carry = (x + c2 + k + z) / base;
							if (carry == c1) {
								cout << "(" << originState <<" e" << bit << " acc)\n";
							}
						}
					}
				}
			}
		}
	}
}


void AutomatonGeneratorCaseA::addDtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int y = 0; y < base; y++) {
					for (int z = 0; z < base; z++) {
						string originState = getStateName(c1, c2, x, y, z);
						for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
							for (int i = 0; i < base; i++) {
								for (int j = 0; j < base; j++) {
									for (int k = 0; k < base; k++) {
										int higherBit = (i + j + newc1 + k) % base;
										int higherCarry = (i + j + newc1 + k) / base;
										int lowerBit = (x + z + c2 + k) % base;
										int lowerCarry = (x + z + c2 + k) / base;
										if (higherCarry == c1) {
											cout << "(" << originState <<" d" << higherBit << lowerBit;
											cout << " " << getStateName(newc1, lowerCarry, y, i, j)<<")\n";
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseA::addCtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 1; x < base; x++) {
			for (int y = 0; y < base; y++) {
				for (int z = 1; z < base; z++) {
					string originState = getStateName(c1, 0, x, y, z);
					for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
						for (int i = 0; i < base; i++) {
							for (int j = 0; j < base; j++) {
								for (int k = 1; k < base; k++) {
									int higherBit = (i + j + newc1 + k) % base;
									int higherCarry = (i + j + newc1 + k) / base;
									int lowerBit = (x + z + k) % base;
									int lowerCarry = (x + z + k) / base;
									if (higherCarry == c1) {
										cout << "(" << originState <<" c" << higherBit << lowerBit;
										cout << " " << getStateName(newc1, lowerCarry, y, i, j)<<")\n";
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseA::addBtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 1; x < base; x++) {
			string originState = getStateName(c1, 0, 0, x, 0);
			for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
				for (int newx = 0; newx < base; newx++) {
					for (int j = 1; j < base; j++) {
						int bit = (newc1 + newx + j) % base;
						int newCarry = (newc1 + newx + j) / base;
						if (newCarry == c1)
							cout << "(" << originState <<" b" << bit << " " << getStateName(newc1, 0, x, newx, j)<<")\n";
					}	
				}
			}
		}
	}
}

void AutomatonGeneratorCaseA::addAtransitions() {
	string initState = getStateName(0, 0, 0, 0, 0);
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int y = 1; y < base; y++) {
			int bit = (c1 + y) % base;
			int newCarry = (c1 + y) / base;
			if ((bit != 0) && (newCarry == 0))
				cout << "(" << initState<<" a" << bit << " " << getStateName(c1, 0, 0, y, 0)<<")\n";
		}
	}
}

void AutomatonGeneratorCaseA::createStates () {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int y = 0; y < base; y++) {
					for (int z = 0; z < base; z++) {
						cout<<getStateName(c1, c2, x, y, z)<<endl;
					}
				}
			}
		}
	}
}
void AutomatonGeneratorCaseA::createInitialStates() {
	cout << getStateName(0, 0, 0, 0, 0)<<endl;
}
void AutomatonGeneratorCaseA::createFinalStates() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 0; x < base; x++) {
			for (int z = 0; z < base; z++) {
				cout<<getStateName(c1, c1, x, x, z)<<endl;
			}
		}
	}
}

void AutomatonGeneratorCaseA::addTransitions() {
	addAtransitions();
	addBtransitions();
	addCtransitions();
	addDtransitions();
	addEtransitions();
}

class AutomatonGeneratorCaseB {
public:
	string name;
	int maxCarry;
	int base;
	void createStates();
	void createInitialStates();
	void createFinalStates();

	string getStateName(int, int, int, int, int);
	void addBtransitions();
	void addAtransitions();
	void addCtransitions();
	void addDtransitions();
	void addEtransitions();
	void addTransitions();

	AutomatonGeneratorCaseB(int);
};

AutomatonGeneratorCaseB::AutomatonGeneratorCaseB(int b) {
	maxCarry = 2;
	base = b;
}
//Q States are 5-tuples (c1, c2, x, y, z)
// c1, expected higher carry
// c2, produced lower carry
// x, the next lower guess of the n-word
// y, the two-away lower guess of the n-word
// z, the next higher guess of the n-3-word

string AutomatonGeneratorCaseB::getStateName(int c1, int c2, int x, int y, int z) {
	return ("B_"+to_string(c1)+"_"+to_string(c2)+"_"+to_string(x)+to_string(y)+to_string(z));
}

void AutomatonGeneratorCaseB::addEtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int y = 0; y < base; y++) {
					for (int z = 0; z < base; z++) {
						string originState = getStateName(c1, c2, x, y, z);
						for (int j = 0; j < base; j++) {
							int bit = (x + j + c2 + z) % base;
							int carry = (x + j + c2 + z) / base;										
							if (carry == c1) {
								cout << "(" << originState <<" e" << bit <<" acc)\n";
							}
						}
					}
				}
			}
		}
	}
}


void AutomatonGeneratorCaseB::addDtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int y = 0; y < base; y++) {
					for (int z = 0; z < base; z++) {
						string originState = getStateName(c1, c2, x, y, z);
						for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
							for (int i = 0; i < base; i++) {
								for (int j = 0; j < base; j++) {
									for (int k = 0; k < base; k++) {
										int higherBit = (i + j + newc1 + z) % base;
										int higherCarry = (i + j + newc1 + z) / base;
										int lowerBit = (x + j + c2 + k) % base;
										int lowerCarry = (x + j + c2 + k) / base;
										if (higherCarry == c1) {
											cout << "(" << originState <<" d" << higherBit << lowerBit;
											cout << " " << getStateName(newc1, lowerCarry, y, i, k)<<")\n";
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseB::addCtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 1; x < base; x++) {
			for (int y = 0; y < base; y++) {
				string originState = getStateName(c1, 0, x, y, 0);
				for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
					for (int i = 0; i < base; i++) {
						for (int j = 1; j < base; j++) {
							for (int k = 1; k < base; k++) {
								int higherBit = (i + j + newc1) % base;
								int higherCarry = (i + j + newc1) / base;
								int lowerBit = (x + j + k) % base;
								int lowerCarry = (x + j + k) / base;
								if (higherCarry == c1) {
									cout << "(" << originState <<" c" << higherBit << lowerBit;
									cout << " " << getStateName(newc1, lowerCarry, y, i, k)<<")\n";
								}
							}
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseB::addBtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 1; x < base; x++) {
			string originState = getStateName(c1, 0, 0, x, 0);
			for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
				for (int newx = 0; newx < base; newx++) {
					int bit = (newc1 + newx) % base;
					int newCarry = (newc1 + newx) / base;
					if (newCarry == c1)
						cout << "(" << originState <<" b" << bit << " " << getStateName(newc1, 0, x, newx, 0)<<")\n";
				}
			}
		}
	}
}

void AutomatonGeneratorCaseB::addAtransitions() {
	string initState = getStateName(0, 0, 0, 0, 0);
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int y = 1; y < base; y++) {
			int bit = (c1 + y) % base;
			int newCarry = (c1 + y) / base;
			if ((bit != 0) && (newCarry == 0))
				cout << "(" << initState<<" a" << bit << " " << getStateName(c1, 0, 0, y, 0)<<")\n";
		}
	}
}

void AutomatonGeneratorCaseB::createStates () {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int y = 0; y < base; y++) {
					for (int z = 0; z < base; z++) {
						cout<<getStateName(c1, c2, x, y, z)<<endl;
					}
				}
			}
		}
	}
}
void AutomatonGeneratorCaseB::createInitialStates() {
	cout << getStateName(0, 0, 0, 0, 0)<<endl;
}
void AutomatonGeneratorCaseB::createFinalStates() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 0; x < base; x++) {
			for (int z = 0; z < base; z++) {
				cout<<getStateName(c1, c1, x, x, z)<<endl;
			}
		}
	}
}

void AutomatonGeneratorCaseB::addTransitions() {
	addAtransitions();
	addBtransitions();
	addCtransitions();
	addDtransitions();
	addEtransitions();
}

class AutomatonGeneratorCaseC {
public:
	int maxCarry;
	int base;
	void createStates();
	void createInitialStates();
	void createFinalStates();

	string getStateName(int, int, int, int);
	void addBtransitions();
	void addAtransitions();
	void addCtransitions();
	void addDtransitions();
	void addEtransitions();
	void addTransitions();
	AutomatonGeneratorCaseC(int);
};

AutomatonGeneratorCaseC::AutomatonGeneratorCaseC(int b) {
	maxCarry = 2;
	base = b;
}

//Q States are -tuples (c1, c2, x, z)
// c1, expected higher carry
// c2, produced lower carry
// x, the next lower guess of the n-1-word
// z, the next higher guess of the n-3-word

string AutomatonGeneratorCaseC::getStateName(int c1, int c2, int x, int z) {
	return ("C_"+to_string(c1)+"_"+to_string(c2)+"_"+to_string(x)+to_string(z));
}

void AutomatonGeneratorCaseC::addEtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int z = 0; z < base; z++) {
					string originState = getStateName(c1, c2, x, z);
					for (int j = 0; j < base; j++) {
							int bit = (x + c2 + j + z) % base;
							int carry = (x + c2 + j + z) / base;
							if (carry == c1) {
								cout << "(" << originState <<" e" << bit << " acc)\n";
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseC::addDtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int z = 0; z < base; z++) {
					string originState = getStateName(c1, c2, x, z);
					for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
						for (int i = 0; i < base; i++) {
							for (int j = 0; j < base; j++) {
								for (int k = 0; k < base; k++) {
									int higherBit = (i + j + newc1 + z) % base;
									int higherCarry = (i + j + newc1 + z) / base;
									int lowerBit = (x + j + c2 + k) % base;
									int lowerCarry = (x + j + c2 + k) / base;
									if (higherCarry == c1) {
										cout << "(" << originState <<" d" << higherBit << lowerBit;
										cout << " " << getStateName(newc1, lowerCarry, i, k)<<")\n";
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseC::addCtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 1; x < base; x++) {
			string originState = getStateName(c1, 0, x, 0);
			for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
				for (int i = 0; i < base; i++) {
					for (int j = 1; j < base; j++) {
						for (int k = 1; k < base; k++) {
							int higherBit = (i + j + newc1) % base;
							int higherCarry = (i + j + newc1) / base;
							int lowerBit = (x + j + k) % base;
							int lowerCarry = (x + j + k) / base;
							if (higherCarry == c1) {
								cout << "(" << originState <<" c" << higherBit << lowerBit;
								cout << " " << getStateName(newc1, lowerCarry, i, k)<<")\n";
							}
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseC::addBtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		string originState = getStateName(c1, 0, 0, 0);
		for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
			for (int newx = 1; newx < base; newx++) {
				int bit = (newc1 + newx) % base;
				int newCarry = (newc1 + newx) / base;
				if (newCarry == c1)
					cout << "(" << originState <<" b" << bit << " " << getStateName(newc1, 0, newx, 0)<<")\n";
			}
		}
	}
}

void AutomatonGeneratorCaseC::addAtransitions() {
	string initState = getStateName(0, 0, 0, 0);
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		int bit = c1 % base;
		if (bit != 0)
			cout << "(" << initState<<" a" << bit << " " << getStateName(c1, 0, 0, 0)<<")\n";
	}
}

void AutomatonGeneratorCaseC::createStates () {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int x = 0; x < base; x++) {
				for (int z = 0; z < base; z++) {
					cout<<getStateName(c1, c2, x, z)<<endl;
				}
			}
		}
	}
}

void AutomatonGeneratorCaseC::createInitialStates() {
	cout << getStateName(0, 0, 0, 0)<<endl;
}

void AutomatonGeneratorCaseC::createFinalStates() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int x = 0; x < base; x++) {
			for (int z = 0; z < base; z++) {
				cout<<getStateName(c1, c1, x, z)<<endl;
			}
		}
	}
}

void AutomatonGeneratorCaseC::addTransitions() {
	addAtransitions();
	addBtransitions();
	addCtransitions();
	addDtransitions();
	addEtransitions();
}

class AutomatonGeneratorCaseD {
public:
	string name;
	int maxCarry;
	int base;
	void createStates();
	void createInitialStates();
	void createFinalStates();
	string getStateName(int, int, int);
	void addBtransitions();
	void addAtransitions();
	void addCtransitions();
	void addDtransitions();
	void addEtransitions();
	void addTransitions();
	AutomatonGeneratorCaseD(int);
};

AutomatonGeneratorCaseD::AutomatonGeneratorCaseD(int b) {
	maxCarry = 2;
	base = b;
}

//Q States are 3-tuples (c1, c2, z)
// c1, expected higher carry
// c2, produced lower carry
// z, the next lower guess of the n-1-word

string AutomatonGeneratorCaseD::getStateName(int c1, int c2, int z) {
	return ("D_"+to_string(c1)+"_"+to_string(c2)+"_"+to_string(z));
}

void AutomatonGeneratorCaseD::addEtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int z = 0; z < base; z++) {
				string originState = getStateName(c1, c2, z);
				for (int k = 0; k < base; k++) {
					int bit = (c2 + k + z) % base;
					int carry = (c2 + k + z) / base;
					if (carry == c1) {
						cout << "(" << originState <<" e" << bit << " acc)\n";
					}
				}
			}
		}
	}
}


void AutomatonGeneratorCaseD::addDtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int z = 0; z < base; z++) {
				string originState = getStateName(c1, c2, z);
				for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
					for (int j = 0; j < base; j++) {
						for (int k = 0; k < base; k++) {
							int higherBit = (j + newc1 + k) % base;
							int higherCarry = (j + newc1 + k) / base;
							int lowerBit = (z + c2 + k) % base;
							int lowerCarry = (z + c2 + k) / base;
							if (higherCarry == c1) {
								cout << "(" << originState <<" d" << higherBit << lowerBit;
								cout << " " << getStateName(newc1, lowerCarry, j)<<")\n";
							}
						}
					}
				}
			}
		}
	}
}

void AutomatonGeneratorCaseD::addCtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int z = 1; z < base; z++) {
			string originState = getStateName(c1, 0, z);
			for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
				for (int j = 0; j < base; j++) {
					for (int k = 1; k < base; k++) {
						int higherBit = (j + newc1 + k) % base;
						int higherCarry = (j + newc1 + k) / base;
						int lowerBit = (z + k) % base;
						int lowerCarry = (z + k) / base;
						if (higherCarry == c1) {
							cout << "(" << originState <<" c" << higherBit << lowerBit;
							cout << " " << getStateName(newc1, lowerCarry, j)<<")\n";
						}
					}
				}
			}
		}
	}
}
void AutomatonGeneratorCaseD::addBtransitions() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		string originState = getStateName(c1, 0, 0);
		for (int newc1 = 0; newc1 <= maxCarry; newc1++) {
			for (int j = 1; j < base; j++) {
				int bit = (newc1  + j) % base;
				int newCarry = (newc1 + j) / base;
				if (newCarry == c1)
					cout << "(" << originState <<" b" << bit << " " << getStateName(newc1, 0, j)<<")\n";
			}	
		}
	}
}

void AutomatonGeneratorCaseD::addAtransitions() {
	string initState = getStateName(0, 0, 0);
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		int bit = c1 % base;
		if (bit != 0)
			cout << "(" << initState<<" a" << bit << " " << getStateName(c1, 0, 0)<<")\n";
	}
}

void AutomatonGeneratorCaseD::createStates () {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int c2 = 0; c2 <= maxCarry; c2++) {
			for (int z = 0; z < base; z++) {
				cout<<getStateName(c1, c2, z)<<endl;
			}
		}
	}
}
void AutomatonGeneratorCaseD::createInitialStates() {
	cout << getStateName(0, 0, 0)<<endl;
}
void AutomatonGeneratorCaseD::createFinalStates() {
	for (int c1 = 0; c1 <= maxCarry; c1++) {
		for (int z = 0; z < base; z++) {
			cout<<getStateName(c1, c1, z)<<endl;
		}
	}
}

void AutomatonGeneratorCaseD::addTransitions() {
	addAtransitions();
	addBtransitions();
	addCtransitions();
	addDtransitions();
	addEtransitions();
}

// This main() generates the code for our base 3 result,
// but can be easily modified to work for base 4. Simply pass '4' as the
// argument to generators for Cases b and c.

int main() {
	AutomatonGeneratorCaseA a (3);
	AutomatonGeneratorCaseB b (3);
	AutomatonGeneratorCaseC c (3);
	AutomatonGeneratorCaseD d (3);
	cout << "FiniteAutomaton palBase3 = (\n";
	//the first input symbol is either a1/a2, then we read b0/b1/b2,
	//then a c-pair, then d-pairs, and finally a terminal e-element if odd-length input
	cout << "alphabet = {a1 a2 b0 b1 b2 c00 c01 c02 c10 c11 c12 c20 c21 c22 " 
			<< "d00 d01 d02 d10 d11 d12 d20 d21 d22 e0 e1 e2},\n";
	cout << "states = { \n";
	a.createStates();
	b.createStates();
	c.createStates();
	d.createStates();
	cout << "acc},\ninitialStates = {";
	a.createInitialStates();
	b.createInitialStates();
	c.createInitialStates();
	d.createInitialStates();
	cout<<"},\nfinalStates = {";
	a.createFinalStates();
	b.createFinalStates();
	c.createFinalStates();
	d.createFinalStates();
	cout<<"acc},\n";
	cout << "transitions = {\n";
	a.addTransitions();
	b.addTransitions();
	c.addTransitions();
	d.addTransitions();
	cout << "}\n";
	cout << ");\nprint(numberOfStates(palBase3));\n";
	cout << "FiniteAutomaton finalAut = shrinkNwa(palBase3);\n";
	cout << "print(numberOfStates(finalAut));\n\n\n\n\n\n";
}