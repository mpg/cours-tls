# Projet 1 : implémentation de l'oracle de padding de POODLE

L'attaque [POODLE][], pour _Padding Oracle On Downgraded Legacy Encryption_,
consiste en deux parties :

1. dégrader la connection en SSL 3.0 en utilisant le mécanisme de fallback non
   sûr du client vu en partie 2 du cours ;
2. utilise un oracle de padding contre CBC tel qu'il est implémenté dans SSL
   3.0, oracle vu en partie 1 du cours.

[POODLE]: https://www.openssl.org/~bodo/ssl-poodle.pdf

Le but du projet est d'implémenter la deuxième partie. Le navigateur
vulnérable exécutant du JS malveillant est simulé par un client acceptant des
arguments en ligne de commande. Le MitM est simulé par un proxy. Le but de
l'attaque est de récupérer un cookie secret.

Note : le cookie "secret" est en fait en clair dans le source du client, et
affiché dans les messages du client et du serveur, pour faciliter le débogage.
Le but est évidemment de le retrouver indépendemment. Vous pouvez supposer
connues la structure de la requête, la longueur du secret (24 octets), et le
fait qu'il est en base64, en particulier tous ses caractères sont imprimables.

Pour implémenter l'attaque, vous devrez :

1. Modifier l'exemple de proxy fournit afin de le rendre actif
2. Écrire un script dans le langage de votre choix pour piloter le client et
   monter le gros de l'attaque.

(Pour info, ma solution d'exemple ajoute 30 lignes à `mitm_proxy.c` et utilise
un script shell d'une cinquantaine de lignes.)

La section suivante donne quelques suggestions, vous pouvez les ignorer et
procéder autrement. Dans tous les cas, n'oubliez pas de progresser par étapes
en testant à chaque étape plutôt qu'à la fin.

Happy hacking!


## Suggestions pour démarrer

0. Parcourir le source du client et du proxy, ainsi que le Makefile.
0. Compiler le client, le serveur et le proxy à l'aide du Makefile fourni.
0. Parcourir les messages d'aide du serveur et du client.
0. Faire communiquer le client directement avec le serveur.
0. Faire communiquer le client avec le serveur via le proxy.

Pour rentrer dans le vif du sujet (les deux premières étapes sont
indépendantes et peuvent être faites dans l'ordre inverse) :

0. Invoquer le client de façon à contrôler la quantité de padding et
   l'emplacement du secret.
0. Dans le proxy-MitM, modifier les records de type ApplicationData du client
   vers le serveur de façon adéquate.
0. Vous pouvez alors observer la fréquence d'acceptation des messages
   modifiés.
0. Vous pouvez alors commencer à attaquer chaque octet du secret.

N'hésitez pas à consulter le [papier][POODLE] !

Une fois que votre attaque fonctionne, vous pouvez la benchmarker (nombre de
requêtes, temps), chercher à l'optimiser, où à l'étendre au cas où la longueur
du secret n'est pas connue a priori, etc. Pensez à documenter les hypothèses
faites.
