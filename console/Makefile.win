# Project: k145
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++.exe -D__DEBUG__
CC   = gcc.exe -D__DEBUG__
WINDRES = windres.exe
RES  = 
OBJ  = main.o cascade.o chargemem.o k145_com.o $(RES)
LINKOBJ  = main.o cascade.o chargemem.o k145_com.o $(RES)
LIBS =  -L"C:/develop/Dev-Cpp/lib"  -g3 -march=pentium4 
INCS =  -I"C:/develop/Dev-Cpp/include" 
CXXINCS =  -I"C:/develop/Dev-Cpp/lib/gcc/mingw32/3.4.2/include"  -I"C:/develop/Dev-Cpp/include/c++/3.4.2/backward"  -I"C:/develop/Dev-Cpp/include/c++/3.4.2/mingw32"  -I"C:/develop/Dev-Cpp/include/c++/3.4.2"  -I"C:/develop/Dev-Cpp/include" 
BIN  = k145.exe
CXXFLAGS = $(CXXINCS)   -g3 -march=pentium4
CFLAGS = $(INCS)   -g3 -march=pentium4
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before k145.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "k145.exe" $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

cascade.o: cascade.cpp
	$(CPP) -c cascade.cpp -o cascade.o $(CXXFLAGS)

chargemem.o: chargemem.cpp
	$(CPP) -c chargemem.cpp -o chargemem.o $(CXXFLAGS)

k145_com.o: k145_com.cpp
	$(CPP) -c k145_com.cpp -o k145_com.o $(CXXFLAGS)
