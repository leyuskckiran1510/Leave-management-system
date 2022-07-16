#if 0
gcc -o ${0}.out $0 && ${0}.out 
rm -f ${0}.out
exit 
#endif


#include<time.h>
#include<stdio.h>




void main(){
    //time_t t;// = time(NULL);
    time_t t= time(NULL);
    __key_t tk;
    printf("%d\n%ld\n",tk,t);
    
}