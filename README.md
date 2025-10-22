# Editeur SVG en ligne de commande

## Description
Ce projet permet de créer, éditer, sauvegarder et charger des structures graphiques similaires au format SVG via une interface en ligne de commande.

## Compilation

### Commandes Makefile disponibles

#### Compilation
```bash
make build    # Compile le projet
make all      # Identique à make build
make re       # Recompile complètement le projet (clean + build)
```

#### Exécution
```bash
make run      # Compile et exécute le programme
./projet      # Exécute directement le programme
```

#### Nettoyage
```bash
make clean    # Supprime l'exécutable et les fichiers objets
make fclean   # Nettoyage complet (inclut les fichiers de backup)
```

#### Utilitaires
```bash
make info     # Affiche les informations de compilation
make check    # Vérifie la syntaxe sans compiler
```

## Formes implémentées

### Formes primaires
- ✅ **Cercle** : Centre (x, y) et rayon
- ✅ **Carré** : Position (x, y) et côté
- ✅ **Rectangle** : Position (x, y), largeur et hauteur ✅ **Ligne** : Points de départ (x1, y1) et d'arrivée (x2, y2)
- ✅ **Polygone** : Tableau de points (minimum 3)
- ✅ **Polyline** : Enchaînement de lignes (minimum 2 points)

### Stylisation des formes
Chaque forme supporte :
- Couleur de trait
- Couleur de fond (sauf ligne et polyline)
- Épaisseur du trait
- Déplacement en X et Y
- Rotation en degrés
- Inversion selon les axes X et Y

### Fonctionnalités par forme
- Création avec paramètres personnalisables
- Affichage des propriétés
- Modification des couleurs
- Déplacement
- Rotation
- Inversion d'axes
- Export au format SVG
- Vérification de validité (viewport)
- Libération mémoire propre

## Structure du projet

```
Projet C/
├── Makefile              # Fichier de compilation
├── projet.c              # Programme principal
├── README.md             # Documentation
└── Shapes/               # Dossier des formes
    ├── cercle.h/.c       # Implémentation du cercle
    ├── carre.h/.c        # Implémentation du carré
    ├── rectangle.h/.c    # Implémentation du rectangle
    ├── ligne.h/.c        # Implémentation de la ligne
    ├── polygone.h/.c     # Implémentation du polygone
    └── polyline.h/.c     # Implémentation de la polyline
```

## Conformité

### Consignes respectées
- ✅ Compilateur GCC
- ✅ Standard C99 (`-std=c99`)
- ✅ Makefile avec `make build` et `make clean`
- ✅ Librairies natives uniquement (stdio, stdlib, string, stdbool, math)
- ✅ Commentaires au format Javadoc
- ✅ Code propre et organisé
- ✅ Pas de fuite mémoire

### Cahier des charges
- ✅ Formes primaires obligatoires implémentées
- ✅ Stylisation complète des formes
- ✅ Export SVG fonctionnel
- ✅ Gestion des erreurs (vérification de validité)

## Auteur
Adam Valsan