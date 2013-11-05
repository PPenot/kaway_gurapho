
#ifndef _PATH_H
#define	_PATH_H

#include "matrix.h"
#include <string>

class Tour
{
    private:
        Matrix tour; //matrice représentant la tournée
        float cost; //coût total de la tournée

    public:
        Tour();
        Tour(const Tour& t);
        ~Tour();

        bool isCircuit() const; //renvoit true si le chemin est un circuit, false sinon
        int* addPath(int source, int dest);
        const std::string toString() const;
        const unsigned int size() const; //retourne la taille de la tournée (en fait, tant que ce n'est pas un cycle, le nombre d'arcs)

        const float getCost() const;
        void setCost(const float newCost);

        void sort(int start); //trie la tournée en la faisant démarrer par le noeud d'id start
        int length() const {return (int)tour.getCols();}; //nombre de noeuds compris dans la tournée
        int* toTab() const;
};

#endif	/* _PATH_H */

