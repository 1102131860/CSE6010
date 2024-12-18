CCFLAGS = -fopenmp -lm
OBJ = PSO.o main.o
.INTERMEDIATE: $(OBJ)

pso: $(OBJ)
	gcc -o $@ $^ $(CCFLAGS)

%.o: %.c
	gcc -c -o $@ $< $(CCFLAGS)

.PHONY: clean
clean:
	rm -f pso
