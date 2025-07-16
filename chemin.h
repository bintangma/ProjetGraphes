#ifndef CHEMIN_H
#define CHEMIN_H
#include "grapheRG.h"
#include "graphe.h"

struct SommetChemin {
    int num;
    struct SommetChemin* suivant;
};

struct Chemin {
    int nbSommets;
    struct SommetChemin* sommet;
};

struct Chemin* shortestPathRG(struct SommetRG* grapheRG[], int s, int p, int nbSommets);
void afficherChemin(struct Chemin* chemin);
void libererChemin(struct Chemin* chemin);
int minCapa(struct SommetRG* grapheRG[], struct Chemin* chemin);
void updateFlowInRG(struct SommetRG* grapheRG[], struct Chemin* chemin, int k);
void updateFlowInNet(struct Sommet* reseau[], struct Chemin* chemin, int k);


#endif