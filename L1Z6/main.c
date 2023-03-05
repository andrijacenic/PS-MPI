/*
Napisati MPI program koji realizuje množenje matrica A i B reda n, čime se dobija
rezultujuća matrica C. Nakon toga, u matrici C pronaći maksimalnu vrednost elemenata
svake kolone. Množenje se obavlja tako što master proces šalje svakom procesu radniku
po jednu vrstu prve matrice i celu drugu matricu. Svaki proces računa po jednu vrstu
rezultujuće matrice i šalje master procesu. Svi procesi učestvuju u izračunavanju.
Štampati dobijenu matricu kao i maksimalne vrednosti elemenata svake kolone. Zadatak
rešiti korišćenjem grupnih operacija

*/

#include <mpi.h>
#include <stdio.h>
#define n 8
#define MCW MPI_COMM_WORLD
int main(int argc, char** argv) {

	int rank, size, i, j, t;
	int A[n][n], localA[n];
	int B[n][n];
	int C[n][n], localC[n];
	int max[n];
	MPI_Status status;
	MPI_Request req;
	MPI_Datatype type, etype;


	MPI_Init(argc, argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);
	if (size != n) {
		printf("broj procesa mora da bude %d", n);
		MPI_Finalize();
		return -1;
	}

	MPI_Type_vector(1, n, n, MPI_INT, &type);
	MPI_Type_commit(&type); 
	//MPI_Type_create_resized(type, 0, 1 * sizeof(int), &etype);
	//MPI_Type_commit(&etype);

	if (rank == 0) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				B[i][j] = i+j == n-1 ? 1 : 0;
				A[i][j] = i * n + j + 1;
			}
		}
	}

	MPI_Bcast(B, n * n, MPI_INT, 0, MCW);
	MPI_Scatter(A, 1, type, localA, 1, type, 0, MCW);

	for (i = 0; i < n; i++) {
		localC[i] = 0;
		for (j = 0; j < n; j++) {
			localC[i] += localA[j] * B[j][i];
		}
		MPI_Reduce(&localC[i], &max[i], 1, MPI_INT, MPI_MAX, 0, MCW);
	}

	MPI_Gather(localC, n, MPI_INT, C, n, MPI_INT, 0, MCW);

	if (rank == 0) {
		printf("Matrica C :\n");
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				printf("%d\t", C[i][j]);
			}
			printf("\n");
		}
		printf("Maksimalni elementi svake kolone : \n");
		for (i = 0; i < n; i++) {
			printf("%d\t", max[i]);
		}
		
	}

	MPI_Finalize();
	return 0;
}