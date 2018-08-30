#include<stdio.h>

int main(){
	char balls[100];
	scanf("%s",balls);
	int p1=0,p2=0,f=0,extra=0,total=0,ball=0,i;
	for(i=0;balls[i]!='\0';i++){
		if(balls[i] != 'W')
			ball++;
		if(balls[i] >= '0' && balls[i] <= '9'){
			int runs = balls[i] - '0';
			if(f){
				p2 += runs;
			}else{
				p1 += runs;
			}
			if(runs%2 != 0)
				f = !f;
		}
		else if(balls[i] == 'N' || balls[i] == 'W'){
			extra +=1;
		}
		
		if(ball % 6 == 0 && balls[i] != 'W')
			f = !f;	
	}
	
	printf("\nP1 : %d%c",p1,f==0?'*':' ');
	printf("\nP2 : %d%c",p2,f==1?'*':' ');
	printf("\nExtra : %d",extra);
	printf("\nOver : %d.%d",ball/6,ball%6);
	printf("\nTotal : %d",p1+p2+extra);
	return 0;
}
