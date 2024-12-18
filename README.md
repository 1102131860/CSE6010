# CSE 6010 Computational Problem Solving for Scientists and Engineers

## Final Project 1
- This project use an optimization technique called particle swarm optimization (PSO) to find parameter values for a model that describes how populations of a predator species and a prey species evolve in time. PSO is a bio-inspired algorithm that iteratively evolves a set of candidate parameterizations toward lower error.
- To run the demo:
    1. Clone the repository and enter the FinalProject1 folder:
        ```bash
        git clone https://github.com/1102131860/CSE6010.git
        cd ./CSE6010/FinalProject1
        ```
    2. Compile the program:
        - With gcc compiler:
            ```bash
            gcc -fopenmp -lm PSO.c main.c -o pso
            ```
        - or cmake (cross-platform build tool):
            ```bash
            cmake -B build
            cmake --build build
            ```
        - or Makefile (on Linux or MacOS):
            ```bash
            make
            ```
    3. Excute the program:
        - On Linux:
            ```bash
            ./pso fit_a.txt 4 0 p
            ```
        - On Winodws:
            ```bash
            .\pso.exe fit_a.txt 1 0 p
            ```