/*
Proces 0 kreira matricu reda n i šalje i-om procesu po dve kvazidijagonale matrice, obe
na udaljenosti i od glavne dijagonale. Proces i kreira svoju matricu tako što smešta
primljene dijagonale u prvu i drugu kolonu matrice a ostala mesta popunjava nulama.
Napisati MPI program koji realizuje opisanu komunikaciju korišćenjem izvedenih tipova
podataka i prikazuje vrednosti odgovarajućih kolona.

salje se dijagonala koja je udaljena za rank od glavne
a0 b0 c0 d0
a1 b1 c1 d1
a2 b2 c2 d2
a3 b3 c3 d3

rank = 0
a0 b1 c2 d3
a0 b1 c2 d3

rank = 1
b0 c1 d2
a1 b2 c3
...
*/
#include <mpi.h>
#include <stdio.h>
#define MCW MPI_COMM_WORLD
#define n 8
int main(int argc, char** argv){

	int rank, size, i, j;
	int A[n][n];
	int localA[n][n];
	MPI_Status status;
	MPI_Request req;
	MPI_Datatype send, recv;

	MPI_Init(argc, argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			localA[i][j] = 0;
		}
	}
	if (rank == 0) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				A[i][j] = n * i + j + 1;
			}
		}
	}
	//MPI_Type_vector(1, 1, n + 1, MPI_INT, &send);
	MPI_Type_create_resized(MPI_INT, 0, (n + 1) * sizeof(int), &send);
	MPI_Type_commit(&send);
	//MPI_Type_vector(1, 1, n, MPI_INT, &recv);
	MPI_Type_create_resized(MPI_INT, 0, n* sizeof(int), &recv);
	MPI_Type_commit(&recv);

	if (rank == 0) {
		printf("original : \n");
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}

		for (int i = 0; i < size; i++) {
			MPI_Isend(&A[0][i], n - i, send, i, 0, MCW, &req);
			MPI_Isend(&A[i][0], n - i, send, i, 1, MCW, &req);
		}
	}

	MPI_Recv(&localA[0][0], n - rank, recv, 0, 0, MCW, &status);
	MPI_Recv(&localA[0][1], n - rank, recv, 0, 1, MCW, &status);

	printf("\nprocess %d : \n", rank);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%d ", localA[i][j]);
		}
		printf("\n");
	}

	MPI_Finalize();

	return 0;
}