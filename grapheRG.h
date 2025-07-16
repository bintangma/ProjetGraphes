#ifndef GRAPHE_RG_H
#define GRAPHE_RG_H
#include "graphe.h"


struct AreteRG {
    int sommetCible;
    int capacite;
    struct AreteRG* suivante;
};

struct SommetRG {
    int id;
    struct AreteRG* liste;
};

void buildRG(struct Sommet* reseau[], struct SommetRG* grapheRG[], int nbSommets);
void afficherGrapheRG(struct SommetRG* grapheRG[], int nbSommets);
void libererGrapheRG(struct SommetRG* grapheRG[], int nbSommets);

#endif
