# Terminux3D : Le Moteur 3D dans le terminal

Ce projet ce veut être une esquisse d'un moteur 3d tournant dans un terminal lambda. Grandement inspiré de la série de vidéo de [Quantale](https://www.youtube.com/watch?v=UkPTyojw7IA&list=PL9V1oyvT8aPwXSj-J3b2OQgcwP63u3f4R), où il implémente un moteur 3D du même genre mais sur Python et assez sommairement.

En effet pour faire les déplacements en 3 dimensions, le vidéaste déplace l'ensemble des triangles de l'environnement ce que je trouve sommaire comme solution. C'est alors ici que je diffère déjà de cette serie de vidéo, j'ai tenté d'avoir une approche moins simple : faire le rendu depuis une camera qui, elle, se déplace dans l'environnement.

Ce projet ne fonctionne que sur **Linux** notamment à cause du binding des touches de claviers qui se fait avec [ncurses](https://invisible-island.net/ncurses/announce.html).

Compiler et exécuter le projet :

```bash
make main && ./main
```
