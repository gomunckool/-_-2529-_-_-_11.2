#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

enum Speciality { COMPUTER_SCIENCE, INFORMATICS, MATH_ECON, PHYS_INF, HANDICRAFT };
string specialityStr[] = { "Comp. Science", "Informatics", "Math & Econ", "Phys & Inf", "Handicraft" };

union ThirdGrade {
    int programming;
    int numericalMethods;
    int pedagogy;
};

struct Student {
    char surname[50];
    int course;
    Speciality speciality;
    int physicsGrade;
    int mathGrade;
    ThirdGrade thirdGrade;
};

void createFile(const string& filename);
void displayFile(const string& filename);
int countExcellentStudents(const string& filename);
double percentPhysicsFive(const string& filename);

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    int choice;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Create file\n";
        cout << "2. Display table\n";
        cout << "3. Count excellent students\n";
        cout << "4. Percentage of 5s in Physics\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createFile(filename); break;
            case 2: displayFile(filename); break;
            case 3: cout << "Excellent students: " << countExcellentStudents(filename) << endl; break;
            case 4: cout << "Percentage of 5s in Physics: " << fixed << setprecision(2) << percentPhysicsFive(filename) << "%" << endl; break;
        }
    } while (choice != 0);

    return 0;
}

void createFile(const string& filename) {
    ofstream f(filename, ios::binary);
    if (!f) return;

    int n;
    cout << "Count of students: "; cin >> n;

    for (int i = 0; i < n; i++) {
        Student s;
        cout << "Student #" << i + 1 << ":\n";
        cout << " Surname: "; cin >> s.surname;
        cout << " Course: "; cin >> s.course;
        cout << " Speciality (0-CS, 1-INF, 2-ME, 3-PI, 4-HC): ";
        int spec; cin >> spec;
        s.speciality = (Speciality)spec;
        cout << " Physics grade: "; cin >> s.physicsGrade;
        cout << " Math grade: "; cin >> s.mathGrade;

        if (s.speciality == COMPUTER_SCIENCE) {
            cout << " Programming grade: "; cin >> s.thirdGrade.programming;
        } else if (s.speciality == INFORMATICS) {
            cout << " Numerical methods grade: "; cin >> s.thirdGrade.numericalMethods;
        } else {
            cout << " Pedagogy grade: "; cin >> s.thirdGrade.pedagogy;
        }
        f.write((char*)&s, sizeof(Student));
    }
    f.close();
}

void displayFile(const string& filename) {
    ifstream f(filename, ios::binary);
    if (!f) return;

    cout << "\n" << setfill('=') << setw(100) << "=" << endl;
    cout << setfill(' ') << "| # | Surname    | Course | Speciality       | Phys | Math | Prog | NumM | Peda |" << endl;
    cout << setfill('-') << setw(100) << "-" << endl;
    cout << setfill(' ');

    Student s;
    int i = 1;
    while (f.read((char*)&s, sizeof(Student))) {
        cout << "| " << setw(1) << i++ << " | "
             << setw(10) << left << s.surname << " | "
             << setw(6) << right << s.course << " | "
             << setw(16) << left << specialityStr[s.speciality] << " | "
             << setw(4) << s.physicsGrade << " | "
             << setw(4) << s.mathGrade << " | ";
        
        if (s.speciality == COMPUTER_SCIENCE)
            cout << setw(4) << s.thirdGrade.programming << " | " << setw(4) << "-" << " | " << setw(4) << "-" << " |";
        else if (s.speciality == INFORMATICS)
            cout << setw(4) << "-" << " | " << setw(4) << s.thirdGrade.numericalMethods << " | " << setw(4) << "-" << " |";
        else
            cout << setw(4) << "-" << " | " << setw(4) << "-" << " | " << setw(4) << s.thirdGrade.pedagogy << " |";
        
        cout << endl;
    }
    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;
    f.close();
}

int countExcellentStudents(const string& filename) {
    ifstream f(filename, ios::binary);
    if (!f) return 0;

    Student s;
    int count = 0;
    while (f.read((char*)&s, sizeof(Student))) {
        int g3; 
        if (s.speciality == COMPUTER_SCIENCE) g3 = s.thirdGrade.programming;
        else if (s.speciality == INFORMATICS) g3 = s.thirdGrade.numericalMethods;
        else g3 = s.thirdGrade.pedagogy;

        if (s.physicsGrade == 5 && s.mathGrade == 5 && g3 == 5) {
            count++;
        }
    }
    f.close();
    return count;
}

double percentPhysicsFive(const string& filename) {
    ifstream f(filename, ios::binary);
    if (!f) return 0.0;

    Student s;
    int total = 0;
    int fives = 0;
    while (f.read((char*)&s, sizeof(Student))) {
        total++;
        if (s.physicsGrade == 5) fives++;
    }
    f.close();
    return (total > 0) ? (double)fives / total * 100.0 : 0.0;
}
