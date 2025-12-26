///////////////////////////////////////////////////////////////////////////////
///
///	\file    EffectiveResolution.cpp
///	\author  Paul Ullrich
///	\version December 26, 2025
///
///	<remarks>
///		Copyright 2025 Paul Ullrich
///
///		This file is distributed as part of the Tempest source code package.
///		Permission is granted to use, copy, modify and distribute this
///		source code and its documentation under the terms of the GNU General
///		Public License.  This software is provided "as is" without express
///		or implied warranty.
///	</remarks>

#include "CommandLine.h"
#include "Exception.h"
#include "Announce.h"
#include "SimpleGrid.h"
#include "NetCDFUtilities.h"
#include "Variable.h"
#include "CoordTransforms.h"

#include "../Eigen/Dense"

#include <set>
#include <queue>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

	// Turn off fatal errors in NetCDF
	NcError error(NcError::silent_nonfatal);

try {

	// Input data file
	std::string strInputData;

	// Input data file list
	std::string strInputDataList;

	// Connectivity file
	std::string strConnectivity;

	// Variable
	std::string strVariable;

	// Output data file
	std::string strOutputData;

	// Sample distance (in degrees)
	double dDistDeg;

	// Regional data
	bool fRegional;

	// Name of latitude dimension
	std::string strLatitudeName;

	// Name of longitude dimension
	std::string strLongitudeName;

	// Parse the command line
	BeginCommandLine()
		CommandLineString(strInputData, "in_data", "");
		CommandLineString(strInputDataList, "in_data_list", "");
		CommandLineString(strOutputData, "out", "");
		CommandLineString(strVariable, "var", "");
		CommandLineDoubleD(dDistDeg, "dist", 1.0, "(degrees)");
		CommandLineBool(fRegional, "regional");

		CommandLineString(strLongitudeName, "lonname", "lon");
		CommandLineString(strLatitudeName, "latname", "lat");

		ParseCommandLine(argc, argv);
	EndCommandLine(argv)

	AnnounceBanner();

	// Check command line arguments
	if ((strInputData.length() == 0) && (strInputDataList.length() == 0)) {
		_EXCEPTIONT("No input data file (--in_data) or (--in_data_list)"
			" specified");
	}
	if ((strInputData.length() != 0) && (strInputDataList.length() != 0)) {
		_EXCEPTIONT("Only one of (--in_data) or (--in_data_list)"
			" may be specified");
	}
	if (strOutputData.length() == 0) {
		_EXCEPTIONT("No output data file (--out) specified");
	}
	if (dDistDeg <= 0.0) {
		_EXCEPTIONT("Distance --dist must be positive");
	}

	// Distance in radians
	double dDistRad = DegToRad(dDistDeg);

	// Create Variable registry and get the Variable being computed
	VariableRegistry varreg;
	VariableIndex varix = varreg.FindOrRegister(strVariable);
	Variable & var = varreg.Get(varix);

	// Load input file list
	std::vector<std::string> vecInputFiles;

	if (strInputData.length() != 0) {
		vecInputFiles.push_back(strInputData);

	} else {
		std::ifstream ifInputFileList(strInputDataList.c_str());
		if (!ifInputFileList.is_open()) {
			_EXCEPTION1("Unable to open file \"%s\"",
				strInputDataList.c_str());
		}
		std::string strFileLine;
		while (std::getline(ifInputFileList, strFileLine)) {
			if (strFileLine.length() == 0) {
				continue;
			}
			if (strFileLine[0] == '#') {
				continue;
			}
			vecInputFiles.push_back(strFileLine);
		}

		if (vecInputFiles.size() == 0) {
			_EXCEPTIONT("File --in_data_list must contain at least one filename");
		}
	}

	// The SimpleGrid used in this calculation
	SimpleGrid grid;

	// Grid size
	size_t sGridSize = 0;

	// Total times
	size_t sTotalTimes = 0;

	// Total sum of squares
	std::vector<float> dTSS;

	// Output data
	std::vector<float> dResults;

	// Sample points
	std::vector< std::vector<size_t> > ipts;

	// Mean value of y at each point
	std::vector<float> dMeanState;

	// Accumulated matrix coefficients of X^T X
	std::vector< Eigen::Matrix<float, 5, 5> > vecematXTX;

	// Vector of coefficients of X^T y
	std::vector< Eigen::Matrix<float, 5, 1> > vecematXTy;

	// Populate the matrices used for linear regression
	for (size_t f = 0; f < vecInputFiles.size(); f++) {

		// Parse the input files on this line
		NcFileVector vecFiles;
		vecFiles.ParseFromString(vecInputFiles[f]);

		// Initialize the grid, if not already initialized
		if (!grid.IsInitialized()) {

			// Check for connectivity file
			if (strConnectivity != "") {
				AnnounceStartBlock("Generating grid information from connectivity file");
				grid.FromFile(strConnectivity);
				AnnounceEndBlock("Done");

			// Try generating grid information from data file
			} else {
				AnnounceStartBlock("No connectivity file specified");
				Announce("Attempting to generate latitude-longitude grid from data file");

				grid.GenerateLatitudeLongitude(
					vecFiles[0],
					strLatitudeName,
					strLongitudeName,
					fRegional,
					false);

				if (grid.m_nGridDim.size() != 2) {
					_EXCEPTIONT("Logic error when generating connectivity");
				}
			}

			// Grid size
			sGridSize = grid.GetSize();

			// Generate the kd-tree
			grid.BuildKDTree();

			// Initialize the sample coordinates
			ipts.resize(sGridSize);
			for (size_t i = 0; i < sGridSize; i++) {
				ipts[i].resize(4);

				double dLonRad0 = grid.m_dLon[i];
				double dLatRad0 = grid.m_dLat[i];

				ipts[i][0] = grid.NearestNode(dLonRad0 + dDistRad, dLatRad0           );
				ipts[i][1] = grid.NearestNode(dLonRad0           , dLatRad0 + dDistRad);
				ipts[i][2] = grid.NearestNode(dLonRad0 - dDistRad, dLatRad0           );
				ipts[i][3] = grid.NearestNode(dLonRad0           , dLatRad0 - dDistRad);
			}

			// Initialize the results array
			dResults.resize(sGridSize, 0.0);

			// Initialize the mean array
			dMeanState.resize(sGridSize, 0.0);

			// Initialize the TSS array
			dTSS.resize(sGridSize, 0.0);
		}

		// Read the time data
		const NcTimeDimension & vecTimes = vecFiles.GetNcTimeDimension(0);

		// Accumulate times
		sTotalTimes += vecTimes.size();

		// Loop through all times
		for (size_t t = 0; t < vecTimes.size(); t++) {

			// Load the search variable data
			vecFiles.SetTime(vecTimes[t]);
			var.LoadGridData(varreg, vecFiles, grid);
			const DataArray1D<float> & dataState = var.GetData();

			// Update coefficients
			for (size_t i = 0; i < sGridSize; i++) {
				float yi = dataState[i];
				float x0i = dataState[ipts[i][0]];
				float x1i = dataState[ipts[i][1]];
				float x2i = dataState[ipts[i][2]];
				float x3i = dataState[ipts[i][3]];

				vecematXTX[i](0, 1) += x0i;
				vecematXTX[i](0, 2) += x1i;
				vecematXTX[i](0, 3) += x2i;
				vecematXTX[i](0, 4) += x3i;

				vecematXTX[i](1, 1) += x0i * x0i;
				vecematXTX[i](1, 2) += x0i * x1i;
				vecematXTX[i](1, 3) += x0i * x2i;
				vecematXTX[i](1, 4) += x0i * x3i;

				vecematXTX[i](2, 2) += x1i * x1i;
				vecematXTX[i](2, 3) += x1i * x2i;
				vecematXTX[i](2, 4) += x1i * x3i;

				vecematXTX[i](3, 3) += x2i * x2i;
				vecematXTX[i](3, 4) += x2i * x3i;

				vecematXTX[i](4, 4) += x3i * x3i;

				vecematXTy[i](0) += yi;
				vecematXTy[i](1) += x0i * yi;
				vecematXTy[i](2) += x1i * yi;
				vecematXTy[i](3) += x2i * yi;
				vecematXTy[i](4) += x3i * yi;
			}
		}
	}

	// Solve the linear regression system
	for (size_t i = 0; i < sGridSize; i++) {

		// Symmetrize the matrix
		vecematXTX[i](0, 0) = static_cast<float>(sTotalTimes);

		vecematXTX[i](1, 0) = vecematXTX[i](0, 1);
		vecematXTX[i](2, 0) = vecematXTX[i](0, 2);
		vecematXTX[i](3, 0) = vecematXTX[i](0, 3);
		vecematXTX[i](4, 0) = vecematXTX[i](0, 4);

		vecematXTX[i](2, 1) = vecematXTX[i](1, 2);
		vecematXTX[i](3, 1) = vecematXTX[i](1, 3);
		vecematXTX[i](4, 1) = vecematXTX[i](1, 4);

		vecematXTX[i](3, 2) = vecematXTX[i](2, 3);
		vecematXTX[i](4, 2) = vecematXTX[i](2, 4);

		vecematXTX[i](4, 3) = vecematXTX[i](3, 4);

		dMeanState[i] = vecematXTy[i](0) / static_cast<float>(sTotalTimes);

		// Solve the matrix
		Eigen::LLT< Eigen::Matrix<float,5,5> > llt(vecematXTX[i]);
		if (llt.info() != Eigen::Success) {
			Announce("Error in cell %lu: Cholesky failed: matrix not SPD", i);
			dResults[i] = -1.0f;
		} else {
			Eigen::Matrix<float, 5, 1> ematResult = llt.solve(vecematXTy[i]);
			vecematXTy[i] = ematResult;
		}
	}

	// Calculate explained sum of squares and total sum of squares and
	// store in dResults and dTSS respectively
	for (size_t f = 0; f < vecInputFiles.size(); f++) {

		// Parse the input files on this line
		NcFileVector vecFiles;
		vecFiles.ParseFromString(vecInputFiles[f]);

		// Read the time data
		const NcTimeDimension & vecTimes = vecFiles.GetNcTimeDimension(0);

		for (size_t t = 0; t < vecTimes.size(); t++) {

			// Load the search variable data
			vecFiles.SetTime(vecTimes[t]);
			var.LoadGridData(varreg, vecFiles, grid);
			const DataArray1D<float> & dataState = var.GetData();

			for (size_t i = 0; i < sGridSize; i++) {
				if (dResults[i] != -1.0) {
					float yi = dataState[i];
					float x0i = dataState[ipts[i][0]];
					float x1i = dataState[ipts[i][1]];
					float x2i = dataState[ipts[i][2]];
					float x3i = dataState[ipts[i][3]];

					float dHatYi =
						vecematXTy[i](0)
						+ vecematXTy[i](1) * x0i
						+ vecematXTy[i](2) * x1i
						+ vecematXTy[i](3) * x2i
						+ vecematXTy[i](4) * x3i;

					float dYiMinusYbar = dataState[i] - dMeanState[i];
					dTSS[i] += dYiMinusYbar * dYiMinusYbar;

					float dHatYiMinusYbar = dHatYi - dMeanState[i];
					dResults[i] += dHatYiMinusYbar * dHatYiMinusYbar;
				}
			}
		}
	}

	// Compute R2 = ESS / TSS
	for (size_t i = 0; i < sGridSize; i++) {
		dResults[i] /= dTSS[i];
	}

	// Write results
	{
		// Parse the input files on this line
		NcFileVector vecFiles;
		vecFiles.ParseFromString(vecInputFiles[0]);

		NcFile & ncinfile = *(vecFiles[0]);

		NcFile ncoutfile(strOutputData.c_str(), NcFile::Write);
		if (!ncoutfile.is_valid()) {
			_EXCEPTION1("Unable to open output datafile \"%s\"",
				strOutputData.c_str());
		}

		// Write grid information to output file
		NcDim * dimGrid[2];

		if (grid.m_nGridDim.size() == 1) {
			dimGrid[0] = ncoutfile.add_dim("ncol", grid.m_nGridDim[0]);

			NcVar * varOut = ncoutfile.add_var("R2", ncFloat, dimGrid[0]);
			if (varOut == NULL) {
				_EXCEPTIONT("Unable to create variable \"R2\" in output file");
			}

			varOut->put(&(dResults[0]), grid.m_nGridDim[0]);

		} else if (grid.m_nGridDim.size() == 2) {
			CopyNcLatitudeLongitude(
				ncinfile,
				ncoutfile,
				strLatitudeName,
				strLongitudeName,
				grid.m_nGridDim,
				&dimGrid[0],
				&dimGrid[1]
			);

			NcVar * varOut = ncoutfile.add_var("R2", ncFloat, dimGrid[0], dimGrid[1]);
			if (varOut == NULL) {
				_EXCEPTIONT("Unable to create variable \"R2\" in output file");
			}

			varOut->put(&(dResults[0]), grid.m_nGridDim[0], grid.m_nGridDim[1]);

		} else {
			_EXCEPTIONT("Only 1D or 2D spatial data supported");
		}
	}

} catch(Exception & e) {
	Announce(e.ToString().c_str());
}
}

///////////////////////////////////////////////////////////////////////////////

