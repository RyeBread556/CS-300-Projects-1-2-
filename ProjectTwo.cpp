//=======================================================
// Name         : ProjectTwo.cpp
// Author       : Ryan Erno
// Date         : 6/17/25
//Description   : CS-300 Project Two
//=======================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

// define course structure
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// hashtable class
class HashTable {
private:
    //creation of hashtable and a default size
    static const unsigned int TABLE_SIZE = 50;
    vector<Course> table[TABLE_SIZE];

    // hashFunction for creating key
    int hashFunction(string& key) {
        int hash = 0;
        for (char course : key) {
            hash += course;
        }
        return hash % TABLE_SIZE;
    }

    // method for inserting object index into table
public:
    void insert(Course course) {
        int index = hashFunction(course.courseNumber);
        table[index].push_back(course);
    }
    //function to set up the vector and add courses
    vector<Course> getCourses() {
        vector<Course> allCourses;
        //put lines into the vector
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (Course& course : table[i]) {
                allCourses.push_back(course);
            }
        }
        return allCourses;
    }

    //search method to find specific course
    Course* search(string& courseNumber) {
        int index = hashFunction(courseNumber);
        //range based loop, loops through buckets and saves address
        for (auto& course : table[index]) {
            //compares the course in structure to the users string
            if (course.courseNumber == courseNumber) {
                //returns the address rather than a copy to the user.
                return &course;
            }
        }

        // if no match found in structure return null.
        return nullptr;
    }
};

void loadStructure(string myfile, HashTable& hashTable) {
    //open the file.
    ifstream file(myfile);
    //verify that the file opened correctly.
    if (!file.is_open()) {
        cout << "File failed to open" << endl;
        return;
    }

    string line;
    //read first file line, creating a stream and defining variables.
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, title, prereq;
        vector<string> prerequisites;

        //splits each line with a comma
        getline(ss, courseNumber, ',');
        getline(ss, title, ',');


        //get the final section of line, prerequisites
        while (getline(ss, prereq, ',')) {
            //checks if prerequisites are not empty and adds to prereq
            if (!prereq.empty()) {
                prerequisites.push_back(prereq);
            }
        }

        // define course object to store contents.
        Course courseData;
        courseData.courseNumber = courseNumber;
        courseData.title = title;
        courseData.prerequisites = prerequisites;
        //call to insert lines into hashtable.
        hashTable.insert(courseData);
    }
    //ensure file is closed after reading.
    file.close();
    cout << "File loaded sucessfully" << endl;
}

//print method for courses.
void PrintAll(HashTable hashTable) {
    vector<Course> allCourses = hashTable.getCourses();
        
    //sort the contents alphanumerically.
    sort(allCourses.begin(), allCourses.end(), [](const Course& a, const Course& b) {
       return a.courseNumber < b.courseNumber;
    });

    //output the sorted lines to the console.
    for (size_t i = 0; i < allCourses.size(); ++i) {
        cout << allCourses[i].courseNumber << ', ' << allCourses[i].title << endl;
    }
}
   

// the one and only main method ()
//command line method to prompt user for file name.
int main(int argc, char* argv[]) {
    string fileName;
    if (argc == 2) {
        fileName = argv[1];
    }
    else {
        cout << "Please enter a file name: " << endl;
        getline(cin, fileName);
    }

    HashTable menu;
    bool fileLoaded = false;
    int userChoice = 0;
    //menu display and options
    while (userChoice != 9) {
        cout << endl << "Welcome to the course planner." << endl << endl;
        cout << "Menu Options" << endl;
        cout << "Option 1: Load Data Structure(Do First)." << endl;
        cout << "Option 2: Print all courses." << endl;
        cout << "Option 3: Print course attributes." << endl;
        cout << "Option 9: Exit the program." << endl;
        cout << "What would you like to do?" << endl;
        cin >> userChoice;

        //pass user input into switch statement.
        switch (userChoice) {

        case 1:
            // call to load csv file into hashtable data structure.
            loadStructure(fileName, menu);
            fileLoaded = true;
            break;

        case 2:
            //prevent use if data is not loaded first.
            if (!fileLoaded) {
                cout << "Error, file needed first!" << endl;
            }
            //if provided, make call to the print function, outputting course/title.
            else {
                PrintAll(menu);
            }
            break;

        case 3:
            if (!fileLoaded) {
                cout << "Error, file needed first!" << endl;
            }
            else {

                cin.ignore();
                string userCourseNum;
                //prompt user and get input.
                cout << "What course do you want to know about?" << endl;
                getline(cin, userCourseNum);

                //toupper function to remove input case sensitivity.
                for (char& c : userCourseNum) {
                    c = toupper(c);
                }
                    
                //call to search function passing user input.
                Course* course = menu.search(userCourseNum);
                //case if no courses match input
                if (!course) {
                    cout << "Course was not found" << endl;
                }
                else {
                    //if found, output the course details.
                    cout << course->courseNumber << ',' << course->title << endl;
                    //check if prerequisites exist for related course.
                    if (course->prerequisites.empty()) {
                        cout << "Prerequisites: none " << endl;
                    }
                    else {
                        //loop through list of prerquisites if exists
                        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
                            cout << course->prerequisites[i];
                            //add comma for each course before list end.
                            if (i < course->prerequisites.size() - 1) {
                                cout << ',';
                            }
                        }  
                        cout << endl;
                    }
                }
            }

        case 9:
            cout << "Thank you for using the Course Planner!" << endl;
            exit(0);
        }
    }
}



        
       


    

