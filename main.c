#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include "grapheRG.h"
#include "chemin.h"
#include "constante.h"


int main(int argc, char *argv[]) {
    if(argc!=2) {
        fprintf(stderr, "Usage: fournir un fichier DIMACS en parametre\n");
        exit(1);
    }
    struct Sommet* reseau[MAX_SOMMETS] = {NULL};
    struct SommetRG* grapheRG[MAX_SOMMETS];
    struct Chemin* chemin;
    int nbSommets = 0;
    int source = -1, puits = -1;
    int flotTotal = 0;
    int k;

    // construction du graphe à partir du fichier DIMACS
    buildGraphe(argv[1], reseau, &nbSommets, &source, &puits);

    // construction du graphe résiduel
    buildRG(reseau, grapheRG, nbSommets);

    while ((chemin = shortestPathRG(grapheRG, source, puits, nbSommets))) {
        k = minCapa(grapheRG, chemin);
        if (k <= 0) {
            libererChemin(chemin);
            break;
        }
    
        // on met à jour le graphe résiduel
        updateFlowInRG(grapheRG, chemin, k);
    
        flotTotal += k;
        libererChemin(chemin);
    }
    // on met à jour le réseau
    updateFlowInNet(reseau, chemin, k);
    printf("Flot maximum trouvé : %d\n", flotTotal);

    // nettoyage mémoire
    libererGraphe(reseau, nbSommets);
    libererGrapheRG(grapheRG, nbSommets);

    return 0;
}
