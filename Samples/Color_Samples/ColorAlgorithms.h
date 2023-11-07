//===========================================================================
//
// Copyright (c) Intel Corporation (2021-2022)
//
// INTEL MAKES NO WARRANTY OF ANY KIND REGARDING THE CODE. THIS CODE IS LICENSED
// ON AN "AS IS" BASIS AND INTEL WILL NOT PROVIDE ANY SUPPORT, ASSISTANCE,
// INSTALLATION, TRAINING OR OTHER SERVICES. INTEL DOES NOT PROVIDE ANY UPDATES,
// ENHANCEMENTS OR EXTENSIONS. INTEL SPECIFICALLY DISCLAIMS ANY WARRANTY OF
// MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR ANY PARTICULAR PURPOSE, OR ANY
// OTHER WARRANTY. Intel disclaims all liability, including liability for
// infringement of any proprietary rights, relating to use of the code. No license,
// express or implied, by estoppel or otherwise, to any intellectual property
// rights is granted herein.
//
//--------------------------------------------------------------------------

/**
 *
 * @file  ColorAlgorithms.h
 * @brief
 *
 */

#pragma once

#include <windows.h>
#include <stdio.h>

// https://en.wikipedia.org/wiki/CIE_1931_color_space#CIE_xy_chromaticity_diagram_and_the_CIE_xyY_color_space
typedef struct
{
    double CIEx;
    double CIEy;
    double CIELuminance;
} IPIXEL_xyY;

typedef struct ColorSpace
{
    IPIXEL_xyY White;
    IPIXEL_xyY Red;
    IPIXEL_xyY Green;
    IPIXEL_xyY Blue;
} ColorSpace;

void MatrixMult3x3With3x1(const double Matrix1[3][3], const double Matrix2[3], double Result[3]);
void MatrixMult3x3(const double Matrix1[3][3], const double Matrix2[3][3], double Result[3][3]);
double MatrixDeterminant3x3(const double Matrix[3][3]);
void MatrixInverse3x3(const double Matrix[3][3], double Result[3][3]);
void MatrixMultScalar3x3(double Matrix[3][3], double Multiplier);
double MatrixMaxSumOfRow3x3(const double Matrix[3][3]);
void MatrixNormalize3x3(double Mat[3][3]);
void CreateRGB2XYZMatrix(ColorSpace Cspace, double RGB2XYZ[3][3]);
void CreateMatrixForMappingPanelToSRGBColorSpace(ColorSpace PanelColorSpace, double GeneratedOutputMatrix[3][3]);
