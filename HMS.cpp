#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip> // Include iomanip for setprecision
#include <ctime> // For current date and time
using namespace std;


// Function to get the current date and time as a formatted string
string getCurrentDateTime() {
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}

// Function to display the current date and time in the top-right corner
void displayCurrentDateTime() {
    string dateTime = getCurrentDateTime();
    cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left
    cout << setw(60) << right << dateTime << endl; // Display date and time aligned to the right
}



vector<string> departmentRepository;


class Service {
public:
    string category; // Medical, Diagnostic, etc.
    vector<string> subDepartments;

    void displayService() const {
        cout << "Category: " << category << "\n";
        cout << "  Sub-Departments: ";
        for (const string& subDepartment : subDepartments) {
            cout << subDepartment << ", ";
        }
        cout << "\n";
    }
};


class Room {
public:
    string type;
    int totalRooms;
    int occupiedRooms;

    Room(string type = "", int total = 0, int occupied = 0)  
        : type(type), totalRooms(total), occupiedRooms(occupied) {} 

    int availableRooms() const {
        return totalRooms - occupiedRooms;
    }

    void displayRoom() const {
        cout << "Room Type: " << type 
             << " | Total: " << totalRooms 
             << " | Occupied: " << occupiedRooms 
             << " | Available: " << availableRooms() << endl;
    }

    
};


// Base Staff class

const int HOURS_IN_DAY = 24;


class Staff {
public:
    string name;
    string department;
    vector<string> timetable;

    // Constructor to initialize the timetable with "Free"
    Staff() : timetable(HOURS_IN_DAY, "Free") {}

    // Display basic staff details
    virtual void displayStaff() const {
        cout << "Name: " << name
             << "\nDepartment: " << department << "\n";
    }

    // Display the timetable
    void displayTimetable() const {
        cout << "\nTimetable for " << name << ":\n";
        cout << "+---------------------------------------------------------------------------------------------------------+\n";
        cout << "| Hour   |  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  |\n";
        cout << "+---------------------------------------------------------------------------------------------------------+\n";
        cout << "| Status | ";
        for (const auto& status : timetable) {
            cout << (status == "Free" ? " #  " : " X  ");
        }
        cout << "|\n";
        cout << "+---------------------------------------------------------------------------------------------------------+\n";
    }

    // Update the timetable with a specific status for a time range
    void updateTimetable(int startHour, int endHour, const string& status) {
        for (int i = startHour; i <= endHour; ++i) {
            if (i >= 0 && i < HOURS_IN_DAY) {
                timetable[i] = status;
            }
        }
    }

    
};

// Derived classes
class Doctor : public Staff {
public:
    void displayStaff() const override {
        cout << "[Doctor]\n";
        Staff::displayStaff();
    }
};

class Nurse : public Staff {
public:
    void displayStaff() const override {
        cout << "[Nurse]\n";
        Staff::displayStaff();
    }
};

class Technician : public Staff {
public:
    void displayStaff() const override {
        cout << "[Technician]\n";
        Staff::displayStaff();
    }
};


// Function to customize staff timetable
void staffScheduling(Staff& staffMember) {
    cout << "\nEditing Timetable for " << staffMember.name << ":\n";
    while (true) {
        cout << "Enter the start hour (0-23, -1 to exit): ";
        int startHour;
        cin >> startHour;
        if (startHour == -1) break;
        if (startHour < 0 || startHour >= HOURS_IN_DAY) {
            cout << "Invalid start hour. Try again.\n";
            continue;
        }

        cout << "Enter the end hour (0-23): ";
        int endHour;
        cin >> endHour;
        if (endHour < startHour || endHour >= HOURS_IN_DAY) {
            cout << "Invalid end hour. Try again.\n";
            continue;
        }

        cout << "Enter status (Work/Free): ";
        string status;
        cin.ignore();
        getline(cin, status);
        status = (status.empty() || (status != "Work" && status != "Free")) ? "Free" : status;

        staffMember.updateTimetable(startHour, endHour, status);
        cout << "Timetable updated.\n";
    }
}


class Patient {
public:
    string id;
    string name;
    int age;
    string reasonForVisit;
    string department;
    bool hospitalized;
    string roomType;
    string hospitalizationDate;
    string dischargeDate;
    vector<string> history;

    Patient(string id = "", string name = "", int age = 0, string reason = "", string dept = "", 
            bool hosp = false, string room = "", string hospDate = "", string discDate = "")
        : id(id), name(name), age(age), reasonForVisit(reason), department(dept),
          hospitalized(hosp), roomType(room), hospitalizationDate(hospDate), dischargeDate(discDate) {}

    void addHistory(const string& event) {
        history.push_back(event);
    }

    void displayPatient() const{
        cout<<"ID: "<< id <<" | Name: "<<name 
            <<" | Age: "<<age 
            <<" | Reason: "<<reasonForVisit 
            <<" | Department: "<<department 
            <<" | Hospitalized: "<<(hospitalized ? "Yes" : "No") 
            <<" | Room Type: " <<roomType 
            <<" | Hospitalization Date: "<<hospitalizationDate 
            <<" | Discharge Date: "<<dischargeDate<<endl
            <<" | History:\n";
        for (const auto& event : history){
            cout<<"  - "<<event<<"\n";
        }
    }

    // Overload operator==
    bool operator==(const Patient& other) const {
        return id == other.id; // Compare based on unique ID
    }
};







// Configure staff
void configureStaff(vector<Doctor>& doctors, vector<Nurse>& nurses, vector<Technician>& technicians) {
    while (true) {
        cout << "\nSelect staff type:\n";
        cout << "1. Doctor\n";
        cout << "2. Nurse\n";
        cout << "3. Technician\n";
        cout << "Type 'done' to finish.\n";

        string choice;
        cin >> choice;
        cin.ignore();

        if (choice == "done") break;

        Staff* staffMember = nullptr;

        if (choice == "1") {
            staffMember = new Doctor();
        } else if (choice == "2") {
            staffMember = new Nurse();
        } else if (choice == "3") {
            staffMember = new Technician();
        } else {
            cout << "Invalid choice. Try again.\n";
            continue;
        }

        cout << "Enter staff member's name: ";
        getline(cin, staffMember->name);

        if (!departmentRepository.empty()) {
            cout << "Select a Department:\n";
            for (size_t i = 0; i < departmentRepository.size(); i++) {
                cout << i + 1 << ". " << departmentRepository[i] << "\n";
            }
            int deptChoice;
            cout << "Enter the corresponding number: ";
            cin >> deptChoice;
            cin.ignore();

            if (deptChoice > 0 && deptChoice <= departmentRepository.size()) {
                staffMember->department = departmentRepository[deptChoice - 1];
            } else {
                cout << "Invalid choice. Skipping department assignment.\n";
            }
        }

        staffScheduling(*staffMember);

        if (Doctor* doc = dynamic_cast<Doctor*>(staffMember)) {
            doctors.push_back(*doc);
        } else if (Nurse* nurse = dynamic_cast<Nurse*>(staffMember)) {
            nurses.push_back(*nurse);
        } else if (Technician* tech = dynamic_cast<Technician*>(staffMember)) {
            technicians.push_back(*tech);
        }

        delete staffMember;
    }
}


void configureServices() {
    vector<Service> serviceList;
    int serviceCount;

    cout << "How many services does the hospital offer? ";
    cin >> serviceCount;
    cin.ignore(); // Clear input buffer

    vector<string> categories(serviceCount);
    cout << "\nEnter all service categories (e.g., Medical, Diagnostic), one per line:\n";
    for (int i = 0; i < serviceCount; i++) {
        cout << "Category " << i + 1 << ": ";
        getline(cin, categories[i]);
    }

    for (int i = 0; i < serviceCount; i++) {
        Service serviceItem;
        serviceItem.category = categories[i];

        cout << "\nEnter sub-departments for " << categories[i] << " (type 'done' to finish):\n";
        while (true) {
            string subDepartment;
            cout << "Sub-Department: ";
            getline(cin, subDepartment);
            if (subDepartment == "done") break;
            serviceItem.subDepartments.push_back(subDepartment);
            departmentRepository.push_back(subDepartment); // Add to global repository
        }

        serviceList.push_back(serviceItem);
    }

    cout << "\nHospital Services:\n";
    for (int i = 0; i < serviceList.size(); i++) {
        cout << "Service " << i + 1 << ":\n";
        serviceList[i].displayService();
    }
}

void configureRooms(vector<Room>& rooms) {
    int typeCount;

    cout << "How many different types of Rooms does your hospital have? ";
    cin >> typeCount;
    cin.ignore();

    for (int i = 0; i < typeCount; i++) {
        Room room;
        cout << "\nEnter the type of room (e.g., ICU, General Ward, etc.): ";
        getline(cin, room.type);

        cout << "Enter the total number of " << room.type << " rooms: ";
        cin >> room.totalRooms;

        cout << "Enter the number of occupied " << room.type << " rooms: ";
        cin >> room.occupiedRooms;
        cin.ignore();

        rooms.push_back(room); // Update the shared rooms vector
    }

    cout << "\nRoom Details:\n";
    for (const auto& room : rooms) {
        room.displayRoom();
    }
}

void registerPatient(vector<Patient>& patientList) {
    Patient patient;

    cout << "\nEnter Patient ID: ";
    cin >> patient.id;
    cin.ignore();

    cout << "Enter Patient Name: ";
    getline(cin, patient.name);

    cout << "Enter Patient Age: ";
    cin >> patient.age;
    cin.ignore();

    cout << "Enter Reason for Visit: ";
    getline(cin, patient.reasonForVisit);

    if (!departmentRepository.empty()) {
        cout << "Select a Department:\n";
        for (size_t i = 0; i < departmentRepository.size(); ++i) {
            cout << i + 1 << ". " << departmentRepository[i] << "\n";
        }
        int choice;
        cout << "Enter the corresponding number: ";
        cin >> choice;
        cin.ignore();
        if (choice > 0 && choice <= departmentRepository.size()) {
            patient.department = departmentRepository[choice - 1];
        } else {
            cout << "Invalid choice. Assigning 'General'.\n";
            patient.department = "General";
        }
    }

    patientList.push_back(patient);
    cout << "Patient registered successfully!\n";
}



void manageStaffSchedules(vector<Doctor>& doctors, vector<Nurse>& nurses, vector<Technician>& technicians) {
    vector<Staff*> allStaff;

    // Combine all staff types into one list for selection
    for (auto& doctor : doctors) allStaff.push_back(&doctor);
    for (auto& nurse : nurses) allStaff.push_back(&nurse);
    for (auto& technician : technicians) allStaff.push_back(&technician);

    cout << "\nManaging staff schedules...\n";
    if (allStaff.empty()) {
        cout << "No staff available to manage.\n";
        return;
    }

    cout << "List of all staff members:\n";
    for (size_t i = 0; i < allStaff.size(); ++i) {
        cout << i + 1 << ". " << allStaff[i]->name << " (" << allStaff[i]->department << ")\n";
    }

    cout << "Enter the number of the staff member to manage (or '0' to go back): ";
    int choice;
    cin >> choice;

    if (choice == 0) return;

    if (choice > 0 && choice <= allStaff.size()) {
        Staff* selectedStaff = allStaff[choice - 1];
        cout << "Managing schedule for " << selectedStaff->name << ":\n";
        selectedStaff->displayTimetable(); // Display current timetable

        staffScheduling(*selectedStaff); // Modify the timetable
        cout << "Updated schedule:\n";
        selectedStaff->displayTimetable(); // Show updated timetable
    } else {
        cout << "Invalid choice. Returning to the menu...\n";
    }
}




void managePatients(vector<Patient>& patientList, vector<Doctor>& doctors, vector<Nurse>& nurses, vector<Technician>& technicians, vector<Room>& rooms) {
    while (true) {
        cout << "\n********** Manage Patients **********\n";
        cout << "List of Registered Patients (by ID):\n";
        for (size_t i = 0; i < patientList.size(); ++i) {
            cout << i + 1 << ". ID: " << patientList[i].id << " | Name: " << patientList[i].name << "\n";
        }
        cout << "Enter the Patient ID to manage (or '0' to go back): ";
        string id;
        cin >> id;

        if (id == "0") {
            cout << "Returning to menu...\n";
            break;
        }

        Patient* selectedPatient = nullptr;
        for (auto& patient : patientList) {
            if (patient.id == id) {
                selectedPatient = &patient;
                break;
            }
        }

        if (!selectedPatient) {
            cout << "Patient not found. Try again.\n";
            continue;
        }

        while (true) {
            cout << "\nManaging: " << selectedPatient->name << "\n";
            cout << "1. Assign/Change Department\n";
            cout << "2. Schedule Appointment\n";
            cout << "3. Hospitalize/Assign Room\n";
            cout << "4. Back to Patient Selection\n";
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;

            if (choice == 4) break;

            switch (choice) {
            case 1: {
                    cout << "\nSelect a Department:\n";
                    for (size_t i = 0; i < departmentRepository.size(); ++i) {
                        cout << i + 1 << ". " << departmentRepository[i] << "\n";
                    }
                    int deptChoice;
                    cout << "Enter the corresponding number: ";
                    cin >> deptChoice;
                    if (deptChoice > 0 && deptChoice <= departmentRepository.size()) {
                        selectedPatient->department = departmentRepository[deptChoice - 1];
                        cout << "Department updated successfully to " << selectedPatient->department << "!\n";
                    } else {
                        cout << "Invalid choice. Try again.\n";
                    }
                    break;
                }

            case 2: {
    if (selectedPatient->department.empty()) {
        cout << "Please assign a department first.\n";
        break;
    }

    vector<Staff*> departmentStaff;
    for (auto& doctor : doctors) {
        if (doctor.department == selectedPatient->department) {
            departmentStaff.push_back(&doctor);
        }
    }
    for (auto& nurse : nurses) {
        if (nurse.department == selectedPatient->department) {
            departmentStaff.push_back(&nurse);
        }
    }
    for (auto& technician : technicians) {
        if (technician.department == selectedPatient->department) {
            departmentStaff.push_back(&technician);
        }
    }

    if (departmentStaff.empty()) {
        cout << "No staff available in the assigned department.\n";
        break;
    }

    cout << "Available Staff in " << selectedPatient->department << ":\n";
    for (size_t i = 0; i < departmentStaff.size(); ++i) {
        cout << i + 1 << ". " << departmentStaff[i]->name << "\n";
        departmentStaff[i]->displayTimetable();
    }

    cout << "Select a staff member by number: ";
    int staffChoice;
    cin >> staffChoice;
    if (staffChoice <= 0 || staffChoice > departmentStaff.size()) {
        cout << "Invalid choice. Try again.\n";
        break;
    }

    Staff* selectedStaff = departmentStaff[staffChoice - 1];
    cout << "Enter the hour for the appointment (0-23): ";
    int hour;
    cin >> hour;
    if (hour >= 0 && hour < HOURS_IN_DAY && selectedStaff->timetable[hour] == "Work") {
        selectedStaff->timetable[hour] = "Appointment";
        string log = "Appointment scheduled with " + selectedStaff->name + " at hour " + to_string(hour) + " on " + getCurrentDateTime();
        selectedPatient->history.push_back(log);
        cout << "Appointment scheduled successfully!\n";
    } else {
        cout << "Invalid hour or the selected time is not available.\n";
    }
    break;
}


            case 3: {
    if (selectedPatient->hospitalized) {
        cout << "Patient is already hospitalized in " << selectedPatient->roomType << " room.\n";
        break;
    }

    cout << "Available Rooms:\n";
    for (size_t i = 0; i < rooms.size(); ++i) {
        if (rooms[i].availableRooms() > 0) {
            cout << i + 1 << ". " << rooms[i].type << " (Available: " << rooms[i].availableRooms() << ")\n";
        }
    }
    cout << "Select a room type by number: ";
    int roomChoice;
    cin >> roomChoice;
    if (roomChoice > 0 && roomChoice <= rooms.size() && rooms[roomChoice - 1].availableRooms() > 0) {
        selectedPatient->hospitalized = true;
        selectedPatient->roomType = rooms[roomChoice - 1].type;
        rooms[roomChoice - 1].occupiedRooms++;
        selectedPatient->hospitalizationDate = getCurrentDateTime();
        string log="Hospitalized in "+selectedPatient->roomType+" on "+selectedPatient->hospitalizationDate;
        selectedPatient->history.push_back(log);
        cout<<"Patient hospitalized successfully in "<<selectedPatient->roomType<<" room.\n";
    } else {
        cout<<"Invalid choice or no rooms available.\n";
    }
    break;
}


            default:
                cout<<"Invalid choice. Try again.\n";
                break;
            }
        }
    }
}

void displayPatientChart(const Patient& patient) {
    cout<< "\n=============================================\n"
        <<setw(20)<<left<< "Name:" << patient.name << "\n"
        <<setw(20)<<left<< "Age:" << patient.age << "\n"
        <<setw(20)<<left<< "Reason for Visit:" << patient.reasonForVisit << "\n"
        <<setw(20)<<left<< "Assigned Department:" << patient.department << "\n"
        <<setw(20)<<left<< "Hospitalized:" << (patient.hospitalized ? "Yes" : "No") << "\n";
    if (patient.hospitalized){
        cout<<setw(20)<<left<<"Assigned Room:"<<patient.roomType<<"\n";
        cout<<setw(20)<<left<<"Hospitalization Date:"<<patient.hospitalizationDate<<"\n";
    }
    cout<<setw(20)<<left<<"Discharge Date:"<<(!patient.dischargeDate.empty()?patient.dischargeDate:"N/A")<<"\n";
    cout<<"\n--- Patient History ---\n";
    for(const auto& event:patient.history){
        cout<<"  - "<<event<<"\n";
    }
    cout<<"=============================================\n";
}



int main() {
    vector<Doctor> doctors;
    vector<Nurse> nurses;
    vector<Technician> technicians;
    vector<Room> rooms;
    vector<Patient> patientList;
   

    vector<Staff> staffList;

    char start;
    do {
        cout<<"Thank You For Choosing Hospital Management System (HMS)!\n";
        cout<<"Press 'E' to Begin Setup: ";
        cin>>start;
    }while(start!='E'&&start!='e');

    short choice;
    do{
        
        cout<<"\n********** Configuration Menu **********\n";
        cout<<"1. Services\t2. Rooms\t3. Staff\t4. Exit\n";
        cout<<"Enter your choice: ";
        cin>>choice;

        switch (choice){
        case 1:
            configureServices();
            break;
        case 2:
            configureRooms(rooms);
            break;
        case 3:
            configureStaff(doctors,nurses,technicians);
            break;
        case 4:
            cout<<"Exiting configuration...\n";
            break;
        default:
            cout<<"Invalid choice. Please enter a number between 1 and 4.\n";
        }
    } while (choice != 4);
    
    short choice2;
    string id; 
    
    //vector to hold all staff 
    vector<Staff*> allStaff;
    do {
        cout << "\n********** Main Menu **********\n";
    
        cout<<"1. Patient Intake\n";       
        cout<<"2. Patient Management\n";    
        cout<<"3. Patient Discharge\n";        
        cout<<"4. Staff Scheduling\n";
        cout<<"5. Room Managemnt\n";          
        cout<<"6. Exit\n";
        cout<<"============================================\n";
        cout<<"Enter your choice: ";

        cin >> choice2;
        
        // Search for the staff member by name
        Staff* selectedStaffMember = nullptr;
        // Search for the patient by name
        Patient* selectedPatient = nullptr;
        

        switch (choice2) {
        case 1:
            registerPatient(patientList);
            break;
        case 2:
            managePatients(patientList, doctors, nurses, technicians, rooms);
            break;
            
        case 3: {
    cout << "\nDischarging a patient...\n";
    cout << "List of all patients:\n";
    for (size_t i = 0; i < patientList.size(); ++i) {
        cout << i + 1 << ". " << patientList[i].name << "\n";
    }
    cout << "Enter the patient's name to discharge (or '0' to go back): ";
    cin.ignore();
    getline(cin, id);

    if (id == "0") {
        cout << "Returning to menu...\n";
        break;
    }

   Patient* selectedPatient = nullptr;
   for (auto& patient : patientList) {
    if (patient.name == id) { 
        selectedPatient = &patient;
        break;
    }
}
   if (selectedPatient) {
    cout << "\n--- Patient Discharge Summary ---\n";
    displayPatientChart(*selectedPatient); 

    // Cost calculation
    int daysHospitalized;
    double hourlyRate, appointmentCost, totalCost;
    cout << "Enter the number of days the patient was hospitalized: ";
    cin >> daysHospitalized;
    cout << "Enter the hourly hospitalization rate: ";
    cin >> hourlyRate;
    cout << "Enter the total cost of all appointments: ";
    cin >> appointmentCost;

    totalCost = (daysHospitalized * 24 * hourlyRate) + appointmentCost;
    cout << fixed << setprecision(2);
    cout << "Total cost: Pkr" << totalCost << "\n";

    // Update patient data
    selectedPatient->dischargeDate = getCurrentDateTime();
    selectedPatient->hospitalized = false;
    string log = "Discharged on " + selectedPatient->dischargeDate;
    selectedPatient->history.push_back(log);

   
} else {
    cout << "No patient selected.\n";
}
    break;
}


    case 4: {
        manageStaffSchedules(doctors, nurses, technicians);
    break;
    }

    case 5: {
    cout << "\n--- Room Management ---\n";
    cout << "+-------------------+--------+------------+------------+\n";
    cout << "| Room Type         | Total  | Occupied   | Available  |\n";
    cout << "+-------------------+--------+------------+------------+\n";

    for (const auto& room : rooms) {
        cout << "| " << setw(17) << left << room.type
             << "| " << setw(6) << room.totalRooms
             << "| " << setw(10) << room.occupiedRooms
             << "| " << setw(10) << room.availableRooms() << " |\n";
    }

    cout << "+-------------------+--------+------------+------------+\n";

    // Optional: Allow updating room data
    cout << "Would you like to update room details? (y/n): ";
    char updateChoice;
    cin >> updateChoice;
    if (tolower(updateChoice) == 'y') {
        int roomIndex;
        cout << "Enter the room index to update (1 to " << rooms.size() << "): ";
        cin >> roomIndex;
        if (roomIndex > 0 && roomIndex <= rooms.size()) {
            Room& selectedRoom = rooms[roomIndex - 1];
            cout << "Enter new total number of rooms for " << selectedRoom.type << ": ";
            cin >> selectedRoom.totalRooms;
            cout << "Enter new number of occupied rooms for " << selectedRoom.type << ": ";
            cin >> selectedRoom.occupiedRooms;
            cout << "Room details updated successfully.\n";
        } else {
            cout << "Invalid room index.\n";
        }
    }
    break;
}

   case 6:
    cout << "Exiting the program...\n";
    return 0;

default:
    cout << "Invalid choice. Please try again.\n";
        }
    } while (true);

}
