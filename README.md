# Creator Studio — SVG Editor (C Terminal Project)

## Description

Creator Studio is a C terminal application for creating, editing, grouping, and visualizing geometric shapes—circles, lines, rectangles, ellipses, polygons, polylines, paths, and more—directly from the terminal.
It features a colorful, intuitive Text User Interface (TUI) for managing SVG elements without a graphical environment.

The project emphasizes visual clarity, text-based ergonomics, and modular code structure, delivering a smooth, fully command-line experience.

🖥️ Compatible with macOS and Linux, using Bash, Zsh, or any POSIX-compatible shell.

Designed for VS Code terminal, it’s recommended to use a sufficiently large width and height to avoid display issues, especially when many shapes are listed.

---

## Main Features

### Main Menu

The user can select from the following options::

| Option | Action |
|:------:|:--------|
| (1) | Create a Shape |
| (2) | Create a Group of Shapes |
| (3) | Edit a Shape or a Group |
| (4) | Delete a Shape or a Group |
| (5) | Open the List |
| (6) | Export to SVG |
| (7) | Import from SVG (soon) |
| (ENTER) | Quit program |

---

## Available Shapes

The user can choose a shape to create from the following list:

- ●  **Circle**
- ■  **Square**
- ▬  **Rectangle**
- /  **Line**
- ⬭  **Ellipse**
- ⬟  **Polygon**
- ╱╲ **Multiline**
- ┼  **Path**

---

## Fonctionnalités à venir
-	Chargement de données à partir de fichiers SVG  
-	Rotation des Formes  
-	Opacité des Formes  
-	Meilleure gestion des formes dans l'édition d'un groupe
-	Gestion des erreurs lors de l'édition d'un élément

---

## Project Structure

```
├── Export/
│   ├── export.c
│   └── export.h
├── Shapes/
│   ├── create.c
│   ├── create.h
│   ├── delete.c
│   ├── delete.h
│   ├── edit.c
│   ├── edit.h
│   ├── group.c
│   ├── group.h
│   ├── shapes.c
│   └── shapes.h
├── User/
│   ├── list.c
│   ├── list.h
│   ├── menu.c
│   └──  menu.h
├── Utils/
│   ├── memory.c
│   ├── memory.h
│   ├── structures.h
│   ├── utils.c
│   └── utils.h
├── bin/
│   └── projet(exe)
├── Makefile
└── README.md
```

---

## Installation & Compilation

### Prérequis

- **Compilateur GCC** (C99 ou supérieur)  
- **make**  
- Terminal compatible **ANSI/VT100** (pour les couleurs)  
- OS : **macOS** ou **Linux** *(Windows via WSL)*

---

### Compilation

- Exécution  

`make build`

- Nettoyage  

`make clean`

## Compatibilité
	•	macOS ✅
	•	Linux ✅ (testé sur Ubuntu & Arch)
	•	Windows ⚠️ (via WSL)
	•	Shells : Bash, Zsh, POSIX ✅

⸻

## Auteur

Développé par : Adam Valsan
École : Coda Dijon — Bachelor 1 Développeur Full-Stack
Date : Octobre 2025 

## 🧱 Compatibilité

- macOS&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;✅&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Compatible avec le terminal natif
- Linux&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;✅&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Testé sur Ubuntu et Arch Linux
- Windows&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;⚠️&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Partiellement compatible via WSL
- Shells&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;✅&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Bash, Zsh, et tout shell POSIX

## 🧑‍💻 Crédits

Développé par **Adam Valsan**  
Ecole Coda Dijon — Bachelor 1 Developpeur Full-Stack  
Premier Projet en langage C
