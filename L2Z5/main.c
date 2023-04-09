/*
Napisati MPI program kojim se kreira dvodimenzionalna Cartesian struktura sa n vrsta i n
kolona. Podeliti procese u grupe koje odgovaraju gornjoj i donjoj trougaonoj matrici kreirane
strukture. Procese na dijagonali proizvoljno dodeliti jednoj od grupa. U okviru svake grupe
sumirati vrednosti identifikatora svih procesa koji pripadaju datoj grupi. Master procesu
komunikatora MPI_COMM_WORLD dostaviti ove vrednosti i odštampati ih.
*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD
#define UP 0
#define DOWN 1
#define n 4
#define m 4
int main(int argc, char* argv[]) {

	int rank, size, rankTrougaona;
	int dims[2] = { m, n }, periods[2] = { 1, 1 }, reorder = 1, coords[2];
	int myrank, sum, rankDown, rankUp;
	int sumDown, sumUp;

	MPI_Comm cartComm, trougaona;
	MPI_Status status;
	MPI_Request req;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	MPI_Cart_create(MCW, 2, dims, periods, reorder, &cartComm);
	MPI_Comm_rank(cartComm, &rank);
	MPI_Cart_coords(cartComm, rank, 2, coords);

	MPI_Comm_split(MCW, coords[0] > coords[1], rank, &trougaona);
	MPI_Comm_rank(trougaona, &rankTrougaona);

	printf("troug : %d, rank : %3d, t rank %3d coords : (%3d, %3d)\n", coords[0] > coords[1], rank, rankTrougaona, coords[0], coords[1]);

	myrank = rank;
	MPI_Reduce(&myrank, &sum, 1, MPI_INT, MPI_SUM, 0, trougaona);
	if (rankTrougaona == 0) {
		printf("%d\n", sum);
		MPI_Isend(&sum, 1, MPI_INT, 0, coords[0] > coords[1] ? 1 : 2, MCW, &req);
	}
	if (rank == 0) {
		coords[0] = 1;
		coords[1] = 0;
		MPI_Cart_rank(cartComm, coords, &rankDown);
		MPI_Recv(&sumDown, 1, MPI_INT, rankDown, 1, MCW, &status);
		MPI_Recv(&sumUp, 1, MPI_INT, 0, 2, MCW, &status);
		printf("%3d %3d %3d", sumUp, sumDown, sumUp + sumDown);
	}

	MPI_Finalize();
	return 0;
}
