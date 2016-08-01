#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/ipc.h>
#define ARRSIZE 15

key_t key = IPC_PRIVATE;

void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;

	int L[n1], R[n2];

	for(i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for(j = 0; j < n2; j++)
		R[j] = arr[m + 1+ j];

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}

	return;
}

void mergeSort(int arr[], int l, int r)
{

	pid_t rightpid,leftpid;
	int lstatus,rstatus;

	if(r-l<4)
	{
		int i;

		for(i=l;i<=r;i++)
		{
			int min = i,j;

			for(j=i;j<=r;j++)
			{
				if(arr[min]>arr[j])
				{
					min = j;
				}
			}

			int temp = arr[i];
			arr[i] = arr[min];
			arr[min] = temp;
		}

		return;
	}
	else
	{
		int m = l+(r-l)/2; 

		if((rightpid=fork())<0)
		{
			perror("fork Failed");
			exit(1);
		}

		if((leftpid=fork())<0)
		{
			perror("fork Failed");
			exit(1);
		}

		if(leftpid==0)
		{
			mergeSort(arr, l, m);
			_exit(0);
		}
		if(rightpid==0)
		{
			mergeSort(arr, m+1, r);
			_exit(0);
		}

		waitpid(rightpid, &rstatus, 0);
		waitpid(leftpid, &lstatus, 0);

		merge(arr, l, m, r);

		return;
	}
}

int main()
{
	int i;

	int *a;
	int shmid,status;

	if((shmid=shmget(key, sizeof(int) *ARRSIZE, IPC_CREAT| 0666)) < 0)
	{
		perror("Shmget failed");
		exit(1);
	}

	if((a = shmat(shmid, 0, 0)) == (int *) -1)
	{
		perror("Shmat failed");
		exit(1);
	}


	for(i=0;i<ARRSIZE;i++)
	{
		scanf("%d",&a[i]);
	}

	mergeSort(a,0,ARRSIZE-1);

	for(i=0;i<ARRSIZE;i++)
	{
		printf("%d ",a[i]);
	}
	printf("\n");

	return 0;
}
