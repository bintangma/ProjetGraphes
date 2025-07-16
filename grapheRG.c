#include "grapheRG.h"
#include "graphe.h"
#include "chemin.h"
#include <stdlib.h>
#include <stdio.h>

struct AreteRG* creerAreteRG(int cible, int capacite) {
    struct AreteRG* a = malloc(sizeof(struct AreteRG));
    a->sommetCible = cible;
    a->capacite = capacite;
    a->suivante = NULL;
    return a;
}

void ajouterAreteRG(struct SommetRG* sommet, int cible, int capacite) {
    struct AreteRG* a = creerAreteRG(cible, capacite);
    a->suivante = sommet->liste;
    sommet->liste = a;
}

/**
 * Construit le graphe résiduel à partir du réseau original.
 *
 * Pour chaque arête (u -> v) du réseau :
 * - Ajoute une arête (u -> v) de capacité résiduelle (capacite - flot), si > 0.
 * - Ajoute une arête retour (v -> u) de capacité égale au flot, si > 0.
 *
 * @param reseau    Réseau original (tableau de pointeurs vers les sommets).
 * @param grapheRG  Graphe résiduel à remplir (tableau de pointeurs vers SommetRG).
 * @param nbSommets Nombre total de sommets dans le graphe.
 */
void buildRG(struct Sommet* reseau[], struct SommetRG* grapheRG[], int nbSommets) {
    for (int i = 1; i <= nbSommets; ++i) {
        grapheRG[i] = malloc(sizeof(struct SommetRG));
        grapheRG[i]->id = i;
        grapheRG[i]->liste = NULL;

        struct Arete* a = reseau[i]->liste;
        while (a) {
            int u = i;
            int v = a->sommetCible;
            int c = a->capacite;
            int f = a->flot;

            if (c - f > 0) {
                ajouterAreteRG(grapheRG[u], v, c - f);  // capacité résiduelle directe
            }
            if (f > 0) {
                ajouterAreteRG(grapheRG[v], u, f);      // arête retour
            }

            a = a->suivante;
        }
    }
}

void afficherGrapheRG(struct SommetRG* grapheRG[], int nbSommets) {
    printf("------ GRAPHE RESIDUEL ------\n");

    for (int i = 1; i <= nbSommets; ++i) {
        if (grapheRG[i] == NULL) continue;

        printf("Sommet %d:", grapheRG[i]->id);
        struct AreteRG* a = grapheRG[i]->liste;
        while (a) {
            printf(" -> %d (capa: %d)", a->sommetCible, a->capacite);
            a = a->suivante;
        }
        printf("\n");
    }

    printf("-----------------------------\n");
}

void libererGrapheRG(struct SommetRG* grapheRG[], int nbSommets) {
    for (int i = 1; i <= nbSommets; ++i) {
        struct AreteRG* a = grapheRG[i]->liste;
        while (a) {
            struct AreteRG* temp = a;
            a = a->suivante;
            free(temp);
        }
        free(grapheRG[i]);
    }
}

