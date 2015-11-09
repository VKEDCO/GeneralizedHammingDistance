/*
======================================================
GHD.cpp: implementation of the CGHD class.
File: GHD.cpp
Description: Generalized Hamming Distance Methods
Author: Vladimir Kulyukin

Copyright (c) 2000-2002
All rights reserved.

The software is provided "AS IS" without any express or implied
warranties. Any use of this software for education and/or research
is hereby granted, provided that the above copyright information 
is explicitly stated in all versions of the source code. Any 
use of this software for commercial purposes requires written 
permission of the copyright holder.
======================================================
*/

#include "GHD.h"
#include <math.h>
#include <iostream.h>

CGHD::CGHD()
{
	m_TableauSize     = 0;
	m_RowBitPosVec[0] = 0;
	m_ColBitPosVec[0] = 0;
	m_RowBitPosVecSize = 0;
	m_ColBitPosVecSize = 0;
}

CGHD::~CGHD()
{
	// Nothing to deallocate so empty.
}

// rows = height
// cols = width
// ci is the cost of insertion; cd is the cost of deletion.
void 
CGHD::InitTableau(short nrows, short ncols, double ci, double cd)
{
	m_TableauSize = nrows * ncols;
	m_RowBitPosVecSize = nrows;
	m_ColBitPosVecSize = ncols;

	for(int c = 0; c < ncols; c++) {
		m_Tableau[c] = c * ci;
	}
	for(int r = 0; r < nrows; r++) {
		m_Tableau[r * ncols] = r * cd;
	}
}

void 
CGHD::DisplayTableau()
{
	for(int i = 0; i < m_RowBitPosVecSize; i++) {
		for(int j = 0; j < m_ColBitPosVecSize; j++)
			cout << m_Tableau[i * m_ColBitPosVecSize + j] << " ";
		cout << endl;
	}
}

short*
CGHD::FillBitPosVec(unsigned char *bitv, short n, short &vlen)
{
	short count = 0;
	short i, j;

	for(i = 0; i < n; i++) {
		if ( bitv[i] == 1 )
			++count;
	}

	vlen = count;

	if ( count == 0 ) {
		return new short[0];
	}

	vlen += 1;

	short *bitPosVec = new short[count + 1];
	bitPosVec[0] = 0;
	
	j = 1;
	for(i = 0; i < n; i++) {
		if ( bitv[i] == 1 ) {
			bitPosVec[j++] = i+1;
		}
	}

	return bitPosVec;
}

/*
This is the main member function of the CGHD class. sv is the
source bit vector (bit map) represented as an array of unsighed 
characters. tv is the target bit vector represented as an array 
of unsigned characters. 

  The function returns the cost of transforming the source bit vector 
into the target bit vector. n is the length of the bit vectors. 

  The vectors must be of the same dimension. ci and cd are
the cost of inserting a bit and deleting a bit respectively. 
a is the constant used in computed the cost of the shift. 
The cost of the shift is computed as a * |i - j|, where a is 
the constant and i and j are the positions indicating the shift. 
In other words, a bit is shifted from i to j. 

  Obviously, the higher the constant the more expensive it is to 
shift. If a is very hight, generalized hamming distance becomes 
equivalent to the classic hamming distance which has only insertion 
and deletion.
*/
double
CGHD::ComputeGHD(unsigned char *sv, unsigned char* tv, short n, 
				 double ci, double cd, double a)
{
	short nrows = 0, ncols = 0;

	// build the bite-site for the target.
	short *rvec = FillBitPosVec(sv, n, nrows);
	// build the bite-site for the source.
	short *cvec = FillBitPosVec(tv, n, ncols);

	//cout << "nrows == " << nrows << endl;
	//cout << "ncols == " << ncols << endl;

	if ( nrows == 0 && ncols == 0 ) {
		delete [] rvec;
		delete [] cvec;
		return 0.0;
	}
	// if there are no bits in the target,
	// return the cost of the insertions.
	else if ( nrows > 0 && ncols == 0 ) {
		delete [] rvec;
		delete [] cvec;
		return (nrows - 1) * ci;
	}
	else if ( nrows == 0 && ncols > 0 ) {
		delete [] rvec;
		delete [] cvec;
		return (ncols - 1) * cd;
	}
	else {
		// both nrows > 0 and ncols > 0
	
		InitTableau(nrows, ncols, ci, cd);
		//DisplayTableau();

		ComputeGHDAux(rvec, cvec, nrows, ncols, ci, cd, a);
	
		delete [] rvec;
		delete [] cvec;

		return m_Tableau[((nrows - 1) * ncols) + (ncols - 1)];
	}
}

void
CGHD::ComputeGHDAux(short *rowv, short *colv, short nrows, short ncols,
					double ci, double cd, double a)
{
	// Nothing to compute because InitTableau takes
	// care of this case.
	//cout << "nrows == " << nrows << endl;
	//cout << "ncols == " << ncols << endl;

	if ( nrows == 1 || ncols == 1 ) 
		return;

	short width = ncols;

	short ri, cj;
	double shiftCost, delCost, insCost, minCost;

	for(int i = 1; i < nrows; i++) {
		ri = rowv[i];
		for(int j = 1; j < ncols; j++) {
			cj = colv[j];
			if ( ri == cj ) {
				m_Tableau[i*width + j] = 
					m_Tableau[(i-1)*width + (j-1)];
			}
			
			// Compute the cost of the shift.
			shiftCost = a * ::fabs((double) (ri - cj));
			shiftCost += m_Tableau[(i-1)*width + (j-1)];

			if ( ri > cj ) {

				delCost = cd + m_Tableau[(i-1)*width + j];

				// compute the minimum cost;
				if ( delCost < shiftCost)
					minCost = delCost;
				else
					minCost = shiftCost;

				m_Tableau[i*width + j] = minCost;
			}
			else if ( ri < cj ) {

				insCost = ci + m_Tableau[i*width + (j-1)];

				// compute the minimum cost;
				if ( insCost < shiftCost )
					minCost = insCost;
				else
					minCost = shiftCost;

				m_Tableau[i*width + j] = minCost;
			}
		}
	}
}





