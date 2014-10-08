// stringArray.h: interface for the CstringArray class.
//
//////////////////////////////////////////////////////////////////////
/*
 * $Id$
 *
 * $Log$
 */

#ifndef __STRINGARRAY_H_
#define __STRINGARRAY_H_

#include <string>
#include <vector>

using namespace std;

// A CStringArray-like version of the STL vector class

class CstringArray : vector<string>
{
public:
	CstringArray() {
		m_strArray = new vector<string>;
	}

	~CstringArray() {
		delete m_strArray;
	}

	int Add ( const string& str ) {
		m_strArray->push_back( str );
		return m_strArray->size() - 1;
	}

	const char *GetAt ( int idx ) const {
		return (m_strArray->at( idx ).c_str());
	}

	const int GetSize ( ) const {
		return m_strArray->size();
	}

	void RemoveAll ( ) {
		m_strArray->clear();
	}

private:
	vector<string> *m_strArray;
};

#endif /* __STRINGARRAY_H_ */