INF1900 PROJET INTEGRATEUR ÉQUIPE 1618

Thierry Champion        2250224
Nikolai Olekhnovitch    2204357
Raissa Oumarou Petitot  2180935
Rym Touati              2134811


## Description

Le Polynator est un projet de système embarqué de robot véhicule pouvant naviguer sur un parcours prédéterminé.

Ce projet est séparé en 2 grandes parties:

-Recherche de coins : le robot va déterminer le coin de départ et son orientation.
-Recherche du chemin le plus court : Entrez le point désiré sur la carte, et le robot déterminera le chemin le plus cours, selon le poids des arêtes.

## Compilation

Pour compiler sur un robot, naviguez à INF1900/1618/projet. Voici les différentes options:

- Pour seulement compiler la libraire, entrez make.
- Pour installer sur un robot branché via USB, entrez make app. Vous aurez à valider que le port B est libre (y/N).

## Branchements

Voici la liste des branchements:

Port A
Broche 0 : capteur infrarouge
Broches 1 à 5 : suiveur de ligne, en ordre
Broche 6 : négatif de la DEL intégrée
Broche 7 : positif de la DEL intégrée

Port B
Broche 0 : bouton interrupt (broche proche de la lettre 'V')
Broche 2 : bouton interrupt (broche proche de la lettre 'N')
Broche 3 : M2 (left) enable
Broche 4 : M1 (right) enable
Broche 5 : M2 (left) direction
Broche 6 : M1 (right) direction

Port C
Broches 0 à 7 : écran LCD

Port D
Broche 2 : output SEL bouton breadboard
Broche 3 : output VAL bouton breadboard
Broche 6 : piezo
Broche 7 : piezo

## Brève description des classes principales

- Menu              :    Point d'entrée de l'application. C'est ici qu'on détermine quel mode est utilisé (PathfindingMode ou CornersDetector). Le mode par défaut est PathfindingMode et on peut basculer à CornersDetector avec le bouton d'interruption sur la carte mère et il s'activera automatiquement.

Les boutons sur le breadboard permettent de sélectionner la destination (ligne et colonne) et valider. Le bouton de sélection (à gauche lorsqu'on regarde le robot de face) permet d'incrémenter et le bouton de validation (à droite) permet de passer au prochain affichage. Une confirmation de destination est demandée. Par défaut, l'option NON est sélectionnée et ramènera l'utilisateur au choix de destination si on appuie sur validation. Confirmez la destination en sélectionnant OUI avec le bouton de sélection. Le robot effectuera le trajet en appuyant à valider.

- Pathfinder        :   Pathfinder applique l'algorithme de Dijkstra sur une liste d'adjacence de nœuds qui représentent les différents points sur la carte. Cette approche minimise l'utilisation de mémoire sur la carte mère, vu qu'il s'agit d'un tableau contigu en mémoire 28 objets de 4 octets. 

Un tableau de nœuds de carte organise le parcours et donne l'information sur les poids des arêtes connectés à chaque nœud. Pathfinder utilise ces nœuds pour déterminer le chemin le plus cours et peut refaire le traitement dans le cas d'un obstacle qui empêche la progression.

- CornersDetector  :   Ce mode détermine l'emplacement et l'orientation du robot lorsqu'il est posé sur un des quatre coins du parcours. Le robot va avancer et, selon les intersections rencontrées et les distances parcourues, il pourra déterminer le point de départ. 

- MasterNavigation :   Cette classe comprend tous les éléments qui permettent le déplacement du robot. Il s'agit de l'implémentation du suiveur de ligne qui ajuste la direction, EventTimer qui gère la mesure des distances et l'activation de la DEL et Navigation qui fait bouger les roues pour effectuer les déplacements et les virages.
