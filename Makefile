BIN = bin/botnet test/test_random test/test_virus test/test_jeu
OBJ= test/test_random.o  bin/computer.o bin/country_list.o bin/botnet.o bin/country.o

bin/ : 
	mkdir bin

bin/botnet: bin/botnet.o bin/jeu.o bin/virus.o bin/upgrade.o bin/country_list.o bin/computer_list.o bin/country.o bin/random_lib.o bin/
	gcc bin/botnet.o bin/jeu.o bin/virus.o bin/upgrade.o bin/country_list.o bin/computer_list.o bin/country.o bin/random_lib.o -o bin/botnet -lm


bin/botnet.o : src/botnet.c lib/country_list.h bin/
	gcc -c src/botnet.c -o ./bin/botnet.o

bin/country_list.o : src/country_list.c lib/country.h bin/
	gcc -c src/country_list.c -o ./bin/country_list.o

bin/computer_list.o: src/computer_list.c lib/computer.h bin/
	gcc -c src/computer_list.c -o bin/computer_list.o

bin/jeu.o : src/jeu.c lib/jeu.h bin/
	gcc -c src/jeu.c -o bin/jeu.o

bin/virus.o : src/virus.c lib/virus.h bin/
	gcc -c src/virus.c -o bin/virus.o

bin/country.o : bin/ src/country.c lib/computer_list.h
	gcc -c src/country.c -o ./bin/country.o

bin/computer.o : bin/ src/computer.c 
	gcc -c src/computer.c -o ./bin/computer.o

bin/upgrade.o: bin/ src/upgrade.c
	gcc -c src/upgrade.c -o ./bin/upgrade.o

test/test_random : test/test_random.o bin/random_lib.o
	gcc bin/random_lib.o test/test_random.o -o test/test_random -lm 

test/test_random.o : test/test_random.c
	gcc -c test/test_random.c -o test/test_random.o

bin/random_lib.o : bin/ src/random_lib.c
	gcc -c src/random_lib.c -o bin/random_lib.o

test/test_virus : test/test_virus.o bin/virus.o bin/country_list.o bin/country.o bin/computer_list.o bin/random_lib.o
	gcc test/test_virus.o bin/virus.o bin/country_list.o bin/country.o bin/computer_list.o bin/random_lib.o -o test/test_virus -lm

test/test_virus.o : test/test_virus.c
	gcc -c test/test_virus.c -o test/test_virus.o


test/test_jeu : test/test_jeu.o bin/jeu.o bin/virus.o bin/upgrade.o bin/country_list.o bin/computer_list.o bin/country.o bin/random_lib.o
	gcc test/test_jeu.o bin/jeu.o bin/virus.o bin/upgrade.o bin/country_list.o bin/computer_list.o bin/country.o bin/random_lib.o -o test/test_jeu -lm

test/test_jeu.o : test/test_jeu.c
	gcc -c test/test_jeu.c -o test/test_jeu.o

doc:
	rm -rf doc/
	mkdir doc
	doxygen doxy_cfg

clean:
	rm -rf bin/*
	rm -rf test/test*.o


all : $(BIN) $(OBJ)
