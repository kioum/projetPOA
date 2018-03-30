#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"

//met les valeurs du chemin
void chemin(int x, int y, int debut)
{

}

class Gardien : public Mover
{
public:
    Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele)
    { this->etat =2;}

    int etat; //0 -> Inactif //1 -> Attaque // 2 -> Defense

    // mon gardien pense très mal!
    void update (void)
    {
        if(this->etat == 1)
        {

        }
        else if(this->etat == 2)
        {
            //char parcours[_l->width()][_l->height()];
           // parcours[0][0] = 1;
            //printf("%i ici",parcours[0][0]);
            chemin((int)_l->_treasor._x, (int)_l->_treasor._y, 1);
            for(int i=0; i<_l->width(); i++){
                for(int j=0; j<_l->height(); j++){
                     printf("%i ", _l->data(i, j));
                }
               printf("\n");
            }
             printf("-----------------------------------\n");
        }


    };
    // et ne bouge pas!
    bool move (double dx, double dy)
    {
        if (EMPTY == _l -> data ((int)((_x + dx) / Environnement::scale),
							 (int)((_y + dy) / Environnement::scale)))
        {
            _x += dx;
            _y += dy;
            return true;
        }
        return false;
    }
    // ne sait pas tirer sur un ennemi.
    void fire (int angle_vertical) {}
    // quand a faire bouger la boule de feu...
    bool process_fireball (float dx, float dy)
    {
        return false;
    }
};
#endif
