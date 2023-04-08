/*
	Napisati MPI program kojim se vrsi podela procesa clanova komunikatora MPI_COMM_WORLD u dve grupe: parni id i neparni id
*/

#include <mpi.h>
#include <stdio.h>
#define MCW MPI_COMM_WORLD
#define n 8
int main1(int argc, char** argv) {
	MPI_Group world, odd, even;
	int i, p, nOdd, nEven, members[n], evenRank, oddRank, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &p);
	MPI_Comm_rank(MCW, &rank);
	MPI_Comm_group(MCW, &world);

	nEven = (p + 1) / 2;
	nOdd = p - nEven;
	for (i = 0; i < nEven; i++)
		members[i] = 2 * i;
	MPI_Group_incl(world, nEven, members, &even);
	MPI_Group_rank(even, &evenRank);
	MPI_Group_excl(world, nEven, members, &odd);
	MPI_Group_rank(odd, &oddRank);

	printf("%d\t%d\t%d\n", rank, evenRank, oddRank);

	MPI_Finalize();
	return 0;
}

//Sa color
int main2(int argc, char** argv) {
	MPI_Group world, newComm;
	int i, p, groupSize, groupRank, rank, color, key;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &p);
	MPI_Comm_rank(MCW, &rank);

	color = rank % 2;
	key = 7;
	MPI_Comm_split(MCW, color, key, &newComm);
	MPI_Comm_size(newComm, &groupSize);
	MPI_Comm_rank(newComm, &groupRank);

	printf("%d\t%d\t%d\n", rank, groupSize, groupRank);

	MPI_Finalize();

	return 0;
}
