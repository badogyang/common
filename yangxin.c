/**
 * @file yangxin.c
 * @author xin.yang
 * @version 0.1
 * @date 2022-09-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1024

// 组合AB排序，返回排序结果
int* sortAAndB(int A[], char B[])
{
    static int C[MAX_LENGTH*2] = {0};
    int i = 0;
    int j = 0;
    int cNum = 0;
    int temp = 0;

    for(i = 0, j = 0; A[i] != '\0' || B[j] != '\0'; )
    {
        if(B[j] != '\0')
        {
            C[cNum] = B[j] + '0';
            cNum++;
            j++;
        }

        if(A[i] != '\0')
        {
            C[cNum] = A[i];
            cNum++;
            i++;
        }
    }

    for (i = 1; i <= cNum; i++)
	{
		for (j = 0; j <= cNum - i; j++)
		{
			if (C[j] > C[j + 1])
			{
				temp = C[j];
				C[j] = C[j + 1];
				C[j + 1] = temp;
			}
		}
	}

    return C;
}

int main()
{
    int A[MAX_LENGTH] = {1, 2};
    char B[MAX_LENGTH] = {'1', '2'};
    int* C;
    int i = 0;

    C = sortAAndB(A, B);

    for(i = 0; C[i] != '\0'; i++)
    {
        printf("%d\n", C[i]);
    }
    return 0;
}