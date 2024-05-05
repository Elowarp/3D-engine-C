# Terminoux3D : Le Moteur 3D dans le terminal

Ce projet se veut être une esquisse d'un moteur 3D affichant le rendu dans un terminal lambda. Grandement inspiré de la série de vidéo de [Quantale](https://www.youtube.com/watch?v=UkPTyojw7IA&list=PL9V1oyvT8aPwXSj-J3b2OQgcwP63u3f4R), où il implémente un moteur 3D du même genre mais sur Python.

Ce projet ne fonctionne que sur **Linux** notamment à cause du binding des touches de claviers qui se fait avec [ncurses](https://invisible-island.net/ncurses/announce.html), le reste étant du C pur.

J'ai rédigé un article expliquant en détail certaines parties du fonctionnement de ce projet et des changements que j'ai apportés par rapport à la série de vidéos initiale, si vous êtes intéressés [ça se passe ici](https://www.elowarp.fr/blog/terminoux3d_rotation/).

Les différentes touches :

- `z` Avancer
- `s` Reculer
- `q` Aller à gauche
- `d` Aller à droite
- `e` Monter
- `a` Descendre
- `c` Se tourner vers la droite
- `w` Se tourner vers la gauche
- `r` Revenir à l'origine

Compiler et exécuter le projet :

```bash
make main && ./main
```

Possibilité d'importation de `.obj` sommaires dans `main.c` (Variable `my_mesh`). Dézoomer le terminal augmente la définition des objets dans le rendu.

## Idées d'approfondissement

Par ordre d'importance :

- Fix le problème de l'axe Y des modèles remplacé par l'axe Z
- Garder la même convention de nommage des variables et fonctions
- Trouver le ratio hauteur/largeur des caractères du terminal directement au lancement du programme, et non par le magic number `ASPECT_RATIO_CHARACTER_SHELL`
- Faire une importation de .obj plus complète par rapport aux standards
- Paramètre de lancement du programme : rajouter un objet à importer au démarrage
- Optimisation : Changement de la représentation de la scène (par un arbre binaire de recherche peut être ? (découpage de l'espace en sous sections comme doom il me semble ?))
- Création de 2 Threads :
  - Un pour la récupération des touches de claviers (dont mouvements)
  - Un pour l'affichage graphique

- Ajout de sprites 2d
- Ajout de couleurs
- Mode d'édition : Après l'appuie sur une touche, on pourrait bouger les objets dans l'espace
