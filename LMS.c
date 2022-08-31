#if 0
gcc -o ${0}.out $0 ./anisc.c -lm && ${0}.out
rm -f *.out
exit ${?}
#endif
// for easy compiling and execution
// can be ingnored if you want to compile and execute separately with gcc

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

// Author:Kiran Raj Dhakal
// Date:2022/7/11
// Description:A program to implemet Leave Management System in C
//             with file handling and error handling
//             The program is used to add,delete,modify and display the leaves of the employee
//             with admin,manager and employee login

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "anisc.h"

#define title(k) printf("\033]0;%s\007", k)
#define ch printf("\033[?25l"); // hiding cursor
#define cs printf("\033[?25h"); // showing cursor
// checking for if the system is running in linux or windows and defining the includes accordingly
// and different functions are used for linux and windows

#ifdef __APPLE__
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#elif defined _WIN32 || defined _WIN64
void press(int *key)
{
    ch;
    fflush(stdin);
    *key = getchar();
    cs;
}
#else
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
void press(int *key)
{
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    /* referenced from https://stackoverflow.com/questions/8101079/making-stdin-non-blocking*/
    // to handel single key stroke at a time as linux/unix terminal won't allow by default due to canonical mode
    // stackoverflow code
    struct timeval tv;
    struct termios ttystate, ttysave;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttysave = ttystate;
    // turn off canonical mode and echo
    ttystate.c_lflag &= ~(ICANON | ECHO);
    // minimum of number input read.
    ttystate.c_cc[VMIN] = 1;
    // set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    // stackoverflow ends
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    //  key press to continue
    fflush(stdin);
    ch;
    *key = getchar();
    cs;
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // stackoverflow code
    // reverting back to canonical mode and echo to avoid terminal crashes after the program is complete
    // please don't press CTRL+C to exit the program as it will cause the terminal to crash
    ttystate.c_lflag |= ICANON | ECHO;
    // set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttysave);
    // stackoveflow ends
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
}
#endif

// referenced from  https://en.wikipedia.org/wiki/ANSI_escape_code
// title of the program using ANSI escape code

struct employe
{
    char id[100];
    char name[100];
    char dob[100];
    char phone[100];
    char email[100];
    char password[100];
    char type[100];
    int leaves;
    long int doj;
};

// struct leaves is used to store the leaves of the employee with
// the employee id and date is used to store the date of the leaves
struct leaves
{
    char id[50];
    int day[12][5][2];
    int pend;
};

struct holiday
{
    int day[12][5];
};

// search engine for the employee id
int search(char *id, struct employe *e)
{
    int i;
    FILE *fp;
    fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e->id, e->name, e->dob,
                  e->phone, e->email, e->password, e->type, &e->leaves, &e->doj) != EOF)
    {
        if (strcmp(id, e->id) == 0)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// function login() is used to login the user with the given credentials
// if the user is admin,manager or employee then it will return the user type
// from user.txt file
int login(char *username, char *password)
{
    FILE *fp;
    struct employe e;
    fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
        // print username and password from the file and given password and username are compared
        if (strcmp(username, e.name) == 0 && strcmp(password, e.password) == 0)
        {
            fclose(fp);
            printf("\033[10;50H\033[6;38;250;255;50;255mLOGIN SUCESSFULLY\033[0m\n");
            // convert the user type to lower case
            char *type = e.type;
            for (int i = 0; i < strlen(type); i++)
            {
                type[i] = tolower(type[i]);
            }
            strcpy(e.type, type);
            if (strcmp(e.type, "admin") == 0)
            {
                return 149;
            }
            else if (strcmp(e.type, "manager") == 0)
            {
                return 150;
            }
            else if (strcmp(e.type, "employee") == 0)
            {
                return 151;
            }
            else
            {
                return 152;
            }
        }
    }
    fclose(fp);
    return 148;
}

void calander_temp()
{
    // make a calander with the dates of the month
    // the calander is made with the help of the ansice code
    // referenced from  https://en.wikipedia.org/wiki/ANSI_escape_code
    printf("\033[0J");
    int count = 32;
    int placex = 10;
    int placey = 30;
    char *week[7] = {
        "SUN",
        "MON",
        "TUE",
        "WED",
        "THU",
        "FRI",
        "SAT"};
    int i, j;
    for (i = 0; i < 7; i++)
    {
        printf("\033[1;32m\033[%d;%dH%s", placex - 2, placey + i * 5, week[i]);
        if (i == 6)
        {
            printf("\033[38;2;220;255;20m\033[48;2;255;30;30m\033[%d;%dH%s  \x1b[0m", placex - 2, placey + i * 5, week[i]);
        }
        // placey+=10;
    }

    printf("\033[%d;%dH", placex, placey);
    for (int i = 1; i < count; i++)
    {
        if (i % 7 == 0)
        {
            placex++;
            printf("\033[38;2;220;255;20m\033[48;2;255;30;30m%2d   \033[0m", i);
            printf("\033[%d;%dH", placex, placey);
        }
        else
        {
            printf("\033[1;32m%2d   \033[0m", i);
        }
    }
    // move the cursor to the bottom left corner
    printf("\033[%d;%dH", placex + 10, placey + 5);
    printf("Press Any Key To Continue");
    press(&count);
}

// function add_employee() is used to add the employee details to the user.txt file
// with struct employee as parameter
void add_employee(struct employe b)
{
    FILE *fp;
    struct employe e;
    int k;
    fp = fopen("user.txt", "r+");
    if (fp == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }
    // check if the employee id is already present in the file
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
        // convert e.id to char and compare with b.id
        if (strcmp(e.id, b.id) == 0)
        {
            printf("Employee id already present\n");
            fclose(fp);
            return;
        }
    }
    // generate time epoch time in milliseconds
    long int time_epoch = time(NULL);
    b.doj = time_epoch;
    fprintf(fp, "%s %s %s %s %s %s %s %d %ld\n", b.id, b.name, b.dob, b.phone, b.email, b.password, b.type, b.leaves, b.doj);
    fclose(fp);
    printf("Employee added successfully\n");
    printf("Your employee id is %s\n", b.id);
    printf("Press Any Key To Continue");
    press(&k);
}

// function delete_employee() is used to delete the employee details from the user.txt file
// with struct employee as parameter
void delete_employee(char *id)
{
    FILE *fp;
    FILE *fp1;
    struct employe e;
    fp = fopen("user.txt", "r");
    fp1 = fopen("temp.txt", "w");
    if (fp == NULL || fp1 == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
        if (strcmp(e.id, id) != 0)
        {
            fprintf(fp1, "%s %s %s %s %s %s %s  %d %ld\n", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, e.leaves, e.doj);
        }
    }
    fclose(fp);
    fclose(fp1);
    remove("user.txt");
    rename("temp.txt", "user.txt");
}

// function modify_employee() is used to modify the employee details from the user.txt file
// with struct employee as parameter
void modify_employee(struct employe b)
{
    FILE *fp;
    FILE *fp1;
    struct employe e;
    int k;
    fp = fopen("user.txt", "r");
    fp1 = fopen("temp.txt", "w");
    if (fp == NULL || fp1 == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }
    // exited with stack smashing error solve it by changing the file pointer to the start of the file
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
        if (strcmp(e.id, b.id) == 0)
        {
            fprintf(fp1, "%s %s %s %s %s %s %s  %d %ld\n", b.id, b.name, b.dob, b.phone, b.email, b.password, b.type, b.leaves, e.doj);
        }
        else
        {
            fprintf(fp1, "%s %s %s %s %s %s %s  %d %ld\n", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, e.leaves, e.doj);
        }
    }

    fclose(fp1);
    fclose(fp);
    remove("user.txt");
    rename("temp.txt", "user.txt");
}

// function display_employee() is used to display the employee details from the user.txt file
// with struct employee as parameter
void display_employee(struct employe b)
{
    title("Employee Details(FILTRED)");
    int i = 0;
    FILE *fp;
    struct employe e;
    char doj[50];
    int count = 0;
    int key;
    char *title[] = {
        "ID",
        "Name",
        "DOB",
        "Phone",
        "Email",
        "Type",
        "Leaves",
        "DOJ"};
    fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }
    printf("\033[0J\033[1;1H\033[0m");
    printf("\033[%d;%dH", 3, 5);
    for (int j = 0; j < 8; j++)
    {
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s\t", 3, 16 * j, 150, 255, j * 31, title[j]);
    }
    i = 2;
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
        if (
            strcmp(e.id, b.id) == 0 || strcmp(e.name, b.name) == 0 ||
            strcmp(e.dob, b.dob) == 0 || strcmp(e.phone, b.phone) == 0 ||
            strcmp(e.email, b.email) == 0 ||
            strcmp(e.type, b.type) == 0 ||
            e.doj == b.doj)
        {
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
            i++;
            count++;
            if (count >= 10)
            {
                printf("\033[%d;%dH%s", 26, 5, "Press S to continue or any other key to exit");
                press(&key);
                if (key == 's' || key == 'S')
                {
                    count = 0; //
                    printf("\033[%d;%dH%s", 26, 5, "                                            ");
                    continue;
                }
                else
                {
                    printf("\033[%d;%dH%s", 26, 5, "                                            ");
                    break;
                }
            }
        }
    }
    printf("Press any key to continue...");
    press(&i);
    fclose(fp);
}

// To display all the employees details from the user.txt file

void display_employees()
{
    title("All Employee Details");
    FILE *fp;
    struct employe e;
    int i = 0;
    int key;
    char doj[50];
    int count = 0;
    char *title[] = {
        "ID",
        "Name",
        "DOB",
        "Phone",
        "Email",
        "Type",
        "Leaves",
        "DOJ"};
    fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }
    printf("\033[0J\033[1;1H\033[0m");
    printf("\033[%d;%dH", 3, 5);
    for (int j = 0; j < 8; j++)
    {
        printf("\033[%d;%dH\033[38;2;%d;%d;%dm%s\t", 3, 16 * j, 150, 255, j * 31, title[j]);
    }
    printf("\033[%d;%dH", 4, 5);
    i = 2;
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
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
        i++;
        count++;
        if (count >= 10)
        {
            printf("\033[%d;%dH%s", 26, 5, "Press S to continue or any other key to exit");
            press(&key);
            if (key == 's' || key == 'S')
            {
                count = 0; //
                printf("\033[%d;%dH%s", 26, 5, "                                            ");
                continue;
            }
            else
            {
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

// function to convert time epoch to date
char *epoch_to_date(int epoch)
{
    char *date;
    time_t t = epoch;
    date = ctime(&t);
    return date;
}
/*
// display employee details and leaves of the employee with the employee id
void display_employee_leaves(char *id)
{
    title("Employee Leaves");
    FILE *fp;
    FILE *fp1;
    struct employe e;
    struct leaves l;
    char doj[50];
    fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
        if (strcmp(e.id, id) == 0)
        {
            time_t t = e.doj;
            strcpy(doj, ctime(&t));
            printf("%s %s %s %s %s %s %s  %d %s\n", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, e.leaves, doj);
            printf("Leaves\n");
            fp1 = fopen("leaves.txt", "r");
            if (fp == NULL)
            {
                printf("Error in opening file\n");
                exit(0);
            }
            while (fscanf(fp1, "%s %ld", l.id, &l.date) != EOF)
            {
                if (strcmp(e.id, l.id) == 0)
                {
                    // convert epoch to date
                    char *date = epoch_to_date(l.date);
                    printf("%s %s\n", l.id, date);
                }
            }
            fclose(fp1);
        }
    }
    fclose(fp);
}

void display_leaves()
{
}
*/
void gen_id(struct employe *b)
{
    // take current time in epoch format and attach employye name to it
    char str[50];
    char pseudo[2];
    long int time_epoch = time(NULL);
    sprintf(str, "%ld", time_epoch);   // converting epoch to string
    sprintf(pseudo, "%c", b->name[0]); // converting first character of name to string
    strcat(str, pseudo);               // concatenating first character of name to epoch string to generate near unique id
    printf("%s\n", str);
    strcpy(b->id, str);
}

void login_screen(char *n, char *p)
{
    // deginse of a login in screen with the name and password as parameters
    // using ansi escape sequence
    title("LOGIN SCREEN");
    cs;
    printf("\033[0J\033[1;1H\033[0m");
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
    ch;
    // printf("\033[0J\033[1;1H\033[0m");
}

// delet employee screen

void delete_employee_screen()
{
    title("EMPLOYEE DELET SCREEN");
    struct employe e;
    char id[50], doj[50];
    int st, key;
    while (1)
    {
        printf("\033[0J\033[1;1H\033[0m");
        printf("\033[9;50H\033[38;2;150;255;0mDelet Employee\033[0m\n");
        printf("\033[12;40H\033[38;2;150;255;31mEnter ID:\033[0m\n");
        printf("\033[12;56H\033[38;2;50;255;255m");
        scanf("%s", id);
        fflush(stdin);
        press(&key);
        printf("\033[0m\n");
        st = search(id, &e);
        if (st == 1)
        {
            printf("\033[0J\033[1;1H\033[0m");
            break;
        }
        else
        {
            printf("\033[8;50H\033[38;2;150;255;0mEmployee with id %s not found Try Again\033[0m\n", id);
            printf("\033[9;50H\033[38;2;150;255;0mPRESS (Y or 1) to Try Again OR other to exit\033[0m\n");
            press(&key);
            if (key != 'y' && key != 'Y' && key != '1')
                return;
        }
    }
    printf("\033[0J\033[1J\033[1;1H\033[0m");
    printf("\033[9;50H\033[38;2;150;255;0mThis informations will be erased forever and can't be recovered.\033[0m\n");
    printf("\033[10;10H\033[38;2;180;255;0mID: %s\033[0m\n", e.id);
    printf("\033[10;65H\033[38;2;220;255;0mJoined AT %s\033[0m\n", doj);
    printf("\033[12;40H\033[38;2;150;255;31m1|CHANGE NAME:    %s\033[0m\n", e.name);
    printf("\033[13;40H\033[38;2;150;255;62m2|CHANGE DOB:     %s\033[0m\n", e.dob);
    printf("\033[14;40H\033[38;2;150;255;93m3|CHANGE PHONE:   %s\033[0m\n", e.phone);
    printf("\033[15;40H\033[38;2;150;255;124m4|CHANGE EMAIL:   %s\033[0m\n", e.email);
    printf("\033[16;40H\033[38;2;150;255;155m5|CHANGE PASSWORD:%s\033[0m\n", e.password);
    printf("\033[17;40H\033[38;2;150;255;181m6|CHANGE TYPE:    %s\033[0m\n", e.type);
    printf("\033[20;40H\033[38;2;250;255;111mPress S to Continue Deletaion\033[0m\n");
    press(&key);
    if (key == 'S' || key == 's')
    {
        delete_employee(e.id);
    }
    else
    {
        return;
    }
    return;
}

// displya employee selectivly

void display_employee_screen()
{
    title("EMPLOYEE ADD SCREEN");
    // initialize empty employee struct
    struct employe e;
    // clear all the values of the employee struct
    // just clear out the memory this is is quickest
    memset(&e, 0, sizeof(e));
    int key;
    printf("\033[0J\033[1;1H\033[0m");
    printf("\033[9;50H\033[38;2;150;255;0mSearch By\033[0m\n");
    printf("\033[12;40H\033[38;2;150;255;31m1|NAME:\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;62m2|DOB:\033[0m\n");
    printf("\033[14;40H\033[38;2;150;255;93m3|PHONE:\033[0m\n");
    printf("\033[15;40H\033[38;2;150;255;124m4|EMAIL:\033[0m\n");
    printf("\033[16;40H\033[38;2;150;255;155m5|DOJ in epochs:\033[0m\n");
    printf("\033[17;40H\033[38;2;150;255;181m6|TYPE:\033[0m\n");
    printf("\033[12;56H\033[38;2;50;255;255m");
    press(&key);
    if (key == 49)
    {
        printf("\033[0m\n");
        printf("\033[12;56H\033[38;2;150;255;255m");
        scanf("%s", e.name);
        fflush(stdin);
        printf("\033[0m\n");
    }
    else if (key == 50)
    {
        printf("\033[0m\n");
        printf("\033[13;56H\033[38;2;150;255;255m");
        scanf("%s", e.dob);
        fflush(stdin);
        printf("\033[0m\n");
    }
    else if (key == 51)
    {
        printf("\033[0m\n");
        printf("\033[14;56H\033[38;2;150;255;255m");
        scanf("%s", e.phone);
        fflush(stdin);
        printf("\033[0m\n");
    }
    else if (key == 52)
    {
        printf("\033[0m\n");
        printf("\033[15;56H\033[38;2;150;255;255m");
        scanf("%s", e.email);
        fflush(stdin);
        printf("\033[0m\n");
    }
    else if (key == 53)
    {
        printf("\033[0m\n");
        printf("\033[16;56H\033[38;2;150;255;255m");
        scanf("%ld", &e.doj);
        fflush(stdin);
        printf("\033[0m\n");
    }
    else if (key == 54)
    {
        printf("\033[0m\n");
        printf("\033[17;56H\033[38;2;150;255;255m");
        scanf("%s", e.type);
        fflush(stdin);
        printf("\033[0m\n");
    }
    ch;
    printf("\033[0J\033[1;1H\033[0m");
    display_employee(e);
}

// employee adding Screen
void employee_add_Screen()
{
    title("EMPLOYEE ADD SCREEN");
    struct employe e;
    /*char *date;
    time_t t = time(NULL);
    date = ctime(&t);
    strcpy(e.doj,date);*/
    long int time_epoch = time(NULL);
    e.doj = time_epoch;
    e.leaves = 5;
    cs;
    printf("\033[0J\033[1;1H\033[0m");
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
    ch;
    gen_id(&e);
    printf("%s", e.id);
    printf("\033[0J\033[1;1H\033[0m");
    add_employee(e);
}

// calander
void calander_edit(int sudo,char *id)
{
    title("CALANDER");
    FILE *fp, *fp1;
    char c[20];
    int placex = 10;
    int placey = 30;
    long int month = 0;
    int start_day = 1, tick = 0;
    int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // declare all the variables
    int current_day = 0, current_month = 0, current_year = 0, current_week = 0, current_weekday = 0, key = 0;
    struct holiday h;
    struct leaves l, l1;
    struct employe e;
    
    time_t current_date;
    char *week[7] = {
        "SUN",
        "MON",
        "TUE",
        "WED",
        "THU",
        "FRI",
        "SAT"};
    char *holidays[60] = {
        "Casual ",
        "Annual",
        "Sick ",
        "Breverity ",
        "Maternity",
        "Paternity",
        "Marriage ",
        "Holiday",
    };
    int colors[9][3] = {
        {254, 125, 67},
        {139, 255, 92},
        {254, 222, 92},
        {255, 244, 44},
        {255, 38, 203},
        {255, 50, 203},
        {179, 36, 144},
        {255, 30, 30},
    };
// calculate the first weekday of the month
change:
    current_date = time(NULL) + (month);
    current_day = localtime(&current_date)->tm_mday;
    placex = 10;
    placey = 30;
    current_date = time(NULL) - (86400 * (current_day - 1)) + (month);
    current_month = localtime(&current_date)->tm_mon + 1;
    current_year = localtime(&current_date)->tm_year + 1900;
    current_day = localtime(&current_date)->tm_mday;
    current_weekday = localtime(&current_date)->tm_wday;
    current_week = localtime(&current_date)->tm_yday / 7;
    start_day = localtime(&current_date)->tm_wday;
    printf("\033[1J");

    // make calander of current month in new year.txt file
    sprintf(c, "%d", current_year);
    strcat(c, ".txt");
    fp = fopen(c, "r");
    memset(&h, 0, sizeof(h));
    //--------------------------------------------
    for (int i = 0; i < 7; i++)
    {
        printf("\033[1;32m\033[%d;%dH%s", placex - 2, placey + i * 5, week[i]);
        if (i == 6)
        {
            printf("\033[38;2;220;255;20m\033[48;2;255;30;30m\033[%d;%dH%s  \x1b[0m", placex - 2, placey + i * 5, week[i]);
        }
        // placey+=10;
    }
    printf("\033[%d;%dHHOLIDAY MENU\033[0m", placex - 7, placey + 5);
    for (int i = 0; i < 8; i++)
    {
        printf("\033[%d;%dH \033[38;2;%d;%d;%dm%s\033[0m", placex - 6, (placey - 20) + i * 15, colors[i][0], colors[i][1], colors[i][2], holidays[i]);
    }
    printf("\033[38;2;25;150;255m\033[%d;%dH YEAR:- %d                    MONTH:- %02d", placex - 4, placey, current_year, current_month);
    printf("\033[%d;%dH", placex, placey);
    //--------------------------------------------
    fread(&h, sizeof(h), 1, fp);
    fclose(fp);
    memset(&l, 0, sizeof(l));
    memset(&l1, 0, sizeof(l1));
    fp = fopen("leave.txt", "r");
    fp1 = fopen("temp.txt", "w");
    while(fread(&l, sizeof(l), 1, fp))
    {
        if(strcmp(l.id, e.id) == 0)
        {
            l1 = l;
        }
        fwrite(&l, sizeof(l), 1, fp1);
    }
    fclose(fp);
    fclose(fp1);
    remove("leave.txt");
    rename("temp.txt", "leave.txt");

    tick = start_day;
    for (int i = 1; i <= month_days[current_month]; i++)
    {
        if (tick % 7 == 0)
        {
            if (h.day[current_month][0] == i || h.day[current_month][1] == i || h.day[current_month][2] == i || h.day[current_month][3] == i || h.day[current_month][4] == i)
            {
                placex++;
                tick = 0;
                printf("\033[38;2;255;30;30m\033[%d;%dH%d \x1b[0m", placex, placey + 5 * tick, i);
            }
            else
            {
                placex++;
                tick = 0;
                printf("\033[38;2;255;255;255m\033[%d;%dH%d \x1b[0m", placex, placey + 5 * tick, i);
            }
        }
        else if (h.day[current_month][0] == i || h.day[current_month][1] == i || h.day[current_month][2] == i || h.day[current_month][3] == i || h.day[current_month][4] == i)
        {
            printf("\033[38;2;255;30;30m\033[%d;%dH%d \x1b[0m", placex, placey + 5 * tick, i);
        }
        else if (tick == 6)
        {
            printf("\033[38;2;255;30;30m\033[%d;%dH%d \x1b[0m", placex, placey + 5 * tick, i);
        }
        else
        {
            printf("\033[38;2;255;255;255m\033[%d;%dH%d \x1b[0m", placex, placey + 5 * tick, i);
        }
        //coloring the holidays taken by employee
        for(int p=0;p<5;p++)
        {
            if(l1.day[current_month][p][0]==i && l1.pend==1)
            {
                printf("\033[48;2;150;150;150m\033[38;2;10;10;10m\033[%d;%dH%d \x1b[0m", placex, placey + 5 * tick, i);
            }
            else if(l1.day[current_month][p][0]==i && l1.pend==-1)
            {
                printf("\033[38;2;255;255;255m\033[%d;%dH%d \x1b[0m", placex, placey + 5 * tick, i);
                l1.day[current_month][p][1]=0;
                l1.day[current_month][p][0]=0;
                l1.pend=0;
            }
            else if(l1.day[current_month][p][0]==i && l1.pend==0)
            {
                printf("\033[38;2;%d;%d;%dm\033[%d;%dH%d \x1b[0m",colors[l1.day[current_month][p][1]][0],colors[l1.day[current_month][p][1]][0]
                ,colors[l1.day[current_month][p][1]][0] ,placex, placey + 5 * tick, i);
            }
            
        }
        tick++;
    }
    press(&key);
    strcpy(l1.id,id);
    fp1=fopen("leave.txt","a");
    fwrite(&l1,sizeof(l1),1,fp1);
    fclose(fp1);
    if (key == 'p')
    {
        month -= 86400 * month_days[current_month - 1];
        goto change;
    }
    else if (key == 'n')
    {
        month += 86400 * month_days[current_month + 1];
        goto change;
    }
    else if (key == 'a' && sudo==1 )
    {
        fp = fopen(c, "w");
        printf("\033[1;32m\033[%d;%dHEnter the day:-  ", placex+7, placey + 5 * tick);
        // count values of h
        int count = 0;
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (h.day[i][j] > 0 && h.day[i][j] < 31)
                {
                    count++;
                }
            }
        }
        scanf("%d", &h.day[current_month][count % 5]);
        printf("\033[1;32m\033[%d;%dH                                ", placex+7, placey + 4 * tick);
        h.day[current_month][current_day] = h.day[current_month][current_day] % 31;
        fwrite(&h, sizeof(h), 1, fp);
        fclose(fp);
        goto change;
    }
    else if (key == 'd'  && sudo==1 )
    {
        fp = fopen(c, "w");
        printf("\033[1;32m\033[%d;%dHEnter the day:- ", placex+ 7, placey + 5 * tick);
        // count values of h
        int day = 0;
        scanf("%d", &day);
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (h.day[i][j] == day)
                {
                    h.day[i][j] = 0;
                }
            }
        }
        printf("\033[1;32m\033[%d;%dH                                ", placex+7, placey + 4 * tick);
        fwrite(&h, sizeof(h), 1, fp);
        fclose(fp);
        goto change;
    }
    else if (key=='l')
    {
        fp = fopen("leave", "a");
        printf("\033[1;32m\033[%d;%dHEnter the day:- ", placex+ 7, placey + 5 * tick);
        // count values of h
        int day = 0,type=0;
        scanf("%d", &day);
        printf("\033[1;32m\033[%d;%dHEnter the Type(1->Casual;2->Anal.... ):- ", placex+ 7, placey + 5 * tick);
        scanf("%d", &type);

        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if(e.leaves<=5){
                    l1.day[current_month][e.leaves][0]=day;
                    l1.day[current_month][e.leaves][1]=type;
                    e.leaves++;
                    l1.pend=1;
                }
                else
                {
                    printf("\033[1;32m\033[%d;%dHYou have already taken 5 leaves", placex+ 7, placey + 5 * tick);
                    goto change;
                }
            }
        }
          strcpy(l1.id,id);
        printf("\033[1;32m\033[%d;%dH                                ", placex+7, placey + 4 * tick);
        fwrite(&l1, sizeof(l1), 1, fp);
        fclose(fp);

        goto change;
    }
    else if (key == 'q')
    {
        return;
    }
    else
    {
        goto change;
    }
}

// employee modifying Screen
int modify_employee_screen()
{
    title("EMPLOYEE ADD SCREEN");
    struct employe e;
    char id[50], doj[50];
    int st, key;
    while (1)
    {
        printf("\033[0J\033[1;1H\033[0m");
        printf("\033[9;50H\033[38;2;150;255;0mModify Employee\033[0m\n");
        printf("\033[12;40H\033[38;2;150;255;31mEnter ID:\033[0m\n");
        printf("\033[12;56H\033[38;2;50;255;255m");
        cs;
        scanf("%s", id);
        ch;
        fflush(stdin);
        press(&key);
        printf("\033[0m\n");
        st = search(id, &e);
        if (st == 1)
        {
            printf("\033[0J\033[1;1H\033[0m");
            break;
        }
        else
        {
            printf("\033[8;50H\033[38;2;150;255;0mEmployee with id %s not found Try Again\033[0m\n", id);
            printf("\033[9;50H\033[38;2;150;255;0mPRESS (Y or 1) to Try Again OR other to exit\033[0m\n");
            press(&key);
            if (key != 'y' && key != 'Y' && key != '1')
                return 0;
        }
    }
    while (1)
    {
        printf("\033[0J\033[1;1H\033[0m");
        time_t t = e.doj;
        strcpy(doj, ctime(&t));
        printf("\033[9;50H\033[38;2;150;255;0mModifying Employee \033[0m\n");
        printf("\033[10;10H\033[38;2;180;255;0mID: %s\033[0m\n", e.id);
        printf("\033[10;65H\033[38;2;220;255;0mJoined AT %s\033[0m\n", doj);
        printf("\033[12;40H\033[38;2;150;255;31m1|CHANGE NAME:    %s\033[0m\n", e.name);
        printf("\033[13;40H\033[38;2;150;255;62m2|CHANGE DOB:     %s\033[0m\n", e.dob);
        printf("\033[14;40H\033[38;2;150;255;93m3|CHANGE PHONE:   %s\033[0m\n", e.phone);
        printf("\033[15;40H\033[38;2;150;255;124m4|CHANGE EMAIL:   %s\033[0m\n", e.email);
        printf("\033[16;40H\033[38;2;150;255;155m5|CHANGE PASSWORD:%s\033[0m\n", e.password);
        printf("\033[17;40H\033[38;2;150;255;181m6|CHANGE TYPE:    %s\033[0m\n", e.type);
        printf("\033[20;40H\033[38;2;250;255;111mPress S to Save and C To Cancel\033[0m\n");
        press(&key);
        cs;
        if (key == 49)
        {
            printf("\033[0m\n");
            printf("\033[12;58H\033[38;2;150;255;255m                                                 ");
            printf("\033[12;58H\033[38;2;150;255;255m");
            scanf("%s", e.name);
            fflush(stdin);
            printf("\033[0m\n");
        }
        else if (key == 50)
        {
            printf("\033[0m\n");
            printf("\033[13;58H\033[38;2;150;255;255m                                                 ");
            printf("\033[13;58H\033[38;2;150;255;255m");
            scanf("%s", e.dob);
            fflush(stdin);
            printf("\033[0m\n");
        }
        else if (key == 51)
        {
            printf("\033[0m\n");
            printf("\033[14;58H\033[38;2;150;255;255m                                                 ");
            printf("\033[14;58H\033[38;2;150;255;255m");
            scanf("%s", e.phone);
            fflush(stdin);
            printf("\033[0m\n");
        }
        else if (key == 52)
        {
            printf("\033[0m\n");
            printf("\033[15;58H\033[38;2;150;255;255m                                                 ");
            printf("\033[15;58H\033[38;2;150;255;255m");
            scanf("%s", e.email);
            fflush(stdin);
            printf("\033[0m\n");
        }
        else if (key == 53)
        {
            printf("\033[0m\n");
            printf("\033[16;58H\033[38;2;150;255;255m                                                 ");
            printf("\033[16;58H\033[38;2;150;255;255m");
            scanf("%s", e.password);
            fflush(stdin);
            printf("\033[0m\n");
        }
        else if (key == 54)
        {
            printf("\033[0m\n");
            printf("\033[17;58H\033[38;2;150;255;255m                                                 ");
            printf("\033[17;58H\033[38;2;150;255;255m");
            scanf("%s", e.type);
            fflush(stdin);
            printf("\033[0m\n");
        }
        else if (key == 's')
        {
            printf("\033[0m\n");
            printf("\033[20;40H\033[38;2;150;255;255m                                                                                 ");
            printf("\033[20;58H\033[38;2;150;255;255mCONFIRMING CHANGES(Y/N)\033[0m");
            press(&key);
            if (key == 'y')
            {
                modify_employee(e);

                break;
                ;
            }
            else if (key == 'n')
            {
                continue;
            }
        }
        else if (key == 'c' || key == 'C')
        {
            printf("EXITING THE MODIFICATION UNSAVED");
            press(&key);
            break;
        }
        ch;
    }
    return 0;
}

void admin_screen()
{
    title("ADMIN SCREEN");
    printf("\033[0J\033[1;1H\033[0m");
    printf("\033[10;50H\033[38;2;150;255;0mAdmin Screen\033[0m\n");
    printf("\033[12;40H\033[38;2;150;255;31m1|Add Employee\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;62m2|View Employee\033[0m\n");
    printf("\033[14;40H\033[38;2;150;255;93m3|View All Employee\033[0m\n");
    printf("\033[15;40H\033[38;2;150;255;124m4|Delete Employee\033[0m\n");
    printf("\033[16;40H\033[38;2;150;255;124m5|Modify Employee\033[0m\n");
    printf("\033[17;40H\033[38;2;150;255;155m6|Calander\033[0m\n");
    printf("\033[20;40H\033[38;2;150;255;186mANY OTHER KEY TO EXIT\033[0m\n");
    printf("\033[0J");
}

void manager_screen()
{
    title("MANAGER SCREEN");
    printf("\033[0J\033[1;1H\033[0m");
    printf("\033[10;50H\033[38;2;150;255;0mManager Screen\033[0m\n");
    printf("\033[12;40H\033[38;2;150;255;31m1|Create Calander\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;62m2|Edit Calander\033[0m\n");
    printf("\033[14;40H\033[38;2;150;255;93m3|Apply For Leave\033[0m\n");
    printf("\033[15;40H\033[38;2;150;255;124m4|Leaves Pending\033[0m\n");
    printf("\033[16;40H\033[38;2;150;255;124m5|View Calander\033[0m\n");
    printf("\033[20;40H\033[38;2;150;255;186mANY OTHER KEY TO EXIT\033[0m\n");
    printf("\033[0J");
}

void menu1()
{
    title("Welcome To LMS by Kiran");
    printf("\033[1J\033[0J\033[1;1H\033[0m");
    printf("\033[10;50H\033[38;2;150;255;0mWelcome to Employee Management System\033[0m\n");
    printf("\033[11;40H\033[38;2;150;255;31mPress 1 to LOGIN\033[0m\n");
    printf("\033[12;40H\033[38;2;150;255;155mPress 2 to DISPLAY CALANDER\033[0m\n");
    printf("\033[13;40H\033[38;2;150;255;181mPress 3 to EXIT\033[0m\n");
    printf("\033[12;56H\033[38;2;50;255;255m");
}

int main()
{
    setupConsole();
    // hide cursor
    calander_edit(1,"1657632109a");
   restoreConsole();
exit(0);
    ch;
    printf("\x1b[?25l");
    char name[50];
    char password[50];
    int key1, key2, key3, key4, key5, key6, test;
    press(&key6);

    // statically declare admin employee details
    // struct employe admin={"admin!","admin","2022/1/1","+9779800000000","admin@admin.com","696969","admin",10,10};
    while (1)
    {
        printf("\033[1J\033[0J\033[1;1H\033[0m");
        menu1();
        press(&key1);
        if (key1 == 49)
        {
        login:
            printf("\033[0J\033[1;1H\033[0m");
            login_screen(name, password);
            getchar();
            key5 = login(name, password);
            if (key5 != 148)
            {
                printf("\033[11;50H\033[38;2;255;50;255mDO YOU WANT TO save Credentials(1/0)");
                press(&key2);
                printf("\033[11;50H\033[38;2;255;50;255m                                         ");
            }
            // if admin
            if (key5 == 149)
            {
            Asaved:
                printf("\033[0J\033[1;1H\033[0m");
                admin_screen();
                press(&key3);
                if (key3 == 49)
                {
                    // clear screen
                    printf("\033[0J\033[1;1H\033[0m");
                    employee_add_Screen();
                }
                else if (key3 == 50)
                {
                    printf("\033[0J\033[1;1H\033[0m");
                    display_employee_screen();
                }
                else if (key3 == 51)
                {
                    printf("\033[0J\033[1;1H\033[0m");
                    display_employees();
                }
                else if (key3 == 52)
                {
                    printf("\033[0J\033[1;1H\033[0m");
                    delete_employee_screen();
                }
                else if (key3 == 53)
                {
                    printf("\033[0J\033[1;1H\033[0m");
                    test = modify_employee_screen();
                }
                else if (key3 == 54)
                {
                    printf("\033[0J\033[1;1H\033[0m");
                    //calander();
                }
                else
                {
                    key2 = 0;
                    continue;
                }
            }
            // if manager
            else if (key5 == 150)
            {
            Msaved:
                printf("\033[0J\033[1;1H\033[0m");
                // employee_add_Screen();
                manager_screen();
                press(&key3);
            }
            // if employee
            else if (key5 == 151)
            {
            Esaved:
                printf("\033[0J\033[1;1H\033[0m");
                display_employees();
            }
            else
            {
                printf("\033[11;45H\033[6mWRONG CREDENTIALS TRY AGAIN %d\033[0m", key5);
                press(&key4);
                goto login;
            }
        }
        else if (key1 == 50)
        {
            printf("\033[0J\033[1;1H\033[0m");
            //calander();
        }
        else if (key1 == 51)
        {
            printf("\033[0J\033[1;1H\033[0m");
            break;
        }
        printf("\033[0J\033[1;1H\033[0m");
        if (key2 == 49 && key5 == 149)
        {
            goto Asaved;
        }
        else if (key2 == 49 && key5 == 150)
        {
            goto Msaved;
        }
        else if (key2 == 49 && key5 == 151)
        {
            goto Esaved;
        }
        else
        {
            // press(&key4);
            continue;
        }
    }
    cs;
    printf("\x1b[?25h");
    printf("\033[0J\033[1;1H\033[0m");
    restoreConsole();
    return 0;
}
