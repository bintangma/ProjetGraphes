#ifndef GRAPHE_H
#define GRAPHE_H
struct Arete {
    int sommetCible;
    int capacite;
    int flot;
    struct Arete* suivante;
};

struct Sommet {
    int id;
    struct Arete* liste;
};

void buildGraphe(const char* nomFichier, struct Sommet* sommets[], int* nbSommets, int* source, int* puits);
void afficherGraphe(struct Sommet* sommets[], int nbSommets, int source, int puits);
void libererGraphe(struct Sommet* sommets[], int nbSommets);

#endif