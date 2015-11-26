# Projet 2 : implémentation d'un callback pour HTTP Public Key Pinning

HPKP (RFC 7469) permet à un client HTTPS d'enregistrer une liste de clés
publiques autorisées pour un domaine, communiquées par le serveur lors d'une
connection précédente. Le but de ce projet est d'implémenter la vérification
du pin, une fois ceux-ci enregistrer.

Afin de simplifier, on aura une liste unique de pin (plutôt qu'un liste par
domaine), qu'on suppose acquise, et on ignorera les donnés autres que les clés
publiques (durée du pin, adresse pour les rapports, etc.). En pratique ils
vous suffit donc de lire la section 2.6 du [RFC 7469] pour la validation.

Un exemple de client TLS est fourni, avec une liste de pins et quelques
certificats associés. Pour réussir ce projet, vous devrez :

1. Compléter le client en implémentant la vérification des pins
2. Générer un ou plusieurs certificats de test supplémentaires
3. Écrire un bref document expliquant/justifiant votre stratégie de test

La section suivante donne quelques suggestions, vous pouvez les ignorer et
procéder autrement. Dans tous les cas, n'oubliez pas de progresser par étapes
en testant à chaque étape plutôt qu'à la fin.

Happy hacking!


## Suggestions pour démarrer

Pour se familiariser avec le matériel fourni :

0. Parcourir le source du client, ainsi que le Makefile.
0. Compiler le client et le serveur à l'aide du Makefile fourni.
0. Compiler les programmes d'exemple de mbed TLS.
0. Parcourir les messages d'aide du serveur et des programmes suivants:
`programs/pkey/gen_key`, `programs/x509/cert_app`, `programs/x509/cert_write`.
0. Faire communiquer le client avec le serveur, en chargeant différents
   certificats sur le serveur.
0. Relire le source du client, en s'attardant sur les zones marquées TODO ou
   XXX et consulter la documentation des fonctions associées :
   <https://tls.mbed.org/api/>

Les certificats de test fournis on été signés par les CA suivantes (les noms
de fichiers sont relatifs à `../mbedtls/tests/data_files`) :

- CA1 : `test-ca.crt`, clé `test-ca.key`, mdp clé `PolarSSLTest`
- CA2 : `test-ca2.crt`, clé `test-ca2.key`

Les certificats de test fournis sont :

- `s1-1.crt` associé à `s1.key`, signé par CA1
- `s2-2.crt` associé à `s2.key`, signé par CA2

Le client fourni comme point de départ a `test-ca.key` et `s1.key` dans sa
liste de pins.

Les deux étapes naturelles sont :

1. Calcul du hash de la clé publique d'un certificat.
2. Validation ds certificats par rapport à la liste de pins.

La première étape peut se faire confortablement hors du client TLS. Dans ce
but, un fichier séparé pour implémenter cette fonction et un programme simple
l'appelant sont fournis.

N'oubliez pas de consulter la documentation de l'API de mbed TLS ainsi que les
exemples !
