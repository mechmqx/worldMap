#include "matrixCalc.h"
#include<stdio.h>



double getMatrixCofator(double arcs[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE],int n)   //按第一行展开计算|A|
{
	double ans = 0;
	double temp[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = {0};
	int i,j,k;
	double t;
	if(n==1)
	{
		return arcs[0][0];
	}

	for(i=0;i<n;i++)
	{
		for(j=0;j<n-1;j++)
		{
			for(k=0;k<n-1;k++)
			{
				temp[j][k] = arcs[j+1][(k>=i)?k+1:k];

			}
		}
		t = getMatrixCofator(temp,n-1);
		if(i%2==0)
		{
			ans += arcs[0][i]*t;
		}
		else
		{
			ans -=  arcs[0][i]*t;
		}
	}
	return ans;
}

//计算每一行每一列的每个元素所对应的余子式，组成A*
void calcInverMatrix(double arcs[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE],int n,double invetM[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE])
{
	int i,j,k,t;
	double temp[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]={0};
	if(n==1)
	{
		invetM[0][0] = 1;
		return;
	}

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			for(k=0;k<n-1;k++)
			{
				for(t=0;t<n-1;t++)
				{
					temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t];
				}
			}


			invetM[j][i]  =  getMatrixCofator(temp,n-1);
			if((i+j)%2 == 1)
			{
				invetM[j][i] = - invetM[j][i];
			}
		}
	}
}

void calcInverMatrix4x4(const double mat_in[16], double mat_out[16])
{
	double arcs[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = {0.0};
	double invetM[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = {0.0};
	int i=0,j=0;
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			arcs[i][j] = mat_in[4*i+j];
		}
	}
	calcInverMatrix(arcs,4, invetM);

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			mat_out[4*i+j] = invetM[i][j];
		}
	}
}

void multiMatrix4x4(const double mat_left[16],const double mat_right[16], double mat_out[16])
{
	int i=0,j=0,k=0;

	double result[16] = {0} ;
	
	for (i=0;i<4;i++)    // go through line
	{
		for (j=0;j<4;j++)    //go through column
		{
			for (k=0;k<4;k++)    // each value at line i and column j need 4 times multiply for sum up
			{
				result[i*4+j] += mat_left[i+k*4]*mat_right[j*4+k];
			}			
		}
	}

	for (i=0;i<16;i++)
	{
		mat_out[i] = result[i];
	}
}

void testMatrix4x4Multi()
{
	int i=0;
	double result[16] = {0};

	double mat_left[16] = {
		1.0,2.0,3.0,4.0,
		1.0,2.0,3.0,4.0,
		1.0,2.0,3.0,4.0,
		1.0,2.0,3.0,4.0,
	};
	double mat_right[16] = {
		1,1,1,1,
		2,2,2,2,
		3,3,3,3,
		4,4,4,4
	};
	multiMatrix4x4(mat_left,mat_right,result);
	printf("[mqx test] matrix multiply test result display:\n");
	for (i=0;i<16;i++)
	{
		printf("\t%5.1lf",result[i]);
		if (i%4==3)
		{
			printf("\n");
		}
	}
}


void vecMultiMatrix4x4(const double mat[16],const double vec_in[4], double vec_out[4])
{
	int j=0,k=0;

	double result[4] = {0} ;

	for (j=0;j<4;j++)    //go through column
	{
		for (k=0;k<4;k++)    // each value at line i and column j need 4 times multiply for sum up
		{
			result[j] += mat[j+k*4]*vec_in[k];
		}			
	}

	for (j=0;j<4;j++)
	{
		vec_out[j] = result[j];
	}
}


void testVecMultiMatrix4x4()
{
	int i=0;
	double result[4] = {0};

	double mat[16] = {
		1.0,2.0,3.0,4.0,
		1.0,2.0,3.0,4.0,
		1.0,2.0,3.0,4.0,
		1.0,2.0,3.0,4.0,
	};
	double vec[4] = {
		2,2,2,2,
	};
	vecMultiMatrix4x4(mat,vec,result);
	printf("[mqx test] matrix multiply test result display:\n");
	for (i=0;i<4;i++)
	{
		printf("\t%5.1lf",result[i]);
		//if (i%4==3)
		{
			printf("\n");
		}
	}
}

void calcMatrix4x4Transposition(const double mat_in[16],double mat_out[16])
{
	int i=0,j=0;
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			mat_out[i*4+j] = mat_in[i+j*4];
		}
	}
}