PARTIE 1.1 :
La variable 'boardSize' correspond à la taille de l'échiquier pris en photo. 
La variable 'cameraMatrix' contient les distances focales et les centres optiques de la caméra utilisée.
Enfin, la variable 'distortion' contient les paramètres k1, k2, p1, p2, et k3.

PARTIE 1.2 :
- On constate que la quasi-totalité des images sont rectifiées par l'algorithme.


On lance l'interface graphique.
f(pix) (des fenêtres 'IN' et 'OUT' de l'interface) permet de zoomer sur l'image ouverte.
	- Si on modifie ce paramètre sur l'image 'IN', on peut augmenter les effets de la distortion en bords de l'image 'OUT'. 
	- Si on modifie ce paramètre sur l'image 'OUT', le zoom de l'image finale est modifié mais les effets de distorsion ne sont alors plus visibles.

Les paramètres x0 et y0 permettent d'accéder aux différents points de l'image ouverte.
Les paramètres k1, k2 et k3 correspondent aux coefficients de distortion radiale. (Arrondissement de l'image)
Les paramètres p1 et p2 correspondent aux coefficients de distortion tangente. (Enroulement de l'image)



PARTIE 2 :

La calibration d'un banc de caméra oblige à apparier les vues de chaque caméra par groupe. Dans notre cas, 
c'est-à-dire le cas d'un banc de caméra stéréo, il faut calibrer la vue de la caméra de droite avec la caméra de gauche.
Pour cela, on fait un programme permettant de stocker les paramètres extrinsèques et intrasèques de chaque caméra respectivement 
dans les fichiers 'extrinsics.yml' & 'intrinsics.yml'.

Le fichier 'extrinsics.yml' contient :
R : la matrice de rotation permettant de passer d'une caméra l'autre (taille : 3x3),
Q  : la matrice de disparité de profondeur,
T : la translation permettant de passer d'une caméra à l'autre,
P1 : la matrice de projection qui permet de passer de l'ancien système de coordonnées au rectifié pour la caméra 1,(taille : 3x4)
P2 : la matrice de projection qui permet de passer de l'ancien système de coordonnées au rectifié pour la caméra 2, (taille : 3x4)
R1 : la transformée de rectification pour la caméra 1 (taille : 3x3),
R2 : la transformée de rectification pour la caméra 2 (taille : 3x3).

PARTIE 3 :

