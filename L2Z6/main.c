/*
Napisati MPI program kojim se kreira dvodimenzionalna Cartesian struktura sa n vrsta i m
kolona. Za svaki skup procesa koji pripadaju istoj koloni strukture kreirati novi komunikator.
Master procesu iz svake kolone poslati koordinate procesa sa najvećim identifikatorom i
prikazati ih.
*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD
#define n 4
#define m 4
int main(int argc, char* argv[]) {

	int rank, size;
	int dims[2] = { m, n }, periods[2] = { 1, 1 }, reorder = 1, coords[2];
	int rankKolona, sizeKolona;
	int rankAndCoord[3], rankAndCoordRecv[3];
	int* temp;

	MPI_Comm cartComm, kolonaComm;
	MPI_Status status;
	MPI_Request req;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	MPI_Cart_create(MCW, 2, dims, periods, reorder, &cartComm);
	MPI_Comm_rank(cartComm, &rank);
	MPI_Cart_coords(cartComm, rank, 2, coords);

	MPI_Comm_split(MCW, rank % m, rank / m, &kolonaComm);
	MPI_Comm_size(kolonaComm, &sizeKolona);
	MPI_Comm_rank(kolonaComm, &rankKolona);

	rankAndCoord[0] = rank;
	rankAndCoord[1] = coords[0];
	rankAndCoord[2] = coords[1];

	if (rankKolona == sizeKolona - 1) {
		MPI_Send(&rankAndCoord, 3, MPI_INT, rankKolona - 1, 1, kolonaComm);
	}
	else{
		MPI_Recv(&rankAndCoordRecv, 3, MPI_INT, rankKolona + 1, 1, kolonaComm, &status);
		temp = rankAndCoord[0] > rankAndCoordRecv[0] ? rankAndCoord : rankAndCoordRecv;
		if (rankKolona > 0)
			MPI_Send(temp, 3, MPI_INT, rankKolona - 1, 1, kolonaComm);
		else {
			printf("rank : %3d at (%2d, %2d)\n", temp[0], temp[1], temp[2]);
		}
	}

	MPI_Finalize();
	return 0;
}
