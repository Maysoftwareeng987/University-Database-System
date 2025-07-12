#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// Chunk 1 - Declarations and Structures
struct Student {
    char FirstName[100] ;
    char LastName[100] ;
    int age ;
    char Gender[20];
    char EmailAddress[100];
    char Username[100];
    char Password[100];
    float CGPA;
    int attendance;
};
struct Teacher {
    char FirstName[100] ;
    char LastName[100] ;
    int age ;
    char Gender[20];
    char EmailAddress[100];
    char Username[100];
    char Password[100];
    char Subject[30];
};
struct Staff {
    char FirstName[100] ;
    char LastName[100] ;
    int age ;
    char Gender[20];
    char EmailAddress[100];
    char Username[100];
    char Password[100];
    char Department[30];
};
struct subjects
{
    char Subjects[50];
    float Percentage;
};
struct subjects sub[15];
struct Student students[100];
struct Teacher Teachers[100];
struct Staff Staffs[100];
// Global variables to keep track of user counts
int studentcount = 0,teachercount = 0,staffcount = 0;

void loadUserCounts();
void saveUserCounts();

char getUserType();
void getInput(char *prompt , char *buffer , int size);
int getValidAge();
void getGender(char *Gender);
void trim(char *str);
int duplicateUserCheck(char UserType, char UserEntryU[], char UserEntryP[]);
void clearscreen();

void login(char UserType);
void signup(char UserType);
void DisplayMainMenu();

void EditUserDetails(char usertype,char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],float CGPA,char Department[],char SUBJECTS[]);
void saveuserdetails(char usertype,char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],float CGPA,char Department[],char SUBJECTS[]);
void CGPACALCULATOR();
int confirmExitWithoutSaving();
char* getLetterGrade(float gpa) ;


void studentmenu(char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],float CGPA);
void staffmenu(char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],char Department[]);
void teachermenu(char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],char SUBJECTS[]);

// Chunk 2 - User Count Management
void loadUserCounts() {
    FILE *file;
    
    // Read student count
    file = fopen("stcount", "r");
    if (file) {
        fscanf(file, "%d", &studentcount);
        fclose(file);
    }
    
    // Read teacher count
    file = fopen("tcount", "r");
    if (file) {
        fscanf(file, "%d", &teachercount);
        fclose(file);
    }
    
    // Read staff count
    file = fopen("stfcount", "r");
    if (file) {
        fscanf(file, "%d", &staffcount);
        fclose(file);
    }
}

void saveUserCounts() {
    FILE *file;
    
    // Save student count
    file = fopen("stcount", "w");
    if (file) {
        fprintf(file, "%d", studentcount);
        fclose(file);
    }
    
    // Save teacher count
    file = fopen("tcount", "w");
    if (file) {
        fprintf(file, "%d", teachercount);
        fclose(file);
    }
    
    // Save staff count
    file = fopen("stfcount", "w");
    if (file) {
        fprintf(file, "%d", staffcount);
        fclose(file);
    }
}

// Chunk 3 - Utility Functions
// Removes trailing newline or space characters from input
void trim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == ' ')) {
        str[--len] = '\0';
    }
}
// Safely gets string input from user
void getInput(char *prompt , char *buffer , int size) {
    printf("%s",prompt);
    fgets(buffer,size,stdin);
    trim(buffer);
    return ;
}
// Safely gets integer input from user
int getValidAge() {
    int age;
    printf("Enter the Age: ");
    while (scanf("%d", &age) != 1 || age < 0) {
        printf("Invalid Age! Enter again: ");
        while (getchar() != '\n'); // Clear buffer
    }
    getchar();
    return age;
}
// Safely gets Gender input from user
void getGender(char *Gender) {
    int choice;
    while(1) {
    printf("What Is Your Gender : -\n(1) Male\n(2) Female\n----> ");
        if (scanf("%d",&choice) == 1 && (choice == 1 || choice == 2))
        {
        strcpy(Gender,(choice == 1) ? "Male" : "Female" );
        getchar();
        break;
        }
        printf("Error: Enter a Valid Gender (1 or 2)!!\n");
        while (getchar() != '\n'); // Clear input buffer
    }
}
// safely gets character input from user
char getUserType() {
    char userType;
    
    while (1) {
        printf("\nSelect User Type:\n");
        printf("(s) Student\n(t) Teacher\n(f) Staff\n----> ");
        scanf("%c",&userType);
        getchar(); // Clear buffer

        userType = tolower(userType);  // Convert to lowercase

        if (userType == 's' || userType == 't' || userType == 'f') {
            return userType;
        } 
        else {
            printf("Error: Invalid User Type! Please try again.\n");
        }
    }
}

int duplicateUserCheck(char UserType, char UserEntryU[], char UserEntryP[]) {
    char UN[100], UP[100];
    FILE *ptr = NULL;
    switch (UserType) {
        case 's' : ptr = fopen("student.txt","r") ; break;
        case 't' : ptr = fopen("teacher.txt","r"); break;
        case 'f' : ptr = fopen("staff.txt","r"); break;
        default:
            printf("Error: Invalid User Type!\n");
            return 0;
    }
    if(ptr == NULL) {
        printf("Error: Could not open file!\n");
    }
    char line[300];
    while (fgets(line, sizeof(line), ptr)) {
        switch(UserType) {
            case 's':
                sscanf(line, "%99[^|]|%99[^|]|", UN, UP);
                break;
            case 't':
                sscanf(line, "%99[^|]|%99[^|]|", UN, UP);
                break;
            case 'f':
                sscanf(line, "%99[^|]|%99[^|]|", UN, UP);
                break;
            default:
                printf("Error: Invalid User Type!\n");
                fclose(ptr);
                return 0;
        }
      if (strcmp(UserEntryU, UN) == 0) {
            fclose(ptr);
            return 1; // Duplicate found, return to caller
        }
        else {
            // If no match found, continue checking
            continue;
        }
    }
    fclose(ptr);
    return 0; // No duplicates fou, continue with signup
}

void clearscreen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[2J\033[1;1H");
    #endif
}

// chunk 4 - DisplayMainMenu, Login and Signup Functions
void DisplayMainMenu() {
    char choice;
    
    while (1) {
        printf("\n=================================================\nDo you want to Login or Signup or Exit ? (L/S/E): ");
        scanf("%c", &choice);
        getchar(); // Clear buffer
        
        // Convert uppercase to lowercase using ctype.h
        choice = tolower(choice) ;

        if (choice == 's') {
            char userType = getUserType();  // Get user type
            if (userType) {
                // Update user count based on user type
                if (userType == 's') { studentcount++; }
                else if (userType == 't') { teachercount++; }
                else if (userType == 'f') { staffcount++; }

                saveUserCounts(); // Save updated user counts after signup
                signup(userType);
            }
        } 
        else if (choice == 'l') {
            char userType = getUserType();
            if (userType) {
                login(userType);
            }
        } 
        else if (choice == 'e') {
            printf("Exiting the program...\n");
            saveUserCounts(); // Save user counts before exiting
            exit(0); // Exit the program
        }  
        else {
            printf("Invalid choice! Please enter L (Login), S (Signup), or E (Exit).\n");
            continue; // Prompt again for valid input
        }
    }
}

void saveuserdetails(char usertype,char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],float CGPA,char Department[],char SUBJECTS[]) {
    char UN[100], UP[100], FN1[100], LN1[100], GENDER[100], DEPARTMENT[100], SUBJECTS1[100];
    int age1;
    float cgpa;
    FILE *ptr = NULL;
    FILE *ptr2 = NULL;
    // Open the appropriate file based on user type
    ptr2 = fopen("temp.txt", "w");
    switch (usertype) {
        case 's': ptr = fopen("student.txt", "r"); break;
        case 't': ptr = fopen("teacher.txt", "r"); break;
        case 'f': ptr = fopen("staff.txt", "r"); break;
        default:
            printf("Invalid user type!\n");
            return;
    }

    if (ptr == NULL || ptr2 == NULL) {
        printf("Error: Could not open file!\n");
        return;
    }
    char line[300];
    int lcount = 0 ;
    while (fgets(line, sizeof(line), ptr)) {
        lcount++ ;
        switch (usertype)
            {
            case 's':
            sscanf(line, "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%d|%99[^|]|%f|%*s",
                UN, UP, FN1, LN1, &age1, GENDER, &cgpa);
                break;
            
            case 't':
            sscanf(line, "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%d|%99[^|]|%99[^|]|%*s",
                UN, UP, FN1, LN1, &age1, GENDER, SUBJECTS1);
                break;
            
            case 'f':
            sscanf(line, "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%d|%99[^|]|%99[^|]|%*s",
                UN, UP, FN1, LN1, &age1, GENDER, DEPARTMENT);
                break;
            default:
            printf("Error : Invalid Usertype! \n");
                break;
            }

        // Trim spaces if any
        UserEntryU[strcspn(UserEntryU, " ")] = 0;
        UserEntryP[strcspn(UserEntryP, " ")] = 0;

        if (strcmp(UserEntryU, UN) == 0) {
            switch (usertype)
            {
                
            case 's':
                fprintf(ptr2, "%s|%s|%s|%s|%d|%s|%.2f\n",
                UserEntryU,UserEntryP,FN,LN,age,Gender,CGPA);
                break;
            
            case 't':
                fprintf(ptr2, "%s|%s|%s|%s|%d|%s|%s\n",
                UserEntryU,UserEntryP,FN,LN,age,Gender,SUBJECTS);
                break;
            
            case 'f':
                fprintf(ptr2, "%s|%s|%s|%s|%d|%s|%s\n",
                UserEntryU,UserEntryP,FN,LN,age,Gender,Department);
                break;
            default:
            printf("Error : Invalid Usertype! \n");
                break;
            }
        }
        else
        {
            switch (usertype)
            {
            case 's':
            fprintf(ptr2, "%s|%s|%s|%s|%d|%s|%.2f\n",
                UserEntryU,UserEntryP,FN1,LN1,age1,GENDER,CGPA);
                break;
            
            case 't':
                fprintf(ptr2, "%s|%s|%s|%s|%d|%s|%s\n",
                UserEntryU,UserEntryP,FN1,LN1,age1,GENDER,SUBJECTS1);
                break;
            
            case 'f':
                fprintf(ptr2, "%s|%s|%s|%s|%d|%s|%s\n",
                UserEntryU,UserEntryP,FN1,LN1,age1,GENDER,DEPARTMENT);
                break;
            default:
            printf("Error : Invalid Usertype! \n");
                break;
            }
        }
        
    }

    fclose(ptr);
    fclose(ptr2);
    switch (usertype) {
        case 's': ptr = fopen("student.txt", "w"); break;
        case 't': ptr = fopen("teacher.txt", "w"); break;
        case 'f': ptr = fopen("staff.txt", "w"); break;
        default:
            printf("Invalid user type!\n");
            remove("temp.txt");
            return;
    }

    ptr2 = fopen("temp.txt", "r");
    char ch;
    while ((ch = fgetc(ptr2)) != EOF) {
        fputc(ch, ptr);  // copy each character
    }

    fclose(ptr2);
    fclose(ptr);
    // Delete the file after use
    remove("temp.txt");
    return;
}

void EditUserDetails(char Usertype,char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],float CGPA,char Department[],char SUBJECTS[]) {
    printf("\n\tLet's Edit Your Details(0^◡`0)\n");
    char Text[100];
    int choice;
    if (Usertype == 's')
    {
        while (1)
        {
            printf("\nChoose what You Want to Update !^_~\n");
            printf("\n(1) First Name\n(2) Last Name\n(3) Age\n(4) Gender\n(5) C.G.P.A.\n(6) Username\n(7) Password\n(8) Save The Changes\n---->");
            if (scanf("%d",&choice) == 1 && choice > 0 && choice < 9)
            {
                getchar();
                switch (choice)
                {
                case 1:
                    getInput("Enter New First Name : ",Text,100);
                    strcpy(FN,Text);
                    printf("First Name Updated! :-)");
                    break;
                
                case 2:
                    getInput("Enter New Last Name : ",Text,100);
                    strcpy(LN,Text);
                    printf("Last Name Updated! :-)");
                    break;

                case 3:
                    age = getValidAge();
                    printf("Age Updated! :-)");
                    break;

                case 4:
                getchar();
                    getGender(Text);
                    strcpy(Gender,Text);
                    printf("Gender Updated! :-)");
                    break;

                case 5:
                    printf("Enter the C.G.P.A. : ");
                        while (scanf("%f", &CGPA) != 1 || CGPA < 0.0) {
                        printf("Error : C.G.P.A. can't be less than 0 ");
                        while (getchar() != '\n'); // Clear buffer
                    }
                    getchar();
                    printf("C.G.P.A. Updated! :-)");
                    break;

                case 6:
                    getInput("Enter New Username : ",Text,100);
                    strcpy(UserEntryU,Text);
                    printf("Username Updated :-)");
                    break;

                case 7:
                    getInput("Enter New Password : ",Text,100);
                    strcpy(UserEntryP,Text);
                    printf("Password Updated :-)");
                    break;

                case 8:
                    saveuserdetails(Usertype,UserEntryU,UserEntryP,FN,LN,age,Gender,CGPA,Department,SUBJECTS);
                    printf("\n:-D Changes have been saved successfully!\n");
                    printf("Returning to the Menu...");
                    studentmenu(UserEntryU,UserEntryP,FN,LN,age,Gender,CGPA);
                    break;

                default:
                    printf("\n! Remember To Save the Changes You Have Made !\n");
                    break;
                }
            }
            if (confirmExitWithoutSaving()) {break; } // Exit if user chooses not to continue editing
        }
        
    } else if(Usertype == 't') {
        while (1)
        {
            printf("\nChoose what You Want to Update !^_~\n");
            printf("\n(1) First Name\n(2) Last Name\n(3) Age\n(4) Gender\n(5) Subject\n(6) Username\n(7) Password\n(8) Save The Changes\n---->");
            if (scanf("%d",&choice) == 1 && choice > 0 && choice < 9)
            {
                getchar();
                switch (choice)
                {
                case 1:
                    getInput("Enter New First Name : ",Text,100);
                    strcpy(FN,Text);
                    printf("First Name Updated! :-)");
                    break;
                
                case 2:
                    getInput("Enter New Last Name : ",Text,100);
                    strcpy(LN,Text);
                    printf("Last Name Updated! :-)");
                    break;

                case 3:
                    age = getValidAge();
                    printf("Age Updated! :-)");
                    break;

                case 4:
                    getGender(Text);
                    strcpy(Gender,Text);
                    printf("Gender Updated! :-)");
                    break;

                case 5:
                    getInput("Enter New Subject : ",Text,100);
                    strcpy(SUBJECTS,Text);
                    printf("Subject Updated! :-)");
                    break;

                case 6:
                    getInput("Enter New Username : ",Text,100);
                    strcpy(UserEntryU,Text);
                    printf("Username Updated :-)");
                    break;

                case 7:
                    getInput("Enter New Password : ",Text,100);
                    strcpy(UserEntryP,Text);
                    printf("Password Updated :-)");
                    break;

                case 8:
                    saveuserdetails(Usertype,UserEntryU,UserEntryP,FN,LN,age,Gender,CGPA,Department,SUBJECTS);
                    printf("\n:-D Changes have been saved successfully!\n");
                    printf("Returning to the Menu...");
                    teachermenu(UserEntryU,UserEntryP,FN,LN,age,Gender,SUBJECTS);
                    break;

                default:
                    printf("\n! Remember To Save the Changes You Have Made !\n");
                    break;
                }
            }
            if (confirmExitWithoutSaving()) {break; } // Exit if user chooses not to continue editing
        }

    } else {
        while (1)
        {
            printf("\nChoose what You Want to Update !^_~\n");
            printf("\n(1) First Name\n(2) Last Name\n(3) Age\n(4) Gender\n(5) Department\n(6) Username\n(7) Password\n(8) Save The Changes\n---->");
            if (scanf("%d",&choice) == 1 && choice > 0 && choice < 9)
            {
                
                getchar();
                switch (choice)
                {
                case 1:
                    getInput("Enter New First Name : ",Text,100);
                    strcpy(FN,Text);
                    printf("First Name Updated! :-)");
                    break;
                
                case 2:
                    getInput("Enter New Last Name : ",Text,100);
                    strcpy(LN,Text);
                    printf("Last Name Updated! :-)");
                    break;

                case 3:
                    age = getValidAge();
                    printf("Age Updated! :-)");
                    break;

                case 4:
                    getGender(Text);
                    strcpy(Gender,Text);
                    printf("Gender Updated! :-)");
                    break;

                case 5:
                    getInput("Enter New Department : ",Text,100);
                    strcpy(Department,Text);
                    printf("Department Updated! :-)");
                    break;

                case 6:
                    getInput("Enter New Username : ",Text,100);
                    strcpy(UserEntryU,Text);
                    printf("Username Updated :-)");
                    break;

                case 7:
                    getInput("Enter New Password : ",Text,100);
                    strcpy(UserEntryP,Text);
                    printf("Password Updated :-)");
                    break;

                case 8:
                    saveuserdetails(Usertype,UserEntryU,UserEntryP,FN,LN,age,Gender,CGPA,Department,SUBJECTS);
                    printf("\n:-D Changes have been saved successfully!\n");
                    printf("Returning to the Menu...");
                    staffmenu(UserEntryU,UserEntryP,FN,LN,age,Gender,Department);
                    break;

                default:
                    printf("\n! Remember To Save the Changes You Have Made !\n");
                    break;
                }
            }
            if (confirmExitWithoutSaving()) {break; } // Exit if user chooses not to continue editing
        }

    }
    
}

int confirmExitWithoutSaving() {
    char c;
    printf("\n-------------------------------------------------\n! Remember To Save the Changes You Have Made !\n-------------------------------------------------\nDo you want to continue editing? (y/n): ");
    scanf(" %c", &c);
    getchar();
    c = tolower(c);
    if (c == 'n') {
        char q;
        printf("You made changes but didn’t save. Exit anyway? (y/n): ");
        scanf(" %c", &q);
        getchar();
        q = tolower(q);
        return (q == 'y') ? 1 : 0; // 1 = exit, 0 = stay
    }
    return 0; // Stay editing
}

int main() {
    printf("Welcome to University Database System !\n");
    
    loadUserCounts();  // Load counts at program start

    DisplayMainMenu();  // Function to handle menu

    return 0;
}

void signup(char UserType) {
    char name[100];
    char *Gender;
    int choice;
    printf("\nLet's SignUp! ^0^\n---------------------------------------\n");
    if (UserType == 's') {
        struct Student newstudent;
        
        getInput("Enter the First Name : ",newstudent.FirstName,100);
    
        getInput("Enter the Last Name : ",newstudent.LastName,100);
    
        newstudent.age = getValidAge();
    
        // Gender selection
        getGender(newstudent.Gender);
    
        getInput("Enter Your Email Address : ",newstudent.EmailAddress,100);

        do
        {
            getInput("Create a Username (AlphaNumeric): ",newstudent.Username,100);
    
            getInput("Enter Your Password : ",newstudent.Password,100);

            if (duplicateUserCheck('s', newstudent.Username, newstudent.Password))
            {
                printf("\n======================================================\n^_____^ Username already exists! Please try a different one.\n======================================================\n");
            }
            
        } while(duplicateUserCheck('s', newstudent.Username, newstudent.Password));

        newstudent.CGPA = 0.00;
        newstudent.attendance = 0;
    
        // **Check if file exists to determine mode**
        FILE *checkFile = fopen("student.txt", "r");
        char mode[2];
        
        if (checkFile) {
            strcpy(mode, "a");  // Append if file exists
            fclose(checkFile);
        } else {
            strcpy(mode, "w");  // Create new file if it doesn't exist
        }
    
        // **Open file safely**
        FILE *ptr = fopen("student.txt", mode);
        if (ptr == NULL) {
            printf("Error: Could not open file!\n");
            return;
        }
    
        fprintf(ptr, "%s|%s|%s|%s|%d|%s|%.2f\n",
                newstudent.Username, newstudent.Password, newstudent.FirstName,
                newstudent.LastName, newstudent.age, newstudent.Gender,
                newstudent.CGPA);
    
        fclose(ptr);
        printf("//--- Student Account Created Successfully ---//\n");
        printf("Press any key to continue...\n");
        getchar(); // Wait for user input before proceeding

        // **Call StudentMenu function to redirect user**
        clearscreen();
        studentmenu(newstudent.Username,newstudent.Password,newstudent.FirstName,newstudent.LastName,newstudent.age,newstudent.Gender,newstudent.CGPA);
    }
    else if (UserType == 't') {
        struct Teacher newteacher;
        
        getInput("Enter the First Name : ",newteacher.FirstName,100);
    
        getInput("Enter the Last Name : ",newteacher.LastName,100);
    
        newteacher.age = getValidAge();
    
        // Gender selection with validation
        getGender(newteacher.Gender);
    
        getInput("Enter Your Email Address : ",newteacher.EmailAddress,100);
    
        do
        {
            getInput("Create a Username (AlphaNumeric): ",newteacher.Username,100);
    
            getInput("Enter Your Password : ",newteacher.Password,100);

            if (duplicateUserCheck('t', newteacher.Username, newteacher.Password))
            {
                printf("\n======================================================\n^_____^ Username already exists! Please try a different one.\n======================================================\n");
            }
            
        } while(duplicateUserCheck('t', newteacher.Username, newteacher.Password));
        
        getInput("Enter Your Field of Subject : ",newteacher.Subject,100);
    
        // **Check if file exists before opening**
        FILE *checkFile = fopen("teacher.txt", "r");
        char mode[2];
    
        if (checkFile) {
            strcpy(mode, "a");  // Append if file exists
            fclose(checkFile);
        } else {
            strcpy(mode, "w");  // Create new file if it doesn't exist
        }
    
        // **Open file safely**
        FILE *ptr = fopen("teacher.txt", mode);
        if (ptr == NULL) {
            printf("Error: Could not open file!\n");
            return;
        }
    
        fprintf(ptr, "%s|%s|%s|%s|%d|%s|%s\n",
                newteacher.Username, newteacher.Password, newteacher.FirstName,
                newteacher.LastName, newteacher.age, newteacher.Gender,
                newteacher.Subject);
    
        fclose(ptr);
        printf("//--- Teacher Account Created Successfully ---//\n");
        printf("Press any key to continue...\n");
        getchar(); // Wait for user input before proceeding

        // **Call TeacherMenu function to redirect user**
        clearscreen();
        teachermenu(newteacher.Username,newteacher.Password,newteacher.FirstName,newteacher.LastName,newteacher.age,newteacher.Gender,newteacher.Subject);
    }
    else if (UserType == 'f') {
        struct Staff newstaff;
    
        getInput("Enter the First Name : ",newstaff.FirstName,100);
    
        getInput("Enter the Last Name : ",newstaff.LastName,100);
    
        newstaff.age = getValidAge();
    
        // Gender selection with validation
        getGender(newstaff.Gender);
    
        getInput("Enter Your Email Address : ",newstaff.EmailAddress,100);
    
        do
        {
            getInput("Create a Username (AlphaNumeric): ",newstaff.Username,100);
    
            getInput("Enter Your Password : ",newstaff.Password,100);

            if (duplicateUserCheck('f', newstaff.Username, newstaff.Password))
            {
                printf("\n======================================================\n^_____^ Username already exists! Please try a different one.\n======================================================\n");
            }
            
        } while(duplicateUserCheck('f', newstaff.Username, newstaff.Password));
        
        getInput("Enter Your Department : ",newstaff.Department,100);
    
        // **Check if file exists before opening**
        FILE *checkFile = fopen("staff.txt", "r");
        char mode[2];
    
        if (checkFile) {
            strcpy(mode, "a");  // Append if file exists
            fclose(checkFile);
        } else {
            strcpy(mode, "w");  // Create new file if it doesn't exist
        }
    
        // **Open file safely**
        FILE *ptr = fopen("staff.txt", mode);
        if (ptr == NULL) {
            printf("Error: Could not open file!\n");
            return;
        }
    
        fprintf(ptr, "%s|%s|%s|%s|%d|%s|%s\n",
                newstaff.Username, newstaff.Password, newstaff.FirstName,
                newstaff.LastName, newstaff.age, newstaff.Gender, 
                newstaff.Department);
    
        fclose(ptr);
        printf("//--- Staff Account Created Successfully ---//\n");
        printf("Press any key to continue...\n");
        getchar(); // Wait for user input before proceeding
    
        // **Call Display function to redirect user**
        clearscreen();
        staffmenu(newstaff.Username,newstaff.Password,newstaff.FirstName,newstaff.LastName,newstaff.age,newstaff.Gender,newstaff.Department);
    }
}

void studentmenu(char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],float CGPA) {
    int choice;
    do {
        printf("\n/-- Hi Student!, Welcome To University Database System :-) --/\n");
        printf("____________________________________________\nWhat Action you want to perform ?\n");
        printf("(1) For Printing Details of Student\n");
        printf("(2) For CGPA Calculator\n");
        printf("(3) Edit Your Profile\n");
        printf("(4) Exit\n____________________________________________\n");
        printf("---> ");
        scanf("%d", &choice);
        getchar(); // Clear buffer
        printf("____________________________________________\n");

        switch(choice) {
            case 1:
                printf("____________________________________________\nThe Name of Student : %s %s\n",FN,LN);
                printf("Age : %d\n",age);
                printf("Gender : %s\n",Gender);
                printf("CGPA : %f\n____________________________________________\n",CGPA);
                break;
            case 2:
                CGPACALCULATOR();
                break;
            case 3:
                EditUserDetails('s',UserEntryU,UserEntryP,FN,LN,age,Gender,CGPA,"","");
            break;
            case 4:
                printf("\nExiting Student Menu...\n");
                break;
            default:
                printf("Invalid choice, please try again!\n");
        }
    } while(choice != 4 );
    DisplayMainMenu();
}

void teachermenu(char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],char SUBJECTS[]) {
    int choice;
    do {
        printf("\n/-- Hi Teacher!, Welcome To University Database System :-) --/\n");
        printf("____________________________________________\nWhat Action you want to perform ?\n");
        printf("(1) For Printing Details of Teacher\n");
        printf("(2) For Subject Information\n");
        printf("(3) Edit Your Profile\n");
        printf("(4) Exit\n____________________________________________\n");
        printf("---> ");
        scanf("%d", &choice);
        getchar(); // Clear buffer
        printf("____________________________________________\n");

        switch(choice) {
            case 1:
                printf("____________________________________________\nThe Name of Teacher : %s %s\n",FN,LN);
                printf("Age : %d\n",age);
                printf("Gender : %s\n",Gender);
                printf("Subject(s) : %s\n____________________________________________\n",SUBJECTS);
                break;
            case 2:
                printf("Subject Info:\nYou are assigned to: %s\n", SUBJECTS);
                printf("You can add class schedules, announcements, etc. in the future!\n");
                break;
            case 3:
                EditUserDetails('t',UserEntryU,UserEntryP,FN,LN,age,Gender,0.0,"",SUBJECTS);
                break;
            case 4:
                printf("\nExiting Teacher Menu...\n");
                break;
            default:
                printf("Invalid choice, please try again!\n");
        }
    } while(choice != 4 );
    DisplayMainMenu();
}

void staffmenu(char UserEntryU[],char UserEntryP[],char FN[],char LN[],int age,char Gender[],char Department[]) {
    int choice;
    do {
        printf("\n/-- Hi Staff!, Welcome To University Database System :-) --/\n");
        printf("____________________________________________\nWhat Action you want to perform ?\n");
        printf("(1) For Printing Details of Staff\n");
        printf("(2) For Department Information\n");
        printf("(3) Edit Your Profile\n");
        printf("(4) Exit\n____________________________________________\n");
        printf("---> ");
        scanf("%d", &choice);
        getchar(); // Clear buffer
        printf("____________________________________________\n");

        switch(choice) {
            case 1:
                printf("____________________________________________\nStaff Name : %s %s\n",FN,LN);
                printf("Age : %d\n",age);
                printf("Gender : %s\n",Gender);
                printf("Department : %s\n____________________________________________\n",Department);
                break;
            case 2:
                printf("\n📚 Department: %s\nNote: Department-wide announcements and schedules will be available here in future updates.\n");
                break;
            case 3:
                EditUserDetails('f',UserEntryU,UserEntryP,FN,LN,age,Gender,0.0,Department,"");
            break;
            case 4:
                printf("\nExiting Staff Menu...\n");
                break;
            default:
                printf("Invalid choice, please try again!\n");
        }
    } while(choice != 4 );
    DisplayMainMenu();
}

void login(char UserType) {
    char UserEntryP[100], UserEntryU[100];
    char UN[100], UP[100], FN[100], LN[100], GENDER[100], DEPARTMENT[100], SUBJECTS[100];
    int age;
    float cgpa;
    printf("\nLock-in Bro! X_X , Let's Login ;_;\n-----------------------------------------\n\n");
    int attempts = 3; // Allow 3 attempts
    while(attempts--) {
    getInput("Enter The Username: ",UserEntryU,100);

    getInput("Enter The Password: ",UserEntryP,100);

    FILE *ptr = NULL;
    switch (UserType) {
        case 's': ptr = fopen("student.txt", "r"); break;
        case 't': ptr = fopen("teacher.txt", "r"); break;
        case 'f': ptr = fopen("staff.txt", "r"); break;
        default:
            printf("Invalid user type!\n");
            return;
    }

    if (ptr == NULL) {
        printf("File Not Opened!\n");
        return;
    }

    char line[300];
    int username_found = 0;
    while (fgets(line, sizeof(line), ptr)) {
        switch (UserType)
            {
            case 's':
            sscanf(line, "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%d|%99[^|]|%f|%*s",
                UN, UP, FN, LN, &age, GENDER, &cgpa);
                break;
            
            case 't':
            sscanf(line, "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%d|%99[^|]|%99[^|]|%*s",
                UN, UP, FN, LN, &age, GENDER, SUBJECTS);
                break;
            
            case 'f':
            sscanf(line, "%99[^|]|%99[^|]|%99[^|]|%99[^|]|%d|%99[^|]|%99[^|]|%*s",
                UN, UP, FN, LN, &age, GENDER, DEPARTMENT);
                break;
            default:
            printf("Error : Invalid Usertype! \n");
                break;
            }

        // Trim spaces if any
        UN[strcspn(UN, " ")] = 0;
        UP[strcspn(UP, " ")] = 0;
        if (strcmp(UserEntryU, UN) == 0) {
            username_found = 1; // Username found
            // If username and password match, print success message and redirect to appropriate menu
            if(strcmp(UserEntryP, UP) == 0){
                printf("\n--------------------------------------------\nLogin Successful! ^o^\n");
            switch (UserType)
            {
            case 's':
                printf("\nWelcome, %s %s!(/=U=)/\n", FN, LN);
                printf("\nPress any key to continue...\n");
                getchar();
                clearscreen();
                // Redirect to student menu
                studentmenu(UserEntryU,UserEntryP,FN,LN,age,GENDER,cgpa);
                break;
            
            case 't':
                printf("Welcome, %s %s! (/=U=)/\n", FN, LN);
                printf("\nPress any key to continue...\n");
                getchar();
                clearscreen();
                // Redirect to teacher menu
                teachermenu(UserEntryU,UserEntryP,FN,LN,age,GENDER,SUBJECTS);
                break;
            
            case 'f':
                printf("Welcome, %s %s! (/=U=)/\n", FN, LN);
                printf("\nPress any key to continue...\n");
                getchar();
                clearscreen();
                // Redirect to staff menu
                staffmenu(UserEntryU,UserEntryP,FN,LN,age,GENDER,DEPARTMENT);
                break;
            default:
            printf("Error : Invalid Usertype! \n");
                break;
            }
            fclose(ptr);
            return;} else {
                printf("\nIncorrect password!\n"); break; }
        }
    }
    // If username not found after checking all lines
        if(!username_found) {printf("\nUsername not found!\n");}
    fclose(ptr);
    printf("\nX_X Incorrect Username or Password. Attempts left: %d\n", attempts);
  }
   printf("\nToo many failed attempts. Redirecting to Main Menu...\n");
   DisplayMainMenu(); // instead of recursion
}

void CGPACALCULATOR() {
    printf("\n___Welcome_TO_CGPA_Calculating_System___\n");
    int numSubjects;
    float sum = 0, cgpa, percentage;

    // Asking the user for the number of subjects
    printf("Enter the number of subjects: ");
    scanf("%d", &numSubjects);
    getchar(); // Clear buffer

    float grades[numSubjects]; // Array to store grade points

    // Taking input for grade points
    printf("____________________________________________\nEnter the grade points for each subject !\n\n");
    for (int i = 0; i < numSubjects; i++) {
        printf("Subject %d: ", i + 1);
        while(scanf("%f", &grades[i]) != 1) {printf("Try Again! Enter Numbers Only") ; while (getchar() != '\n'); } // Clear invalid input from buffer
        getchar(); // Clear buffer
        if (grades[i] < 0.0 || grades[i] > 10.0) {
            printf("Invalid grade! Enter a value between 0 and 10.\n");
             i--; // retry this subject
             continue;
        }
        sum += grades[i]; // Summing up grade points
    }

    // Calculating CGPA
    cgpa = sum / numSubjects;

    // Calculating Percentage (CGPA * 9.5)
    percentage = cgpa * 9.5;

    // Displaying the results
    printf("\n____________________________________________\nTotal Grade Points: %.2f\n", sum);
    printf("CGPA: %.2f\n", cgpa);
    printf("Percentage: %.2f%%\n____________________________________________\n", percentage);
    printf("Grade: %s\n",getLetterGrade(cgpa));
    printf("Press any key to return to the Main Menu...\n");
    getchar(); // Wait for user input before returning to menu
    printf("Returning to the Main Menu...\n");
    clearscreen();
    return ;
}

char* getLetterGrade(float gpa) {
    if (gpa >= 9) return "A+";
    else if (gpa >= 8) return "A";
    else if (gpa >= 7) return "B";
    else if (gpa >= 6) return "C";
    else return "F";
}