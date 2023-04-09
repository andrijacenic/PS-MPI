/*
Napisati MPI program koji kreira komunikator comm1 koji se sastoji od svih procesa sa
identifikatorima deljivim sa 3. Master proces (P0) svim procesima ove grupe šalje po jednu
vrstu matrice A. Odštampati identifikatore procesa koji pripadaju comm1 i čija je suma
elemenata primljene vrste matrice A manja od zadate vrednosti v
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
	int A[m][n];
	int localA[n];
	int sum = 0;
	int v = 10;
	int isDiv3;

	MPI_Comm comm1;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);
	isDiv3 = rank % 3;
	MPI_Comm_split(MCW, isDiv3 == 0, rank, &comm1);
	MPI_Comm_size(comm1, &sizeComm);
	MPI_Comm_rank(comm1, &rankComm);

	if (isDiv3 == 0) {
		if (rankComm == 0) {
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					A[i][j] = rand() % 10  ;
				}
			}
		}
		MPI_Scatter(&A[0][0], n, MPI_INT, &localA[0], n, MPI_INT, 0, comm1);
		sum = 0;
		for (i = 0; i < n; i++) sum += localA[i];
		if (sum > v) {
			printf("rank : %2d sum : %3d\n", rank, sum);
		}
	}

	MPI_Finalize();

	return 0;
}