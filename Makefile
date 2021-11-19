version4:
	gcc-11 -o version4 version4.c -O3 -march=native -ffast-math -funroll-loops -lm -fopenmp
clean:
	rm -f version4
