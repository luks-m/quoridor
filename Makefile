all:toto

clean:
	rm -rf install/*
	rm -rf src/*~
	rm -rf src/player_bis.c

toto : src/graph_pattern.c src/server.c src/player.c src/utils.c src/main.c
	cp src/player.c src/player_bis.c
	gcc -shared -fPIC src/player.c src/utils.c -o install/libplayer_1.so `gsl-config --cflags --libs` -ldl
	gcc -shared -fPIC src/player.c src/utils.c -o install/libplayer_2.so `gsl-config --cflags --libs` -ldl
	gcc src/main.c src/utils.c src/graph_pattern.c install/libplayer_1.so install/libplayer_2.so src/server.c -o install/server `gsl-config --cflags --libs` -ldl
