default:
	gcc -o exe main.c -pthread

clean:
	rm exe

exec:
	./exe