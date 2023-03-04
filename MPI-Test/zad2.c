#include <mpi.h>
#include <stdio.h>
/*
Zadatak 1 :
	Napisati program kojim se elementi gornje trougaone matrice A nxn procesa 0 salju u gornju trougaonu matricu T nxn procesa 1
*/
#define n 10
#define MCW MPI_COMM_WORLD
int main(int argc, char* argv[]) {

	int rank;
	int A[n][n];
	int T[n][n];
	MPI_Status status;
	MPI_Datatype ntype;
	int i, j;
	int blocklenghts[n];
	int displacement[n];


	MPI_Init(argc, &argv);

	MPI_Comm_rank(MCW, &rank);
	
	for (i = 0; i < n; i++) {
		blocklenghts[i] = n - i;
		displacement[i] = i * n + i;
		for (j = 0; j < n; j++) {
			A[i][j] = i * n + j;
			T[i][j] = 0;
		}
	}

	MPI_Type_indexed(n, blocklenghts, displacement, MPI_INT, &ntype);
	MPI_Type_commit(&ntype);
	
	if(rank == 0) {
		MPI_Send(&A[0][0], 1, ntype, 1, 1, MCW);
	}
	else if(rank == 1){
		MPI_Recv(&T[0][0], 1, ntype, 0, 1, MCW, &status);
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				printf("%d ", T[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Finalize();

	return 0;
}