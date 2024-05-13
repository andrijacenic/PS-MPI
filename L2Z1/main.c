/*

Napisati MPI program koji kreira komunikator koji se sastoji od dijagonalnih procesa u
kvadratnoj mreži procesa. Iz master procesa novog komunikatora poslati poruku svim ostalim
procesima. Svaki proces novog komunikatora treba da prikaže primljenu poruku, identifikator
procesa u novom komunikatoru i stari identifikator procesa

*/
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD

int main(int argc, char* argv[]) {

	int size, rank, rankDiag, rankRow, rankColl, sizeDiag;
	int i, j;
	int n;
	int podatak;

	MPI_Comm commDiag, commRow, commColl;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	n = sqrt(size);

	MPI_Comm_split(MCW, rank / n, rank % n, &commRow);
	MPI_Comm_rank(commRow, &rankRow);

	MPI_Comm_split(MCW, rank % n, rank / n, &commColl);
	MPI_Comm_rank(commColl, &rankColl);

	// samo glavna dijagonala
	//MPI_Comm_split(MCW, rankRow == rankColl, rankRow, &commDiag);
	//MPI_Comm_rank(commDiag, &rankDiag);

	//if (rankRow == rankColl) {
	//	podatak = 1122;
	//	if (rank == 0) {
	//		for (i = 1; i < n; i++) {
	//			MPI_Send(&podatak, 1, MPI_INT, i, 1, commDiag);
	//		}
	//	}
	//	else {
	//		MPI_Recv(&podatak, 1, MPI_INT, 0, 1, commDiag, &status);
	//		printf("diag rank : %2d, rank : %2d, podatak : %5d", rankDiag, rank, podatak);
	//	}
	//}
	// sve dijagonale
	MPI_Comm_split(MCW, rank % (n + 1) + (rankColl > rankRow ? 50 : 0), rankRow, &commDiag);
	MPI_Comm_rank(commDiag, &rankDiag);
	MPI_Comm_size(commDiag, &sizeDiag);

	if (rankDiag == 0) {
		podatak = rank;
		for (i = 1; i < sizeDiag; i++)
			MPI_Send(&podatak, 1, MPI_INT, i, 1, commDiag);
	}
	else {
		MPI_Recv(&podatak, 1, MPI_INT, 0, 1, commDiag, &status);
		printf("rank : %2d, diag rank : %2d, podatak : %5d", rank, rankDiag, podatak);
	}

	MPI_Finalize();
	return 0;
}