///////////////////////////////////////////////////////////////////////////////
///
///	\file    StructuredDataset.cpp
///	\author  Paul Ullrich
///	\version December 30, 2024
///

#include "StructuredDataset.h"
#include "Exception.h"

#include <string>
#include <netcdf.h>

///////////////////////////////////////////////////////////////////////////////

MultiTypeArray::MTType _MTTypeFromNcType(
	nc_type nctypeid
) {
	if (nctypeid == NC_BYTE) {
		return MultiTypeArray::mtUChar;
	}
	if (nctypeid == NC_CHAR) {
		return MultiTypeArray::mtChar;
	}
	if (nctypeid == NC_SHORT) {
		return MultiTypeArray::mtShort;
	}
	if (nctypeid == NC_INT) {
		return MultiTypeArray::mtInt;
	}
	if (nctypeid == NC_LONG) {
		return MultiTypeArray::mtInt64;
	}
	if (nctypeid == NC_FLOAT) {
		return MultiTypeArray::mtFloat;
	}
	if (nctypeid == NC_DOUBLE) {
		return MultiTypeArray::mtDouble;
	}
	if (nctypeid == NC_UBYTE) {
		return MultiTypeArray::mtUChar;
	}
	if (nctypeid == NC_USHORT) {
		return MultiTypeArray::mtUShort;
	}
	if (nctypeid == NC_UINT) {
		return MultiTypeArray::mtUInt;
	}
	if (nctypeid == NC_INT64) {
		return MultiTypeArray::mtInt64;
	}
	if (nctypeid == NC_UINT64) {
		return MultiTypeArray::mtUInt64;
	}
	return MultiTypeArray::mtUnknown;
}

///////////////////////////////////////////////////////////////////////////////

void _GetNcAttribute(
	const std::string & filename,
	int ncid,
	int varid,
	const std::string & attrname,
	StructuredDataset::Attribute & attr
) {
	// NetCDF status
	int status = NC_NOERR;

	// Attribute type
	nc_type atttype;

	// Attribute length
	size_t attlen;

	// Get the type and length of this attribute
	status = nc_inq_att(ncid, varid, attrname.c_str(), &atttype, &attlen);
	if (status != NC_NOERR) {
		_EXCEPTION2("NetCDF unable to inquiry attribute \"%s\" in \"%s\"", attrname.c_str(), filename.c_str());
	}

	attr.m_type = (int)(atttype);
	attr.m_values.resize(attlen);

	// Get the values of this BYTE attribute
	if (atttype == NC_BYTE) {
		unsigned char * values = new unsigned char[attlen];
		status = nc_get_att_ubyte(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as ubyte in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((int) values[attnum]));
		}
		delete[] values;

	// Get the values of this CHAR attribute
	} else if (atttype == NC_CHAR) {
		char * values = new char[attlen+1];
		status = nc_get_att_text(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as char/text in \"%s\"", attrname.c_str(), filename.c_str());
		}
		values[attlen] = '\0';
		attr.m_values.push_back(values);
		delete[] values;

	// Get the values of this SHORT attribute
	} else if (atttype == NC_SHORT) {
		short * values = new short[attlen];
		status = nc_get_att_short(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as short in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((int) values[attnum]));
		}
		delete[] values;

	// Get the values of this INT attribute
	} else if (atttype == NC_INT) {
		int * values = new int[attlen];
		status = nc_get_att_int(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as int in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((int) values[attnum]));
		}
		delete[] values;

	// Get the values of this FLOAT attribute
	} else if (atttype == NC_FLOAT) {
		float * values = new float[attlen];
		status = nc_get_att_float(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as float in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((float) values[attnum]));
		}
		delete[] values;

	// Get the values of this DOUBLE attribute
	} else if (atttype == NC_DOUBLE) {
		double * values = new double[attlen];
		status = nc_get_att_double(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as double in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((double) values[attnum]));
		}
		delete[] values;

	// Get the values of this USHORT attribute
	} else if (atttype == NC_USHORT) {
		unsigned short * values = new unsigned short[attlen];
		status = nc_get_att_ushort(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as ushort in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((unsigned int) values[attnum]));
		}
		delete[] values;

	// Get the values of this UINT attribute
	} else if (atttype == NC_UINT) {
		unsigned int * values = new unsigned int[attlen];
		status = nc_get_att_uint(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as uint in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((unsigned int) values[attnum]));
		}
		delete[] values;

	// Get the values of this INT64 attribute
	} else if (atttype == NC_INT64) {
		int64_t * values = new int64_t[attlen];
		status = nc_get_att_longlong(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as int64 in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((long long) values[attnum]));
		}
		delete[] values;

	// Get the values of this UINT64 attribute
	} else if (atttype == NC_UINT64) {
		uint64_t * values = new uint64_t[attlen];
		status = nc_get_att_ulonglong(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as uint64 in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(std::to_string((unsigned long long) values[attnum]));
		}
		delete[] values;

	// Get the values of this STRING attribute
	} else if (atttype == NC_STRING) {
		char ** values = new char *[attlen];
		status = nc_get_att_string(ncid, varid, attrname.c_str(), values);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to get attribute \"%s\" as string in \"%s\"", attrname.c_str(), filename.c_str());
		}
		for (size_t attnum = 0; attnum < attlen; attnum++) {
			attr.m_values.push_back(values[attnum]);
		}
		status = nc_free_string(attlen, values);
		if (status != NC_NOERR) {
			_EXCEPTIONT("NetCDF error calling nc_free_string()");
		}
		delete[] values;

	// Invalid attribute type
	} else {
		_EXCEPTION2("Invalid attribute type %i in \"%s\"", (int)atttype, filename.c_str());
	}

}

///////////////////////////////////////////////////////////////////////////////

void StructuredDataset::AppendFile(
	const std::string & filename
) {
	// NetCDF status
	int status = NC_NOERR;

	// NetCDF name buffer
	std::string strBuffer(NC_MAX_NAME, '*');

	// NetCDF dimension id buffer
	int vardimids[NC_MAX_VAR_DIMS];

	// Append the file to the list of filenames
	DataFile datafile;
	datafile.m_name = filename;

	// Open the file
	int ncid;
	status = nc_open(filename.c_str(), 0, &ncid);
	if (status != NC_NOERR) {
		_EXCEPTION1("NetCDF unable to open file \"%s\"", filename.c_str());
	}

	// Load in number of dimensions, variables and global attributes
	int nfiledims;
	int nfilevars;
	int nfileatts;
	status = nc_inq(ncid, &nfiledims, &nfilevars, &nfileatts, NULL);
	if (status != NC_NOERR) {
		_EXCEPTION1("NetCDF unable to inquiry file \"%s\"", filename.c_str());
	}

	// Loop through all global attributes in the file
	for (int attid = 0; attid < nfileatts; attid++) {

		// Get the name of this attribute
		status = nc_inq_attname(ncid, NC_GLOBAL, attid, &(strBuffer[0]));
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to inquiry attribute %i in \"%s\"", attid, filename.c_str());
		}

		Attribute attr;

		_GetNcAttribute(
			filename,
			ncid,
			NC_GLOBAL,
			strBuffer,
			attr);

		// Store this global attribute at either the dataset or file level
		if (m_datafiles.size() == 0) {
			m_common_attrs.push_back(attr);
		} else {
			bool exists_in_common_attrs = false;
			for (size_t attix = 0; attix < m_common_attrs.size(); attix++) {
				if (m_common_attrs[attix] == attr) {
					exists_in_common_attrs = true;
					break;
				}
				if (m_common_attrs[attix].name() == attr.name()) {
					for (DataFile & other_datafile : m_datafiles) {
						other_datafile.m_attrs.push_back(m_common_attrs[attix]);
					}
					m_common_attrs.erase(m_common_attrs.begin() + attix);
					datafile.m_attrs.push_back(attr);
					break;
				}
			}
			if (!exists_in_common_attrs) {
				datafile.m_attrs.push_back(attr);
			}
		}
	}

	// Loop through all dimensions in the file
	for (int dimid = 0; dimid < nfiledims; dimid++) {

		// Get the length of this dimension
		size_t dimlen;
		status = nc_inq_dim(ncid, dimid, &(strBuffer[0]), &dimlen);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to inquiry dimension %i in \"%s\"", dimid, filename.c_str());
		}

		// Check if this dimension has a coordinate variable
		int dimvar_id;
		status = nc_inq_varid(ncid, strBuffer.c_str(), &dimvar_id);
		if (status == NC_NOERR) {

			// Get information on the coordinate variable
			nc_type dimvar_type;
			int dimvar_ndims;
			int dimvar_natts;
			status = nc_inq_var(ncid, dimvar_id, &(strBuffer[0]), &dimvar_type, &dimvar_ndims, vardimids, &dimvar_natts);
			if (status != NC_NOERR) {
				_EXCEPTION2("NetCDF unable to inquiry variable %i in \"%s\"", dimvar_id, filename.c_str());
			}

			// Make sure coordinate variable is 1D
			if (dimvar_ndims != 1) {
				_EXCEPTION2("Coordinate variable \"%s\" must have dimension 1 in \"%s\"",
					strBuffer.c_str(), filename.c_str());
			}
			if (vardimids[0] != dimid) {
				_EXCEPTION2("Coordinate variable \"%s\" must have dimension with same name in \"%s\"",
					strBuffer.c_str(), filename.c_str());
			}

			// Create a new Coordinate for this file
			Coordinate coord(strBuffer, _MTTypeFromNcType(dimvar_type), dimlen);

		} else if (status == NC_ENOTVAR) {
		} else {
			_EXCEPTION2("NetCDF unable to inquiry varid \"%s\" in \"%s\"", strBuffer.c_str(), filename.c_str());
		}
	}

	// Loop through all variables in the file
	for (int varid = 0; varid < nfilevars; varid++) {
		nc_type vartype;
		int nvardims;
		int nvaratts;

		status = nc_inq_var(ncid, varid, &(strBuffer[0]), &vartype, &nvardims, vardimids, &nvaratts);
		if (status != NC_NOERR) {
			_EXCEPTION2("NetCDF unable to inquiry variable \"%s\" %i", filename.c_str(), varid);
		}
	}

	// Close the file
	status = nc_close(ncid);
	if (status != NC_NOERR) {
		_EXCEPTION1("NetCDF unable to close file \"%s\"", filename.c_str());
	}

	// Append this datafile
	m_datafiles.push_back(datafile);
}

///////////////////////////////////////////////////////////////////////////////

void StructuredDataset::IndexFiles(
	const std::vector<std::string> & filenames
) {
	for (const std::string & fname : filenames) {
		AppendFile(fname);
	}
}

///////////////////////////////////////////////////////////////////////////////

