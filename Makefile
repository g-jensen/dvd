compile:
	g++ -c src/*.cpp
	mv main.o ./objects/main.o
	g++ objects/*.o -o main -lsfml-graphics -lsfml-window -lsfml-system -lGL

gui:
	g++ -c imgui/*.cpp

clean:
	rm *.o
	rm objects/*.o