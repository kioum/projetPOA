#include "Chasseur.h"

//Pour le cast
#include "Labyrinthe.h"
#include "Gardien.h"
#include <cstdlib>
#include <ctime>
/*
 *	Tente un deplacement.
 */

bool Chasseur::move_aux (double dx, double dy)
{
	int new_x = (int)((_x + dx) / Environnement::scale);
	int new_y = (int)((_y + dy) / Environnement::scale);
	if (EMPTY == _l -> data (new_x, new_y))
	{
		_x += dx;
		_y += dy;
		return true;
	}else if(_l->_treasor._x == new_x && _l->_treasor._y == new_y){
		partie_terminee(true);
	}
	return false;
}

/*
 *	Constructeur.
 */

Chasseur::Chasseur (Labyrinthe* l) : Mover (100, 80, l, 0)
{
	srand (time(NULL));
	_hunter_fire = new Sound ("sons/hunter_fire.wav");
	_hunter_hit = new Sound ("sons/hunter_hit.wav");
	if (_wall_hit == 0)
		_wall_hit = new Sound ("sons/hit_wall.wav");
	
	this->pdv = 45;
	this->coefTouche = 3;
	this->timeTouche = 1000;
}

/*
 *	Fait bouger la boule de feu (ceci est une exemple, à vous de traiter les collisions spécifiques...)
 */

bool Chasseur::process_fireball (float dx, float dy)
{
	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb -> get_x ()) / Environnement::scale;
	float	y = (_y - _fb -> get_y ()) / Environnement::scale;
	float	dist2 = x*x + y*y;
	// on bouge que dans le vide!
	int new_fbX = (int)((_fb -> get_x () + dx) / Environnement::scale);
	int new_fbY = (int)((_fb -> get_y () + dy) / Environnement::scale);
	if (EMPTY == _l -> data (new_fbX, new_fbY))
	{
		//message ("Woooshh ..... %d", (int) dist2);
		// il y a la place.
		return true;
	}
	
	// collision...
	// calculer la distance maximum en ligne droite.
	float	dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
	
	int i = 1;
	for (i = 1; i < _l->_nguards; i++){
		if(new_fbX == (int)_l->_guards[i]->_x/Environnement::scale && 
		   new_fbY == (int)_l->_guards[i]->_y/Environnement::scale){
			//On cast le Mover en tant que Gardien
			Gardien* guard = (Gardien*)_l->_guards[i];

			//On enleve de l'hp et on le fait tomber
			guard->pdv -= 25;
			guard->_guards_hit->play(1. - dist2/dmax2);
			guard->tomber();

			//On l'enleve de la liste
			if(guard->pdv <= 0){
				guard->_guards_die->play(1. - dist2/dmax2);
				((Labyrinthe*)_l)->setData(new_fbX, new_fbY, 0);
				for (int j = i; j < _l->_nguards-1; ++j){
					_l->_guards[j] = _l->_guards[j + 1];
				}
				_l->_nguards--;
			}
			return false;
		}
	}

	// faire exploser la boule de feu avec un bruit fonction de la distance.
	_wall_hit -> play (1. - dist2/dmax2);
	//message ("Booom...");
	return false;
}

/*
 *	Tire sur un ennemi.
 */

void Chasseur::fire (int angle_vertical)
{
	//message ("Woooshh...");
	_hunter_fire -> play ();
	int random = rand() % 2;
	int angle_;
	
	// A revoir
	int random2 = rand() % ((int)(100*this->coefTouche/this->pdv));
	
	if(random)
		angle_ = (_angle-this->coefTouche)+(this->coefTouche - random2);
	else
		angle_ = (_angle+this->coefTouche)-(this->coefTouche - random2);
	_fb -> init (/* position initiale de la boule */ _x, _y, 10.,
				 /* angles de visée */ angle_vertical, angle_);
}

/*
 *	Clic droit: par défaut fait tomber le premier gardien.
 *	Inutile dans le vrai jeu, mais c'est juste pour montrer
 *	une utilisation des fonctions « tomber » et « rester_au_sol »
 */

void Chasseur::right_click (bool shift, bool control) {
	if (shift)
		_l -> _guards [1] -> rester_au_sol ();
	else
		_l -> _guards [1] -> tomber ();
}

/*Update du chasseur*/
void Chasseur::update (void){
	message("Point de vie : %i%%", this->pdv);
	//Permet d'incrementer la vie perdu
	if(this->pdv < 25 && this->timeTouche >= 1000){
		this->pdv++;
	}else if(this->timeTouche < 1000){
		this->timeTouche++;
	}
	
	if(this->pdv < 0)
		partie_terminee(false);
}