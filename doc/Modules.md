### Interface Graphique
L'interface graphique est découpé en 2 parties distinctes :
- La partie affichage pour afficher la caméra.
- La partie contrôle dispose de plusieurs bouttons pour pouvoir accéder aux fonctionnalités décrites dans l'introduction facilement.

### Clusterisation de textes
Pour les aveugles, le dispositif doit pouvoir réaliser une lecture des magazines montrés à la caméra. Le problème, c'est que les magazines actuelles ne possèdent pas une *lecture linéaire* (TODO img), mais une lecture par bloc (TODO img). Le logiciel doit donc être capable de trouver ces blocs, de leur donner un ordre de lecture et de les faire lire par la synthèse vocale. La partie découpage de l'image capturée en blocs de textes est réalisée par le Clusteriseur.
Pour le moment, la clusterisation se base sur un algorithme naïf, mais qui donne des résultats plutôt correct, mais aucun ordre de lecture est présent.
TODO img

### OCR
Même si réaliser un OCR basique n'est pas une tâche si difficile que ça, offrir un OCR pratique et fonctionnel prend du temps. OpenVideoMagnifier se base donc sur des OCR libre existants.
TODO add tests
Comme nous pouvons le voir. GOCR est le meilleur OCR testé pour le moment. 

### Correction orthographique
Comme nous l'avons vu dans les tests précédents, les OCR ne sont pas parfaits et commettent pas mal d'erreur de reconnaissance, une partie correction des textes de sortie est donc obligatoire.
Pour le moment aucun algorithme n'a été mis en place/décidé pour permettre une correction des textes

### Synthèse vocale
La réalisation d'une synthèse vocale depuis zéro est un projet non négligeable. N'ayant pas le temps de le faire, j'utilise des solutions déjà existantes.
Petite note personnelle : Les synthèses vocales libres ne m'ont jamais satisfait. Je trouve qu'elles donnent des résultats assez pitoyables au vu de ce que l'on peut faire aujourd'hui et c'est la partie qui me déçoit le plus dans le projet
Faute de mieux, la synthèse vocale utilisée par défaut dans le projet est Espeak. Ce qui est très loin du résultat qu'obtient la synthèse vocale de Google, ou de Apple...
