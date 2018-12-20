AES_main.exe : AES_main.o   AES_Cipher.o
	g++ -o AES_main AES_main.o AES_Cipher.o

AES_main.o : AES_main.cpp
	g++ -c AES_main.cpp
	
AES_Cipher.o : AES_Cipher.cpp
	g++ -c AES_Cipher.cpp


.PHONY : clean
clean :
	del AES_main.o AES_Cipher.o