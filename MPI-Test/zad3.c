#include <mpi.h>
#include <stdio.h>
/*
Zadatak 3 :
	Napisati program kojim se elementi donje trougaone matrice A nxn procesa 0 salju u gornju trougaonu matricu T nxn procesa 1,
	modifikazija zadatka 2
Zadatak 4 : 
	isto samo obrnuto - dodat koja u koju...
*/
#define n 5
#define MCW MPI_COMM_WORLD
#define kojaukoju 1
int main3(int argc, char* argv[]) {

	int rank;
	int A[n][n];
	int T[n][n];
	MPI_Status status;
	MPI_Datatype gornjaMatrica;
	MPI_Datatype donjaMatrica;
	int i, j;
	int blocklenghtsGornja[n];
	int blocklenghtsDonja[n];
	int displacementGornja[n];
	int displacementDonja[n];


	MPI_Init(argc, &argv);

	MPI_Comm_rank(MCW, &rank);

	for (i = 0; i < n; i++) {
		blocklenghtsGornja[i] = n - i;
		blocklenghtsDonja[i] = i+1;
		displacementGornja[i] = i * n + i;
		displacementDonja[i] = i * n;
		for (j = 0; j < n; j++) {
			A[i][j] = i * n + j + 1;
			T[i][j] = 0;
		}
	}

	MPI_Type_indexed(n, blocklenghtsGornja, displacementGornja, MPI_INT, &gornjaMatrica);
	MPI_Type_commit(&gornjaMatrica);
	MPI_Type_indexed(n, blocklenghtsDonja, displacementDonja, MPI_INT, &donjaMatrica);
	MPI_Type_commit(&donjaMatrica);


	if (rank == 0) {
		MPI_Send(&A[0][0], 1, kojaukoju == 0 ? donjaMatrica : gornjaMatrica, 1, 1, MCW);
	}
	else if (rank == 1) {
		MPI_Recv(&T[0][0], 1, kojaukoju == 0 ? gornjaMatrica : donjaMatrica, 0, 1, MCW, &status);
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