#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"

#define	LAB_WIDTH	80
#define	LAB_HEIGHT	25

class Labyrinthe : public Environnement {
private:
	char	_data [LAB_WIDTH][LAB_HEIGHT];

public:
	Labyrinthe (char*);
	int width () { return LAB_WIDTH;}	// retourne la largeur du labyrinthe.
	int height () { return LAB_HEIGHT;}	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return _data [i][j];
	}	// retourne la case (i, j).
};

#endif

/*
	// les 4 murs.
	static Wall walls [] = {
		{ 0, 0, LAB_WIDTH-1, 0, 0 },
		{ LAB_WIDTH-1, 0, LAB_WIDTH-1, LAB_HEIGHT-1, 0 },
		{ LAB_WIDTH-1, LAB_HEIGHT-1, 0, LAB_HEIGHT-1, 0 },
		{ 0, LAB_HEIGHT-1, 0, 0, 0 },
	};
	// une affiche.
	//  (attention: pour des raisons de rapport d'aspect, les affiches doivent faire 2 de long)
	static Wall affiche [] = {
		{ 4, 0, 6, 0, 0 },		// premi?re affiche.
		{ 8, 0, 10, 0, 0 },		// autre affiche.
	};
	// 3 caisses.
	static Box	caisses [] = {
		{ 70, 12, 0 },
		{ 10, 5, 0 },
		{ 65, 22, 0 },
	};
	// juste un mur autour et les 3 caisses et le tr?sor dedans.
	for (int i = 0; i < LAB_WIDTH; ++i)
		for (int j = 0; j < LAB_HEIGHT; ++j) {
			if (i == 0 || i == LAB_WIDTH-1 || j == 0 || j == LAB_HEIGHT-1)
				_data [i][j] = 1;
			else
				_data [i][j] = EMPTY;
		}
	// indiquer qu'on ne marche pas sur une caisse.
	_data [caisses [0]._x][caisses [0]._y] = 1;
	_data [caisses [1]._x][caisses [1]._y] = 1;
	_data [caisses [2]._x][caisses [2]._y] = 1;
	// les 4 murs.
	_nwall = 4;
	_walls = walls;
	// deux affiches.
	_npicts = 2;
	_picts = affiche;
	// la deuxi?me ? une texture diff?rente.
	char	tmp [128];
	sprintf (tmp, "%s/%s", texture_dir, "voiture.jpg");
	_picts [1]._ntex = wall_texture (tmp);
	// 3 caisses.
	_nboxes = 3;
	_boxes = caisses;
	// le tr?sor.
	_treasor._x = 10;
	_treasor._y = 10;
	_data [_treasor._x][_treasor._y] = 1;	// indiquer qu'on ne marche pas sur le tr?sor.
	// le chasseur et les 4 gardiens.
	_nguards = 5;
	_guards = new Mover* [_nguards];
	_guards [0] = new Chasseur (this);
	_guards [1] = new Gardien (this, "drfreak");
	_guards [2] = new Gardien (this, "Marvin"); _guards [2] -> _x = 90.; _guards [2] -> _y = 70.;
	_guards [3] = new Gardien (this, "Potator"); _guards [3] -> _x = 60.; _guards [3] -> _y = 10.;
	_guards [4] = new Gardien (this, "garde"); _guards [4] -> _x = 130.; _guards [4] -> _y = 100.;
	// indiquer qu'on ne marche pas sur les gardiens.
	_data [(int)(_guards [1] -> _x / scale)][(int)(_guards [1] -> _y / scale)] = 1;
	_data [(int)(_guards [2] -> _x / scale)][(int)(_guards [2] -> _y / scale)] = 1;
	_data [(int)(_guards [3] -> _x / scale)][(int)(_guards [3] -> _y / scale)] = 1;
	_data [(int)(_guards [4] -> _x / scale)][(int)(_guards [4] -> _y / scale)] = 1;
}

*/
