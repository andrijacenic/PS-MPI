#define MSMPI_NO_DEPRECATE_20
#include <mpi.h>
#define MCW MPI_COMM_WORLD
int main(int argc, char* argv[]) {
	int rank;
	struct { int a; double b;} value;

	int blocklen[2] = { 1,1 };
	MPI_Aint adr[2] = { 0,0 };
	MPI_Datatype types[2] = { MPI_INT, MPI_DOUBLE };
	MPI_Datatype ntype;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MCW, &rank);

	MPI_Address(&value.a, &adr[0]);
	MPI_Address(&value.b, &adr[1]);

	adr[1] -= adr[0];
	adr[0] = 0;
	MPI_Type_struct(2, blocklen, adr, types, &ntype);
	MPI_Type_commit(&ntype); 

	if (rank == 0) {
		value.a = 1;
		value.b = 1.1;
		MPI_Send(&value, 1, ntype, 1, 1, MCW);
	}
	else {
		MPI_Recv(&value, 1, ntype, 0, 1, MCW, &status);
		printf("%d %f", value.a, value.b);
	}
	MPI_Finalize();
	return 0;
}