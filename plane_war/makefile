prom = plane_war
cc = gcc
ld = gcc
cflags = -Wfatal-errors -Wall -Wextra -pthread
ldflags = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -pthread
objects = plane_war.o ttf.o sound.o  level.o\
		  bullet_new.o sprite.o list.o math.o time.o animate.o\
		  dlink.o

$(prom):$(objects)
	$(ld) -o $(prom) $(objects) $(ldflags)

plane_war.o:plane_war.c
	$(cc) -g -c $< -o $@ 

bullet_new.o:bullet_new.c 
	$(cc) -g -c $(cflags) $< -o $@

sprite.o:sprite.c
	$(cc) -g -c $(cflags) $< -o $@

math.o:math.c
	$(cc) -g -c $(cflags) $< -o $@

level.o:level.c
	$(cc) -g -c $< -o $@

ttf.o:ttf.c
	$(cc) -g -c $(cflags) $< -o $@

sound.o:sound.c
	$(cc) -g -c $(cflags) $< -o $@

list.o:list.c
	$(cc) -g -c $(cflags) $< -o $@

dlink.o:dlink.c
	$(cc) -g -c $(cflags) $< -o $@

time.o:time.c
	$(cc) -g -c $(cflags) $< -o $@

animate.o:animate.c
	$(cc) -g -c $(cflags) $< -o $@

.PHONY:clean
clean:
	rm -rf *.o
