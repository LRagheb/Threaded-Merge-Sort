#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *Array;
int size;

//division occurs in MergeSort which has two arguments: int left and int right;
//thread is created each time mergeSort is called

typedef struct arguments{ //arguments that are passed to merge sort are saved in a struct (for thread creation)
int left,right;
}args;


void merge(int left,int middle,int right) //function that merges two array halves into one sorted array
{
int sizeL=middle-left+1; //sizes two create two auxilliary arrays
int sizeR=right-middle;
int leftArray[sizeL],rightArray[sizeR];
int i,j; //index

//copy data into two auxilliary arrays for merging
for (i=0;i<sizeL;i++)
{

leftArray[i]=Array[left+i];
}
for (j=0;j<sizeR;j++)
{

rightArray[j]=Array[middle+1+j];
}
i=j=0;
int k=left; //start position of the array being handled (because it's part of the main array

//Merging
while (i<sizeL && j<sizeR)
{
//if left element is smaller -> pick and move left index
if (leftArray[i]<=rightArray[j])
{
Array[k]=leftArray[i];
i++;
}
else{ //right element is smaller -> pick and move right index
Array[k]=rightArray[j];
j++;

}
//move array index
k++;

}

//One array of either left or right will mostly still contain elements
while (i<sizeL)
{
Array[k]=leftArray[i];
i++;k++;
}
while (j<sizeR)
{
Array[k]=rightArray[j];
j++;k++;
}

}

void *mergeSort (void *param) //function that creates threads
{
args *data=(args *)param; //upcasting
int left=data->left;
int right=data->right;
if (left<right)//return if left index passes right index
{
pthread_t tid1,tid2; //threads to be created
int middle=(left+right)/2;

//parameters for each struct are saved in a struct

args *l=(args *)malloc(sizeof(args));
args *r=(args *)malloc(sizeof(args));
l->left=left;
l->right=middle;
r->left=middle+1;
r->right=right;

//threads created and mergeSort is called

pthread_create(&tid1,NULL,mergeSort,(void *)l);
pthread_create(&tid2,NULL,mergeSort,(void *)r);

//wait for threads to finish
pthread_join(tid1,NULL);
pthread_join(tid2,NULL);
free(r);
free(l);
merge(left,middle,right);

}
}


int main()
{
FILE *fp;
fp=fopen("input.txt","r");
if(fp==NULL)
{
printf("Error! couldn't open file.");
exit(1);
}
fscanf(fp,"%d",&size);
//create array to hold numbers
Array=malloc(size*sizeof(int));
printf("\nUnsorted input:\n");

int i;
for (i=0;i<size;i++)
{
fscanf(fp,"%d",&Array[i]);

printf("%d ",Array[i]);
}

pthread_t tidMain; //parent thread
args *a=(args *)malloc(sizeof(args));
a->left=0;
a->right=size-1;
pthread_create(&tidMain,NULL,mergeSort,(void *)a);
pthread_join(tidMain,NULL);
free(a);
printf("\n\nSORTED:\n");

for (i=0;i<size;i++)
{

printf("%d ",Array[i]);
}

}
