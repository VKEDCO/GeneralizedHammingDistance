/*
=============================================
File: main.cpp
Author: Vladimir Kulyukin
Description: Examples of compute ghd of various
bit vectors.

Copyright (c) 2000-2002
All rights reserved.

The software is provided "AS IS" without any express or implied
warranties. Any use of this software for education and/or research
is hereby granted, provided that the above copyright information 
is explicitly stated in all versions of the source code. Any 
use of this software for commercial purposes requires written 
permission of the copyright holder.
=============================================
*/

#include <iostream.h>
#include "GHD.h"

// Define a global GHD variable.
CGHD gGHD;

// Test bit vectors for tests.
unsigned char gTbitv[10]   = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0};
unsigned char gSbitv_1[10] = {1, 1, 0, 0, 0, 1, 0, 0, 0, 0};
unsigned char gSbitv_2[10] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 1};

unsigned char gTbitv_2[3]  = {0, 1, 1};
unsigned char gSbitv_3[3]  = {1, 1, 0};

unsigned char gTbitv_3[1]  = {1};
unsigned char gSbitv_4[1]  = {0};

unsigned char gTbitv_4[3]  = {1, 1, 1};
unsigned char gSbitv_5[3]  = {0, 0, 0};

unsigned char gTbitv_5[3]  = {0, 0, 0};
unsigned char gSbitv_6[3]  = {1, 1, 1};

// Test functions that run ComputeGHD on the various above bit vectors.
double GHDTest1()
{
	return gGHD.ComputeGHD(gTbitv, gSbitv_1, 10, 1.0, 1.0, 0.5);
}

double GHDTest2()
{
	return gGHD.ComputeGHD(gTbitv, gSbitv_2, 10, 1.0, 1.0, 0.5);
}

double GHDTest3()
{
	return gGHD.ComputeGHD(gTbitv_2, gSbitv_3, 3, 1.0, 1.0, 0.5);
}

double GHDTest4()
{
	return gGHD.ComputeGHD(gTbitv_3, gSbitv_4, 1, 1.0, 1.0, 0.5);
}

double GHDTest5()
{
	return gGHD.ComputeGHD(gTbitv_4, gSbitv_5, 3, 1.0, 1.0, 0.5);
}

// Note that the cost of deletion is 2.0, hence the value of
// 6.0.
double GHDTest6()
{
	return gGHD.ComputeGHD(gTbitv_5, gSbitv_6, 3, 1.0, 2.0, 0.5);
}


// Run tests and print out results.
void main()
{
	cout << "Test1: GHD = " << GHDTest1() << endl;
	cout << "Test2: GHD = " << GHDTest2() << endl;
	cout << "Test3: GHD = " << GHDTest3() << endl;
	cout << "Test4: GHD = " << GHDTest4() << endl;
	cout << "Test5: GHD = " << GHDTest5() << endl;
	cout << "Test6: GHD = " << GHDTest6() << endl;
}
