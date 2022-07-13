#if 0
gcc -o ${0}.out $0 ./anisc.c -lm && ${0}.out
rm -f *.out
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
#include "anisc.h"


//checking for if the system is running in linux or windows and defining the includes accordingly
//and different functions are used for linux and windows

#ifdef __APPLE__
#include<unistd.h>
#include<termios.h>
#include<fcntl.h>

#elif defined _WIN32 || defined _WIN64
void press(int *key){
        * key = getchar();
    }
#else
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
#endif

//referenced from  https://en.wikipedia.org/wiki/ANSI_escape_code
//title of the program using ANSI escape code
#define title(k) printf("\033]0;%s\007", k)


struct employe {
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
struct leaves {
    char id[50];
    long int date;

};
//function login() is used to login the user with the given credentials
//if the user is admin,manager or employee then it will return the user type
//from user.txt file
int login(char * username, char * password) {
    FILE * fp;
    struct employe e;
    fp = fopen("user.txt", "r");
    if (fp == NULL) {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, & e.leaves, & e.doj) != EOF) {
        //print username and password from the file and given password and username are compared
        if (strcmp(username, e.name) == 0 && strcmp(password, e.password) == 0) {
            fclose(fp);
            printf("Login Successful\n");
            
            //return 1 if the user is admin , 2 if the user is manager , 3 if the user is employee
            if (strcmp(e.type, "admin") == 0)
                return 49;
            else if (strcmp(e.type, "manager") == 0)
                return 50;
            else
                return 51;
        }
    }
    getchar();
    fclose(fp);
    return 48;
}

void calander() {
    //make a calander with the dates of the month
    //the calander is made with the help of the ansice code
    //referenced from  https://en.wikipedia.org/wiki/ANSI_escape_code
    printf("\033[0J");
    int count = 32;
    int placex = 10;
    int placey = 30;
    char * week[7] = {
        "SUN",
        "MON",
        "TUE",
        "WED",
        "THU",
        "FRI",
        "SAT"
    };
    int i, j;
    for (i = 0; i < 7; i++) {
        printf("\033[1;32m\033[%d;%dH%s", placex - 2, placey + i * 5, week[i]);
        if (i == 6) {
            printf("\033[38;2;220;255;20m\033[48;2;255;30;30m\033[%d;%dH%s  \x1b[0m", placex - 2, placey + i * 5, week[i]);
        }
        //placey+=10;
    }

    printf("\033[%d;%dH", placex, placey);
    for (int i = 1; i < count; i++) {
        if (i % 7 == 0) {
            placex++;
            printf("\033[38;2;220;255;20m\033[48;2;255;30;30m%2d   \033[0m", i);
            printf("\033[%d;%dH", placex, placey);
        } else {
            printf("\033[1;32m%2d   \033[0m", i);
        }
    }
    //move the cursor to the bottom left corner
    printf("\033[%d;%dH", placex + 10, placey + 5);
    printf("Press Any Key To Continue");
    press(&count);

}

//function add_employee() is used to add the employee details to the user.txt file
//with struct employee as parameter
void add_employee(struct employe b) {
    FILE * fp;
    struct employe e;
    fp = fopen("user.txt", "r+");
    if (fp == NULL) {
        printf("Error in opening file\n");
        exit(0);
    }
    //check if the employee id is already present in the file
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, & e.leaves, & e.doj) != EOF) {
        //convert e.id to char and compare with b.id
        if (strcmp(e.id, b.id) == 0) {
            printf("Employee id already present\n");
            fclose(fp);
            return;
        }
    }
    //generate time epoch time in milliseconds
    long int time_epoch = time(NULL);
    b.doj = time_epoch;
    fprintf(fp, "%s %s %s %s %s %s %s %d %ld\n", b.id, b.name, b.dob, b.phone, b.email, b.password, b.type, b.leaves, b.doj);
    fclose(fp);
}

//function delete_employee() is used to delete the employee details from the user.txt file
//with struct employee as parameter
void delete_employee(struct employe b) {
    FILE * fp;
    FILE * fp1;
    struct employe e;
    fp = fopen("user.txt", "r");
    fp1 = fopen("temp.txt", "w");
    if (fp == NULL || fp1 == NULL) {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, & e.leaves, & e.doj) != EOF) {
        if (strcmp(e.id, b.id) != 0) {
            fprintf(fp1, "%s %s %s %s %s %s %s  %d %ld\n", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, e.leaves, e.doj);
        }
    }
    fclose(fp);
    fclose(fp1);
    remove("user.txt");
    rename("temp.txt", "user.txt");
}

//function modify_employee() is used to modify the employee details from the user.txt file
//with struct employee as parameter
void modify_employee(struct employe b) {
    FILE * fp;
    FILE * fp1;
    struct employe e;
    fp = fopen("user.txt", "r");
    fp1 = fopen("temp.txt", "w");
    if (fp == NULL || fp1 == NULL) {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, & e.leaves, & e.doj) != EOF) {
        if (strcmp(e.id, b.id) == 0) {
            fprintf(fp1, "%s %s %s %s %s %s %s  %d %ld\n", b.id, b.name, b.dob, b.phone, b.email, b.password, b.type, b.leaves, e.doj);
        } else if (strcmp(e.id, b.id) != 0) {
            fprintf(fp1, "%s %s %s %s %s %s %s  %d %ld\n", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, e.leaves, e.doj);
        }
    }
    fclose(fp);
    fclose(fp1);
    remove("user.txt");
    rename("temp.txt", "user.txt");
}

//function display_employee() is used to display the employee details from the user.txt file
//with struct employee as parameter
void display_employee(char b[]) {
    printf("\033]0;%s Detail's\007",b);
    int i=0;
    FILE * fp;
    struct employe e;
    char doj[50];
    fp = fopen("user.txt", "r");
    if (fp == NULL) {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, & e.leaves, & e.doj) != EOF) {
        if (strcmp(e.id, b) == 0) {
            time_t t = e.doj;
            strcpy(doj, ctime( & t));
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 0, 150, 255, 0 * 31, e.id);
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (1), 150, 255, (1) * 31, e.name);
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (2), 150, 255, 2 * 31, e.dob);
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (3), 150, 255, 3 * 31, e.phone);
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (4), 150, 255, 4 * 31, e.email);
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (5), 150, 255, 5 * 31, e.type);
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%d", 4 + i, 16 * (6), 150, 255, 6 * 31, e.leaves);
            printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (7), 150, 255, 7 * 31, doj);
        }
    }
    fclose(fp); 
}

//To display all the employees details from the user.txt file

void display_employees() {
    title("All Employee Details");
    FILE * fp;
    struct employe e;
    int i = 0;
    int key;
    char doj[50];
    int count = 0;
    char * title[] = {
        "ID",
        "Name",
        "DOB",
        "Phone",
        "Email",
        "Type",
        "Leaves",
        "DOJ"
    };
    fp = fopen("user.txt", "r");
    if (fp == NULL) {
        printf("Error in opening file\n");
        exit(0);
    }
    printf("\033[0J\033[1;1H\033[?25l\033[0m");
    printf("\033[%d;%dH", 3, 5);
    for (int j = 0; j < 8; j++) {
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s\t", 3, 16 * j, 150, 255, j * 31, title[j]);
    }
    printf("\033[%d;%dH", 4, 5);
    i = 2;
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, & e.leaves, & e.doj) != EOF) {
        time_t t = e.doj;
        strcpy(doj, ctime(&t));
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 0, 150, 255, 0 * 31, e.id);
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (1), 150, 255, (1) * 31, e.name);
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (2), 150, 255, 2 * 31, e.dob);
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (3), 150, 255, 3 * 31, e.phone);
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (4), 150, 255, 4 * 31, e.email);
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (5), 150, 255, 5 * 31, e.type);
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%d", 4 + i, 16 * (6), 150, 255, 6 * 31, e.leaves);
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s", 4 + i, 16 * (7), 150, 255, 7 * 31, doj);
        //printf("\033[%d;%dH%s %s %s %s %s %s %s  %d %s",3,16*i,e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,doj);
        i++;
        count++;
        if (count >= 10) {
            printf("\033[%d;%dH%s", 26, 5, "Press S to continue or any other key to exit");
            press( & key);
            if (key == 's' || key == 'S') {
                count = 0; //
                printf("\033[%d;%dH%s", 26, 5, "                                            ");
                continue;
            } else {
                printf("\033[%d;%dH%s", 26, 5, "                                            ");
                break;
            }
        }
    }
    fclose(fp);
    printf("\033[%d;%dH%s\033[26,32H", 26, 5, "Press any key to continue");
    press(&key);
    printf("\033[0J\033[1;1H");
}

//function to convert time epoch to date
char * epoch_to_date(int epoch) {
    char * date;
    time_t t = epoch;
    date = ctime( & t);
    return date;
}

//display employee details and leaves of the employee with the employee id
void display_employee_leaves(char * id) {
    title("Employee Leaves");
    FILE * fp;
    FILE * fp1;
    struct employe e;
    struct leaves l;
    char doj[50];
    fp = fopen("user.txt", "r");
    if (fp == NULL) {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, & e.leaves, & e.doj) != EOF) {
        if (strcmp(e.id, id) == 0) {
            time_t t = e.doj;
            strcpy(doj, ctime( & t));
            printf("%s %s %s %s %s %s %s  %d %s\n", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, e.leaves, doj);
            printf("Leaves\n");
            fp1 = fopen("leaves.txt", "r");
            if (fp == NULL) {
                printf("Error in opening file\n");
                exit(0);
            }
            while (fscanf(fp1, "%s %ld", l.id, & l.date) != EOF) {
                if (strcmp(e.id, l.id) == 0) {
                    //convert epoch to date
                    char * date = epoch_to_date(l.date);
                    printf("%s %s\n", l.id, date);
                }
            }
            fclose(fp1);
        }
    }
    fclose(fp);
}

void display_leaves(){}


void gen_id(struct employe * b) {
    //take current time in epoch format and attach employye name to it
    char str[50];
    char pseudo[2];
    long int time_epoch = time(NULL);
    sprintf(str, "%ld", time_epoch); //converting epoch to string
    sprintf(pseudo, "%c", b -> name[0]); //converting first character of name to string
    strcat(str, pseudo); //concatenating first character of name to epoch string to generate near unique id
    printf("%s\n", str);
    strcpy(b -> id, str);
}

void login_screen(char * n, char * p){
    //deginse of a login in screen with the name and password as parameters
    //using ansi escape sequence
    title("LOGIN SCREEN");
    printf("\033[0J\033[1;1H\033[?25h\033[0m");
    printf("\033[10;50H\033[38;2;150;255;0mLogin Screen\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;31mEnter name:\033[0m\n");
    printf("\033[15;40H\033[38;2;150;255;62mEnter password:\033[0m\n");
    printf("\033[13;56H\033[38;2;150;255;93m");
    scanf("%s", n);
    fflush(stdin);
    printf("\033[15;56H\033[38;2;255;50;255m");
    scanf("%s", p);
    fflush(stdin);
    printf("\033[0m\n");
    //printf("\033[0J\033[1;1H\033[?25h\033[0m");

}
//employee adding Screen
void employee_add_Screen() {
    title("EMPLOYEE ADD SCREEN");
    struct employe e;
    /*char *date;
    time_t t = time(NULL);
    date = ctime(&t);
    strcpy(e.doj,date);*/
    long int time_epoch = time(NULL);
    e.doj = time_epoch;
    e.leaves = 5;
    printf("\033[0J\033[1;1H\033[?25h\033[0m");
    printf("\033[9;50H\033[38;2;150;255;0mEmployee Add Screen\033[0m\n");
    printf("\033[12;40H\033[38;2;150;255;31mEnter NAME:\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;62mEnter DOB:\033[0m\n");
    printf("\033[14;40H\033[38;2;150;255;93mEnter PHONE:\033[0m\n");
    printf("\033[15;40H\033[38;2;150;255;124mEnter EMAIL:\033[0m\n");
    printf("\033[16;40H\033[38;2;150;255;155mEnter PASSWORD:\033[0m\n");
    printf("\033[17;40H\033[38;2;150;255;181mEnter TYPE:\033[0m\n");
    printf("\033[12;56H\033[38;2;50;255;255m");
    scanf("%s", e.name);
    printf("\033[13;56H\033[38;2;50;255;255m");
    scanf("%s", e.dob);
    printf("\033[14;56H\033[38;2;50;255;255m");
    scanf("%s", e.phone);
    printf("\033[15;56H\033[38;2;50;255;255m");
    scanf("%s", e.email);
    printf("\033[16;56H\033[38;2;50;255;255m");
    scanf("%s", e.password);
    printf("\033[17;56H\033[38;2;50;255;255m");
    scanf("%s", e.type);
    strcpy(e.id, "Blank");
    gen_id( & e);
    printf("%s", e.id);
    printf("\033[0J\033[1;1H\033[?25h\033[0m");
    add_employee(e);
}


void admin_screen( char * n, char * p) {
    title("ADMIN SCREEN");
    printf("\033[0J\033[1;1H\033[?25h\033[0m");
    printf("\033[10;50H\033[38;2;150;255;0mAdmin Screen\033[0m\n");
    printf("\033[12;40H\033[38;2;150;255;31mAdd Employee\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;62mView Employee\033[0m\n");
    printf("\033[14;40H\033[38;2;150;255;93mView All Employee\033[0m\n");
    printf("\033[15;40H\033[38;2;150;255;124mDelete Employee\033[0m\n");
    printf("\033[16;40H\033[38;2;150;255;124mModify Employee\033[0m\n");
    
}

void menu1(){
    title("Welcome To LMS by Kiran");
    printf("\033[0J\033[1;1H\033[?25l\033[0m");
    printf("\033[10;50H\033[38;2;150;255;0mWelcome to Employee Management System\033[0m\n");
    printf("\033[11;40H\033[38;2;150;255;31mPress 1 to LOGIN\033[0m\n");
    printf("\033[12;40H\033[38;2;150;255;155mPress 2 to DISPLAY CALANDER\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;181mPress 3 to EXIT\033[0m\n");
    printf("\033[12;56H\033[38;2;50;255;255m");
}


int main() {
    setupConsole();
    int status;
    //hide cursor
    printf("\x1b[?25l");
    char name[50];
    char password[50];
    char a;
    int fake_key;
    //statically declare admin employee details 
    //struct employe admin={"admin!","admin","2022/1/1","+9779800000000","admin@admin.com","696969","admin",10,10};
    //gen_id(&admin);
    //printf("%s\n",admin.id);
    //add_employee(admin);
    //delete_employee(e);
    //modify_employee(e);
    //display_employee("1657632111a");
    //press(&status);
    //login_screen(name,password);
    //status=login(name,password);
    //printf("%d\n",status);
    //employee_add_Screen();

    //alert("Hello");
    //display_employees();
    /*int p;
    press(&p);
    printf("%d\n",p);*/
    //display_leaves();
    //welcome screen wir text
    while(1){
    menu1();
    press(&status);
    //ascii values of 1,2,3,4 are 49,50,51,52
    if(status==49) {
        printf("\033[0J\033[1;1H\033[?25l\033[0m");
        login_screen(name,password);
        getchar();
        status=login(name,password);
        //if admin
        if(status==49) {
                                printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                admin_screen(name,password);
                                press(&status);
                                if(status==49) {
                                    //clear screen
                                    printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                    employee_add_Screen();
                                }
                                else if(status==50) {
                                    printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                    display_employees();
                                }
                                else if(status==51) {
                                    printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                    display_leaves();
                                }
                                else if(status==52) {
                                    printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                    display_leaves();
                                }
                                else if(status==53) {
                                    printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                    calander();
                                }
                                else if(status==54) {
                                    printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                    break;
                                }
        }
        //if manager
        else if(status==50) {
                                printf("\033[0J\033[1;1H\033[?25l\033[0m");
                                employee_add_Screen();
        }
        //if employee
        else if(status==51) {
            printf("\033[0J\033[1;1H\033[?25l\033[0m");
            display_employees();
        }
        else if(status==48) {
            printf("\033[11;45H\033[?25l\033[6mWRONG CREDENTIALS TRY AGAIN\033[0m");
            press(&fake_key);
        }
        else{
            printf("\033[11;45H\033[?25l\033[6mWRONG CREDENTIALS TRY AGAIN %d\033[0m",status);
            press(&fake_key);
        }
    }
    else if(status==50) {
        printf("\033[0J\033[1;1H\033[?25l\033[0m");
        calander();
    }
    else if(status==51) {
        printf("\033[0J\033[1;1H\033[?25l\033[0m");
        break;
    }
    printf("\033[0J\033[1;1H\033[?25l\033[0m");

    //show cursor again
    }
    printf("\x1b[?25h");
    printf("\033[0J\033[1;1H\033[?25l\033[0m");
    restoreConsole();
    return 0;
}