/*
Napisati MPI program koji pronalazi minimalnu vrednost u delu matrice reda n (n-parno)
koga čine kolone matrice sa parnim indeksom (j=0,2,4,...). Matrica je inicijalizovana u
master procesu (P0). Svaki proces treba da dobije elemente kolona sa parnim indeksom iz
odgovarajućih n/p vrsta (p-broj procesa, n deljivo sa p) i nađe lokalni minimum. Na taj
način, P0 dobija elemente kolona sa parnim indeksom iz prvih n/p vrsta i nalazi lokalni
minimum, P1 dobija elemente kolona sa parnim indeksom iz sledećih n/p vrsta i nalazi
lokalni minimum itd. Nakon toga, u master procesu se izračunava i na ekranu prikazuje
globalni minimum u traženom delu matrice. Zadatak realizovati korišćenjem isključivo
grupnih operacija i izvedenih tipova podataka.

mat
a0 b0 c0 d0 e0 f0 g0 h0
a1 b1 c1 d1 e1 f1 g1 h1
a2 b2 c2 d2 e2 f2 g2 h2
a3 b3 c3 d3 e3 f3 g3 h3
a4 b4 c4 d4 e4 f4 g4 h4
a5 b5 c5 d5 e5 f5 g5 h5
a6 b6 c6 d6 e6 f6 g6 h6
a7 b7 c7 d7 e7 f7 g7 h7
8x8

broj procesa p=4;
svaki proces dobija 2 vrste
p0 : 
 a0 c0 e0 g0 a1 c1 e1 g1
p1 : 
 a2 c2 e2 g2 a3 c3 e3 g3
 ...
*/
#include <mpi.h>
#include <stdio.h>
#define MCW MPI_COMM_WORLD
#define n 8
//#define p 4

int main(int argc, char* argv[]) {
	int rank;
	int size;
	int A[n][n];
	int localA[n*n];
	int i, j;
	int min;
	int globalMin;
	MPI_Status status;
	MPI_Datatype type, etype;

	MPI_Init(argc, argv);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_size(MCW, &size);

	if (rank == 0) {
		for (i = 0; i < n; i++) {
			if (i % (n / size) == 0)
				printf("Proces %d dobija : \n", i);
			for (j = 0; j < n; j++){
				A[i][j] = rand() % 50;
				if(j % 2 == 0)
					printf("%d ", A[i][j]); // stampamo samo sta je bitno da bi videli gde se sta treba da se obradi
			}
			if ((i + 1) % (n / size) == 0)
				printf("\n");
			printf("\n");
		}
	}
	// broj vrsta * broj kolona
	MPI_Type_vector(n / size * n / 2, 1, 2, MPI_INT, &type);
	MPI_Type_commit(&type);
	// broj vrsta * broj kolona, ovde se gledaju sve kolone, bez izostavljenih
	MPI_Type_create_resized(type, 0, n / size * n * sizeof(int), &etype);
	MPI_Type_commit(&etype);

	MPI_Scatter(&A[0][0], 1, etype, localA, n / size * n / 2, MPI_INT, 0, MCW);

	min = INT32_MAX;
	for (i = 0; i < n / size * n / 2; i++) {
		if (min > localA[i])
			min = localA[i];
	}
	printf("lokalni minimum %d : %d\n", rank, min);
	MPI_Reduce(&min, &globalMin, 1, MPI_INT, MPI_MIN, 0, MCW);
	if (rank == 0) {
		printf("globalni minimum je : %d", globalMin);
	}

	MPI_Finalize();

	return 0;
}