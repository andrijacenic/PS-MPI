
/*

Napisati MPI program koji na osnovu podataka o n zaposlenih, nalazi onog sa najmanjom prosecnom platon.
Podaci o zaposlenima se unose sa tastature u master procesu. Za zaposlenog se pamti indetifikator, ime, prezime i prosecna plata.
Svaki proces dobija podatke o n/p zaposlenih (n- broj zaposlenih, p-broj procesa).
Svi podaci se procesima prosledjuju odjednom svaki proces ucestvuje u nalazaenju zaposlenog sa najmanjom prosecnom platom.
Iz master procesa treba odstampati indetifikator i prosecnu platu nadjenog zaposlenog. Grupne operacije + izvedeni tipovi.

*/
#define MSMPI_NO_DEPRECATE_20
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string>
#define MCW MPI_COMM_WORLD

#define n 12 //broj zaposlenih
#define p 4 //broj processa

// n % p  = 0 !!!!

#define MASTER 0
struct Zaposleni
{
	int prosecna_plata;
	int id;
	char ime[12];
	char prezime[12];
};

int main(int argc, char* argv[])
{
	int size, rank;
	int i, j;
	
	Zaposleni zaposleni[n];
	Zaposleni moji_zaposleni[n / p];

	MPI_Datatype zaposleniTip;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	int block_lenghts[4] = { 1, 1, 12, 12 };
	MPI_Aint displacments[4] = { 0, 1 * sizeof(int), 2*sizeof(int), 2 * sizeof(int) + 12 * sizeof(char)};
	MPI_Datatype types[4] = { MPI_INT, MPI_INT, MPI_CHAR, MPI_CHAR };
	MPI_Type_struct(4, block_lenghts, displacments, types, &zaposleniTip);
	MPI_Type_commit(&zaposleniTip);

	//MPI_Type_vector(1, 2 * sizeof(int) + 24 * sizeof(char), 0, MPI_CHAR, &zaposleniTip);
	int prosecna_plata[n] = {
		12000,
		13000,
		15000,
		80000,
		60000,
		19000,
		1000,
		130000,
		150000,
		3000,
		100,
		190000
	};

	if (rank == MASTER) {
		for (i = 0; i < n; i++) {
			zaposleni[i].id = i;
			zaposleni[i].prosecna_plata = prosecna_plata[i];
			strcpy_s(zaposleni[i].ime, "Zika");
			strcpy_s(zaposleni[i].prezime, "Zikic");
		}
	}

	MPI_Scatter(&zaposleni[0], n / p, zaposleniTip, &moji_zaposleni[0], n / p, zaposleniTip, MASTER, MCW);

	Zaposleni* najmanjaPlataKodSebe = &moji_zaposleni[0];
	for (i = 1; i < n / p; i++) {
		if (moji_zaposleni[i].prosecna_plata < najmanjaPlataKodSebe->prosecna_plata)
			najmanjaPlataKodSebe = &moji_zaposleni[i];
	}
	
	int moj[2] = { najmanjaPlataKodSebe->prosecna_plata, najmanjaPlataKodSebe->id };
	int prima[2];

	MPI_Reduce(moj, prima, 1, MPI_2INT, MPI_MINLOC, MASTER, MCW);


	if (rank == MASTER) {
		printf("%d %d", prima[0], prima[1]);
	}

	MPI_Finalize();

}