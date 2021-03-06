
Sur la première image d’une séquence vidéo, on repère les points d'intérêts de l’objet à suivre.
Les points d'intérêts d’un objet dans une image représentent les points pour lesquels une double discontinuité de l’intensité st observable, cette double discontinuité correspondant à des coins. Il existe plusieurs méthode permettant de détecter ces points d'intérêt. Une méthode rapide, connue et mise en oeuvre au cours des travaux dirigés de vision par ordinateur est la mise en place d'un détecteur de Harris.

Après avoir repéré ces points d'intérêts sur la première image de la séquence, le but de la méthode est de retrouver ces points sur l’image suivante. Puis ,à son tour, cette dernière image servira de base à la recherche de ces mêmes points d'intérêts sur l’image suivante et ainsi de suite sur toute la séquence vidéo.

Cette méthode s’appuie sur la minimisation d’une somme de différences carrées en utilisant un modèle de translation. En fonction des dimensions de l’objet à suivre ainsi que des zones texturée des images, une fenêtre de pixels autour de chaque point d'intérêt est déterminée. La minimisation de l’erreur résiduelle entre le point d'intérêt de l’image au temps t et des pixels de la fenêtre au temps  t+dt donne le déplacement d’un point d'intérêt. Sur chaque image, les déplacements de tous les points d'intérêts  de l’objet sont calculés, donnant ainsi le déplacement général de celui-ci dans la séquence vidéo.

Rem. : Le programme KLT permet de compter et d'afficher les points d'intérêt d'une vidéo. On remarque qu'au fur et à mesure du défilement des images, le nombres de points d'intérêt diminue.

