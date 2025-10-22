CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -I. -IUtils -IUser -IExport -IShapes -IParser
OBJDIR = bin
BINDIR = bin

# Liste de tous les fichiers .c
SRCS = Utils/utils.c \
       Utils/memory.c \
       User/menu.c \
       User/list.c \
       Shapes/create.c \
       Shapes/edit.c \
       Shapes/delete.c \
       Shapes/shapes.c \
       Shapes/group.c \
       Export/export.c

# Corresponding object files
OBJS = $(SRCS:.c=.o)
OBJS := $(patsubst %,$(OBJDIR)/%,$(OBJS))

# Nom de l'exécutable
TARGET = $(BINDIR)/projet

# Création des dossiers bin et bin/Shapes etc. si manquants
$(shell mkdir -p $(OBJDIR)/Utils $(OBJDIR)/User $(OBJDIR)/Export $(OBJDIR)/Shapes $(OBJDIR)/Parser)

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJDIR)/* $(TARGET)