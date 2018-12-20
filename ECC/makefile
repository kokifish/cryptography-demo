ECC_main.exe : ECC_main.o ECC.o
	g++ -o ECC_main ECC_main.o ECC.o

ECC.o : ECC.cpp
	g++ -c ECC.cpp

ECC_main.o : ECC_main.cpp
	g++ -c ECC_main.cpp


.PHONY : clean
clean :
	del ECC_main.o ECC.o