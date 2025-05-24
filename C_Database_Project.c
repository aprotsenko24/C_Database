#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TABLE_SIZE 100
#define STRING_SIZE 1024
typedef struct Student
{
    char name[30];
    char id[10];
    char email[30];
    char major[30];
    char phone_number[20];
    char year[5];
    struct Student *next;
} Student;
Student *HashTable[TABLE_SIZE];
int i = 0, sum = 0, currentID = 0;
void EmailGenerator(Student *NewStudent)
{
    char temp[30], out[30];
    strcpy(temp, NewStudent->name);
    int i = 0, k = 0;

    while (temp[i] != '\0')
    {
        if (i == 0)
        {
            out[k] = tolower(temp[i]);
            k++;
        }
        else if (temp[i] == ' ')
        {
            i++;
            while (temp[i] != '\0')
            {
                out[k++] = tolower(temp[i++]);
            }
            break;
        };
        i++;
    }
    int len = strlen(NewStudent->year);
    out[k++] = NewStudent->year[len - 2];
    out[k++] = NewStudent->year[len - 1];
    out[k] = '\0';
    sprintf(NewStudent->email, "%s@bard.edu", out);
}
void ID(Student *NewStudent)
{
    sprintf(NewStudent->id, "SR%03d", currentID++);
}
void init()
{
    for (; i < TABLE_SIZE; i++)
    {
        HashTable[i] = NULL;
    }
}

int hash(char *name)
{
    sum = 0;
    i = 0;
    while (name[i] != '\0')
    {
        sum += name[i];
        i++;
    }
    return sum % TABLE_SIZE;
}
void hashtable(int *index, Student *S)
{
    if (!HashTable[(*index)])
    {
        HashTable[*index] = S;
        S->next = NULL;
    }
    else
    {
        S->next = HashTable[*index];
        HashTable[*index] = S;
    }
}
void print_out_data(Student *NewStudent)
{
    printf("\nHere is the student's data retrieved from the database:\n");
    printf("%s,", NewStudent->name);
    printf("%s,", NewStudent->id);
    printf("%s,", NewStudent->email);
    printf("%s,", NewStudent->phone_number);
    printf("%s,", NewStudent->major);
    printf("%s\n", NewStudent->year);
}
void print_in_data(FILE *fptr, Student *NewStudent)
{
    fprintf(fptr, "%s,", NewStudent->name);
    fprintf(fptr, "%s,", NewStudent->phone_number);
    fprintf(fptr, "%s,", NewStudent->major);
    fprintf(fptr, "%s\n", NewStudent->year);
}
void insert_data(Student *NewStudent, char *name, char *phone_number, char *major, char *year)
{
    strcpy(NewStudent->name, name);
    strcpy(NewStudent->phone_number, phone_number);
    strcpy(NewStudent->major, major);
    strcpy(NewStudent->year, year);
    ID(NewStudent);
    EmailGenerator(NewStudent);
}
Student *search(char *name_of_student)
{
    int index = 0;
    index = hash(name_of_student);
    Student *S = HashTable[index];
    while (S != NULL)
    {
        if (strcmp(S->name, name_of_student) == 0)
        {
            return S;
        }
        else
        {
            S = S->next;
        }
    }
    return NULL;
}
void search_student(char *name)
{
    if (search(name) == NULL)
    {
        printf("\nSorry, the student named '%s' was not found in the database.\n", name);
    }
    else
    {
        print_out_data(search(name));
    }
}
int add_student(char *name, char *phone_number, char *major, char *year, FILE *fptr)
{
    if (search(name) != NULL)
    {
        printf("\nSorry, the student '%s' is already in the database. Cannot add again.\n", name);
        return 1;
    }
    Student *NewStudent = (Student *)malloc(sizeof(Student));
    int index = 0;
    index = hash(name);
    hashtable(&index, NewStudent);
    NewStudent = HashTable[index];
    printf("\nAdding the following information to the database for '%s':\n", name);
    insert_data(NewStudent, name, phone_number, major, year);
    print_out_data(NewStudent);
    print_in_data(fptr, NewStudent);
    return 0;
}
FILE *delete_records(Student *S, FILE *fptr)
{
    FILE *temp;
    temp = fopen("temp.txt", "w");
    if (!temp || !fptr)
    {
        return NULL;
    }
    char buffer[STRING_SIZE];
    rewind(fptr);
    while (fgets(buffer, STRING_SIZE, fptr))
    {
        const char *c = strchr(buffer, ',');
        size_t len = c ? (size_t)(c - buffer) : strlen(c);
        if (strncmp(buffer, S->name, len) != 0)
        {
            fputs(buffer, temp);
        }
    }
    fclose(temp);
    fclose(fptr);
    fptr = fopen("Database.txt", "w");
    temp = fopen("temp.txt", "r");
    while (fgets(buffer, STRING_SIZE, temp))
    {
        fputs(buffer, fptr);
    }
    fclose(fptr);
    fclose(temp);
    remove("temp.txt");
    fptr = fopen("Database.txt", "a+");
    return fptr;
}
int delete_student(char *name, FILE *fptr)
{
    if (search(name) == NULL)
    {
        printf("\nCould not find the student '%s' in the database.\n", name);
        return 1;
    }
    if (delete_records == NULL)
    {
        printf("Error: Could not open the files.\n");
        return 1;
    }
    int index = 0;
    index = hash(name);
    Student *S = HashTable[index];
    fptr = delete_records(S, fptr);
    Student *prev = NULL;
    while (S != NULL)
    {
        if (strcmp(S->name, name) == 0)
        {
            if (prev == NULL)
            {
                HashTable[index] = S->next;
                free(S);
                printf("\nThe student '%s' has been successfully deleted from the database.\n", name);
                return 0;
            }
            else
            {
                HashTable[index] = prev;
                prev->next = S->next;
                free(S);
                printf("\nThe student '%s' has been successfully deleted from the database.\n", name);
                return 0;
            }
        }
        else
        {
            if (prev == NULL)
            {
                prev = S;
                S = S->next;
            }
            else
            {
                prev = prev->next;
                S = S->next;
            }
        }
    }
    printf("\nSorry, I did not find the name of this student in my database\n");
    return 1;
}
void read_file(FILE *fptr)
{
    int index = 0;
    char line[200], name[30], phone_number[30], major[30], year[5];
    while (fgets(line, sizeof(line), fptr))
    {
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,\n]",
                   name, phone_number, major, year) == 4)
        {
            Student *NewStudent = (Student *)malloc(sizeof(Student));
            index = hash(name);
            hashtable(&index, NewStudent);
            insert_data(NewStudent, name, phone_number, major, year);
        }
    }
}
int main()
{
    init();
    FILE *fptr;
    fptr = fopen("Database.txt", "a+");
    if (!fptr)
    {
        printf("Error: Could not open the file.\n");
        return 1;
    }
    read_file(fptr);
    add_student("Artem Portsenko", "+1(413)673-1614", "Computer Engineering", "2024", fptr);
    /* Add a student with two letters reversed in the name to test collision handling
   for the search() and hashtable() functions. */
    search_student("Oleksandr");
    /* Check if the search function correctly displays a message when the student
       is not found in the database. */
    search_student("Artem Portsenko");
    /* Otherwise, verify that the program can locate both students that
       map to the same index in the hash table. */
    delete_student("Artem Protsenko", fptr);
    /* Now test the delete function by removing an existing student. */
    search_student("Artem Protsenko");
    /* If we try to search for a student who was already deleted, we should see
        that they’re no longer found—neither in the file nor in the hash table. */
    delete_student("Oleksandr", fptr);
    /* Try deleting a student who doesn't exist in the hash table,
    and make sure it prints a message that the student isn’t found. */
    fclose(fptr);
    // Close the file after all operations are done.
    return 0;
}
/*
You can now use the "Database.txt" file with the following functions:

- Use `search_student(char *name)` to search for a student by name.

- Add a new student using:
    int add_student(char *name, char *phone_number, char *major, char *year, FILE *fptr)
  This function returns 0 if the student is successfully added,
  or 1 if the student already exists — with an appropriate message.

- Remove a student with:
    int delete_student(char *name, FILE *fptr)
  This function deletes the student both from the hash table and the file.
  It rewrites "Database.txt" without the deleted student’s information.
  It returns 0 if deletion was successful, or 1 if the student wasn’t found,
  along with a printed message.
*/
