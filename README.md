# Graphe-Projet

Ce projet implémente un algorithme de flot maximum dans un graphe orienté. 
Il utilise des structures de données pour représenter les sommets et les arêtes, ainsi que des graphes résiduels pour calculer le flot maximum.

## Contenu de l'archive

- **`graphe.c`** : Contient les fonctions pour construire et manipuler le graphe principal.
- **`graphe.h`** : Déclare les structures et fonctions associées au graphe principal.
- **`grapheRG.c`** : Implémente les fonctions pour construire et manipuler le graphe résiduel.
- **`grapheRG.h`** : Déclare les structures et fonctions associées au graphe résiduel.
- **`chemin.c`** : Implémente les fonctions pour trouver des chemins augmentants et mettre à jour les flots.
- **`chemin.h`** : Déclare les structures et fonctions associées aux chemins augmentants.
- **`constante.h`** : Définit des constantes globales, comme `MAX_SOMMETS`.
- **`main.c`** : Point d'entrée du programme. Gère la construction du graphe, l'exécution de l'algorithme de flot maximum, et l'affichage des résultats.
- **`data/`** : Contient des fichiers de données au format DIMACS pour tester le programme
- **`Raport_du_projet.pdf`** : Le travail dérigé qui élabore les étapes critiques sur ce projet.
- **`README.md`** : Ce fichier, décrivant le projet et son utilisation.

## Compilation

Pour compiler le projet, utilisez la commande suivante dans le terminal :

```bash
gcc main.c graphe.c chemin.c grapheRG.c -o main -Wall
