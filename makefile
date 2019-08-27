#Dependencias:
#sudo apt-get install libglew-dev
#sudo apt-get install libglfw3-dev
#sudo apt-get install libglm-dev

CPPSOURCES = $(wildcard *.cpp)

Target: carRace

all:	Target

carRace: $(CPPSOURCES:.cpp=.o)
	g++ $< common/shader.cpp -o $@ -lglfw -lGLEW -lGL

run:
	./carRace
