BASE = main

all: $(BASE)

OS := $(shell uname -s)

ifeq ($(OS), Linux)
  LIBS += -lGL -lGLU -lglfw3 -lGLEW -lGLM
endif

ifeq ($(OS), Darwin) # Assume OS X
  LIBS += -lglfw3 -lGLEW
  CPPFLAGS += -D__MAC__ -std=c++11 -stdlib=libc++
  LDFLAGS += -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon
endif

ifdef OPT
  #turn on optimization
  CXXFLAGS += -O2
else
  #turn on debugging
  CXXFLAGS += -g
endif

CXX = g++

OBJ = $(BASE).o mesh.o shader.o window.o camera.o texture.o tree.o branch.o

$(BASE): $(OBJ)
	$(LINK.cpp) -o $@ $^ $(LIBS)

clean:
	rm -f $(OBJ) $(BASE) *.tmp
