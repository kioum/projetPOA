#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"

class Labyrinthe : public Environnement {
private:
	char	**_data;
	int largeur;
	int longueur;

public:
	Labyrinthe (char*);
	int setHeightWidth(int w, int h) {largeur = w; longueur = h;} //permet de donner une taille du labyrinthe
	int width () { return largeur;}	// retourne la largeur du labyrinthe.
	int height () { return longueur;}	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return _data [i][j];
	}	// retourne la case (i, j).
	char setData (int i, int j, int val)
	{
		_data [i][j] = val;
	}	// donne une valeur la case (i, j).
};

#endif
