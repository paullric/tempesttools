///////////////////////////////////////////////////////////////////////////////
///
///	\file    MultiTypeArray.h
///	\author  Paul Ullrich
///	\version December 30, 2024
///

#ifndef _MULTITYPEARRAY_H_
#define _MULTITYPEARRAY_H_

#include "Exception.h"
#include "Announce.h"

#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

///	<summary>
///		An object that stores arrays without exposing the type.
///	</summary>
class MultiTypeArray {

public:
	///	<summary>
	///		Type of data currently stored in the MultiTypeArray.
	///	</summary>
	enum MTType {
		mtUnknown,
		mtChar,
		mtUChar,
		mtShort,
		mtUShort,
		mtInt,
		mtUInt,
		mtInt64,
		mtUInt64,
		mtFloat,
		mtDouble
	};

public:
	///	<summary>
	///		Constructor.
	///	</summary>
	MultiTypeArray(
		MTType type,
		size_t count = 0
	) :
		m_type(type),
		m_count(count),
		m_chardata(NULL),
		m_uchardata(NULL),
		m_shortdata(NULL),
		m_ushortdata(NULL),
		m_intdata(NULL),
		m_uintdata(NULL),
		m_int64data(NULL),
		m_uint64data(NULL),
		m_floatdata(NULL),
		m_doubledata(NULL)
	{
		if (type == mtChar) {
			m_chardata = new char[count];
		}
		if (type == mtUChar) {
			m_uchardata = new unsigned char[count];
		}
		if (type == mtShort) {
			m_shortdata = new short[count];
		}
		if (type == mtUShort) {
			m_ushortdata = new unsigned short[count];
		}
		if (type == mtInt) {
			m_intdata = new int[count];
		}
		if (type == mtUInt) {
			m_uintdata = new unsigned int[count];
		}
		if (type == mtInt64) {
			m_int64data = new int64_t[count];
		}
		if (type == mtUInt64) {
			m_uint64data = new uint64_t[count];
		}
		if (type == mtFloat) {
			m_floatdata = new float[count];
		}
		if (type == mtDouble) {
			m_doubledata = new double[count];
		}
	}

	///	<summary>
	///		Duplicate.
	///	</summary>
	void duplicate(
		const MultiTypeArray & mta
	) {
	}

	///	<summary>
	///		Destructor.
	///	</summary>
	~MultiTypeArray() {
		if (m_chardata != NULL) {
			delete[] m_chardata;
		}
		if (m_uchardata != NULL) {
			delete[] m_uchardata;
		}
		if (m_shortdata != NULL) {
			delete[] m_shortdata;
		}
		if (m_ushortdata != NULL) {
			delete[] m_ushortdata;
		}
		if (m_intdata != NULL) {
			delete[] m_intdata;
		}
		if (m_uintdata != NULL) {
			delete[] m_uintdata;
		}
		if (m_int64data != NULL) {
			delete[] m_int64data;
		}
		if (m_uint64data != NULL) {
			delete[] m_uint64data;
		}
		if (m_floatdata != NULL) {
			delete[] m_floatdata;
		}
		if (m_doubledata != NULL) {
			delete[] m_doubledata;
		}
	}

protected:
	///	<summary>
	///		Type of data stored.
	///	</summary>
	MTType m_type;

	///	<summary>
	///		Number of values in the array.
	///	</summary>
	size_t m_count;

	///	<summary>
	///		Data stored as a char.
	///	</summary>
	char * m_chardata;

	///	<summary>
	///		Data stored as an unsigned char.
	///	</summary>
	unsigned char * m_uchardata;

	///	<summary>
	///		Data stored as a short.
	///	</summary>
	short * m_shortdata;

	///	<summary>
	///		Data stored as an unsigned short.
	///	</summary>
	unsigned short * m_ushortdata;

	///	<summary>
	///		Data stored as an int.
	///	</summary>
	int * m_intdata;

	///	<summary>
	///		Data stored as an unsigned int.
	///	</summary>
	unsigned int * m_uintdata;

	///	<summary>
	///		Data stored as an int64.
	///	</summary>
	int64_t * m_int64data;

	///	<summary>
	///		Data stored as an unsigned int64.
	///	</summary>
	uint64_t * m_uint64data;

	///	<summary>
	///		Data stored as a float.
	///	</summary>
	float * m_floatdata;

	///	<summary>
	///		Data stored as a double.
	///	</summary>
	double * m_doubledata;

};

///////////////////////////////////////////////////////////////////////////////

#endif

