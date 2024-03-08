# Terminoux3D : Le Moteur 3D dans le terminal

Ce projet se veut être une esquisse d'un moteur 3D affichant le rendu dans un terminal lambda. Grandement inspiré de la série de vidéo de [Quantale](https://www.youtube.com/watch?v=UkPTyojw7IA&list=PL9V1oyvT8aPwXSj-J3b2OQgcwP63u3f4R), où il implémente un moteur 3D du même genre mais sur Python.

Ce projet ne fonctionne que sur **Linux** notamment à cause du binding des touches de claviers qui se fait avec [ncurses](https://invisible-island.net/ncurses/announce.html), le reste étant du C pur.

J'ai rédigé un article expliquant en détail certaines parties du fonctionnement de ce projet, si vous êtes intéressés [ça se passe ici](https://www.elowarp.fr/blog/terminoux3d_rotation/).

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
