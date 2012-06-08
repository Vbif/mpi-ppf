// test.cpp : Defines the entry point for the console application.
//

/* testprint.c */
/* John May -- 23 Oct 96 */
/* Tests the parallel print function.  */
/* Copyright (c) 1996-1998 The Regents of the University of California
 *			and Lawrence Livermore National Laboratory
 * All rights reserved.
 */

/* Modification history */
/*
 *	31 Oct 96	johnmay	Test NULL input string.
 *	13 Jan 98	johnmay Use old or new names for header and function
 *	30 Apr 98	johnmay Add barrier after individual writes
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PPF_USE_OLD_NAMES
#include "ptools_ppf.h"
#else
#define PPF_Print PTC_Print
#include "PTCPrint.h"
#endif

char * strings[] = { "zero", "one", "two", "three",
			"three and a half", "four" };

int main( int argc, char ** argv )
{
	int mpi_rank, mpi_size;
	int size;

	int i;

	MPI_Init( &argc, &argv );

	MPI_Comm_rank( MPI_COMM_WORLD, &mpi_rank );
	MPI_Comm_size( MPI_COMM_WORLD, &mpi_size );

	PPF_Print( MPI_COMM_WORLD, "Message from %N\n" );
	PPF_Print( MPI_COMM_WORLD, (mpi_rank < (mpi_size/2) )
				? "Message from first half of nodes (%N)\n"
				: "Message from second half of nodes\n" );
	PPF_Print( MPI_COMM_WORLD, (mpi_rank % 2)
				? "[%N] Message from odd numbered nodes\n"
				: "%N Message from even numbered nodes\n" );

	/* Prevent single-node output from being printed before the
	 * multi-node output above is complete.
	 */
	MPI_Barrier( MPI_COMM_WORLD );

	PPF_Print( MPI_COMM_SELF, "Message from individual nodes\n" );

	MPI_Barrier( MPI_COMM_WORLD );

	PPF_Print( MPI_COMM_WORLD,
				"Message from node %d (%g in dog years)\n",
				mpi_rank, (double)mpi_rank / 7 );
	PPF_Print( MPI_COMM_WORLD, "Here's my string: %s\n",
		strings[mpi_rank % 3] );

	/* Conditional printing */
	if( mpi_rank == 2 || mpi_rank == 3 ) {
		PPF_Print( MPI_COMM_WORLD, "I'm a special node.\n" );
	} else {
		PPF_Print( MPI_COMM_WORLD, NULL );
	}

	/* oddball cases */
	PPF_Print( MPI_COMM_WORLD, "Hello" );
	PPF_Print( MPI_COMM_WORLD, " world\n" );
	PPF_Print( MPI_COMM_WORLD,
	"%N: Only one %%N is converted; the rest come out like this %N\n" );
	PPF_Print( MPI_COMM_WORLD,
	"Testing NULL print; nothing should appear between this line...\n" );
	PPF_Print( MPI_COMM_WORLD, NULL );
	PPF_Print( MPI_COMM_WORLD, "...and this line\n" );

	MPI_Finalize();

	return 0;
}
