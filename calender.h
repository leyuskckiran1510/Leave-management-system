#if 0
clear
gcc -o ${0}.out ./$0 -lm && ./${0}.out
rm -f *.out


exit ${?}
#endif
// Calender for leave managment system .......

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
//#include"keypress_setup.h" // calender contains keypress_setup.h file to set up key press for windows mac and linux , 



#define title(k) printf("\033]0;%s\007", k)


int calendar(int,int,int);

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
void press(int * key) {
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    /* referenced from https://stackoverflow.com/questions/8101079/making-stdin-non-blocking*/
    //to handel single key stroke at a time as linux/unix terminal won't allow by default due to canonical mode
    //code between '//||' is from stackoverflow
    struct timeval tv;
    struct termios ttystate, ttysave;
    tcgetattr(STDIN_FILENO, & ttystate);
    ttysave = ttystate;
    //turn off canonical mode and echo
    ttystate.c_lflag &= ~(ICANON | ECHO);
    //minimum of number input read.
    ttystate.c_cc[VMIN] = 1;
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, & ttystate);
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    //key press to contin
    * key = getchar();
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    //stackoverflow code 
    //reverting back to canonical mode and echo to avoid terminal crashes after the program is complete
    //please don't press CTRL+C to exit the program as it will cause the terminal to crash
    ttystate.c_lflag |= ICANON | ECHO;
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, & ttysave);
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
}


void calenderconf(){
//void clearScreen(void);
printf("\033[2J");

title("############   WELCOME TO CALENDER      ##############");
// prints index...
printf("\x1b[%d;%dH\033[39m\033[6m* Calender *\x1b[0m",1,58);
printf("\033[38;2;220;255;20m\033[48;2;255;30;30m\033[%d;%dH \x1b[0m",2,125);
printf("\033[82;24;100;255;20m\033[48;2;25;230;30m\033[%d;%dH \x1b[0m",3,125);
printf("\033[82;24;100;255;20m\033[%d;%dH: HOLIDAY\x1b[0m",2,127);
printf("\033[82;24;100;255;20m\033[%d;%dH: WORKDAY\x1b[0m",3,127);

int j,r,b=0;
for (j=2;j<21;j+=9){
for (r=2;r<27;r+=8){
calendar(r,j,b);
b++;
}}

    printf("\033[%d;%dH",50,  130);
    printf("....");
    press(& b);	
    press(& b);
    
}
int calendar(int r,int j,int b){




int i,k ,q=4,z=0;
 char * week[7] = {"SUN","MON","TUE","WED","THU","FRI", "SAT"};
 char * month[12]={"JAN","FEB","MAR","APR","MAY","JUN","JULY","AUG","SEP","OCT","NOV","DEC"};
int t=1,count,key=0;

for (k=0;k<=4;k++){
int f=j;
// prints months 
if(k<1) printf("\033[%d;%dH\033[4m\033[38;2;%d;%d;%dm[  %s  ]\033[0m ",f-1,(q)*(r+1),244,255,55,month[b]);

for (i=0;i<=6;i++){

// prints weeks
if(k<1) printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s\033[0m",f,q*(r-1),140,55,155,week[i]);

// simple print of date 
if((t<=30)) printf("\033[%d;%dH\033[38;2;%d;%d;%dm %2d\033[0m",f,r*q,45,255,55,t);
// print for HOliday
if ((i==6) && (t<=30) ||((i+t+f*r-q)%7==3) &&(t<=30))printf("\033[%d;%dH\033[7m\033[38;2;%d;%d;%dm %2d\033[0m",f,r*q,255,0,0,t);

// print for month 'b' with date to 31 days ...
if ((t==31) && ((b==0) ||(b==1) || (b==8) || (b==2) || (b==11))) printf("\033[%d;%dH\033[38;2;%d;%d;%dm %2d\033[0m",f,r*q,45,255,55,t);

// print for feburary month to remove if 31 and 30 allocated ..
if ((b==1) && ((t==31) ||(t==30))) printf("\033[%d;%dH\033[38;2;%d;%d;%dm   \033[0m",f,r*q,45,255,55);
z=z+t;

// day start from Tuesday for provided months 'b's
if (((z==1)||(z==2)||(z==4)) && ((b==0) ||(b==1) || (b==8) || (b==9) || (b==11))) {printf("\033[%d;%dH\033[38;2;%d;%d;%dm    \033[0m",f,r*q,0,0,0); t-=1;}

// day starts from monday
if (((z==1)||(z==2)) && ((b==3) ||(b==5) || (b==7) || (b==12))) {printf("\033[%d;%dH\033[38;2;%d;%d;%dm    \033[0m",f,r*q,0,0,0); t-=1;}
// day starts from monday for months b=2 @ i.e MARCH and b=10 @ i.e OCTOBER..
if ((z==1) && ((b==2) ||(b==10))) {printf("\033[%d;%dH\033[38;2;%d;%d;%dm    \033[0m",f,r*q,0,0,0); t-=1;}


    t+=1; 
    f+=1; 
    if(k>=1) z+=1;
    }
        r+=1;
    
   }
 }

int main(){
calenderconf();
return 0;
}