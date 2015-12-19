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

Vous ne devez modifier ni le client ni le serveur, ni utiliser la sortie du
client ou du serveur (sauf pour deboguer). La seule information du client que
vous utiliserez est son code de retour (0 si le serveur a accepté le message,
autre chose sinon).

(Pour info, ma solution d'exemple ajoute 30 lignes (commentaires compris) à
`mitm_proxy.c` et utilise un script shell d'une cinquantaine de lignes.)

La section suivante donne quelques suggestions, vous pouvez les ignorer et
procéder autrement. Dans tous les cas, n'oubliez pas de progresser par étapes
en testant à chaque étape plutôt qu'à la fin.

Happy hacking!


## Suggestions pour démarrer

Pour se familiariser avec le matériel fourni :

0. Parcourir le source du client et du proxy, ainsi que le Makefile.
0. Compiler le client, le serveur et le proxy à l'aide du Makefile fourni.
0. Parcourir les messages d'aide du serveur et du client.
0. Faire communiquer le client directement avec le serveur.
0. Faire communiquer le client avec le serveur via le proxy.
0. Examiner les messages de débogage du client ou du server avec
   `debug_level=4` et repérer l'endroit où les message sont (dé)chiffrés, en
particulier la quantité de padding est indiquée.

Pour rentrer dans le vif du sujet (les deux premières étapes sont
indépendantes et peuvent être faites dans l'ordre inverse) :

0. Invoquer le client de façon à ce qu'il y ait exactement un bloc de padding
   à la fin de la requête, et vérifier dans la sortie de debug du client ou du
serveur que votre calcul est juste.
0. Dans le proxy-MitM, repérer les records de type ApplicationData du client
   et modifier leur dernier bloc de façon « bête » (par exemple changer la
valeur du dernier octet).
0. Vous pouvez alors observer la fréquence d'acceptation des messages
   modifiés (environ 1/256, vous pouvez tester sur 1024 essais par exemple).
0. Invoquer le client de façon à avoir exactement un bloc de padding *et* à
   placer l'octet que vous voulez du cookie à la fin d'un bloc. (Pro-tip:
faire en sorte que ce soit toujours le même bloc, quel que soit l'octet visé.)
Vous pouvez contrôler que vous n'avez pas régressé (fréquence d'acceptation).
0. Dans le proxy, dans les records AppData c2s, copier le bloc visé à la fin.
   Vous pouvez contrôler votre changement dans la sortie de debug du serveur.
Vous pouvez contrôler que vous n'avez pas régressé (fréquence d'acceptation).
0. Dans le proxy, afficher la valeur de l'octet ciblé en supposant que le
   serveur accepte le message. Faire des requêtes jusqu'à ce qu'une soit
acceptée, et vérifier que la dernière valeur affichée par le proxy est bonne.
0. Félicitations, vous venez de casser un octet du secret. Itérez.

N'hésitez pas à relire les explications du cours (partie 1, slides 21 à 28) et
à consulter le [papier][POODLE] ! Quelques références supplémentaires :

- blog de [Mathew Green][CE]
- blog d'[Adam Langley][IV] (attention, explications avec des blocs de 8
  octets, pensez à adapter pour des blocs AES de 16 octets).

Surtout, pour voir comment retrouver la valeur de l'octet recherché, pensez à
prendre une respiration profonde, du papier et un crayon, et à écrire calmement
ce qui se passe. C'est vraiment le genre de calcul qui est plus facile quand
on le fait soi-même que quand on regarde quelqu'un le faire au tableau.

[CE]: http://blog.cryptographyengineering.com/2014/10/attack-of-week-poodle.html
[IV]: https://www.imperialviolet.org/2014/10/14/poodle.html

Une fois que votre attaque fonctionne, vous pouvez la benchmarker (nombre de
requêtes, temps), chercher à l'optimiser, où à l'étendre au cas où la longueur
du secret n'est pas connue a priori, etc. Pensez à documenter les hypothèses
faites.

Pour info, ma solution d'exemple a besoin en moyenne de 6000 à 7000 requêtes.
Sur une de mes machines (Core i5 à 1.8 GHz), cela prend environ 10 minutes.
Soyez patients.
