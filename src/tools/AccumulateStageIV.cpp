///////////////////////////////////////////////////////////////////////////////
///
///	\file    AccumulateStageIV.cpp
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

#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

#if defined(TEMPEST_MPIOMP)
	// Initialize MPI
	MPI_Init(&argc, &argv);
#endif

	// Turn off fatal errors in NetCDF
	NcError error(NcError::silent_nonfatal);

try {

	if (argc <= 3) {
		Announce("Usage: %s <dimension name> <input file list> <output file>", argv[0]);
		exit(-1);
	}

	std::string strDimensionName = argv[1];
	std::vector<std::string> vecInputList;
	for (int i = 2; i < argc-1; i++) {
		vecInputList.push_back(argv[i]);
	}
	std::string strOutputFile = argv[argc-1];

	// Open output file
	NcFile ncoutfile(strOutputFile.c_str(), NcFile::Replace);
	if (!ncoutfile.is_valid()) {
		_EXCEPTION1("Unable to open output datafile \"%s\"",
			strOutputFile.c_str());
	}

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

