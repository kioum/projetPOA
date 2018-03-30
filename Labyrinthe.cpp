#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"

#include <iostream>
#include <string>
#include <fstream>

#include <vector>

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
        int i=0, j=0;
        int nbWall=0, nbCaisses=0, nbGuard=0;

        //initialisation du labyrinthe
        int nbColonne = 0;
        vector<string> laby;

        //Obtenir les affiches
        int nbAffiche=0, nbAfficheDispo=0;
        char afficheDispo[24]; // lettre pour une affiche
        const char* textAffiche[24]; // lettre pour une affiche

        // Test du premier parcours et initalisation d'un tableau char
        string ligne;
        while(getline(fichier, ligne))
        {
            if(ligne[0] != ' ' && ligne[0] != '+' && ligne[0] != '|' &&
               estDansTabChar(afficheDispo, nbAfficheDispo, ligne[0]) == -1)
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
            else
            {
                if(ligne.length() > nbColonne)
                    nbColonne = ligne.length();

                for(int k=0; k < ligne.length(); k++){
                    if(ligne[k] == 'G' || ligne[k] == 'C')
                        nbGuard++;
                    if(ligne[k]== 'X')
                        nbCaisses++;
                    if(ligne[k]== '+')
                        nbWall++;
                    if(estDansTabChar(afficheDispo, nbAfficheDispo, ligne[k]) != -1)
                        nbAffiche++;
                }
                cout << ligne << endl; // ligne de debuguage
                laby.push_back(ligne);
            }
        }

        //Initialisation des gardiens + chasseur
        _nguards = nbGuard;
        _guards = new Mover* [_nguards];
        nbGuard = 0;

        //Tableau des caisses
        _nboxes = nbCaisses;
        _boxes = new Box[_nboxes];
        nbCaisses = 0;

        //Tableau des walls
        _nwall = nbWall;
        _walls = new Wall[_nwall];
        nbWall = 0;

        //Tableau des affiches
        _npicts = nbAffiche;
        _picts = new Wall[_npicts];
        nbAffiche = 0;

        // Wall horizontaux
        for(i = 0; i < laby.size(); i++)
        {
            int wall[4];
            wall[1]=-1;
            wall[3]=-1;
            wall[0] = i;
            wall[2] = i;

            for(j = 0; j < laby[i].length(); j++)
            {
                if(wall[1] != -1 && laby[i][j] == '+')
                {
                    wall[3] = j;
                    _walls[nbWall] = {wall[0], wall[1], wall[2], wall[3], 0};
                    nbWall++;
                    wall[1] = -1;
                }

                if(laby[i][j] == '+' && (laby[i][j+1] == '-' || laby[i][j+1] == '+'))
                {
                    wall[1] = j;
                }

                //Tous les murs  on ne peut pas empecher
                if(laby[i][j] == '+' || laby[i][j] == '-' || laby[i][j] == '|' ||
                   estDansTabChar(afficheDispo, nbAfficheDispo, laby[i][j]) != -1)
                {
                    _data [i][j] = 1;
                }

                //Initialisation des caisses
                if(laby[i][j] == 'X')
                {
                    _boxes[nbCaisses] = {i,j, 0};
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

                // Initialisation du chasseur
                if(laby[i][j] == 'C')
                {
                    _guards [0] = new Chasseur (this);
                    _guards [0] -> _x = i*10.; _guards [0] -> _y = j*10.;
                }

                // Initialisation du chasseur
                if(laby[i][j] == 'G')
                {
                    _guards [nbGuard+1] = new Gardien (this, "Potator");
                    _guards [nbGuard+1] -> _x = i*10.; _guards [nbGuard+1] -> _y = j*10.;
                    _data [(int)(_guards [nbGuard+1] -> _x / scale)][(int)(_guards [nbGuard+1] -> _y / scale)] = 1;
                    nbGuard++;
                }

                //initialisation des affiches horizontaux
                if(estDansTabChar(afficheDispo, nbAfficheDispo, laby[i][j]) != -1 && laby[i][j+1] == '-'
                        && laby[i][j-1] == '-')
                {
                    int texture = estDansTabChar(afficheDispo, nbAfficheDispo, laby[i][j]);
                    char tmp [128];
                    sprintf (tmp, "%s/%s", texture_dir, textAffiche[texture]);
                    _picts[nbAffiche] = {wall[0], j-1, wall[2], j+1, 0};
                    _picts[nbAffiche]._ntex = wall_texture(tmp);
                    nbAffiche++;
                }
            }
        }

        // Wall verticaux
        j = 0;
        for(i = 0; i < nbColonne; i++)
        {
            int wall[4];
            wall[1] = -1;
            wall[3] = -1;
            wall[0] = i;
            wall[2] = i;

            for(j = 0; j < laby.size(); j++)
            {
                if(wall[1] != -1 && laby[j][i] == '+')
                {
                    wall[3] = j;
                    //cout << "[" << wall[1] <<","<< wall[0] << "]" << "["<< wall[3] <<","<< wall[2] << "]" << endl;
                    _walls[nbWall] = {wall[1], wall[0], wall[3], wall[2], 0};
                    nbWall++;
                    wall[1]=-1;
                }
                if(j+1 < laby.size() && laby[j][i] == '+' && (laby[j+1][i] == '|' || laby[j+1][i] == '+'))
                {
                    wall[1] = j;
                }

                //initialisation des affiches verticaux
                 if(j-1 > 0 && j+1 < laby.size() && estDansTabChar(afficheDispo, nbAfficheDispo, laby[j][i]) != -1
                    && laby[j+1][i] == '|' && laby[j-1][i] == '|'){
                    int texture = estDansTabChar(afficheDispo, nbAfficheDispo, laby[j][i]);
                    char tmp [128];
                    sprintf (tmp, "%s/%s", texture_dir, textAffiche[texture]);
                    _picts[nbAffiche] = {j-1, wall[0], j+1, wall[2], 0};
                    _picts[nbAffiche]._ntex = wall_texture(tmp);
                    nbAffiche++;
                 }
            }
        }

        fichier.close();
    }
    else cerr << "Fichier : " << filename << " inexistant !" << endl;
}
