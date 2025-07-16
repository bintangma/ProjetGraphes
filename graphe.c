#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"

/**
 * Alloue et initialise un sommet avec un identifiant donné.
 *
 * @param id Identifiant unique du sommet.
 * @return Pointeur vers le sommet nouvellement créé.
 */
struct Sommet* creerSommet(int id) {
    struct Sommet* sommet = malloc(sizeof(struct Sommet));
    sommet->id = id;
    sommet->liste = NULL;
    return sommet;
}

/**
 * Alloue et initialise une arête avec un sommet cible et une capacité donnée.
 * Le flot initial est mis à 0.
 *
 * @param cible     Identifiant du sommet cible de l’arête.
 * @param capacite  Capacité maximale de l’arête.
 * @return Pointeur vers l’arête qu'on vient de créer.
 */
struct Arete* creerArete(int cible, int capacite) {
    struct Arete* arete = malloc(sizeof(struct Arete));
    arete->sommetCible = cible;
    arete->capacite = capacite;
    arete->flot = 0;
    arete->suivante = NULL;
    return arete;
}

/**
 * Ajoute une arête en tête de la liste chainée d’arêtes d’un sommet donné.
 *
 * @param sommet    Pointeur vers le sommet auquel ajouter l’arête.
 * @param cible     Identifiant du sommet cible de la nouvelle arête.
 * @param capacite  Capacité maximale de la nouvelle arête.
 */
void ajouterArete(struct Sommet* sommet, int cible, int capacite) {
    struct Arete* nouvelle = creerArete(cible, capacite);
    nouvelle->suivante = sommet->liste;
    sommet->liste = nouvelle;
}

/**
 * Construit un graphe à partir d’un fichier texte au format DIMACS.
 *
 * Lit les sommets, la source, le puits et les arêtes (avec capacités) depuis le fichier.
 * Initialise le tableau sommets[] et renseigne nbSommets, source, et puits.
 *
 * @param nomFichier  Nom du fichier d'entrée (format DIMACS).
 * @param sommets     Tableau de pointeurs vers les sommets du graphe.
 * @param nbSommets   Pointeur vers le nombre total de sommets (rempli par la fonction).
 * @param source      Pointeur vers le sommet source (rempli si type 's' trouvé).
 * @param puits       Pointeur vers le sommet puits (rempli si type 't' trouvé).
 */
void buildGraphe(const char* nomFichier, struct Sommet* sommets[], int* nbSommets, int* source, int* puits) {
    FILE* fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[256];
    int nbAretes = 0;
    *source = -1;
    *puits = -1;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == 'p') {
            sscanf(ligne, "p %d %d", nbSommets, &nbAretes);
            for (int i = 1; i <= *nbSommets; ++i) {
                sommets[i] = creerSommet(i);
            }
        } else if (ligne[0] == 'n') {
            int id;
            char type;
            sscanf(ligne, "n %d %c", &id, &type);
            if (type == 's') *source = id;
            if (type == 't') *puits = id;
        } else if (ligne[0] == 'a') {
            int de, vers, capacite;
            sscanf(ligne, "a %d %d %d", &de, &vers, &capacite);
            ajouterArete(sommets[de], vers, capacite);
        }
    }

    fclose(fichier);
}


void afficherGraphe(struct Sommet* sommets[], int nbSommets, int source, int puits) {
    printf("------ GRAPHE ------\n");
    printf("Source : %d\n", source);
    printf("Puits  : %d\n", puits);

    for (int i = 1; i <= nbSommets; ++i) {
        printf("Sommet %d:", sommets[i]->id);
        struct Arete* a = sommets[i]->liste;
        while (a) {
            printf(" -> %d (capa: %d, flot: %d)", a->sommetCible, a->capacite, a->flot);
            a = a->suivante;
        }
        printf("\n");
    }

    printf("--------------------\n");
}

void libererGraphe(struct Sommet* sommets[], int nbSommets) {
    for (int i = 1; i <= nbSommets; ++i) {
        struct Arete* a = sommets[i]->liste;
        while (a) {
            struct Arete* temp = a;
            a = a->suivante;
            free(temp);
        }
        free(sommets[i]);
    }
}


