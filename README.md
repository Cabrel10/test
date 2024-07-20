# test MINI JEU HAMSTER 
Ce projet est un jeu de type "sliding block" où l'objectif est de déplacer des blocs pour permettre à la clé (un bloc doré) d'atteindre l'extrémité droite de l'écran. Les blocs rouges ne peuvent se déplacer que verticalement, les blocs verts horizontalement, et la clé peut uniquement s

Étape
1.
P

ba

Copier le code
pip install pygame
2.
Le co

Initialisation de Pygame: C
Configuration de la Fenêtre de Jeu :
Définition de la classeBlock:
Création des Blocs et de la Clé: Initialisation
Fonctions de Résolution Automatique: Au
Boucle Principale du Jeu : G
3. Exécution d
enregistrer le code dans un fichier Python, par exemple hamster_kombat.py,

frapper

Copier le code
python hamster_kombat.py
4. Jouer au jeu
Quand vous

Cliquer et glisser : C
Déplacement des Blocs :
5
L

Description
ClasseBlock :
Fonctionmove_block_temp: Dép
Fonctionbfs_solve : Utiliser la
Fonctionapply_solution :bfs_solvepo
Exemple
V

pyt

Copier le code
blocks.add(Block(RED, (0 * BLOCK_SIZE, 3 * BLOCK_SIZE), (BLOCK_SIZE, 3 * BLOCK_SIZE), False))  # R1
blocks.add(Block(RED, (
blocks.add(Block(RED

blocks.add(Block

blocks.add
1 * BLOCK_SIZE, 0 * BLOCK_SIZE), (BLOCK_SIZE, 2 * BLOCK_SIZE), False))  # R2
blocks.add(Block(GREEN, (
blocks.add(Block(GREEN

blocks
1 * BLOCK_SIZE, 5 * BLOCK_SIZE), (2 * BLOCK_SIZE, BLOCK_SIZE), True))  # V1

`
Chaque bloc est défini par sa couleur, sa position, sa taille et son orientation de déplacement (horizontal ou vert).

Co
Ce projet de jeu permet de comprendre les bases de la programmation de jeux avec Pygame, la manipulation de blocs, et l'utilisation d'algorithmes de recherche pour résoudre des puzzles. Essayez de jouer avec les paramètres et les positions des blocs pour créer vos propres
