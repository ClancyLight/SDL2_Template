# gcc -std=c99 ./src/*.c -lSDL2 -o "mySDLapp"".o"

appName="templateApp"
execExt=".o"
exec=$(appName)$(execExt)


build:
	gcc -std=c99 ./src/*.c -lSDL2 -lSDL2_ttf -o ./bin/$(exec)

run:
	./bin/$(exec)

clean:
	rm ./bin/$(exec)

