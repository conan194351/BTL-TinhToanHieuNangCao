mpi:
	mpic++ mpi-$(PROG).cpp -o mpi-$(PROG)
	mpirun -np $(NP) ./mpi-$(PROG)

cpp:
	g++ -o $(PROG) $(PROG).cpp
	./$(PROG)