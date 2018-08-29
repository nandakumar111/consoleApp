#include<stdio.h>
#include<stdlib.h>

int main(){
	int n,*a,i,j,m=0,*b;
	scanf("%d",&n);
	a = (int *)malloc(n*sizeof(int));
	b = (int *)malloc(sizeof(int));
	for(i=0;i<n;i++)
		scanf("%d",a+i);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(*(a+i) < *(a+j)){
				int temp = *(a+i);
				*(a+i) = *(a+j);
				*(a+j) = temp;
			}
		}
	}
	for(i=0;i<n;i++){
		int f=1;
		for(j=i+1;j<n;j++){
			if(*(a+i) == *(a+j)){
				f=0;
			}
		}
		if(f){
			*(b+m) = *(a+i);
			m++;
		}
	}
	for(i=0;i<m;i++)
		printf("%d ",*(b+i));
	free(a);
	free(b);
	return 0;
}
