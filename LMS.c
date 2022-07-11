//for easy compiling and execution 
#if 0
gcc -o ${0}.out $0 -lm && ${0}.out
rm -f ${0}.out
exit ${?}
#endif
//can be ingnored if you want to compile and execute separately with gcc


//Author:Kiran Raj Dhakal
//Date:2022/7/11
//Description:A program to implemet Leave Management System in C 
//            with file handling and error handling
//            The program is used to add,delete,modify and display the leaves of the employee
//            with admin,manager and employee login

//take user password and md5 hash it and store it in a file
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
//include for md5 hash
#include<openssl/md5.h>


struct employe{
    int id;
    char name[20];
    char dob[20];
    char phone[20];
    char email[20];
    char password[20];
    char type[20];
    int leaves;
    char doj[20];
};

//struct leaves is used to store the leaves of the employee with 
//the employee id and date is used to store the date of the leaves
struct leaves{
    int id;
    long int date;
    
};


//function login() is used to login the user with the given credentials
//if the user is admin,manager or employee then it will return the user type
//from user.txt file
char *login(char *username,char *password){
    FILE *fp;
    struct employe e;
    fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%d %s %s %s %s %s %s %d %s",&e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(strcmp(username,e.email)==0 && strcmp(password,e.password)==0){
            fclose(fp);
            return "Logged";
        }
    }
    fclose(fp);
    return "None";
}

//function add_employee() is used to add the employee details to the user.txt file
//with struct employee as parameter
void add_employee(struct employe e){
    FILE *fp;
    fp=fopen("user.txt","a");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    //hash the password and store it in the file
    e.password=md5(e.password);
    fprintf(fp,"%d %s %s %s %s %s %s %d %s\n",e.id,e.name,e.dob,e.phone,e.email,*hash,e.type,e.leaves,e.doj);
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
    while(fscanf(fp,"%d %s %s %s %s %s %s %d %s",&e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(e.id!=b.id){
            fprintf(fp1,"%d %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
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
    while(fscanf(fp,"%d %s %s %s %s %s %s %d %s",&e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(e.id==b.id){
            fprintf(fp1,"%d %s %s %s %s %s %s  %d %s\n",b.id,b.name,b.dob,b.phone,b.email,b.password,b.type,b.leaves,b.doj);
        }
        else if(e.id!=b.id){
            fprintf(fp1,"%d %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
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
    fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%d %s %s %s %s %s %s %d %s",&e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(e.id==b.id){
            printf("%d %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
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
void display_employee_leaves(int id){
    FILE *fp;
    struct employe e;
    struct leaves l;
    fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%d %s %s %s %s %s %s %d %s",&e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(e.id==id){
            printf("%d %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
            printf("Leaves\n");
            fp=fopen("leaves.txt","r");
            if(fp==NULL){
                printf("Error in opening file\n");
                exit(0);
            }
            while(fscanf(fp,"%d %ld",&l.id,&l.date)!=EOF){
                if(l.id==id){
                    //convert epoch to date
                    char *date=epoch_to_date(l.date);
                    printf("%d %s\n",l.id,date);
            }
            fclose(fp);
        }
    }
    fclose(fp);
}
}

int main(){
    struct employe e={10,"liran","2022/1/1","+977980000","k@gmail.com","134564","admin",10,"2022"};
    add_employee(e);
    //delete_employee(e);
    //modify_employee(e);
    //display_employee(e);    
    printf("addeed employee sucesssfully\n");


    return 0;
}