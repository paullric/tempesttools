///////////////////////////////////////////////////////////////////////////////
///
///	\file    StructuredDataset.h
///	\author  Paul Ullrich
///	\version December 11, 2024
///

#ifndef _STRUCTUREDDATASET_H_
#define _STRUCTUREDDATASET_H_

#include "Exception.h"
#include "MultiTypeArray.h"

#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

///	<summary>
///		A class that stores a range.
///	</summary>
template <typename IXTYPE>
class IndexRange :
	public std::pair<IXTYPE,IXTYPE> {
};

///	<summary>
///		A pair whose first element is a variable name and whose second element
///		is an IndexRange.
///	</summary>
template <typename IXTYPE>
class VariableNameAndIndexRange :
	public std::pair< std::string, IndexRange<IXTYPE> > {
};

///	<summary>
///		A class that stores multiple dimension names and their ranges.
///	</summary>
template <typename IXTYPE>
class MultiIndexRange : public std::vector< VariableNameAndIndexRange<IXTYPE> > {
};

///	<summary>
///		A class that stores values for a given dimension variable.
///	</summary>
template <typename T>
class DimensionVarValues : public std::pair< std::string, std::vector<T> > {
};

///////////////////////////////////////////////////////////////////////////////

class StructuredDataset {

public:
	///	<summary>
	///		Attribute.
	///	</summary>
	class Attribute {

	public:
		///	<summary>
		///		Get the name of the attribute.
		///	</summary>
		const std::string & name() const {
			return m_name;
		}

		///	<summary>
		///		Get the type of the attribute.
		///	</summary>
		int type() const {
			return m_type;
		}

		///	<summary>
		///		Number of values.
		///	</summary>
		size_t length() const {
			return m_values.size();
		}

		///	<summary>
		///		Get a specific value as a string.
		///	</summary>
		const std::string & as_string(size_t ix) const {
			if (ix >= m_values.size()) {
				_EXCEPTION2("Attribute::value(ix) out of range (%lu >= %lu)", ix, m_values.size());
			}
			return m_values[ix];
		}

		///	<summary>
		///		Equality operator.
		///	</summary>
		bool operator==(const Attribute & attr) const {
			if ((m_name == attr.m_name) &&
			    (m_type == attr.m_type) &&
				(m_values == attr.m_values)
			) {
				return true;
			}
			return false;
		}

	public:
		///	<summary>
		///		Name of the attribute.
		///	</summary>
		std::string m_name;
	
		///	<summary>
		///		Type of the attribute.
		///	</summary>
		int m_type;
	
		///	<summary>
		///		Values of the attribute.
		///	</summary>
		std::vector< std::string > m_values;
	};

public:
	///	<summary>
	///		Coordinate variables are 1D variables which assign monotonic values
	///		to each dimension.
	///	</summary>
	class Coordinate {

	public:
		///	<summary>
		///		Constructor.
		///	</summary>
		Coordinate(
			const std::string name,
			MultiTypeArray::MTType mttype,
			size_t count
		) :
			m_name(name),
			m_values(mttype, count)
		{ }

	public:
		///	<summary>
		///		Name of the coordinate.
		///	</summary>
		std::string m_name;

		///	<summary>
		///		Number of values in the coordinate.
		///	</summary>
		MultiTypeArray m_values;
	};

public:
	///	<summary>
	///		Data variables.
	///	</summary>
	class Variable {
	public:
		///	<summary>
		///		Name of the variable.
		///	</summary>
		std::string m_name;

		///	<summary>
		///		Variable attributes.
		///	</summary>
		std::vector<Attribute> m_attrs;
	};

public:
	///	<summary>
	///		Files within the StructuredDataset.
	///	</summary>
	class DataFile {
	
	public:
		///	<summary>
		///		Filename.
		///	</summary>
		std::string m_name;

		///	<summary>
		///		Vector of Attributes.
		///	</summary>
		std::vector<Attribute> m_attrs;

		///	<summary>
		///		Vector of Coordinates.
		///	</summary>
		std::vector<Coordinate> m_coords;
	};

public:
	///	<summary>
	///		Constructor.
	///	</summary>
	StructuredDataset()
	{ }

public:
	///	<summary>
	///		Add the given file to the StructuredDataset.
	///	</summary>
	void AppendFile(
		const std::string & filename
	);

	///	<summary>
	///		Build the StructuredDataset for the given set of files.
	///	</summary>
	void IndexFiles(
		const std::vector<std::string> & filenames
	);

	///	<summary>
	///		Load the given hyperslab of data.
	///	</summary>
	void LoadData(
		const MultiIndexRange<int> & multirange
	);

private:
	///	<summary>
	///		If true then load all attributes.
	///	</summary>
	bool m_fLoadAttributes;

public:
	///	<summary>
	///		A vector of DataFiles in this dataset.
	///	</summary>
	std::vector<DataFile> m_datafiles;

	///	<summary>
	///		A vector of ranges for each coordinate stored in a file.
	///	</summary>
	std::vector< MultiIndexRange<int> > m_filecoordranges;

	///	<summary>
	///		A vector of coordinates in the dataset.
	///	</summary>
	std::vector<Coordinate> m_coordinates;

	///	<summary>
	///		A vector of variables in this dataset.
	///	</summary>
	std::vector<Variable> m_variables;

	///	<summary>
	///		Global attributes that are common across the dataset.
	///	</summary>
	std::vector<Attribute> m_common_attrs;

};

///////////////////////////////////////////////////////////////////////////////

#endif
