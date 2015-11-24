# Projet 1 : implémentation de l'oracle de padding de POODLE

L'attaque POODLE, pour _Padding Oracle On Downgraded Legacy Encryption_,
consiste en deux étapes :

1. dégrader la connection en SSL 3.0 en utilisant le mécanisme de fallback non
   sûr du client vu en partie 2 du cours ;
2. utilise un oracle de padding contre CBC tel qu'il est implémenté dans SSL
   3.0, oracle vu en partie 1 du cours.

Le but du projet est d'implémenter la deuxième partie. Le navigateur
vulnérable exécutant du JS malveillant est simulé par un client acceptant des
arguments en ligne de commande. Le MitM est simulé par un proxy. Le but de
l'attaque est de récupérer un cookie secret.

Note : le cookie "secret" est en fait en clair dans le source du client, et
affiché dans les messages du client et du serveur, pour faciliter le débogage.
Le but est évidemment de le retrouver indépendemment.

Pour implémenter l'attaque, vous devrez :

1. Modifier l'exemple de proxy fournit afin de le rendre actif
2. Écrire un script dans le langage de votre choix pour piloter le client et
   monter le gros de l'attaque.

TODO: ref vers l'article POODLE

## Se familiariser avec les outils (suggestions)

0. Parcourir le source du client et du proxy, ainsi que le Makefile.
0. Compiler le client, le serveur et le proxy à l'aide du Makefile fourni.
0. Parcourir les messages d'aide du serveur et du client.
0. Faire communiquer le client directement avec le serveur.
0. Faire communiquer le client avec le serveur via le proxy.
