CC=gcc
#CC=arm-linux-gcc
demo=main.o lrc.o start_mplayer.o console.o 
target=main 
CFLAGS=-Wall 

$(target):$(demo) 
	@$(CC) $^ -o $@ $(CFLAGS)  -lpthread
main.o:main.c  
	@$(CC) -c $< -o $@ $(CFLAGS)
#lrc.o:lrc.c lrc.h
#	@$(CC) -c $< -o $@ $(CFLAGS)
start_mplayer.o:start_mplayer.c
	@$(CC) -c $< -o $@ $(CFLAGS)
console.o:console.c console.h
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@rm $(demo) $(target)  -rf
