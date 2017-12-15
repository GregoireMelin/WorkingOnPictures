La méthode appelée KLT est une méthode de suivie d’objet en mouvement dans une séquence vidéo

    Sur la première image d’une séquence vidéo, on repère les points d'intérêts de l’objet à suivre. Pour rappel, les points d'intérêts d’un objet dans une image représentent les points pour lesquels nous pouvons observer une double discontinuité de l’intensité. Cette double discontinuité correspondent à des coins, nous pouvons l’observer sur l’image ci dessus où les points d'intérêts sont marqués d’un petit carré noir. Il existe plusieurs méthode permettant de détecter ces points d'intérêt. Une méthode rapide connue et mise en oeuvre au cours des travaux dirigés d’imagerie numérique est la méthode de Harris.


    Après avoir repéré ces points d'intérêts sur la première image de la séquence, le but de la méthode est de retrouver ces points sur l’image suivante. Puis ,à son tour, cette dernière image servira de base à la recherche de ces mêmes points d'intérêts sur l’image suivante et ainsi de suite sur toute la séquence vidéo. 
    Cette méthode s’appuie sur la minimisation d’une somme de différences carrées en utilisant un modèle de translation. En fonction des dimensions de l’objet à suivre ainsi que des zones texturée des images, une fenêtre de pixels autour de chaque point d'intérêt est déterminée.  La taille de cette fenêtre s’étalonne généralement de [3x3] à [25x25]. La minimisation de l’erreur résiduelle entre le point d'intérêt de l’image au temps t et des pixels de la fenêtre au temps  t+dt donne le déplacement d’un point d'intérêt. Sur chaque image, les déplacements de tous les points d'intérêts  de l’objet sont calculés, donnant ainsi le déplacement général de celui-ci dans la séquence vidéo.


