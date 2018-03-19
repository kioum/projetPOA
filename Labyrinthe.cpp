#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touch?.
Sound*	Chasseur::_wall_hit;	// on a tap? un mur.

Environnement* Environnement::init (char* filename)
{
    return new Labyrinthe (filename);
}

//Permet de savoir si un char est dans un char*
int estDansTabChar(char* tab, int nb, char c)
{
    for(int i =0; i< nb; i++)
        if(tab[i] == c)
             return i;

    return -1;
}

// 0 vide, 1 mur, 2 caisse, 3 tresor, 4 gardien, 5 chasseur
Labyrinthe::Labyrinthe (char* filename)
{
    ifstream fichier(filename, ios::in);

    if(fichier)
    {
        // creation des variables necessaires
        static Wall walls[10]; //murs
        static Box caisses[10]; //caisses

        int i=0, j=0;
        int nbWall=0, nbCaisses=0;

        //initialisation du labyrinthe
        int nbLigne = 10;
        int nbColonne = 10;
        char laby[nbLigne][nbColonne];

        //Obtenir les affiches
        int nbAffiche=0, nbAfficheDispo=0;
        char afficheDispo[10]; // lettre pour une affiche
        const char* textAffiche[10]; // lettre pour une affiche
        static Wall affiche[10]; //affiche

        // Test du premier parcours et initalisation d'un tableau char
        string ligne;
        while(getline(fichier, ligne))
        {
            if(ligne[0] != '+' && ligne[0] != '|' && estDansTabChar(afficheDispo, nbAfficheDispo, ligne[0]) == -1)
            {
                // faire des trucs
                if(ligne[0] != ' ')
                {
                    afficheDispo[nbAfficheDispo] = ligne[0];
                    j=1;
                    while(ligne[j] == ' ') j++; // ici on passe tous les espaces
                    if(ligne.length() > j)
                    {
                        textAffiche[nbAfficheDispo] = (ligne.substr(j, ligne.length())).c_str();
                        nbAfficheDispo++;
                    }
                }

            }
            else
            {
                for(j = 0; j < ligne.length(); j++)
                {
                    laby[i][j] = ligne[j];
                }
                i++;
            }
        }

        // Wall horizontaux
        for(i = 0; i < nbLigne; i++)
        {
            int wall[4];
            wall[1]=-1;
            wall[4]=-1;
            wall[0] = i;
            wall[2] = i;

            for(j = 0; j < nbColonne; j++)
            {
                if(wall[1] != -1 && laby[i][j] == '+')
                {
                    wall[3] = j;
                    walls[nbWall] = {wall[0], wall[1], wall[2], wall[3], 0};
                    nbWall = nbWall + 1;
                }

                if(laby[i][j] == '+' && laby[i][j+1] == '-')
                {
                    wall[1] = j;
                }

                //Initialisation des caisses
                if(laby[i][j] == 'X')
                {
                    caisses[nbCaisses] = {i,j, 0};
                    _data [i][j] = 1;
                    nbCaisses++;
                }

                // Initialisation du tresor
                if(laby[i][j] == 'T')
                {
                    _treasor._x = i;
                    _treasor._y = j;
                    _data [_treasor._x][_treasor._y] = 1;
                }

                //initialisation des affiches horizontaux
                if(estDansTabChar(afficheDispo, nbAfficheDispo, laby[i][j]) != -1 && laby[i][j+1] == '-'
                        && laby[i][j-1] == '-')
                {
                    int texture = estDansTabChar(afficheDispo, nbAfficheDispo, laby[i][j]);
                    char tmp [128];
                    sprintf (tmp, "%s/%s", texture_dir, textAffiche[texture]);
                    affiche[nbAffiche] = {wall[0], j-1, wall[2], j+1, 0};
                    affiche[nbAffiche]._ntex = wall_texture(tmp);
                    nbAffiche++;
                }
            }
        }

        // Wall verticaux
        for(i = 0; i < nbColonne; i++)
        {
            int wall[4];
            wall[1]=-1;
            wall[4]=-1;
            wall[0] = i;
            wall[2] = i;

            for(j = 0; j < nbLigne; j++)
            {
                if(wall[1] != -1 && laby[j][i] == '+')
                {
                    wall[3] = j;
                    walls[nbWall] = {wall[1], wall[0], wall[3], wall[2], 0};
                    nbWall = nbWall + 1;
                }
                if(laby[j][i] == '+' && laby[j+1][i] == '|')
                {
                    wall[1] = j;
                }
/*
                //initialisation des affiches verticaux
                 if(estDansTabChar(afficheDispo, nbAfficheDispo, laby[i][j]) && laby[j+1][i] == '|'
                    && laby[j-1][i] == '|'){
                    int texture = estDansTabChar(afficheDispo, nbAfficheDispo, laby[i][j]);
                    char tmp [128];
                    sprintf (tmp, "%s/%s", texture_dir, textAffiche[texture]);
                    affiche[nbAffiche] = {wall[1], j-1, wall[3], j+1, 0};
                    affiche[nbAffiche]._ntex = wall_texture(tmp);
                    nbAffiche++;
                 }*/
            }
        }

        _nwall = nbWall;
        _walls = walls;

        _nboxes = nbCaisses;
        _boxes = caisses;


        _npicts = nbAffiche;
        _picts = affiche;
        //temporaire pour tester
        _nguards = 1;
        _guards = new Mover* [_nguards];
        _guards [0] = new Chasseur (this);

        fichier.close();
    }
    else cerr << "Fichier : " << filename << " inexistant !" << endl;
}


/*
Labyrinthe::Labyrinthe (char* filename)
{
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
