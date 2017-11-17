
#include <stdio.h>

typedef unsigned char* byte_pointer;

void show_bytes(byte_pointer start,int len)
{
	int i = 0;
	for(;i < len;++i)
	{
		printf(" %.2x",start[i]);
	}
	printf("\n");
}
void show_int(int x)
{
	show_bytes(&x,sizeof(int));
}
void show_float(float x)
{
	show_bytes(&x,sizeof(float));
}

void show_pointer(void* x)
{
	show_bytes(x,sizeof(void*));
}

int main01()
{
	int a = 4;
	show_int(a);
	show_pointer(&a);
	float b = 10;
	show_float(b);
}


void 
inplace_swap(int *a,int *b)
{
	*b = *a ^ *b;
	*a= *a ^ *b;
	*b = *a ^ *b;
}

void reverse_array(int array[],int cnt)
{
	int first;int last;
	for(first = 0,last = cnt-1;first < last;first++,last--)
	{
		inplace_swap(&array[first],&array[last]);
	}
}


int
main(int argc,char**argv)
{
	int a[] = {1,2,3,4,5};
	size_t len = sizeof(a)/sizeof(a[0]);
	reverse_array(a,len);
	for(int i = 0;i < len;++i)
	{
		printf("%d",a[i]);
	}
}