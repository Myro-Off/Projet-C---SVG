# Creator Studio — SVG Editor (C Terminal Project)

## Description

Creator Studio is a C terminal application for creating, editing, grouping, and visualizing geometric shapes—circles, lines, rectangles, ellipses, polygons, polylines, paths, and more—directly from the terminal.
It features a colorful, intuitive Text User Interface (TUI) for managing SVG elements without a graphical environment.

The project emphasizes visual clarity, text-based ergonomics, and modular code structure, delivering a smooth, fully command-line experience.

Designed for VS Code terminal, it’s recommended to use a sufficiently large width and height to avoid display issues, especially when many shapes are listed.

---

## Available Shapes

The user can choose a shape to create from the following list:

- ● **Circle**
- ■ **Square**
- ▬ **Rectangle**
- / **Line**
- ⬭ **Ellipse**
- ⬟ **Polygon**
- ╱╲ **Multiline**
- ┼  **Path**

---

## Upcoming Features
- Load data from existing SVG files
- Shape rotation support
- Shape opacity control
- Improved shape management within group editing
- Better error handling during element editing

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

### Requirements
- A C99+ compiler (GCC or Clang)
- make
- An ANSI-compatible terminal
- macOS / Linux (Windows via WSL or Git Bash)

---

### Compilation

- Build the project 

`make build`

- Run the program

`./bin/projet`

- Clean build files

`make clean`

⸻

## Autor

Developed by: Adam Valsan
School: Coda Dijon — Bachelor 1 Full-Stack Developer
Date: October 2025
