QUESTIONS

Les explications qui suivent se base sur le résultat obtenu avec la vidéo 'tabletennis.avi',
généré par la commande : ./movement_detection ../data/tabletennis.avi
On remarque que dans le cas d'une différence de fond sans mélange de gaussienne, la balle est visible sur le résultat.
Dans le cas du mélange de gaussienne la balle n'est pas visible, on a donc un résultat 'meilleur' dans le sens où on a grâce au mélange de gaussienne vraiment récupérer le fond de la scène (le mur).

UTILISATION
Pour changer de mode (Différence d'images successives, Différence de fonds, Différence de fonds avec mélange de gaussiennes...),
il suffit de décommenter l'appel de la fonction associée dans le main().

//TODO :
difference(cap);  				[1]
//difference_background(cap);			[2]
//difference_back_MOG(cap);			[3]

Si on veut faire une différence de fonds par exemple (ligne 2),
commenter la ligne 1 et 3 et décommenter la ligne 2.
