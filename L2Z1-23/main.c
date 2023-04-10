/*

Napisati MPI program koji kreira komunikator koga čine svi procesi sa identifikatorima
deljivim sa 5. Master proces (P0) svim procesima ove grupe šalje po jednu kolone matrice A.
Odštampati identifikatore procesa koji pripadaju novom komunikatoru, a čija suma elemenata
primljene kolone matrice A nije manja od zadate vrednosti v

*/
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD
#define n 4
#define m 6
int main(int argc, char* argv[]) {

	int size, rank, rankComm, sizeComm;
	int i, j;
	int A[n][n];
	int localA[n];
	int sum = 0;
	int v = 10;
	int isDiv5;

	MPI_Comm comm1;
	MPI_Datatype kolona;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	MPI_Type_vector(n, 1, n, MPI_INT, &kolona);
	MPI_Type_commit(&kolona);

	isDiv5 = rank % 5;
	MPI_Comm_split(MCW, isDiv5 == 0, rank, &comm1);
	MPI_Comm_size(comm1, &sizeComm);
	MPI_Comm_rank(comm1, &rankComm);

	if (isDiv5 == 0) {
		if (rankComm == 0) {
			printf("%d", sizeComm);
			for (i = 0; i < n; i++) {
				for (j = 0; j < n; j++) {
					A[i][j] = rand() % 10;
				}
			}
		}
		//MPI_Scatter(&A[0][0], n, MPI_INT, &localA[0], n, MPI_INT, 0, comm1);
		
		if (rankComm == 0) {
			for (i = 0; i < n; i++) localA[i] = A[i][0];
			for (i = 1; i < sizeComm; i++) {
				MPI_Send(&A[0][i], 1, kolona, i, 1, comm1);
			}
		}
		else {
			MPI_Recv(localA, n, MPI_INT, 0, 1, comm1, &status);
		}

		sum = 0;
		for (i = 0; i < n; i++) sum += localA[i];
		if (sum > v) {
			printf("rank : %2d sum : %3d\n", rank, sum);
		}
	}

	MPI_Finalize();

	return 0;
}