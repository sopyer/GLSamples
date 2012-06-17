# Linux (default)
EXE = nzfill
LDFLAGS = -lGL -lGLU -lglut

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE = nzfill.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X
ifeq "$(OSTYPE)" "darwin"
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

$(EXE) : nzfill.cpp
	gcc -o $@ $< $(CFLAGS) $(LDFLAGS)

