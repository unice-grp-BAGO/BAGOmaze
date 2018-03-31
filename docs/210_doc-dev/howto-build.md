---
layout: default
---

# Compilation du projet BAGOmaze

## Récupération des sources

Les sources sont hébergées sous Github. Pour les récupérer, les télécharger
sous forme d'archive (cf. liens en haut de la page) ou clôner le dépôt :

~~~~~{sh}
~$ git clone https://github.com/unice-grp-BAGO/BAGOmaze.git
~$ cd BAGOmaze
~~~~~


## Récupération des dépendances

Ce projet nécessite plusieurs dépendances externes.

Dans le répertoire du projet, effectuer la commande suivante pour les
installer :

~~~~~{sh}
~$ sudo make install-dependencies
~~~~~



## Compilation du projet

Dans le répertoire du projet, effectuer les commandes suivantes :

~~~~~{sh}
~$ mkdir build
~$ cd build
~$ cmake ..
~$ make
~~~~~

L'exécutable doit maintenant se situer dans le dossier `build/out/app/`.
