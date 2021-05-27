#pragma once
#include <string>

class CMorpion {
	int tab[3][3];
public:
	CMorpion();
	void print();
	std::string getJoueur(int, int);
	void setJoueur(int, int, int);
	int IsVainqueur();

	void getMove(int);

	void win();
	void loss();

};