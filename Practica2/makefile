# project name (generate executable with this name)
TARGET   = main

CC       = gcc
# compiling flags here
CFLAGS   =  -ansi -Wall -pedantic -I.

LINKER   = gcc
# linking flags here
LFLAGS   =  -ansi -I. -pedantic -lm

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(LINKER) $(LFLAGS)  -g -o $@ $(OBJECTS)
	@echo "Linking complete!"


$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -g $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"


valgrind: 
	valgrind --leak-check=full --track-origins=yes $(BINDIR)/$(TARGET)

exec:
	./bin/main
	
.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"