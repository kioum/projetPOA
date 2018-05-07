#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"
#include "Chasseur.h"
#include "math.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#define PI 3.14159265

using namespace std;

class Gardien : public Mover {
public:
	/*
	 *	Le son...
	 */
    Sound*	_guards_fire;	// bruit de l'arme du gardien.
	Sound*	_guards_hit;	// cri du gardien touché.
    Sound*	_guards_die;	// cri du gardien mort.
    Sound*	_wall_hit;		// on a tapé un mur.
    
    Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele)
    { 
		srand (time(NULL));
        this->etat =1; 
        this->pdv = 100;
		this->portee = 10;
		this->coefTouche = 5;
		this->timeTire = 0;
        this->entrainTire = false;

        this->PP=rand()%100+1;
        this->seuilH=75;
        this->seuilB=25;
	    this->posxT=l->_treasor._x;
        this->posyT=l->_treasor._y;
		this->timebouge=0;

        
        _guards_fire = new Sound ("sons/guard_fire.wav");
	    _guards_hit = new Sound ("sons/guard_hit.wav");
        _guards_die = new Sound ("sons/guard_die.wav");
	    if (_wall_hit == 0)
		    _wall_hit = new Sound ("sons/hit_wall.wav");
    
    }

    int etat; //0 -> Inactif //1 -> Defense // 2 -> Attaque
    int pdv; // point de vie du gardien
	int portee;
	int coefTouche;
    bool entrainTire;
	int timeTire;

    int PP;
    int seuilH;
    int seuilB;
    int posxT;
    int posyT;
    int posxC;
    int posyC;
	int timebouge;
	vector<int> *graph;

    // mon gardien pense très mal!
    void update (void)
    {
		Mover* chass = _l->_guards[0];
		if(_angle < 0) _angle += 360;
		if(_angle > 360) _angle -= 360;
		
		int angle_temp = (atan2((_x - chass->_x), (chass->_y - _y)) * 180.0 / PI);
		if(angle_temp < 0) angle_temp += 360;
		if(angle_temp > 360) angle_temp -= 360;
		
		bool test1 = _angle + 75 >= angle_temp && _angle -75 < angle_temp;
		bool test2 = (chass->_x - _x) < this->portee*10. && (chass->_x - _x) > -this->portee*10. &&
		   (chass->_y - _y) < this->portee*10. && (chass->_y - _y) > -this->portee*10.;
		//printf("%d\n", test1 && test2);
		if(test1 && test2){
			this->etat = 2;
			_angle = (atan2((_x - chass->_x), (chass->_y - _y)) * 180.0 / PI);
			if(_angle < 0)
				_angle += 360;
			if(_angle > 360)
				_angle -= 360;
			if(!entrainTire && timeTire > 100){
				//printf("%i\n", _angle);
            	fire(0);
				//printf("%i\n", _angle);
            	entrainTire = true;
				timeTire =0;
        	}
			//printf("%i\n", _angle);
		}
		if (!entrainTire && timeTire <= 100)
				timeTire++;
        if(this->etat == 1)
        {

            ////////////////////////////////////////////////////////////////////
            double r1=(rand()%2) ; //BFS target Tresor
			double r2=(rand()%2);
			
			int taille=_l->width()*_l->height();
			vector<bool> trouve (taille, false);
			
            ////////////////////////////////////////////////////////////////////
	

			
			if(timebouge>=16){
				Init_graph(taille);
				//On recupere la queue du chemin
				queue<int> q = BFS(graph,(_x/10.)*_l->height()+(_y/10.) ,trouve);
				
				//Affichage debug
				/*queue<int> q2 = q;
				while(!q2.empty()){
					 int v = q2.front();
					 q2.pop();
					printf("chemin %i\n", v);
				}*/

				timebouge=0;

				int new_y = q.front();
				int new_x = 0;

				while(new_y > _l->height()){
					new_x++;
					new_y -= _l->height();
				}
				//printf("%i, %i\n", new_x, new_y);
               if(!move((new_x*10. - _x), (new_y*10. - _y))){
                 //printf("Changement %i\n", (int) r1);
				  // printf("raté");
			   }
				//printf("%f, %f\n", _x, _y);
        	}else {timebouge++;}
		}
        else if(this->etat == 2)
        {
            ////////////////////////////////////////////////////////////////////
            double r1=rand()%10; //BFS target Chasseur
			double r2=rand()%10;

            ////////////////////////////////////////////////////////////////////

           /*if(timebouge>=20){
				timebouge=0;
               if(!move(r1,r2)){
					r1=-r1;
				   	r2=-r2;
				   	move(r1,r2);
                 //printf("Changement %i\n", (int) r1);
			   }

        	}else {timebouge++;}*/
        }
    }
	
	//Initialisation du graph
	void Init_graph (int n){
		graph = new vector<int>[n];
		for (int i=0; i<_l->width();i++){
			for (int j=0; j<_l->height()-1;j++){
				//Index de la case courant
				int index = i*_l->height()+j;
				if(_l->data(i,j)==0 || (i == _x/10. && j == _y/10.) ||  (_l->_treasor._x == i && j == _l->_treasor._y)) {

					if(i+1 < _l->width())
						if(_l->data(i+1,j)==0 || (_l->_treasor._x == i+1 && j == _l->_treasor._y)
						  || (i+1 == _x/10. && j == _y/10.)){
								graph[index].push_back((i+1)*_l->height()+j);
								graph[(i+1)*_l->height()+j].push_back(index);
						}

					if(j+1 < _l->height())
						if( _l->data(i,j+1)==0 || (_l->_treasor._x == i && j+1 == _l->_treasor._y)
						   ||  (i == _x/10. && j+1 == _y/10.)){
							graph[index].push_back(i*_l->height()+(j+1));
							graph[i*_l->height()+(j+1)].push_back(index);
						}		
				}
			}
			
		}
		//For affichage
		/*for (int j=0; j<n-1;j++){
			printf("%i  -> ",  j);
			for (int k=0; k<graph[j].size();k++){
				if(graph[j].size()!= 0)
					printf("%i ", graph[j][k]);
			}
			cout << endl;
		}*/
		
	}
		
	queue<int> BFS(vector <int> *graph, int v, vector<bool> &trouve)
	{
    	// create a queue used to do BFS
   	 	queue<int> q;
		queue<int> path;

   		 // mark source vertex as trouve
   		 trouve[v] = true;

    	// push source vertex into the queue
    	q.push(v);
		int i=0; 
		bool finded=false;
    	// run till queue is not empty
   		 while (!q.empty())
   		 { 
        	// pop front node from queue and print it
     		v = q.front();
     		q.pop();
			 
			 path.push(v);
			 
			 //Si on trouve le tresor on arrete
			if (_l->_treasor._x*_l->height()+_l->_treasor._y == v) return bestWay(path);

       		// do for every edge (v -> u)
			for (int j=0; j<graph[v].size();j++){
				int u = graph[v][j];
				if (!trouve[u])
				{
					// mark it trouve and push it into queue
					trouve[u] = true;
					q.push(u);
				}
			}
		}
		
		return q;
	}
    
	//Recupere le meilleur chemin
	queue<int> bestWay(queue<int> chemin){
		queue<int> path;
		reverseQueue(chemin);
		
		bool estParent = true;
		int v;
		while (!chemin.empty())
   		 { 
			 if(estParent){
				v = chemin.front();
				chemin.pop();
				path.push(v);
				 estParent = false;
			 }
			 
			for (int j=0; j<graph[v].size();j++){
				int u = graph[v][j];
				//Si c'est un parent 
				if (chemin.front() == u)
				{
					estParent = true;
				}
		 	}
			//Si on a pas trouvé de parent
			if(!estParent && !chemin.empty())
				chemin.pop();
		    }
		reverseQueue(path);
		path.pop();
		return path;
	}
	
	// Function to reverse the queue
	void reverseQueue(queue<int>& Queue)
	{
		stack<int> Stack;
		while (!Queue.empty()) {
			Stack.push(Queue.front());
			Queue.pop();
		}
		while (!Stack.empty()) {
			Queue.push(Stack.top());
			Stack.pop();
		}
	}
	
	
    // et ne bouge pas!
    bool move (double dx, double dy)
    {
		if(_x + dx > _l->width()*10. || (_y + dy > _l->height()*10.))
			return false;
		
		int new_fbX =  (int)((_x + dx) / Environnement::scale);
		int new_fbY = (int)((_y + dy) / Environnement::scale);
		
        if (EMPTY == _l -> data (new_fbX, new_fbY) || (new_fbX == _x/10. && new_fbY == _y/10.))
        {
			//Ancienne position a 0
			((Labyrinthe*)_l)->setData ((int)((_x) / Environnement::scale),
							 (int)((_y) / Environnement::scale), 0);
            _x += dx;
            _y += dy;
			
			//Nouvelle position a 1
			((Labyrinthe*)_l)->setData ((int)((_x) / Environnement::scale),
							 (int)((_y) / Environnement::scale), 1);
			
			//il regarde ou il va
			_angle = (atan2(((_x-dx) -_x), _y - (_y-dy)) * 180.0 / PI);
            return true;
        }
        return false;
    }
    
    // ne sait pas tirer sur un ennemi.
    void fire (int angle_vertical) {
		Mover* chass = _l->_guards[0];
        //_guards_fire -> play ();
	    int random = rand() % 2;
		int angle_= (atan2((chass->_x - _x), (chass->_y - _y)) * 180.0 / PI);
		if(angle_ < 0) angle_ += 360;
		if(angle_ > 360) angle_ -= 360;
		
		int random2 = rand() % this->coefTouche*(this->pdv/100);
		
		if(random)
			angle_ = (angle_-this->coefTouche)+random2;
		else
			angle_ = (angle_+this->coefTouche)-random2;
		
		if(angle_ < 0) angle_ += 360;
		if(angle_ > 360) angle_ -= 360;
		_fb -> init (/* position initiale de la boule */ _x, _y, 6.,/* angles de visée */ angle_vertical, angle_);
		
		
    }
    
    // quand a faire bouger la boule de feu...
    bool process_fireball (float dx, float dy)
    {
		dx=0;
		dy=0;
        // on bouge que dans le vide!
        int new_fbX = (int)((_fb -> get_x () + dx) / Environnement::scale);
        int new_fbY = (int)((_fb -> get_y () + dy) / Environnement::scale);
        
        //On cast le Mover en tant que Chasseur
		Mover* m = _l->_guards[0];
        
        if(new_fbX == (int)m->_x/Environnement::scale && 
		   new_fbY == (int)m->_y/Environnement::scale){
           //ici code si un joueur a été touché
           Chasseur* c = (Chasseur*)m;
            c->pdv -= 5;
            c->timeTouche = 0;
			this->entrainTire = false;
           return false;
        }
        
        if (EMPTY == _l -> data (new_fbX, new_fbY) || (new_fbX == _x/10. && new_fbY == _y/10.))
        {
            //message ("Woooshh ..... %d", (int) dist2);
            // il y a la place.
            return true;
        }

        this->entrainTire = false;
        
        return false;
    }
};
#endif
