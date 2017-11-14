init:
	mkdir -p out/

build:
	make init
	g++ \
	src/ant.cpp \
	src/cell.cpp \
	src/controller.cpp \
	src/creator.cpp \
	src/doodlebug.cpp \
	src/grid.cpp \
	src/main.cpp \
	src/organism.cpp \
	src/organismimpl.cpp \
	-o out/doodlebug

run:
	make build
	out/doodlebug
	
check:
	valgrind --leak-check=full out/doodlebug

graph:
	python scripts/graphit.py out/doodlebug.csv out/doodlebug.png

.PHONY: init build run check
