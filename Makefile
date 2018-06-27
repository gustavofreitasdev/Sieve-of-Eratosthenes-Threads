default:
	gcc -o exe main.c -pthread

teste:
	gcc -o exe main_teste.c -pthread

clean:
	rm exe

exec:
	./exe