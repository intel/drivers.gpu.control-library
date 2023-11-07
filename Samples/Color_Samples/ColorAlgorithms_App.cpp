//===========================================================================
// Copyright (C) 2022 Intel Corporation
//
//
//
// SPDX-License-Identifier: MIT
//--------------------------------------------------------------------------

/**
 *
 * @file  ColorAlgorithms_App.cpp
 * @brief This file contains the algorithms for Color Sample APP.
 *
 */

#include "ColorAlgorithms_App.h"

/***************************************************************
 * @brief
 * MatrixMult3x3With3x1
 * @param Matrix1, Matrix2, Result
 * @return void
 ***************************************************************/
void MatrixMult3x3With3x1(const double Matrix1[3][3], const double Matrix2[3], double Result[3])
{
    double Tmp[3];

    Tmp[0] = Matrix1[0][0] * Matrix2[0] + Matrix1[0][1] * Matrix2[1] + Matrix1[0][2] * Matrix2[2];
    Tmp[1] = Matrix1[1][0] * Matrix2[0] + Matrix1[1][1] * Matrix2[1] + Matrix1[1][2] * Matrix2[2];
    Tmp[2] = Matrix1[2][0] * Matrix2[0] + Matrix1[2][1] * Matrix2[1] + Matrix1[2][2] * Matrix2[2];

    Result[0] = Tmp[0];
    Result[1] = Tmp[1];
    Result[2] = Tmp[2];
}

/***************************************************************
 * @brief
 * MatrixMult3x3
 * @param Matrix1[3][3], Matrix2[3][3], Result[3][3]
 * @return void
 ***************************************************************/
void MatrixMult3x3(const double Matrix1[3][3], const double Matrix2[3][3], double Result[3][3])
{
    double Tmp[3][3];

    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            Tmp[y][x] = Matrix1[y][0] * Matrix2[0][x] + Matrix1[y][1] * Matrix2[1][x] + Matrix1[y][2] * Matrix2[2][x];
        }
    }

    memcpy_s(Result, sizeof(Tmp), Tmp, sizeof(Tmp));
}

/***************************************************************
 * @brief
 * MatrixDeterminant3x3
 * @param Matrix[3][3]
 * @return void
 ***************************************************************/
double MatrixDeterminant3x3(const double Matrix[3][3])
{
    double Result;

    Result = Matrix[0][0] * (Matrix[1][1] * Matrix[2][2] - Matrix[1][2] * Matrix[2][1]);
    Result -= Matrix[0][1] * (Matrix[1][0] * Matrix[2][2] - Matrix[1][2] * Matrix[2][0]);
    Result += Matrix[0][2] * (Matrix[1][0] * Matrix[2][1] - Matrix[1][1] * Matrix[2][0]);

    return Result;
}

/***************************************************************
 * @brief
 * MatrixInverse3x3
 * @param Matrix[3][3], Result[3][3]
 * @return void
 ***************************************************************/
void MatrixInverse3x3(const double Matrix[3][3], double Result[3][3])
{
    double Tmp[3][3];
    double Determinant = MatrixDeterminant3x3(Matrix);

    if (0 != Determinant)
    {
        Tmp[0][0] = (Matrix[1][1] * Matrix[2][2] - Matrix[1][2] * Matrix[2][1]) / Determinant;
        Tmp[0][1] = (Matrix[0][2] * Matrix[2][1] - Matrix[2][2] * Matrix[0][1]) / Determinant;
        Tmp[0][2] = (Matrix[0][1] * Matrix[1][2] - Matrix[0][2] * Matrix[1][1]) / Determinant;
        Tmp[1][0] = (Matrix[1][2] * Matrix[2][0] - Matrix[1][0] * Matrix[2][2]) / Determinant;
        Tmp[1][1] = (Matrix[0][0] * Matrix[2][2] - Matrix[0][2] * Matrix[2][0]) / Determinant;
        Tmp[1][2] = (Matrix[0][2] * Matrix[1][0] - Matrix[0][0] * Matrix[1][2]) / Determinant;
        Tmp[2][0] = (Matrix[1][0] * Matrix[2][1] - Matrix[1][1] * Matrix[2][0]) / Determinant;
        Tmp[2][1] = (Matrix[0][1] * Matrix[2][0] - Matrix[0][0] * Matrix[2][1]) / Determinant;
        Tmp[2][2] = (Matrix[0][0] * Matrix[1][1] - Matrix[0][1] * Matrix[1][0]) / Determinant;

        Result[0][0] = Tmp[0][0];
        Result[0][1] = Tmp[0][1];
        Result[0][2] = Tmp[0][2];
        Result[1][0] = Tmp[1][0];
        Result[1][1] = Tmp[1][1];
        Result[1][2] = Tmp[1][2];
        Result[2][0] = Tmp[2][0];
        Result[2][1] = Tmp[2][1];
        Result[2][2] = Tmp[2][2];
    }
}

/***************************************************************
 * @brief
 * MatrixMultScalar3x3
 * @param  Matrix[3][3], double
 * @return void
 ***************************************************************/
void MatrixMultScalar3x3(double Matrix[3][3], double Multiplier)
{
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            Matrix[y][x] *= Multiplier;
        }
    }
}

/***************************************************************
 * @brief
 * MatrixMaxSumOfRow3x3
 * @param Matrix[3][3]
 * @return void
 ***************************************************************/
double MatrixMaxSumOfRow3x3(const double Matrix[3][3])
{
    double Val, MaxVal = -10.0;

    for (int y = 0; y < 3; y++)
    {
        Val    = Matrix[y][0] + Matrix[y][1] + Matrix[y][2];
        MaxVal = max(Val, MaxVal);
    }

    return MaxVal;
}

/***************************************************************
 * @brief
 * MatrixNormalize3x3
 * @param Mat[3][3]
 * @return void
 ***************************************************************/
void MatrixNormalize3x3(double Mat[3][3])
{
    double Val    = 0;
    double Maxrow = MatrixMaxSumOfRow3x3(Mat);

    if (Maxrow > 1)
    {
        MatrixMultScalar3x3(Mat, (1.0 / Maxrow));
    }
}

/***************************************************************
 * @brief
 * CreateRGB2XYZMatrix
 * @param ColorSpace, RGB2XYZ[3][3]
 * return void
 ***************************************************************/
void CreateRGB2XYZMatrix(ColorSpace Cspace, double RGB2XYZ[3][3])
{
    double XYZsum[3];
    double Z[4];
    double XYZw[3];

    Z[0] = 1 - Cspace.White.CIEx - Cspace.White.CIEy;
    Z[1] = 1 - Cspace.Red.CIEx - Cspace.Red.CIEy;
    Z[2] = 1 - Cspace.Green.CIEx - Cspace.Green.CIEy;
    Z[3] = 1 - Cspace.Blue.CIEx - Cspace.Blue.CIEy;

    XYZw[0] = Cspace.White.CIEx / Cspace.White.CIEy;
    XYZw[1] = 1;
    XYZw[2] = Z[0] / Cspace.White.CIEy;

    double XYZRGB[3][3] = { { Cspace.Red.CIEx, Cspace.Green.CIEx, Cspace.Blue.CIEx }, { Cspace.Red.CIEy, Cspace.Green.CIEy, Cspace.Blue.CIEy }, { Z[1], Z[2], Z[3] } };
    double Mat1[3][3];

    MatrixInverse3x3(XYZRGB, Mat1);
    MatrixMult3x3With3x1(Mat1, XYZw, XYZsum);

    double Mat2[3][3] = { { XYZsum[0], 0, 0 }, { 0, XYZsum[1], 0 }, { 0, 0, XYZsum[2] } };

    MatrixMult3x3(XYZRGB, Mat2, RGB2XYZ);
}

/***************************************************************
 * @brief
 * CreateMatrixToScaleAndRotatePanelToContentColorSpace
 * This funtion is called with PanelColorSpace which maps it with the Content color space
 * @param ColorSpace,ContentColorSpace, GeneratedOutputMatrix[3][3]
 * @return void
 ***************************************************************/
void CreateMatrixToScaleAndRotatePanelToContentColorSpace(ColorSpace PanelColorSpace, ColorSpace ContentColorSpace, double GeneratedOutputMatrix[3][3])
{
    double RGB2XYZ_Content[3][3];
    double RGB2XYZ_Panel[3][3];
    double XYZ2RGB_Panel[3][3];

    CreateRGB2XYZMatrix(ContentColorSpace, RGB2XYZ_Content); // convert ContentColorSpace to XYZ
    CreateRGB2XYZMatrix(PanelColorSpace, RGB2XYZ_Panel);     // convert Panel to XYZ
    MatrixInverse3x3(RGB2XYZ_Panel, XYZ2RGB_Panel);
    MatrixMult3x3(XYZ2RGB_Panel, RGB2XYZ_Content, GeneratedOutputMatrix);
    MatrixNormalize3x3(GeneratedOutputMatrix);
}