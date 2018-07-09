#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

// 1 data has 1024 dimension of float feature
const int dimension = 1024;
const int data_1_size = dimension * 4;
const long data_num_1GB = 1024l * 1024l * 1024l / data_1_size;

int data_size_GB = 0; // GB
long data_num = 0;

float *data;

int main()
{
	printf("print data size in GB\n");
	scanf("%d", &data_size_GB);
	data_num = data_size_GB * data_num_1GB;
	//printf("data_num_1GB : %ld\n", data_num_1GB);
	printf("%d GB data_num : %ld\n", data_size_GB, data_num);

	data = (float*)malloc(sizeof(float)*data_num_1GB*dimension);

	FILE *fin;
	fin = fopen("data.bin","wb"); 
	
	for(int gb=0; gb<data_size_GB; gb++)
	{
		for(int i=0; i<data_num_1GB; i++)
		{
			for(int d=0; d<dimension; d++)
			{
				float tmp = ((double)(rand())/RAND_MAX*1000.0);
				// float tmp = 100 * i + d * 1;
				data[i*dimension+d] = tmp;
			}
		}
		fwrite(data, sizeof(float), data_num_1GB * dimension, fin);
	}
	fclose(fin);

	printf("finish\n");

	return 0;
}