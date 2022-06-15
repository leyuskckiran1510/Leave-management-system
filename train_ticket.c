#if 0 
dir=$(mktemp -d)
gcc -o ${dir}/a.out ${0} && ${dir}/a.out ${@:1} ; RV=${?}
rm -rf ${dir}
exit ${RV}
#endif

# include <stdio.h>
# include <string.h>
#include<stdlib.h>
#include <time.h>
#define print printf
void cls(){
	int kl;
	for(kl=0;kl<25;kl++){
		print("\n\r");
	}
}

void save(int *st){
	FILE *file;
	char *data;
    int i;
    for(i=0;i<300;i++){
    	data[i]=i;
    	printf("%d",i);
    }
    file= fopen("dump.txt", "a") ;
    if ( strlen ( data) > 0 )
    {
        fputs(data, file) ;
        fputs("\n",file);
    }
    fclose(file) ;

}
void read(){
	FILE *file;
	char *data;
	file= fopen("dump.txt", "r") ;
	while( fgets ( data, 5, file) != NULL )
        {
            printf( "%s" , data) ;
        }
    fclose(file) ;

}
void Aseat(int *st){
		int i,j;
	print("Seats  '1'-Booked\n");
	for(i=0;i<300;i++){
		if(st[i]==0){
			print("\x1b[33m0\x1b[0m| ");
		}
		else{
			print("\x1b[31m1\x1b[0m | ");
		}
	if(i%30==0 && i!=0){
	print("\n");
}
	}
	print("\n");
}

void banner(){

	print("\nWlecome to The Train Station\n\tChoose Your Option\n\t\t '1' for Booking\n\
		'2' for canceling\n\t\t'3' for exiting\n\f");
	
}



int booking(int *seat){
    static int i=0;
    char name[20];
    print("\nEnter Your Name:- ");
    scanf("%s",name);
    print("\n");
    print("Enter Seat No:- ");
    scanf("%d",&i);
    while(seat[i]!=0){
    print("\rAlready Takken !!\tEnter Seat No:- ");
    scanf("%d",&i);
    }
    return i;
    
    

	print("Hear Book");

}

int cancelling(int *seat){
	int i=0,j;
    char name[20];
    print("\n");
    print("Enter Booking Code No:- ");
    scanf("%d",&i);
    for(j=0;j<300;j++){
    	if(seat[j]==i){
    		return j;
    	}
    }
    print("Soory Invalid Booking Code Please Contact Authority If you Have Forgetton");
    return 301;


}


int main( )
{	
	int *seat,ch=1;
	int code,p,i;
	
	for (i = 0; i < 300; ++i)
	{
		seat[i]=0;

	}

    while(ch==1 || ch==2){
   	banner();
   	printf("\r");
   	scanf("%d",&ch);
    if(ch==1){
    	   	Aseat(seat);
    	p=booking(seat);
    	//1654
    	code =time(NULL)-1654500000;
    	cls();
    	print("Your Booking Code Is:- %d\n",code);
    	seat[p]=code;
    	

    }
    else if(ch==2){
    	Aseat(seat);
    	p = cancelling(seat);
    	cls();
    	if(p<=300){
    	seat[p]=0;
    	
    }


    }
    }

    return 0;       
}