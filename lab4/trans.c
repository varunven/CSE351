/*
CSE 351 Lab 4, Part 2 - Matrix Transposes

Varun Venkatesh
varunven
*/

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part I of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M])
{
    //S=5, E=1, k=5
    //k=log(K), K=32
    //s=log(C/K), C=1024
    int blockSize;
    if (N == 32)
    {
        blockSize = 8;
    }
    if (N == 64)
    {
        blockSize = 4;
    }
    int diagBlock = 0;
    for (int i = 0; i < M; i += blockSize)
    {
        for (int j = 0; j < N; j += blockSize)
        {
            for (int iblock = i; iblock < i + blockSize; iblock++)
            {
                for (int jblock = j; jblock < j + blockSize; jblock++)
                {
                    if (iblock != jblock)
                    {
                        B[jblock][iblock] = A[iblock][jblock];
                    }
                    else
                    {
                        diagBlock = A[iblock][jblock];
                    }
                }
                if (i == j)
                {
                    B[iblock][iblock] = diagBlock;
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M])
{
    int i, j, tmp;

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M])
{
    int i, j;

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
