#if 0 
dir=$(mktemp -d)
gcc -o ${dir}/a.out ${0} && ${dir}/a.out ${@:1} ; RV=${?}
rm -rf ${dir}
exit ${RV}
#endif

#include <stdio.h>
#include<stdlib.h>
#define red(str) printf("\x1b[48;2;220;20;60m" "%s" "\x1b[0m",str)
#define blue(str) printf("\x1b[48;2;0;56;147m" "%s" "\x1b[0m",str)
#define white(str) printf("\x1b[48;2;255;255;255m" "%s" "\x1b[0m",str)



void main(){
	int i,j,k,t;
	int size=25;
	printf("P.S reduce your terminal font size for better experience\nEnter The Size of Flag:- ");
	scanf("%d",&size);
	t=size/7;
	for (i = 0; i <2; ++i)
	{
		for (j= i*2; j <=size; ++j)
	{
		for (k= 0; k <= j*2+(size/2)*i; ++k)
		{
			if(k==0 || k==j*2+(size/2)*i || j==size || k==1 || k==j*2+(size/2)*i -1){
				blue(" ");
			}
			else if((k>=(j*2+(size/2)*i)/2-t && j>=((size/4)*3)-(t/3))&&(k<=(j*2+(size/2)*i)/2+t && j<=((size/4)*3)+(t/3))){
				white(" ");
			}
			else{
			red(" ");
		}
		/* if your terminal has NerdFace Fonts
		if(k==j*2+(size/2)*i && j==size){
			printf("\x1b[34m\x1b[0m");
		}*/
		}
		printf("\n");
	}
	}
	
}
