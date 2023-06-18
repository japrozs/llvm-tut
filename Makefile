CC = clang++
CFLAGS = -I include -std=gnu++2a -g
LIBFLAGS= -I/usr/local/Cellar/llvm@12/12.0.1_1/include  -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/local/Cellar/llvm@12/12.0.1_1/lib -Wl,-search_paths_first -Wl,-headerpad_max_install_names -lLLVM-12
SRC = $(wildcard src/*.cc src/**/*.cc)
HEAD = $(wildcard include/*.h include/**/*.h)
OBJ = $(SRC:.cc=.o)
EXEC = out/vex

all: clean $(OBJ) $(EXEC) $(HEAD)

$(EXEC): $(OBJ)
	$(CC) $(LIBFLAGS) $^ -o $@
	rm -rf src/*.o src/*.dSYM src/**/*.o

%.o: %.cc
	$(CC) $(CFLAGS) $(LIBFLAGS) $^ -c -o $@

clean:
	rm -rf *.o src/*.o src/*.dSYM $(EXEC)
