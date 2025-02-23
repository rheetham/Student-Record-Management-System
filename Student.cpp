#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

const string FILE_NAME = "students.csv";

class Student
{
public:
    int rollNumber;
    string name;
    int age;

    Student() : rollNumber(0), name(""), age(0) {}
    Student(int roll, const string &name, int age) : rollNumber(roll), name(name), age(age) {}
};

// Global vector to hold student records
vector<Student> students;

// Utility: load student records from CSV file into vector
void loadData()
{
    students.clear();
    ifstream file(FILE_NAME);
    if (!file)
    {
        // File might not exist yet. Not an error.
        return;
    }
    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string token;
        int roll;
        string name;
        int age;
        // Parse roll number
        if (getline(ss, token, ','))
        {
            roll = stoi(token);
        }
        // Parse name
        if (getline(ss, token, ','))
        {
            name = token;
        }
        // Parse age
        if (getline(ss, token, ','))
        {
            age = stoi(token);
        }
        students.push_back(Student(roll, name, age));
    }
    file.close();
}

// Utility: write student records from vector into CSV file (sorted by roll number)
void saveData()
{
    // sort students by rollNumber
    sort(students.begin(), students.end(), [](const Student &a, const Student &b)
         { return a.rollNumber < b.rollNumber; });
    ofstream file(FILE_NAME);
    for (auto &s : students)
    {
        file << s.rollNumber << "," << s.name << "," << s.age << "\n";
    }
    file.close();
}

// Utility: check if roll number exists in the vector (except optional index for update)
bool rollExists(int roll, int ignoreIndex = -1)
{
    for (int i = 0; i < students.size(); i++)
    {
        if (i == ignoreIndex)
            continue;
        if (students[i].rollNumber == roll)
            return true;
    }
    return false;
}

// Create a new student entry
void createStudent()
{
    int roll, age;
    string name;
    cout << "Enter roll number: ";
    cin >> roll;

    // Reload data to check for duplicates
    loadData();
    if (rollExists(roll))
    {
        cout << "Error: A student with roll number " << roll << " already exists.\n";
        return;
    }

    cout << "Enter name: ";
    cin.ignore(); // clear newline
    getline(cin, name);

    cout << "Enter age: ";
    cin >> age;
    if (age <= 0 || age > 150)
        throw invalid_argument("Invalid age. Age must be between 1 and 150.");

    students.push_back(Student(roll, name, age));
    saveData();
    cout << "Student record created successfully.\n";
}

// View all students
void viewStudents()
{
    loadData();
    if (students.empty())
    {
        cout << "No student records found.\n";
        return;
    }
    cout << "Roll Number\tName\t\tAge\n";
    cout << "-------------------------------------\n";
    for (auto &s : students)
    {
        cout << s.rollNumber << "\t\t" << s.name << "\t\t" << s.age << "\n";
    }
}

// Search student by roll number
void searchStudent()
{
    int roll;
    cout << "Enter roll number to search: ";
    cin >> roll;
    loadData();
    bool found = false;
    for (auto &s : students)
    {
        if (s.rollNumber == roll)
        {
            cout << "Student Found:\n";
            cout << "Roll Number: " << s.rollNumber << "\n";
            cout << "Name: " << s.name << "\n";
            cout << "Age: " << s.age << "\n";
            found = true;
            break;
        }
    }
    if (!found)
        cout << "Student with roll number " << roll << " not found.\n";
}

// Update student by roll number
void updateStudent()
{
    int roll;
    cout << "Enter roll number of student to update: ";
    cin >> roll;
    loadData();
    int index = -1;
    for (int i = 0; i < students.size(); i++)
    {
        if (students[i].rollNumber == roll)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        cout << "Student with roll number " << roll << " not found.\n";
        return;
    }

    int newRoll;
    string newName;
    int newAge;

    cout << "Enter new roll number (current: " << students[index].rollNumber << "): ";
    cin >> newRoll;
    // If roll number is changed, check for duplicates
    if (newRoll != students[index].rollNumber && rollExists(newRoll))
    {
        cout << "Error: Another student with roll number " << newRoll << " already exists.\n";
        return;
    }

    cout << "Enter new name (current: " << students[index].name << "): ";
    cin.ignore();
    getline(cin, newName);

    cout << "Enter new age (current: " << students[index].age << "): ";
    cin >> newAge;
    if (newAge <= 0 || newAge > 150)
        throw invalid_argument("Invalid age. Age must be between 1 and 150.");

    // Update record
    students[index].rollNumber = newRoll;
    students[index].name = newName;
    students[index].age = newAge;

    saveData();
    cout << "Student record updated successfully.\n";
}

// Delete student by roll number
void deleteStudent()
{
    int roll;
    cout << "Enter roll number of student to delete: ";
    cin >> roll;
    loadData();
    auto it = remove_if(students.begin(), students.end(), [roll](const Student &s)
                        { return s.rollNumber == roll; });
    if (it == students.end())
    {
        cout << "Student with roll number " << roll << " not found.\n";
    }
    else
    {
        students.erase(it, students.end());
        saveData();
        cout << "Student record deleted successfully.\n";
    }
}

int main()
{
    int choice;
    while (true)
    {
        cout << "\n--- Student Record Management System ---\n";
        cout << "1. Create new student entry\n";
        cout << "2. View all students\n";
        cout << "3. Search student details by roll number\n";
        cout << "4. Update student by roll number\n";
        cout << "5. Delete student by roll number\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try
        {
            switch (choice)
            {
            case 1:
                createStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "Exception: " << e.what() << "\n";
        }
    }
    return 0;
}
