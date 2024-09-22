///////////////////////////////////////////////////////////////////////////////
///
///	\file    Test.cpp
///	\author  Paul Ullrich
///	\version September 21, 2024
///
///	<remarks>
///		Copyright 2024 Paul Ullrich
///
///		This file is distributed as part of the Tempest source code package.
///		Permission is granted to use, copy, modify and distribute this
///		source code and its documentation under the terms of the GNU General
///		Public License.  This software is provided "as is" without express
///		or implied warranty.
///	</remarks>

#if defined(TEMPEST_MPIOMP)
#include <mpi.h>
#endif

#include "Exception.h"
#include "Announce.h"

#include "netcdfcpp.h"

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

#if defined(TEMPEST_MPIOMP)
	// Initialize MPI
	MPI_Init(&argc, &argv);
#endif

	// Turn off fatal errors in NetCDF
	NcError error(NcError::silent_nonfatal);

try {

	AnnounceBanner();

} catch(Exception & e) {
	AnnounceOutputOnAllRanks();
	AnnounceSetOutputBuffer(stdout);
	Announce(e.ToString().c_str());

#if defined(TEMPEST_MPIOMP)
	MPI_Abort(MPI_COMM_WORLD, -1);
#endif
}

#if defined(TEMPEST_MPIOMP)
	// Deinitialize MPI
	MPI_Finalize();
#endif

}

///////////////////////////////////////////////////////////////////////////////

