/*

	Realizovati MPI program kojim se realizuje mnozenje matrica Anxn i vektora Bn, cime se dobija vektor Cn.
	Matrica A i vektro B se inicijalizuju u master procesu. Broj procesa je p i uredjeni su kao matrica
	qxq = p.
	Matrica A je podeljena u podmatrice dimenzija kxk (k = n/q) i master proces distribuira odgovarajuce blokove matrica A po procesima.
	n = 8
	p = 16

	Vektor se distribuira u delovima od po n/q elemenata (k) tako da nakon slanja procesi u prvoj koloni matrice procesa sadrzi prvih n/q elemenata u 2 koloni drugi n/q...
	na osnovu primljenih podataka svaki rpoces obavlja odgovarajuca izracunavanja i ucestuje u gen rez koji se 
	prikazuje u master procesu
	Slanje podmatrica matrice A svakom pocesu obalja po jednom MPI_Send 1 izvedeni tip podataka
	Slanje blokova vektora b i generisanje rezultata implementirati koriscenjem grupnih operacija i funkcija ze kreiranje 
	novih komunikatora

*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD
#define n 8
#define p 16
int main5(int argc, char** argv) {
	int worldSize, rank, rankCol, rankRow;
	int q = sqrt(p);
	int k = n / q;
	int A[n][n], B[n], C[n];

	//int* localA = (int*)malloc(k * k * sizeof(int));
	//int* localB = (int*)malloc(k * sizeof(int));
	//int* localC = (int*)malloc(k * sizeof(int));
	//int* temp = (int*)malloc(k * sizeof(int));
	int localA[4], localB[2], localC[2], temp[2];
	int i, j;

	MPI_Status status;
	MPI_Request req;
	MPI_Datatype kolone;
	MPI_Group world;
	MPI_Comm rowComm, collumComm;
	MPI_Datatype vrste;
	
	//check if worldSize and p match if not error

	MPI_Init(&argc, &argv);

	printf("Zaba\n");

	MPI_Comm_size(MCW, &worldSize);
	MPI_Comm_rank(MCW, &rank);

	MPI_Comm_split(MCW, rank / q, rank % q, &rowComm);
	MPI_Comm_rank(rowComm, &rankRow);

	MPI_Comm_split(MCW, rank % q, rank / q, &collumComm);
	MPI_Comm_rank(collumComm, &rankCol);

	MPI_Type_vector(k, k, n, MPI_INT, &vrste);
	MPI_Type_commit(&vrste);

	if (rank == 0) {
		//Inicijalizacija
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				A[i][j] = i == j ? 1 : 0;
			}
			B[i] = 1;
		}

		for (i = 0; i < k; i++) {
			for (j = 0; j < k; j++) {
				localA[i * k + j] = A[i][j];
			}
		}

		//slanje
		for (i = 0; i < q; i++) {
			for (j = 0; j < q; j++) {
				if(!(i == 0 && j == 0))
					MPI_Send(&A[i * k][j * k], 1, vrste, i * q + j, 1, MCW, &req);
			}
		}
	}else
		MPI_Recv(localA, k * k, MPI_INT, 0, 1, MCW, &status);
	 
	if(rankCol == 0)
		MPI_Scatter(B, k, MPI_INT, localB, k, MPI_INT, 0, rowComm);
	MPI_Bcast(localB, k, MPI_INT, 0, collumComm);

	for (i = 0; i < k; i++) {
		localC[i] = 0;
		for (j = 0; j < k; j++) {
			localC[i] += localA[i*k + j] * localB[j];
		}
	}

	MPI_Reduce(localC, temp, k, MPI_INT, MPI_SUM, 0, rowComm);
	if (rankCol == 0)
		MPI_Gather(temp, k, MPI_INT, C, k, MPI_INT, 0, collumComm);

	if (rank == 0) {
		for (i = 0; i < n; i++) {
			printf("%3d ", C[i]);
		}
	}

	MPI_Finalize();

	free(localA);
	free(localB);
	free(localC);
	free(temp);
	return 0;
}