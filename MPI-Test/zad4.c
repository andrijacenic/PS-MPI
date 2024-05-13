#define MSMPI_NO_DEPRECATE_20
#include <mpi.h>
#define MCW MPI_COMM_WORLD

int main(int argc, char* argv[]) {
	int rank;
	MPI_Status status;

	struct { int a; double b; float c; } value;

	int blocklen[3] = { 1,1,1 };
	MPI_Datatype types[3] = { MPI_INT, MPI_DOUBLE, MPI_FLOAT };
	MPI_Datatype ntype;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);

	MPI_Aint adr[3] = { 0,0, 0 };

	MPI_Address(&value.a, &adr[0]);
	MPI_Address(&value.b, &adr[1]);
	MPI_Address(&value.c, &adr[2]);

	adr[2] -= adr[0];
	adr[1] -= adr[0];
	adr[0] = 0;

	MPI_Type_struct(3, blocklen, adr, types, &ntype);
	MPI_Type_commit(&ntype);

	if (rank == 0) {
		value.a = 1;
		value.b = 1.1;
		value.c = 2.1;
		MPI_Send(&value, 1, ntype, 1, 1, MCW);
	}
	else {
		MPI_Recv(&value, 1, ntype, 0, 1, MCW, &status);
		printf("%d %f %f", value.a, value.b, value.c);
	}
	MPI_Finalize();
	return 0;
}