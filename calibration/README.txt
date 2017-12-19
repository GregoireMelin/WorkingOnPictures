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



PARTIE 1.3 :

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

---------------------------- intrinsics.yml obtenu lors des tests -----------------------------------
%YAML:1.0
M1: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 5.9750447637371326e+02, 0., 2.8932467237780622e+02, 0.,
       5.9750447637371326e+02, 2.2804637872589501e+02, 0., 0., 1. ]
D1: !!opencv-matrix
   rows: 1
   cols: 8
   dt: d
   data: [ -1.2041619090867069e-01, -3.1813675891015442e-02, 0., 0., 0.,
       0., 0., -2.1462663103066487e-01 ]
M2: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 5.9750447637371326e+02, 0., 3.3223973812722846e+02, 0.,
       5.9750447637371326e+02, 2.4092669105455604e+02, 0., 0., 1. ]
D2: !!opencv-matrix
   rows: 1
   cols: 8
   dt: d
   data: [ -1.2758226065690437e-01, -6.9682768704669787e-02, 0., 0., 0.,
       0., 0., -3.6833740895549166e-01 ]
---------------------------- extrinsics.yml obtenu lors des tests ------------------------------------
%YAML:1.0
R: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 9.9997100137831507e-01, -1.7993904276756021e-03,
       -7.3999051709181538e-03, 1.8364106304403511e-03,
       9.9998581846620238e-01, 4.9990461565838831e-03,
       7.3908049931112026e-03, -5.0124904556557350e-03,
       9.9996012467547213e-01 ]
T: !!opencv-matrix
   rows: 3
   cols: 1
   dt: d
   data: [ -7.8751503011888415e+00, -6.5340191907635736e-02,
       1.4527572428727188e-02 ]
R1: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 9.9993648525357737e-01, 6.5065124777872844e-03,
       -9.2027579615461645e-03, -6.4833090353491659e-03,
       9.9997573449160027e-01, 2.5489472212866498e-03,
       9.2191194088460608e-03, -2.4891210017080347e-03,
       9.9995440501753075e-01 ]
R2: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 9.9996388025368876e-01, 8.2967091849235203e-03,
       -1.8446692622918369e-03, -8.3013297605941411e-03,
       9.9996238958910177e-01, -2.5114401166156691e-03,
       1.8237631952399912e-03, 2.5266626118812829e-03,
       9.9999514492014063e-01 ]
P1: !!opencv-matrix
   rows: 3
   cols: 4
   dt: d
   data: [ 5.3845139040381059e+02, 0., 3.1471402359008789e+02, 0., 0.,
       5.3845139040381059e+02, 2.3458003044128418e+02, 0., 0., 0., 1.,
       0. ]
P2: !!opencv-matrix
   rows: 3
   cols: 4
   dt: d
   data: [ 5.3845139040381059e+02, 0., 3.1471402359008789e+02,
       -4.2405387964996726e+03, 0., 5.3845139040381059e+02,
       2.3458003044128418e+02, 0., 0., 0., 1., 0. ]
Q: !!opencv-matrix
   rows: 4
   cols: 4
   dt: d
   data: [ 1., 0., 0., -3.1471402359008789e+02, 0., 1., 0.,
       -2.3458003044128418e+02, 0., 0., 0., 5.3845139040381059e+02, 0.,
       0., 1.2697711688153215e-01, 0. ]

