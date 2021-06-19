#ifndef __MATRIX_CALCULATION__
#define __MATRIX_CALCULATION__

#define MAX_MATRIX_SIZE 10

double getMatrixCofator(double arcs[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE],int n);
void calcInverMatrix(double arcs[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE],int n,double invetM[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]);
void calcInverMatrix4x4(const double mat_in[16], double mat_out[16]);
void multiMatrix4x4(const double mat_left[16],const double mat_right[16], double mat_out[16]);
void testMatrix4x4Multi();
void vecMultiMatrix4x4(const double mat[16],const double vec_in[4], double vec_out[4]);
void calcMatrix4x4Transposition(const double mat_in[16],double mat_out[16]);
void testVecMultiMatrix4x4();
#endif