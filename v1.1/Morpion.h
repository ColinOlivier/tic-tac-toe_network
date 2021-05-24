#pragma once
class CMorpion {
	int tab[3][3];
public:
	CMorpion();
	void print();
	char getJoueur(int, int);
	void setJoueur(int, int, int);
	int IsVainqueur();
};