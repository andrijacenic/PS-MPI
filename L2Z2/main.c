/*
Napisati MPI program kojim se kreira dvodimenzionalna Cartesian struktura sa n vrsta i m
kolona. U svakom od nxm procesa odštampati identifikatore procesa njegovog levog i desnog
suseda na udaljenosti 2. Smatrati da su procesi u prvoj i poslednjoj koloni jedne vrste susedni
*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define MCW MPI_COMM_WORLD
#define UP 0
#define DOWN 1
#define n 4
#define m 4
int main(int argc, char* argv[]) {

	int rank, size, source, dest, outbuf, i, tag = 1, nbrs[4];
	int dims[2] = { m, n }, periods[2] = { 1, 0 }, reorder = 0, coords[2];
	int susedLevo, susedDesno;

	MPI_Comm cartComm;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MCW, &size);

	MPI_Cart_create(MCW, 2, dims, periods, reorder, &cartComm);
	MPI_Comm_rank(cartComm, &rank);

	MPI_Cart_shift(cartComm, 1, 2, &susedLevo, &susedDesno);
	
	printf("rank : %2d  levo : %2d  desno : %2d", rank, susedLevo, susedDesno);

	MPI_Finalize();

	return 0;
}