mybash:mybash.c
	gcc -o $@ $^ -g
.PHONY:clean
clean:
	rm -f mybash
