#if 0
gcc -o ${0}.out $0 -lm && ${0}.out
rm -f ${0}.out
exit ${?}
#endif
//for easy compiling and execution 
//can be ingnored if you want to compile and execute separately with gcc


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


struct employe{
    char id[20];
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
    char id[20];
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
        printf("%s %s %s %s\n",e.name,e.password,username,password);
        printf("%d\n",strcmp(e.name,username));
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
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %s",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        //convert e.id to char and compare with b.id

        if(strcmp(e.id,b.id)==0){
            printf("Employee id already present\n");
            fclose(fp);
            return;
        }
    }
    //hash the password in md5 using openssl library and store it in e.password
    fprintf(fp,"%s %s %s %s %s %s %s %d %s\n",b.id,b.name,b.dob,b.phone,b.email,b.password,b.type,b.leaves,b.doj);
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
            fprintf(fp1,"%s %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
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
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %s",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(strcmp(e.id,b.id)==0){
            fprintf(fp1,"%s %s %s %s %s %s %s  %d %s\n",b.id,b.name,b.dob,b.phone,b.email,b.password,b.type,b.leaves,b.doj);
        }
        else if(strcmp(e.id,b.id)!=0){
            fprintf(fp1,"%s %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
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
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %s",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
       if(strcmp(e.id,b.id)==0){
            printf("%s %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
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
    fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Error in opening file\n");
        exit(0);
    }
    while(fscanf(fp,"%s %s %s %s %s %s %s %d %s",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,&e.leaves,e.doj)!=EOF){
        if(strcmp(e.id,id)==0){
            printf("%s %s %s %s %s %s %s  %d %s\n",e.id,e.name,e.dob,e.phone,e.email,e.password,e.type,e.leaves,e.doj);
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


int main(){
    //statically declare admin employee details 
    //struct employe admin={"admin!","admin","2022/1/1","+9779800000000","admin@admin.com","696969","admin",10,"2022"};
    //gen_id(&admin);
    //printf("%s\n",admin.id);
    //add_employee(admin);
    //delete_employee(e);
    //modify_employee(e);
    //display_employee(e);
    return 0;
}