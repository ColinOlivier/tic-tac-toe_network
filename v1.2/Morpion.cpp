#include "Morpion.h"
#include <string>
#include <iostream>

CMorpion::CMorpion()
{
	for (size_t i = 0; i <= 2; i++)
	{
		for (size_t j = 0; j <= 2; j++)
		{
			tab[i][j] = 0;
		}
	}
}

void CMorpion::print()
{
	std::cout << " " + getJoueur(0, 0) + " | " + getJoueur(0, 1) + " | " + getJoueur(0, 2) << std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << " " + getJoueur(1, 0) + " | " + getJoueur(1, 1) + " | " + getJoueur(1, 2) << std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << " " + getJoueur(2, 0) + " | " + getJoueur(2, 1) + " | " + getJoueur(2, 2) << std::endl;
	std::cout << "\n";
}



std::string CMorpion::getJoueur(int x, int y)
{
	if (x < 0 || x > 2 || y < 0 || y > 2) return " ";
	switch (tab[x][y])
	{
	case 1:
		return "O";
		break;
	case 2:
		return "X";
		break;
	default:
		return " ";
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
	for (size_t i = 0; i <= 2; i++)
	{
		if (tab[0][i] == tab[1][i] && tab[0][i] == tab[2][i] && tab[0][i] != 0) return tab[0][i]; // Verification des lignes
		if (tab[i][0] == tab[i][1] && tab[i][0] == tab[i][2] && tab[i][0] != 0) return tab[i][0]; // Verification des colonnes
	}
	if (tab[0][0] == tab[1][1] && tab[0][0] == tab[2][2] && tab[0][0] != 0) return tab[0][0]; // Verification des diagonales
	if (tab[2][0] == tab[1][1] && tab[2][0] == tab[0][2] && tab[2][0] != 0) return tab[2][0];
	return 0;
}

void CMorpion::getMove(int joueur)
{
	std::cout << "\t Vous etes le joueur " << joueur << " ! \n" << std::endl;
	int x = -1;
	int y = -1;
	bool jouable = false;
	std::cout << "A vous de jouez : " << std::endl;
	print();
	while (!jouable)
	{
		std::cout << "Position : " << std::endl;
		std::cout << "Colonne : ";
		std::cin >> x;

		std::cout << "Ligne : ";
		std::cin >> y;
		if (x >= 0 && x <= 2 && y >= 0 && y <= 2) {
			if (tab[x][y] == 1 || tab[x][y] == 2) {
				std::cout << "Position deja jouer ! " << std::endl;
			}
			else {
				jouable = true;
			}
		}
	}

	setJoueur(x, y, joueur);
	system("cls");
	print();
}

void CMorpion::win()
{
	system("cls");
	std::cout << "\n\n\n";
	std::cout << "\t_________________________________" << std::endl;
	std::cout << "\t|                               |" << std::endl;
	std::cout << "\t|            BRAVO !!!          |" << std::endl;
	std::cout << "\t|        Vous avez gagné !      |" << std::endl;
	std::cout << "\t|_______________________________|" << std::endl;
	std::cout << "\n\n";
	std::cout << "\t_________________________________" << std::endl;
	std::cout << "\t|                               |" << std::endl;
	std::cout << "\t|          Tic-Tac-Toe          |" << std::endl;
	std::cout << "\t|        by Colin OLIVIER       |" << std::endl;
	std::cout << "\t|                               |" << std::endl;
	std::cout << "\t|          28/05/2021           |" << std::endl;
	std::cout << "\t|_______________________________|" << std::endl;
	std::cout << "\n\n\n";
}

void CMorpion::loss()
{
	system("cls");
	std::cout << "\n\n\n";
	std::cout << "\t_________________________________" << std::endl;
	std::cout << "\t|                               |" << std::endl;
	std::cout << "\t|           Dommage !!!         |" << std::endl;
	std::cout << "\t|        Vous avez perdu !      |" << std::endl;
	std::cout << "\t|_______________________________|" << std::endl;
	std::cout << "\n\n";
	std::cout << "\t_________________________________" << std::endl;
	std::cout << "\t|                               |" << std::endl;
	std::cout << "\t|          Tic-Tac-Toe          |" << std::endl;
	std::cout << "\t|        by Colin OLIVIER       |" << std::endl;
	std::cout << "\t|                               |" << std::endl;
	std::cout << "\t|          28/05/2021           |" << std::endl;
	std::cout << "\t|_______________________________|" << std::endl;
	std::cout << "\n\n\n";
}
