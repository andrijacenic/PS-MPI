/*
cartasian n m , kolone u novi komunikator, kooridinate procesa sa najecim indetifikatorom se salju
*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define n 4
#define m 6
int main(int argc, char* argv[]) {

	int rank, size;
	int dims[2] = { m, n }, periods[2] = { 1, 1 }, reorder = 1, coords[2];
	int rankKolona, sizeKolona;
	int rankAndCoord[3], rankAndCoordRecv[3];
	int* temp;

	MPI_Comm cartComm, kolonaComm;
	MPI_Status status;
	MPI_Request req;

	// inicijalizacija mpi
	MPI_Init(&argc, &argv);

	// uzimamo rank i size za 
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// kreira kartazionu strukturu od MPI_COMM_WORLD, to su svi procesi, 2 dimenzionalna struktura, dimenzije su m i n periodi su oba 1, reorderuje se
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartComm);
	// uzima se rank iz cartComm, i koordinate
	MPI_Comm_rank(cartComm, &rank);
	MPI_Cart_coords(cartComm, rank, 2, coords);

	//kreiramo novi komunikator gde je color ( ono po cemu se dele) druga koordinata, znaci da se razdvajaju kolone
	MPI_Comm_split(MPI_COMM_WORLD, coords[1], coords[0], &kolonaComm);
	// uzimamo velicini i rank u tom komunikatoru za kolonu
	MPI_Comm_size(kolonaComm, &sizeKolona);
	MPI_Comm_rank(kolonaComm, &rankKolona);

	//Upisujemo vrednosti u niz da bi mogli da saljemo procesima
	rankAndCoord[0] = rank;
	rankAndCoord[1] = coords[0];
	rankAndCoord[2] = coords[1];

	if (rankKolona == sizeKolona - 1) {
		//krecemo od poslednjeg procesa koji nista ne prima samo salje, on salje onom pre njega (rank -1) njegov rak i coordinate
		MPI_Send(&rankAndCoord, 3, MPI_INT, rankKolona - 1, 1, kolonaComm);
	}
	else {
		//ostali procesi i primaju i salju osim prvog. Ovde primamo podatke poslane od predhodnog procesa
		MPI_Recv(&rankAndCoordRecv, 3, MPI_INT, rankKolona + 1, 1, kolonaComm, &status);
		// u temp stavljamo onu pomocnu strukturu za koju je rank veci
		temp = rankAndCoord[0] > rankAndCoordRecv[0] ? rankAndCoord : rankAndCoordRecv;

		if (rankKolona > 0)
			//Ako rank nije 0 (nije master) on salje procesu sa manjem ranku
			MPI_Send(temp, 3, MPI_INT, rankKolona - 1, 1, kolonaComm);
		else {
			//U suprotnom on samo stampa proces koji ima najveci rank
			printf("rank : %3d at (%2d, %2d)\n", temp[0], temp[1], temp[2]);
		}
	}

	MPI_Finalize();
	return 0;
}
