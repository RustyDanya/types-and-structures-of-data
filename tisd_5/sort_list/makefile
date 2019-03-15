CC:= gcc -std=c99

app.exe: main.o  
	$(CC) -o app.exe main.o 
	
main.o: main.c
	$(CC) -c main.c

clean:
	rm *.exe *.o