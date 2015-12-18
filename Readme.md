# Notes de cours sur TLS

Notes préparées pour le cours de cryptologie appliquée de
l'[EPITA](http://www.epita.fr/) fin 2015.

## Projets

Pour démarer les projets, cloner ce repo, et lire le Readme de chaque projet.
Utiliser une branche git pour travailler sur les projets est probablement une
bonne idée ; il n'est pas impossible que la branche master soit mise à jour de
temps en temps.

Certains projets contenant du code de support sont fournis avec Makefile ; le
code a des chances de fonctionner sous Windows (pas testé !) mais vous devrez
alors vous débrouiller pour le compiler.

## Rendu du projet

Seul le projet 1 est à rendre. La date limite est le 28 décembre à 8h du
matin.

Vous devez rendre une archive (zip ou tar) contenant :
- le source du proxy modifié (`mitm_proxy.c`)
- votre ou vos scripts de contrôle (y compris les scripts de test
  intermédiaires si vous en avez utilisé)
- de préférence un fichier texte expliquant la stratégie adoptée et vos
  résultats (nombre de requêtes, etc)

Il va de soi que le source (proxy, scripts) doit être commenté.

L'archive doit être nommée d'une façon qui me permet de vous identifier
facilement, et doit contenir les fichiers à sa racine (pas de
sous-répertoire). Par exemple, je pourrais générer cette archive ainsi :

```shell
cd project-1
zip pegourie-gonnard.zip mitm_proxy.c find_secret.sh freq.sh Notes
```

ou bien :

```shell
cd project-1
tar czf manuel-pg.tgz  mitm_proxy.c find_secret.sh freq.sh Notes
```

Le tout est à rendre par mail (adresse dans les slides de cours) ; le sujet du
mail doit contenir `[crypto-p1]`.
