/*
Napisati MPI program kojim se kreira dvodimenzionalna Cartesian struktura sa n vrsta i m
kolona. Svaki od nxm procesa izračunava sumu identifikatora njegovog gornjeg i donjeg
suseda. Obezbediti da se u master procesu na kraju programa nađe ukupna izračunata suma
*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD
#define n 4
#define m 4
int main(int argc, char* argv[]) {

	int rank, size, source, dest, outbuf, i, tag = 1, nbrs[4];
	int dims[2] = { m, n }, periods[2] = { 1, 1 }, reorder = 0, coords[2];
	int gore, dole;
	int sum;
	int finalSum;

	MPI_Comm cartComm;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	MPI_Cart_create(MCW, 2, dims, periods, reorder, &cartComm);
	MPI_Comm_rank(cartComm, &rank);

	MPI_Cart_shift(cartComm, 0, 1, &gore, &dole);

	sum = gore + dole;
	MPI_Reduce(&sum, &finalSum, 1, MPI_INT, MPI_SUM, 0, MCW);
	printf("rank : %2d  gore : %2d  dole : %2d\n", rank, gore, dole);
	if (rank == 0) {
		printf("FINAL SUM : %3d\n", finalSum);
	}

	MPI_Finalize();

	return 0;
}
