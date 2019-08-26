#Dependencias:
#sudo apt-get install libglew-dev
#sudo apt-get install libglfw3-dev
#sudo apt-get install libglm-dev

CPPSOURCES = $(wildcard *.cpp)

Target: trianguloCor

all:	Target

trianguloCor: $(CPPSOURCES:.cpp=.o)
	g++ $< common/shader.cpp -o $@ -lglfw -lGLEW -lGL

run:
	./trianguloCor
