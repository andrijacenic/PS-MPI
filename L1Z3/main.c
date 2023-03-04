/*
Proces 0 kreira matricu reda n i šalje i-om procesu po dve kvazidijagonale matrice, obe
na udaljenosti i od glavne dijagonale. Proces i kreira svoju matricu tako što smešta
primljene dijagonale u prvu i drugu kolonu matrice a ostala mesta popunjava nulama.
Napisati MPI program koji realizuje opisanu komunikaciju korišćenjem izvedenih tipova
podataka i prikazuje vrednosti odgovarajućih kolona.

*/
#include <mpi.h>
