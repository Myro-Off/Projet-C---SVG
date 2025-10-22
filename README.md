🎨 Creator Studio — Projet SVG en C

🧭 Description

Creator Studio est un programme en C permettant de créer, éditer, regrouper et visualiser des formes géométriques — cercles, lignes, rectangles, ellipses, polygones, multiligne, chemins, etc. — via une interface conçue pour le terminal.

Ce projet met l’accent sur la clarté visuelle, l’ergonomie textuelle et la structuration modulaire du code.
L’objectif est d’offrir une expérience fluide et agréable, tout en restant purement en ligne de commande.

🖥️ Compatible macOS et Linux, avec les shells Bash, Zsh ou tout autre environnement POSIX-compatible.

⸻

⚙️ Compilation et exécution

🧩 Prérequis
	•	GCC (ou tout compilateur C compatible C99 ou supérieur)
	•	make
	•	Un terminal compatible ANSI
	•	bash, zsh ou shell équivalent

🔧 Compilation

▶️ Exécution

./bin/projet

🧹 Nettoyage

make clean


⸻

✨ Fonctionnalités principales

🟢 Création de formes
	•	Cercle
	•	Carré
	•	Rectangle
	•	Ligne
	•	Ellipse
	•	Polygone
	•	Multiline (ligne brisée ouverte)
	•	Path (chemin complexe)

🟡 Édition
	•	Modification des coordonnées, tailles, couleurs, etc.

🔵 Groupes
	•	Création et gestion de groupes de formes
	•	Suppression et affichage détaillé des groupes
	•	Édition interne sans repasser par le menu principal

🟣 Interface terminale
	•	Menus encadrés stylisés avec séquences ANSI
	•	Navigation fluide au clavier
	•	Messages de succès et d’erreur encadrés
	•	Couleurs et symboles pour une meilleure lisibilité

⸻

🚧 Fonctionnalités à venir
	•	Chargement de données à partir de fichiers SVG
    •   Rotation des Formes
    •   Opacité des Formes
    •   Meilleure gestion des formes dans l'édition d'un groupe
    •   Etc.

⸻

🧱 Compatibilité

Environnement	Supporté	         Détails

macOS	          ✅	    Compatible avec le terminal natif
Linux	          ✅	    Testé sur Ubuntu et Arch Linux
Windows	          ⚠️	    Partiellement compatible via WSL
Shells	          ✅	    Bash, Zsh, et tout shell POSIX


⸻

🧑‍💻 Crédits

Développé par Adam Valsan
Ecole Coda Dijon — Bachelor 1 Developpeur Full-Stack
Projet académique 2025 en langage C
