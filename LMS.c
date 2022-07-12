#if 0
gcc -o ${0}.out $0 -lm && ${0}.out
rm -f ${0}.out
exit ${?}
#endif
//for easy compiling and execution 
//can be ingnored if you want to compile and execute separately with gcc


//###############################################################################//
/*  ::           ::::    ::::     :::::::  (Leave Management System)
    ::           :: ::  :: ::    :: 
    ::           ::  ::::  ::    ::
    ::           ::        ::     :::::: 
    ::           ::        ::           ::
    ::           ::        ::           ::
    ::::::::::   ::        ::     :::::::

                            BY- Kiran Raj Dhakal
*/
///////////////////////////////////////////////////////////////////////////////////





//Author:Kiran Raj Dhakal
//Date:2022/7/11
//Description:A program to implemet Leave Management System in C 
//            with file handling and error handling
//            The program is used to add,delete,modify and display the leaves of the employee
//            with admin,manager and employee login

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

//ansice code for screen clearing and moving cursor to the top left corner
//referenced from  https://en.wikipedia.org/wiki/ANSI_escape_code







struct employe{
    char id[50];
    char name[50];
    char dob[50];
    char phone[50];
    char email[50];
    char password[50];
    char type[50];
    int leaves;
    long int doj;
};

//struct leaves is used to store the leaves of the employee with 
//the employee id and date is used to store the date of the leaves
struct leaves{
    char id[50];
    long int date;
    
};


//function login() is used to login the user with the given credentials
//if the user is admin,manager or employee then it will return the user type
//from user.txt file




int login(char *username,char *password){
    FILE *fp;
    struct employe e;
    fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %s",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(strcmp(e.name,username)==0 && strcmp(e.password,password)==0){
            fclose(fp);
            printf("Login Successful\n");
            //return 1 if the user is admin , 2 if the user is manager , 3 if the user is employee
            if(strcmp(e.type,"admin")==0)
                return 1;
            else if(strcmp(e.type,"manager")==0)
                return 2;
            else
                return 3;
        }
    }
    fclose(fp);
    return 0;
}

//function add_employee() is used to add the employee details to the user.txt file
//with struct employee as parameter
void add_employee(struct employe b){
    FILE *fp;
    struct employe e;
    fp=fopen("user.txt","r+");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    //check if the employee id is already present in the file
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %d",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,&e.doj)!=EOF){
        //convert e.id to char and compare with b.id
        if(strcmp(e.id,b.id)==0){
            printf("Employee id already present\n");
            fclose(fp);
            return;
        }
    }
    //hash the password in md5 using openssl library and store it in e.password
    fprintf(fp,"%s %s %s %s %s %s %s %d %d\n",b.id,b.name,b.dob,b.phone,b.email,b.password,b.type,b.leaves,b.doj);
    fclose(fp);
}

//function delete_employee() is used to delete the employee details from the user.txt file
//with struct employee as parameter
void delete_employee(struct employe b){
    FILE *fp;
    FILE *fp1;
    struct employe e;
    fp=fopen("user.txt","r");
    fp1=fopen("temp.txt","w");
    if(fp==NULL || fp1==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %s",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(strcmp(e.id,b.id)!=0){
            fprintf(fp1,"%s %s %s %s %s %s %s  %d %d\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
        }
    }
    fclose(fp);
    fclose(fp1);
    remove("user.txt");
    rename("temp.txt","user.txt");
}

//function modify_employee() is used to modify the employee details from the user.txt file
//with struct employee as parameter
void modify_employee(struct employe b){
    FILE *fp;
    FILE *fp1;
    struct employe e;
    fp=fopen("user.txt","r");
    fp1=fopen("temp.txt","w");
    if(fp==NULL || fp1==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %d",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,&e.doj)!=EOF){
        if(strcmp(e.id,b.id)==0){
            fprintf(fp1,"%s %s %s %s %s %s %s  %d %d\n",b.id,b.name,b.dob,b.phone,b.email,b.password,b.type,b.leaves,e.doj);
        }
        else if(strcmp(e.id,b.id)!=0){
            fprintf(fp1,"%s %s %s %s %s %s %s  %d %d\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
        }
    }
    fclose(fp);
    fclose(fp1);
    remove("user.txt");
    rename("temp.txt","user.txt");
}

//function display_employee() is used to display the employee details from the user.txt file
//with struct employee as parameter
void display_employee(struct employe b){
    FILE *fp;
    struct employe e;
    char doj[50];
    fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %d",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,&e.doj)!=EOF){
       if(strcmp(e.id,b.id)==0){
        strcpy(doj,ctime(e.doj));
        printf("%s %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,doj);
        }
    }
    fclose(fp);
}

//function to convert time epoch to date
char *epoch_to_date(int epoch){
    char *date;
    time_t t = epoch;
    date = ctime(&t);
    return date;
}

//display employee details and leaves of the employee with the employee id
void display_employee_leaves(char *id){
    FILE *fp;
    FILE *fp1;
    struct employe e;
    struct leaves l;
    char doj[50];
    fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %d",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,&e.doj)!=EOF){
        if(strcmp(e.id,id)==0){
            strcpy(doj,ctime(e.doj));
            printf("%s %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,doj);
            printf("Leaves\n");
            fp1=fopen("leaves.txt","r");
            if(fp==NULL){
                printf("Error in opening file\n");
                exit(0);
            }
            while(fscanf(fp1,"%s %ld",l.id,&l.date)!=EOF){
                if(strcmp(e.id,l.id)==0){
                    //convert epoch to date
                    char *date=epoch_to_date(l.date);
                    printf("%s %s\n",l.id,date);
            }
        }
        fclose(fp1);
    }
}
fclose(fp);
}




void gen_id(struct employe *b){
    //take current time in epoch format and attach employye name to it
    char str[11];
    int epoch=time(NULL);
    sprintf(str, "%d", epoch);
    str[strlen(str)-1]=b->name[0];
    printf("%s\n",str);
    strcpy(b->id,str);
    }


void alert(char *text){
    char key;
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    /* referenced from https://stackoverflow.com/questions/8101079/making-stdin-non-blocking*/
    //to handel single key stroke at a time as linux/unix terminal won't allow by default due to canonical mode
    //code between '//||' is from stackoverflow
    struct timeval tv;
    struct termios ttystate, ttysave;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttysave = ttystate;
    //turn off canonical mode and echo
    ttystate.c_lflag &= ~(ICANON | ECHO);
    //minimum of number input read.
    ttystate.c_cc[VMIN] = 1;
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    
    printf("Press any key to continue\n");
    //key press to contin
    key=getchar();
    if(key=='\n'){
            
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    //stackoverflow code 
    //reverting back to canonical mode and echo to avoid terminal crashes after the program is complete
    //please don't press CTRL+C to exit the program as it will cause the terminal to crash
    ttystate.c_lflag |= ICANON | ECHO;
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttysave);
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
        return;
    }
    else{
        alert(text);
    }
    
    
}


void login_screen(char *n,char *p){
    //deginse of a login in screen with the name and password as parameters
    //using ansi escape sequence
    printf("\033[2J\033[1;1H\033[?25h\033[0m");
    printf("\033[10;50H\033[38;2;150;15;245mLogin Screen\033[0m\n");
    printf("\033[13;40H\033[38;2;40;205;205mEnter name:\033[0m\n");
    printf("\033[15;40H\033[38;2;205;40;205mEnter password:\033[0m\n");
    printf("\033[13;56H\033[38;2;50;255;255m");
    scanf("%s",n);
    fflush(stdin);
    printf("\033[15;56H\033[38;2;255;50;255m");
    scanf("%s",p);
    printf("\033[0m\n");
    printf("\033[2J\033[1;1H\033[?25h\033[0m");

}
//employee adding Screen
void employee_add_Screen(){
    struct employe e;
    /*char *date;
    time_t t = time(NULL);
    date = ctime(&t);
    strcpy(e.doj,date);*/
    e.doj=time(NULL);
    e.leaves=5;
    printf("\033[2J\033[1;1H\033[?25h\033[0m");
    printf("\033[9;50H\033[38;2;150;15;245mEmployee Add Screen\033[0m\n");
    printf("\033[12;40H\033[38;2;40;205;205mEnter NAME:\033[0m\n");
    printf("\033[13;40H\033[38;2;205;40;205mEnter DOB:\033[0m\n");
    printf("\033[14;40H\033[38;2;40;205;205mEnter PHONE:\033[0m\n");
    printf("\033[15;40H\033[38;2;205;40;205mEnter EMAIL:\033[0m\n");
    printf("\033[16;40H\033[38;2;205;40;205mEnter PASSWORD:\033[0m\n");
    printf("\033[17;40H\033[38;2;40;205;205mEnter TYPE:\033[0m\n");
    printf("\033[12;56H\033[38;2;50;255;255m");
    scanf("%s",e.name);
    printf("\033[13;56H\033[38;2;50;255;255m");
    scanf("%s",e.dob);
    printf("\033[14;56H\033[38;2;50;255;255m");
    scanf("%s",e.phone);
    printf("\033[15;56H\033[38;2;50;255;255m");
    scanf("%s",e.email);
    printf("\033[16;56H\033[38;2;50;255;255m");
    scanf("%s",e.password);
    printf("\033[17;56H\033[38;2;50;255;255m");
    scanf("%s",e.type);
    strcpy(e.id,"Blank");
    gen_id(&e);
    printf("%s",e.id);
    printf("\033[2J\033[1;1H\033[?25h\033[0m");
    add_employee(e);
}


int main(){
    int status;
    //char name[50];
    //char password[50];
    //statically declare admin employee details 
    //struct employe admin={"admin!","admin","2022/1/1","+9779800000000","admin@admin.com","696969","admin",10,"2022"};
    //gen_id(&admin);
    //printf("%s\n",admin.id);
    //add_employee(admin);
    //delete_employee(e);
    //modify_employee(e);
    //display_employee(e);
    //login_screen(name,password);
    //status=login(name,password);
    //printf("%d\n",status);
    employee_add_Screen();
    
    //alert("Hello");
    return 0;
}