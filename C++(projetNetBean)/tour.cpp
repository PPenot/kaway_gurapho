#include "tour.h"
#include <sstream>

using namespace std;

string convert(float number)
{
     std::ostringstream buff;
     buff<<number;
     return buff.str();
}

Tour::Tour()
{
    tour = Matrix(2,0);
    cost = 0;
}

Tour::~Tour()
{
}

Tour::Tour(const Tour& t):tour(t.tour)
{
    cost = t.cost;
}

const unsigned int Tour::size() const
{
    return tour.getCols();
}

const float Tour::getCost() const
{
    return cost;
}

void Tour::setCost(const float newCost)
{
    cost = newCost;
}

//prend les id des noeuds de départ et d'arrivé de la chaine,
//retourne le chemin parasite dans un tableau [source,dest]
int* Tour::addPath(int source, int dest)
{
    int* parasite = new int[2];
    parasite[0] = dest;     //chemin parasite par défaut
    parasite[1] = source;

    unsigned int length = size();
    unsigned int i = 1;

    while( (i<=length) && ( (int)(tour(2,i))!=source) )
    {
        i++;
    }

    if(i<= length && (int)(tour(2,i))==source)
        i++;
    tour.addColumn(i);
    tour(1,i) = source;
    tour(2,i) = dest;

    while(i-1 > 0 && tour(1,i)==tour(2,i-1))
    {
        parasite[1] = tour(1,i-1);
        i--;
    }

    return parasite;
}

const string Tour::toString() const
{
    unsigned int length = size();
    unsigned int i=1;
    string s;

    while(i<=length)
    {
        s = s + "(" + convert(tour(1,i)) + "," + convert(tour(2,i)) + ")-";
        i++;
    }
    return s;
}


bool Tour::isCircuit() const
{
    unsigned int length = size();
    unsigned int i = 1;
    bool flag = (length>=2) && ( tour(1,1)==tour(2,length) );

    while(i+1 <= length && flag)
    {
        flag = ( tour(2,i) == tour(1,i+1) );
        i++;
    }

    return flag;
}

void Tour::sort(int start)
{
    if(isCircuit())
    {
        int pos_start=1; //colonne du noeud start
        int size = tour.getCols();
        bool boule = false; //sert à savoir si on a trouvé le noeud
        while(pos_start<=size && !boule)
        {
            boule = (tour(1,pos_start)==start);
            pos_start++;
        }

        if(!boule) return;

        Matrix tmp(tour);

        for(int i=1;i<=size;i++)
            tour.removeColumn(1);

        pos_start--;

        int j=1;
        for(int i=pos_start;i<=size;i++)
        {
            tour.addColumn(j);
            tour(1,j) = tmp(1,i);
            tour(2,j) = tmp(2,i);
            j++;
        }
        for(int i=1;i<pos_start;i++)
        {
            tour.addColumn(j);
            tour(1,j) = tmp(1,i);
            tour(2,j) = tmp(2,i);
            j++;
        }

    }
}

int* Tour::toTab() const
{
    int* tab = new int[length()];
    for(int i=0;i<size();i++)
    {
        tab[i] = tour(1,i+1);
    }
    return tab;
}