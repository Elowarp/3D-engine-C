# Terminoux3D : Le Moteur 3D dans le terminal

Ce projet se veut être une esquisse d'un moteur 3d affichant le rendu dans un terminal lambda. Grandement inspiré de la série de vidéo de [Quantale](https://www.youtube.com/watch?v=UkPTyojw7IA&list=PL9V1oyvT8aPwXSj-J3b2OQgcwP63u3f4R), où il implémente un moteur 3D du même genre mais sur Python.

En effet, pour faire les déplacements en 3 dimensions, le vidéaste déplace l'ensemble des triangles de l'environnement ce que je trouve comme non pratique comme solution. C'est alors ici que je diffère déjà de cette serie de vidéo, j'ai tenté d'avoir une approche moins simple : faire le rendu depuis une camera qui, elle, se déplace dans l'environnement.

Réalisant ainsi les rotations dans l'espace de cette dernière par des matrices de rotations.

Ce projet ne fonctionne que sur **Linux** notamment à cause du binding des touches de claviers qui se fait avec [ncurses], les reste étant du C pur.(https://invisible-island.net/ncurses/announce.html).

Les différentes touches :

- `z` Avancer
- `s` Reculer
- `q` Aller à gauche
- `d` Aller à droite
- `e` Descendre
- `a` Monter
- `c` Se tourner vers la droite
- `w` Se tourner vers la gauche

Compiler et exécuter le projet :

```bash
make main && ./main
```
