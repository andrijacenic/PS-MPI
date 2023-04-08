
/*
	Podeli po komunikatore po vrstama, matrica 8x2
*/

#include <mpi.h>
#include <stdio.h>
#define MCW MPI_COMM_WORLD
#define n 8
#define m 2
int main3(int argc, char** argv) {
	MPI_Group world, rowComm, collumComm;
	int i, p, rowRank, collumRank, rank, color, key;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &p);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_group(MCW, &world);

	color = (rank / m);
	key = (rank % m);

	MPI_Comm_split(MCW, color, key, &rowComm);
	MPI_Comm_split(MCW, key, color, &collumComm);

	MPI_Comm_rank(rowComm, &rowRank);
	MPI_Comm_rank(collumComm, &collumRank);

	printf("%3d %3d %6d %6d \n", color, rank, rowRank, collumRank);

	MPI_Finalize();
	return 0;
}
