/*
Proces 0 kreira matricu reda n i šalje i-om procesu po dve kvazidijagonale matrice, obe
na udaljenosti i od sporedne dijagonale. Proces i kreira svoju matricu tako što smešta
primljene dijagonale u prvu i drugu vrstu matrice a ostala mesta popunjava nulama.
Napisati MPI program koji realizuje opisanu komunikaciju korišćenjem izvedenih tipova
podataka i prikazuje vrednosti odgovarajućih kolona.

a0 b0 c0 d0
a1 b1 c1 d1
a2 b2 c2 d2
a3 b3 c3 d3

po : 
d0 c1 b2 a3
d0 c1 b2 a3
0  0  0  0
0  0  0  0

p1:
c0 b1 a2 0
d1 c2 b3 0
0  0  0  0
0  0  0  0
...

*/

#include <mpi.h>
#include <stdio.h>
#define n 8
#define MCW MPI_COMM_WORLD

int main(int argc, char* argv[]) {

	int rank, size, i, j;
	MPI_Status status;
	MPI_Request req;
	MPI_Datatype send, recv;
	int A[n][n], localA[n][n];

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	MPI_Type_create_resized(MPI_INT, 0, (n-1)*sizeof(int), &send);
	MPI_Type_commit(&send);
	MPI_Type_vector(1, n - rank, n, MPI_INT, &recv);
	MPI_Type_commit(&recv);

	if (rank == 0) {
		printf("original : \n");
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				A[i][j] = n * i + j + 1;
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}
		for (i = 0; i < size; i++) {
			MPI_Isend(&A[0][(n-1)-i], n - i, send, i, 0, MCW, &req);
			MPI_Isend(&A[i][n-1], n - i, send, i, 1, MCW, &req);
		}
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			localA[i][j] = 0;
		}
	}
	MPI_Recv(localA, 1, recv, 0, 0, MCW, &status);
	MPI_Recv(&localA[1][0], 1, recv, 0, 1, MCW, &status);

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