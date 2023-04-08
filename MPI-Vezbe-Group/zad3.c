/*
	Mnozenje matrice A i B, nxn, C = A * B, 0 master, salja svima po vrtu i kolonu
	Kolonu matrice A i vrstu matrice B , master proces ne ucestvuje u izracunavanju 
*/

#include <mpi.h>
#include <stdio.h>
#define MCW MPI_COMM_WORLD
#define m 2
#define n 3
#define k 4
// A mxn, B nxk
int main4(int argc, char** argv) {
	int i,j,l, p, rank, matRank;
	int A[m][n], B[n][k], C[m][k];
	int kolona[m], vrsta[k], tmp[m][k];
	int n0[1] = { 0 };

	MPI_Status status;
	MPI_Datatype kolone;
	MPI_Group world, mat;
	MPI_Comm comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &p);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_group(MCW, &world);
	if (p != n+1) {
		if (rank == 0)
			printf("Size should be %d\n", n);
		MPI_Finalize();
		return;
	}
	MPI_Type_vector(m, 1, m, MPI_INT, &kolone);
	MPI_Type_commit(&kolone);

	MPI_Group_excl(world, 1, n0, &mat);
	MPI_Comm_create(MCW, mat, &comm);
	MPI_Group_rank(mat, &matRank);

	
	if (rank == 0) {
		//inicijalizujemo A i B
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				A[i][j] = i == j ? 1 : 0;
			}
		}
		for (i = 0; i < n; i++) {
			for (j = 0; j < k; j++) {
				B[i][j] = i == j ? 2 : 0;
			}
		}
		for (i = 0; i < n; i++) {
			// slanje vrsta
			MPI_Send(&B[i][0], k, MPI_INT, i + 1, 1, MCW);
			//slanje Kolona
			MPI_Send(&A[0][i], 1, kolone, i + 1, 2, MCW);
		}
	}
	else {
		//Primi vrstu
		MPI_Recv(vrsta, k, MPI_INT, 0, 1, MCW, &status);
		//Primi kolonu
		MPI_Recv(kolona, m, MPI_INT, 0, 2, MCW, &status);

		//Racunica
		for (i = 0; i < m; i++) {
			for (j = 0; j < k; j++) {
				tmp[i][j] = kolona[i] * vrsta[j];
			}
		}
		// sumira se sve
		MPI_Reduce(&tmp[0][0], &C[0][0], m * k, MPI_INT, MPI_SUM, 0, comm);
	}
	if (matRank == 0) {
		for (i = 0; i < m; i++) {
			for (j = 0; j < k; j++) {
				printf("%2d ", C[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Finalize();
	return 0;
}