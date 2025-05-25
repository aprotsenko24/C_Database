C_Database: Student Database in C
This project is an independently developed student database system built in C. The database parses data from a CSV file and stores it in a hash table. It includes functionalities for adding, searching, deleting, and updating student records, with collision handling using linked lists.

Features:
Efficient Storage: Uses a hash table to store student information.

Collision Handling: Utilizes linked lists to manage collisions in the hash table.

CRUD Operations:

Add new students.

Search for students by name.

Delete student records.

Update student details.

Email Generation: Generates emails based on the student’s name and year (e.g., name22@domain.com).

Technologies:
C programming language

Data structures: Hash table, Linked lists

File handling: Parsing CSV and updating files

Getting Started:
Clone the repository:

bash
Copy
git clone https://github.com/yourusername/C_Database.git
Open the project folder in your C compiler.

Compile the C_Database_Project.c file.

Run the program:

bash
Copy
gcc C_Database_Project.c -o C_Database
./C_Database
File Structure:
C_Database_Project.c: Contains the core functionality of the database.

Database.txt: The CSV file storing the student data.

README.md: Project documentation.

How to Use:
Add new student records, search for a student by name, delete, and update records using the provided functions.

The program supports basic database operations and manages student information efficiently with hash tables.
