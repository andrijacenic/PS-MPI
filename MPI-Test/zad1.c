#include <mpi.h>
#include <stdio.h>
/*
Zadatak 1 :
	Napisati program kojim se koriscenjem izvedenih tipova podataka vrsi slanje kolone 0 procesa 0 u vrstu 0 procesa 1
*/
#define n 10
#define MCW MPI_COMM_WORLD
int main1(int argc, char* argv[]) {
	
	int rank;
	float A[n][n];
	MPI_Status status;
	MPI_Datatype ntype;
	int i, j;

	MPI_Init(argc, &argv);

	MPI_Comm_rank(MCW, &rank);
	MPI_Type_vector(n, 1, n, MPI_FLOAT, &ntype);
	MPI_Type_commit(&ntype);
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			A[i][j] = i * rank;
		}
	}

	if(rank == 1){
		MPI_Send(&A[0][0], 1, ntype, 0, rank, MCW);
	}
	else if (rank == 0) {
		MPI_Recv(&A[0][0], n, MPI_FLOAT, 1, 1, MCW, &status);

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				printf("%f ", A[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Finalize();

	return 0;
}