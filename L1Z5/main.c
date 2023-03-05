/*
Napisati MPI program koji realizuje množenje matrice Amxn i matrice Bnxk, čime se
dobija rezultujuća matrica Cmxk. Množenje se obavlja tako što master proces šalje svakom
procesu celu matricu A i po k/p kolona matrice B (p-broj procesa, k je deljivo sa p). Svi
procesi učestvuju u izračunavanju. Konačni rezultat množenja se nalazi u master procesu
koji ga i prikazuje. Predvideti da se slanje k/p kolona matrice B svakom procesu obavlja
odjednom i to direktno iz matrice B. Zadatak rešiti korišćenjem grupnih operacija i
izvedenih tipova podataka.

*/

#include <mpi.h>
#include <stdio.h>
#define n 8
#define m 6
#define k 8
#define MCW MPI_COMM_WORLD

int main(int argc, char** argv) {

	int rank, size, i, j, t;
	int A[m][n];
	int B[n][k], localB[n][k];
	int C[m][k], localC[m][k];
	MPI_Status status;
	MPI_Request req;
	MPI_Datatype type, etype;


	MPI_Init(argc, argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	if (rank == 0) {
		printf("Mat A : \n");
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				A[i][j] = rand() % 40;
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}
		printf("Mat B : \n");

		for (i = 0; i < n; i++) {
			for (j = 0; j < k; j++) {
				B[i][j] = i == j ? 1 : 0;
				printf("%d ", B[i][j]);

			}
			printf("\n");
		}
	}
	for (i = 0; i < m; i++) {
		for (j = 0; j < k; j++) {
			localC[i][j] = 0;
		}
	}


	MPI_Bcast(&A[0][0], m * n, MPI_INT, 0, MCW);

	MPI_Type_vector(n, k / size, k, MPI_INT, &type);
	MPI_Type_commit(&type);
	MPI_Type_create_resized(type, 0, k / size * sizeof(int), &etype);
	MPI_Type_commit(&etype);

	MPI_Scatter(&B[0][0], 1, etype, localB, 1, etype, 0, MCW);

	/*if (rank == 0) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < k / size; j++) {
				printf("%d ", localB[i][j]);
			}
			printf("\n");
		}
	}*/
	//printf("rank : %d\n", rank);
	for (i = 0; i < m; i++) {
		for (j = 0; j < k / size; j++) {
			for (t = 0; t < n; t++) {
				localC[i][j] += A[i][t] * localB[t][j];
			}
			//printf("%d ", localC[i][j]);
		}
		//printf("\n");
	}

	MPI_Gather(localC, 1, etype, C, 1, etype, 0, MCW);

	if (rank == 0) {
		printf("final matrix C : \n");
		for (i = 0; i < m; i++) {
			for (j = 0; j < k; j++) {
				printf("%d ", C[i][j]);

			}
			printf("\n");
		}
	}

	MPI_Finalize();
	
	return 0;
}
