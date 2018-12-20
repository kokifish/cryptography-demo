SHA_1_main.exe : SHA_1_main.o SHA_1.o
	g++ -o SHA_1_main SHA_1_main.o SHA_1.o

SHA_1.o : SHA_1.cpp
	g++ -c SHA_1.cpp

SHA_1_main.o : SHA_1_main.cpp
	g++ -c SHA_1_main.cpp


.PHONY : clean
clean :
	del SHA_1_main.o SHA_1.o