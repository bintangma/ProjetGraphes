#include "chemin.h"
#include "grapheRG.h"
#include <stdlib.h>
#include <stdio.h>
#include "constante.h"

/**
 * Calcule le plus court chemin (en nombre d’arcs) entre la source s et le puits p
 * dans un graphe résiduel à l’aide d’un parcours en largeur (BFS).
 *
 * Seuls les arcs avec une capacité strictement positive sont considérés.
 * Si un chemin est trouvé, il est retourné sous forme de structure Chemin.
 * Sinon, la fonction retourne NULL.
 *
 * @param grapheRG Graphe résiduel (tableau de pointeurs vers les sommets).
 * @param s        Numéro du sommet source.
 * @param p        Numéro du sommet puits.
 * @param nbSommets Nombre total de sommets dans le graphe.
 *
 * @return Pointeur vers un chemin valide (de s à p), ou NULL si aucun chemin n'existe.
 */
struct Chemin* shortestPathRG(struct SommetRG* grapheRG[], int s, int p, int nbSommets) {
    int visite[MAX_SOMMETS] = {0};
    int precedent[MAX_SOMMETS];
    for (int i = 0; i < MAX_SOMMETS; ++i) precedent[i] = -1;

    int queue[MAX_SOMMETS];
    int front = 0, rear = 0;

    queue[rear++] = s;
    visite[s] = 1;

    while (front < rear) {
        int courant = queue[front++];
        if (courant == p) break;

        struct AreteRG* arc = grapheRG[courant]->liste;
        while (arc) {
            int voisin = arc->sommetCible;
            if (!visite[voisin] && arc->capacite > 0) {
                queue[rear++] = voisin;
                visite[voisin] = 1;
                precedent[voisin] = courant;
            }
            arc = arc->suivante;
        }
    }

    if (!visite[p]) return NULL; // aucun chemin trouvé

    struct Chemin* chemin = malloc(sizeof(struct Chemin));
    chemin->nbSommets = 0;
    chemin->sommet = NULL;

    struct SommetChemin* tete = NULL;
    int current = p;

    while (current != -1) {
        struct SommetChemin* nouveau = malloc(sizeof(struct SommetChemin));
        nouveau->num = current;
        nouveau->suivant = tete;
        tete = nouveau;
        chemin->nbSommets++;
        current = precedent[current];
    }

    chemin->sommet = tete;
    return chemin;
}

/**
 * Calcule la capacité minimale parmi les arcs d’un chemin dans le graphe résiduel.
 *
 * Parcourt le chemin donné et retourne la plus petite capacité des arcs (u -> v).
 * Si un arc du chemin est manquant ou invalide, retourne -1.
 *
 * @param grapheRG Graphe résiduel
 * @param chemin   Chemin à analyser
 *
 * @return Capacité minimale le long du chemin, ou -1 si chemin invalide ou arc manquant.
 */
int minCapa(struct SommetRG* grapheRG[], struct Chemin* chemin) {
    if (!chemin || !chemin->sommet || !chemin->sommet->suivant) {
        return -1;  // chemin invalide ou vide
    }

    int min = -1;
    struct SommetChemin* courant = chemin->sommet;

    while (courant->suivant) {
        int u = courant->num;
        int v = courant->suivant->num;

        // parcourir les arcs du sommet u dans le graphe résiduel
        struct AreteRG* arc = grapheRG[u]->liste;
        int trouve = 0;
        while (arc) {
            if (arc->sommetCible == v) {
                if (min == -1 || arc->capacite < min) {
                    min = arc->capacite;
                }
                trouve = 1;
                break;
            }
            arc = arc->suivante;
        }

        if (!trouve) {
            // chemin incorrect : arc manquant dans le graphe
            return -1;
        }

        courant = courant->suivant;
    }

    return min;
}


/**
 * Met à jour les flux dans le graphe résiduel après avoir trouvé un chemin augmentant.
 *
 * Pour chaque arc du chemin, diminue la capacité de l'arc direct (u → v) de k.
 * Si la capacité devient 0, l'arc est supprimé. Ajoute ou met à jour l'arc retour (v → u) avec +k.
 *
 * @param grapheRG Graphe résiduel
 * @param chemin Chemin augmentant (liste de sommets parcourus dans le graphe).
 * @param k Quantité de flux à ajouter dans ce chemin 
 */
void updateFlowInRG(struct SommetRG* grapheRG[], struct Chemin* chemin, int k) {
    if (!chemin || !chemin->sommet || k <= 0) return;

    struct SommetChemin* courant = chemin->sommet;

    while (courant && courant->suivant) {
        int u = courant->num;
        int v = courant->suivant->num;

        // 1. trouver l’arc direct u -> v et diminuer sa capacité
        struct AreteRG* arc = grapheRG[u]->liste;
        struct AreteRG* prec = NULL;
        while (arc && arc->sommetCible != v) {
            prec = arc;
            arc = arc->suivante;
        }

        if (arc) {
            arc->capacite -= k;
            if (arc->capacite == 0) {
                // supprimer l’arc s’il n’a plus de capacité
                if (prec)
                    prec->suivante = arc->suivante;
                else
                    grapheRG[u]->liste = arc->suivante;
                free(arc);
            }
        }

        // ajouter ou mettre à jour l’arc retour v -> u
        struct AreteRG* retour = grapheRG[v]->liste;
        while (retour && retour->sommetCible != u) {
            retour = retour->suivante;
        }

        if (retour) {
            retour->capacite += k;
        } else {
            struct AreteRG* newArc = malloc(sizeof(struct AreteRG));
            newArc->sommetCible = u;
            newArc->capacite = k;
            newArc->suivante = grapheRG[v]->liste;
            grapheRG[v]->liste = newArc;
        }

        courant = courant->suivant;
    }
}

/**
 * Met à jour les flots dans le réseau original en fonction d’un chemin augmentant.
 *
 * Pour chaque arc du chemin, augmente le flot sur l’arc direct (u → v) de k.
 * Si l’arc direct n’existe pas, diminue le flot sur l’arc retour (v → u) de k.
 *
 * @param reseau Réseau d’origine
 * @param chemin Chemin augmentant parcouru (liste de sommets).
 * @param k Quantité de flux à ajouter (doit être > 0).
 */
void updateFlowInNet(struct Sommet* reseau[], struct Chemin* chemin, int k) {
    if (!chemin || !chemin->sommet || k <= 0) return;

    struct SommetChemin* courant = chemin->sommet;

    while (courant && courant->suivant) {
        int u = courant->num;
        int v = courant->suivant->num;

        // Chercher l'arc u -> v dans le réseau (sens direct)
        struct Arete* arc = reseau[u]->liste;
        int directFound = 0;

        while (arc) {
            if (arc->sommetCible == v) {
                arc->flot += k;
                directFound = 1;
                break;
            }
            arc = arc->suivante;
        }

        if (!directFound) {
            // Sinon, chercher arc retour v -> u pour le diminuer
            arc = reseau[v]->liste;
            while (arc) {
                if (arc->sommetCible == u) {
                    arc->flot -= k;
                    break;
                }
                arc = arc->suivante;
            }
        }

        courant = courant->suivant;
    }
}

/**
 * Affiche les sommets d’un chemin sous forme de liste.
 * @param chemin Pointeur vers une structure Chemin à afficher.
 */

void afficherChemin(struct Chemin* chemin) {
    if (!chemin) {
        printf("Aucun chemin trouvé.\n");
        return;
    }

    printf("Chemin (%d sommets) : ", chemin->nbSommets);
    struct SommetChemin* courant = chemin->sommet;
    while (courant) {
        printf("%d ", courant->num);
        courant = courant->suivant;
    }
    printf("\n");
}

/**
 * Libère la mémoire allouée pour un chemin.
 * @param chemin Pointeur vers le chemin à désallouer.
 */
void libererChemin(struct Chemin* chemin) {
    struct SommetChemin* courant = chemin->sommet;
    while (courant) {
        struct SommetChemin* tmp = courant;
        courant = courant->suivant;
        free(tmp);
    }
    free(chemin);
}
