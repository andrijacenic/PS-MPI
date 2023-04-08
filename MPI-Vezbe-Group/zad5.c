#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD
#define UP 0
#define DOWN 1
#define worldSize 12 
int main(int argc, char* argv[]) {

	int rank, size, source, dest, outbuf, i, tag = 1, nbrs[4];
	int dims[2] = { 3, 4 }, periods[2] = { 1, 0 }, reorder = 0, coords[2];

	MPI_Comm cartComm;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);
	MPI_Cart_create(MCW, 2, dims, periods, reorder, &cartComm);
	MPI_Comm_rank(cartComm, &rank);

	MPI_Cart_coords(cartComm, rank, 2, coords);
	MPI_Cart_shift(cartComm, 0, coords[1], &nbrs[UP], &nbrs[DOWN]);

	outbuf = rank;
	dest = nbrs[1];
	source = nbrs[0];

	MPI_Sendrecv_replace(&outbuf, 1, MPI_INT, dest, tag, source, tag, cartComm, &status);
	
	printf("%3d %3d : (%3d, %3d)", coords[0], coords[1], rank, outbuf);

	MPI_Finalize();

	return 0;
}