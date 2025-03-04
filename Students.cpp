#include <iostream>
#include <fstream>   //File Handling
#include <vector>    //Vector class
#include <sstream>   //Stringstream
#include <algorithm> //sort()

const std::string FILE_NAME = "students.csv";

class Student
{
public:
    int rollNumber;
    std::string name;
    int age;

    Student() : rollNumber(0), name(""), age(0) {}
    Student(int s_rno, const std::string &s_name, int s_age) : rollNumber(s_rno), name(s_name), age(s_age) {}
};

std::vector<Student> students;

// Returns the index where the roll no is found
// If not found returns -1
int rollNumberExists(int rollNumber)
{
    for (int i = 0; i < students.size(); ++i)
    {
        if (students[i].rollNumber == rollNumber)
        {
            return i;
        }
    }
    return -1;
}

void loadStudentsData()
{
    students.clear();
    std::ifstream file(FILE_NAME);
    if (!file)
    {
        // No data exists yet, which is not an error.
        return;
    }
    std::string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            // Edge case for empty line in the csv file.
            continue;
        }
        int s_rno;
        std::string s_name;
        int s_age;
        std::stringstream ss(line);
        std::string token;
        if (getline(ss, token, ','))
        {
            s_rno = stoi(token);
        }
        if (getline(ss, token, ','))
        {
            s_name = token;
        }
        if (getline(ss, token, ','))
        {
            s_age = stoi(token);
        }
        students.push_back(Student(s_rno, s_name, s_age));
    }
    file.close();
    return;
}

void saveStudentData()
{
    sort(students.begin(), students.end(), [](const Student &a, const Student &b)
         { return a.rollNumber < b.rollNumber; });
    std::ofstream file(FILE_NAME);
    for (Student &s : students)
    {
        file << s.rollNumber << "," << s.name << "," << s.age << "\n";
    }
    file.close();
    return;
}

void createNewStudent()
{
    int s_rno;
    std::cout << "Enter a new Roll Number: ";
    std::cin >> s_rno;
    if (rollNumberExists(s_rno) >= 0)
    {
        std::cout << "Error: Roll Number " << s_rno << " already exists";
        return;
    }
    std::string s_name;
    int s_age;
    std::cout << "Enter Name of Student: ";
    std::cin.ignore();
    getline(std::cin, s_name);
    std::cout << "Enter Age of Student: ";
    std::cin >> s_age;
    if (s_age < 0 || s_age > 80)
    {
        throw std::invalid_argument("Invalid age provided!");
    }
    students.push_back(Student(s_rno, s_name, s_age));
    saveStudentData();
    std::cout << "Student record created successfully!\n";
    return;
}

void viewAllStudents()
{
    loadStudentsData();
    if (students.empty())
    {
        std::cout << "No records to display.\n";
        return;
    }
    std::cout << "Roll Number\t\t" << "Name\t\t\t" << "Age\n";
    std::cout << "-----------------------------------------------------------\n";
    for (Student &s : students)
    {
        std::cout << s.rollNumber << "\t\t\t" << s.name << "\t\t\t" << s.age << "\n";
    }
    return;
}

void searchStudentByRollNumber()
{
    loadStudentsData();
    int s_rno;
    std::cout << "Enter the Roll Number to search: ";
    std::cin >> s_rno;
    int index = rollNumberExists(s_rno);
    if (index >= 0)
    {
        std::cout << "Roll Number\t\t" << "Name\t\t\t" << "Age\n";
        std::cout << "-----------------------------------------------------------\n";
        std::cout << students[index].rollNumber << "\t\t\t" << students[index].name << "\t\t\t" << students[index].age << "\n";
        return;
    }
    std::cout << "Student with Roll Number " << s_rno << " not found!\n";
    return;
}

void updateStudentByRollNumber()
{
    loadStudentsData();
    int s_rno;
    std::cout << "Enter the Roll Number to update: ";
    std::cin >> s_rno;
    int index = rollNumberExists(s_rno);
    if (index >= 0)
    {
        int newRollNumber;
        std::cout << "Enter new Roll Number (Currently: " << students[index].rollNumber << "): ";
        std::cin >> newRollNumber;
        if (newRollNumber != students[index].rollNumber && rollNumberExists(newRollNumber) >= 0)
        {
            std::cout << "Roll Number " << newRollNumber << " already exists!\n";
            return;
        }
        std::string newName;
        int newAge;
        std::cout << "Enter new Name (Currently: " << students[index].name << "): ";
        std::cin.ignore();
        getline(std::cin, newName);
        std::cout << "Enter new Age (Currently: " << students[index].age << "): ";
        std::cin >> newAge;
        students[index].rollNumber = newRollNumber;
        students[index].name = newName;
        students[index].age = newAge;
        saveStudentData();
        std::cout << "Student record updated successfully!\n";
        return;
    }
    std::cout << "Student with Roll Number " << s_rno << " not found!\n";
    return;
}

void deleteStudentByRollNumber()
{
    loadStudentsData();
    int s_rno;
    std::cout << "Enter Roll Number to Delete: ";
    std::cin >> s_rno;
    int index = rollNumberExists(s_rno);
    if (index >= 0)
    {
        students.erase(students.begin() + index);
        saveStudentData();
        std::cout << "Student record deleted successfully!\n";
        return;
    }
    std::cout << "Student with Roll Number " << s_rno << " not found!\n";
    return;
}

int main()
{
    int choice;
    do
    {
        std::cout << "\n--- Student Record Management System ---\n";
        std::cout << "1. Create new student entry\n";
        std::cout << "2. View all students\n";
        std::cout << "3. Search student details by roll number\n";
        std::cout << "4. Update student by roll number\n";
        std::cout << "5. Delete student by roll number\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        try
        {
            switch (choice)
            {
            case 1:
                createNewStudent();
                break;
            case 2:
                viewAllStudents();
                break;
            case 3:
                searchStudentByRollNumber();
                break;
            case 4:
                updateStudentByRollNumber();
                break;
            case 5:
                deleteStudentByRollNumber();
                break;
            case 0:
                std::cout << "Exiting Program!\n";
                return 0;
            default:
                std::cout << "Enter Valid Choice!\n";
            }
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what() << '\n';
        }
    } while (choice);
    return 0;
}
