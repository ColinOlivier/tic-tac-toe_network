#include "Morpion.h"
#include <iostream>

CMorpion::CMorpion()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			tab[i][j] = 0;
		}
	}
}

void CMorpion::print()
{
	std::cout << " " + getJoueur(0,0) + ' | ' + getJoueur(0, 1) + ' ┃ ' + getJoueur(0, 2) << std::endl;
	std::cout << "━━━╋━━━╋━━━" << std::endl;
	std::cout << " " + getJoueur(1, 0) + ' | ' + getJoueur(1, 1) + ' ┃ ' + getJoueur(1, 2) << std::endl;
	std::cout << "━━━╋━━━╋━━━" << std::endl;
	std::cout << " " + getJoueur(2, 0) + ' | ' + getJoueur(2, 1) + ' ┃ ' + getJoueur(2, 2) << std::endl;
}

char CMorpion::getJoueur(int x, int y)
{
	if (x < 0 || x > 2 || y < 0 || y > 2) return '?';
	switch (tab[x][y])
	{
	case 0:
		return ' ';
		break;
	case 1:
		return 'O';
		break;
	case 2:
		return 'X';
		break;
	default:
		return '?';
		break;
	} 
}

void CMorpion::setJoueur(int x, int y, int joueur)
{
	if (x < 0 || x > 2 || y < 0 || y > 2) return;
	tab[x][y] = joueur;
}

int CMorpion::IsVainqueur()
{
	for (size_t i = 0; i < 2; i++)
	{
		if (getJoueur(0, i) == getJoueur(1, i) && getJoueur(0, i) == getJoueur(2, i) && getJoueur(0, i) != 0) return getJoueur(0, i);
		if (getJoueur(i, 0) == getJoueur(i, 1) && getJoueur(i, 0) == getJoueur(i, 2) && getJoueur(i, 0) != 0) return getJoueur(i, 0);
	}
	if (getJoueur(0, 0) == getJoueur(1, 1) && getJoueur(0, 0) == getJoueur(2, 2) && getJoueur(0, 0) != 0) return getJoueur(0, 0);
	if (getJoueur(2, 0) == getJoueur(1, 1) && getJoueur(0, 0) == getJoueur(2, 0) && getJoueur(0, 2) != 0) return getJoueur(2, 0);
	return 0;
}
