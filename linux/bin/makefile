VPATH = ../src / ../tdd/ ../ulrica/

CC = cc
CFLAGS  = -std=c11 -pedantic -Wall -Werror -g -funsigned-char -I../include/
OFLAGS = -g

SERVER_OBJS = server.o game.o extras.o
SERVER_LIBS = -pthread

CLIENT_OBJS = client.o client_graphics.o
CLIENT_LIBS = -lSDL2 -lSDL2_ttf -pthread

.PHONY: clean clean_test all test compile

all: server_run.out client_run.out

server_run.out: $(SERVER_OBJS) server_run.o
	$(CC) $(OFLAGS) -o $@ $^ $(SERVER_LIBS)

client_run.out: $(CLIENT_OBJS) client_run.o
	$(CC) $(OFLAGS) -o $@ $^ $(CLIENT_LIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f *.o *.out


############### ULRICA ################
#u_game.out: u_game.o extras.o client_graphics.o 
#	$(CC) $(OFLAGS) -o $@ $^ -lSDL2 -lSDL2_ttf -pthread


u_game.out: u_game.o extras.o client_graphics.o client_run3.o game3.o
	$(CC) $(OFLAGS) -o $@ $^ -lSDL2 -lSDL2_ttf -pthread

game3.o: game.c
	$(CC) $(CFLAGS) -c -D U_GAME $<
	mv game.o game3.o

client_run3.o: client_run.c
	$(CC) $(CFLAGS) -c -pthread -D main=notmain -include extern_client.h $<
	mv client_run.o client_run3.o




################## TDD #####################

SERVER_TEST_CASES = server_case_chat server_case_game
CLIENT_TEST_CASES = client_case_chat client_case_game

test: test_server test_client clean_test
	@echo Done all

clean_test: clean_test_server clean_test_client
	rm output 

clean_test_server: 
	rm test_server.o compile_server.out server_run2.o game2.o

clean_test_client:
	rm test_client.o compile_client.out client_run2.o


###server###
ACCEPTS = 3
test_server: compile_server $(SERVER_TEST_CASES)
	@echo Done server

compile_server.out: test_server.o server.o extras.o game2.o server_run2.o
	$(CC) $(OFLAGS) -o $@ $^ $(SERVER_LIBS)

test_server.o: test_server.c
	$(CC) $(CFLAGS) -c $< -I../include/ -D DEF_ACCEPTS=$(ACCEPTS)

server_run2.o: server_run.c
	$(CC) $(CFLAGS) -c $< -I../tdd/ -D DEBUG
	mv server_run.o server_run2.o

game2.o: game.c
	$(CC) $(CFLAGS) -c $< -I../tdd/ -D DEBUG
	mv game.o game2.o

server_case_chat:
	rm output
	./compile_server.out < tdd/server_input_chat > output
	diff output tdd/server_output_chat

server_case_game:
	rm output
	./compile_server.out < tdd/server_input_game > output
	diff output tdd/server_output_game



###client###
test_client: compile_client $(CLIENT_TEST_CASES)
	@echo Done client

compile_client.out: test_client.o client_graphics.o client.o client_run2.o
	$(CC) $(OFLAGS) -o $@ $^ $(CLIENT_LIBS)

test_client.o: test_client.c 
	$(CC) $(CFLAGS) -c $< ../include/client.h

test_graphics.out: test_graphics.c client_graphics.o extras.o
	$(CC) $(OFLAGS) -o $@ $^ $(CLIENT_LIBS)

client_run2.o: client_run.c
	$(CC) $(CFLAGS) -c $< -I../tdd/ -D DEBUG
	mv client_run.o client_run2.o

client_case_chat:
	rm -f output
	./compile_client.out < tdd/client_input_chat > output
	diff output tdd/client_output_chat
	
client_case_game:
	rm output
	./compile_client.out < tdd/client_input_game > output
	diff output tdd/client_output_game
