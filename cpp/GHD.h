/*
=================================================================
File: GHD.h
Author: Vladimir Kulyukin
Desciption: Interface for the Generalized Hamming Distance Class.

Copyright (c) 2000-2002
All rights reserved.

The software is provided "AS IS" without any express or implied
warranties. Any use of this software for education and/or research
is hereby granted, provided that the above copyright information 
is explicitly stated in all versions of the source code. Any 
use of this software for commercial purposes requires written 
permission of the copyright holder.
=================================================================
*/

/*
#if !defined(AFX_GHD_H__73E7A630_D705_4B32_ADC5_9F585B11D0B6__INCLUDED_)
#define AFX_GHD_H__73E7A630_D705_4B32_ADC5_9F585B11D0B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/

#define MAX_GHD_COEFF 1000000.0

class CGHD  
{
public:
	CGHD();
	virtual ~CGHD();
	void InitTableau(short nrows, short ncols, double ci, double cd);
	short* FillBitPosVec(unsigned char *bitv, short n, short &vlen);
	// This is the main member function. See comments in GHD.cpp.
	double ComputeGHD(unsigned char *source, unsigned char *target, 
					  short n, double ci, double cd, double a);
	// For debugging only.
	void DisplayTableau();

private:
	// This restrics the size of the bitmaps to 1,000 bits.
	// the size of short is 2 bytes. so we have the range [0, 65,536],
	// which is adequate for now.
	double m_Tableau[1000000];
	short m_TableauSize;
	short m_RowBitPosVec[1000];
	short m_ColBitPosVec[1000];
	short m_RowBitPosVecSize;
	short m_ColBitPosVecSize;


	void ComputeGHDAux(short *rowv, short *colv, short nrows, short ncols,
					   double ci, double cd, double a);
};

/*
#endif // !defined(AFX_GHD_H__73E7A630_D705_4B32_ADC5_9F585B11D0B6__INCLUDED_)
*/
