///////////////////////////////////////////////////////////////////////////////
///
///	\file    ColorMap.cpp
///	\author  Paul Ullrich
///	\version July 22, 2023
///

#include "ColorMap.h"
#include <iostream>
#include <sstream>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////

ColorMapLibrary::ColorMapLibrary(
	const std::string & strResourceDir
) :
	m_strResourceDir(strResourceDir)
{
/*
	// Get all *.rgb files in the resource directory
	wxDir dirResources(strResourceDir);
	if (!dirResources.IsOpened()) {
		std::cerr << "ERROR: Cannot open directory \"" << m_strResourceDir << "\". Resources will not be populated." << std::endl;
		return;
	}

	std::vector<std::string> vecFilenames;
	{
		std::string strFilename;
		bool cont = dirResources.GetFirst(&strFilename, _T("*.rgb"), wxDIR_FILES);
		while (cont) {
			vecFilenames.push_back(strFilename);
			cont = dirResources.GetNext(&strFilename);
		}
	}

	for (const auto& path: vecFilenames) {

		// Extract the colormap from the path name in a portable way
		wxFileName wxfilePath(path);
		std::string strColorMapName = wxfilePath.GetName();
		
		// Add the default colormap to the beginning of the list
		if (strColorMapName == DEFAULT_COLORMAP) {
			m_vecColorMapNames.insert(m_vecColorMapNames.begin(), strColorMapName.ToStdString());

		// Add the colormap to the list
		} else {
			m_vecColorMapNames.push_back(strColorMapName.ToStdString());
		}

	}
*/
	// Register the hand-coded colormaps
	m_vecColorMapNames.push_back("jet");
	m_vecColorMapNames.push_back("bluered");
	m_vecColorMapNames.push_back("gray");

	m_vecColorMapNames.push_back("INVALID");
}

////////////////////////////////////////////////////////////////////////////////

size_t ColorMapLibrary::GetColorMapCount() {
	return m_vecColorMapNames.size()-1;
}

////////////////////////////////////////////////////////////////////////////////

const std::string & ColorMapLibrary::GetColorMapName(
	size_t ix
) {
	if (ix >= m_vecColorMapNames.size()) {
		return m_vecColorMapNames[m_vecColorMapNames.size()-1];
	} else {
		return m_vecColorMapNames[ix];
	}
}

////////////////////////////////////////////////////////////////////////////////

void ColorMapLibrary::GenerateColorMap(
	const std::string & strColorMap,
	ColorMap & colormap
) const {
	colormap.resize(256, std::vector<unsigned char>(3, 0) );

	// "gray" color map
	if (strColorMap == "gray") {
		for (int i = 0; i < 256; i++) {
			colormap[i][0] = i;
			colormap[i][1] = i;
			colormap[i][2] = i;
		}

	// "jet" color map
	} else if (strColorMap == "jet") {
		for (int i = 0; i <= 32; i++) {
			colormap[i][0] = 0;
			colormap[i][1] = 0;
			colormap[i][2] = (i+32)*4-1;
		}
		for (int i = 33; i <= 96; i++) {
			colormap[i][0] = 0;
			colormap[i][1] = (i-32)*4-1;
			colormap[i][2] = 255;
		}
		for (int i = 97; i < 160; i++) {
			colormap[i][0] = (i-96)*4-1;
			colormap[i][1] = 255;
			colormap[i][2] = (160-i)*4-1;
		}
		for (int i = 160; i <= 224; i++) {
			colormap[i][0] = 255;
			colormap[i][1] = (224-i)*4-1;
			colormap[i][2] = 0;
		}
		for (int i = 224; i < 256; i++) {
			colormap[i][0] = (288-i)*4-1;
			colormap[i][1] = 0;
			colormap[i][2] = 0;
		}

	// "bluered" colormap
	} else if (strColorMap == "bluered") {
		for (int i = 0; i < 128; i++) {
			colormap[i][0] = i*2;
			colormap[i][1] = i*2;
			colormap[i][2] = 127+i;
		}
		for (int i = 128; i < 256; i++) {
			colormap[i][0] = 382-i;
			colormap[i][1] = (255-i)*2;
			colormap[i][2] = (255-i)*2;
		}

	// Load colormap from resource directory
	} /*else {

		wxFileName wxfileColormapPath(m_strResourceDir, strColorMap, _T("rgb"));

		// Attempt to find a colormap file with the corresponding name
		std::string strColormapPath = wxfileColormapPath.GetFullPath();
		std::ifstream infile(strColormapPath.ToStdString());

		// Read the current rgb values
		if (infile.good()){
			int r,g,b;
			for (int i = 0; i < 256; i++){
				if (!(infile >> r >> g >> b)){
					_EXCEPTION1("Error parsing colormap file \"%s\"", strColormapPath.ToStdString().c_str());
				}
				colormap[i][0] = r;
				colormap[i][1] = g;
				colormap[i][2] = b;
			}

		// Colormap doesn't exist
		} else {
			_EXCEPTION1("Invalid colormap \"%s\"", strColorMap.c_str());
		}
	}*/
}

////////////////////////////////////////////////////////////////////////////////

