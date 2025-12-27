#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// --- Struct for Patient ---
struct Patient {
    int id;
    string name;
    int age;
    string gender;
    string illness;
    string doctor;
    int priority; // Higher value = higher priority for urgent cases
};

// Struct for Doctor 
struct Doctor {
    string name;
    vector<int> patients; // store patient IDs
};

// --- Global containers ---
vector<Patient> patientList;           // List of all patients
map<int, Patient*> patientByID;        // Quick access by ID
set<string> uniqueIllnesses;           // Track unique illnesses
map<string, vector<int>> doctorPatients; // Doctor -> Patient IDs
queue<int> waitingList;                // Normal waiting list 
priority_queue<pair<int, int>> urgentQueue; // Priority queue (priority, patient ID)
stack<string> actionHistory;           // Track last actions (add, delete, assign)

// --- Helper Functions ---
Patient* findPatientByID(int id) {
    if (patientByID.find(id) != patientByID.end())
        return patientByID[id];
    return nullptr;
}

void addPatient() {
    Patient p;
    cout << "Enter patient ID: "; cin >> p.id;
    cin.ignore();
    cout << "Enter name: "; getline(cin, p.name);
    cout << "Enter age: "; cin >> p.age;
    cin.ignore();
    cout << "Enter gender: "; getline(cin, p.gender);
    cout << "Enter illness: "; getline(cin, p.illness);
    cout << "Enter priority (higher = more urgent): "; cin >> p.priority;
    cin.ignore();

    p.doctor = "";
    patientList.push_back(p);
    patientByID[p.id] = &patientList.back();
    uniqueIllnesses.insert(p.illness);
    waitingList.push(p.id);
    urgentQueue.push({ p.priority, p.id });
    actionHistory.push("Added patient ID " + to_string(p.id));

    cout << "Patient added successfully!\n";
}

void displayPatients() {
    if (patientList.empty()) {
        cout << "No patients currently.\n";
        return;
    }
    cout << "--- All Patients ---\n";
    for (auto& p : patientList) {
        cout << "ID: " << p.id << " | Name: " << p.name << " | Age: " << p.age
            << " | Gender: " << p.gender << " | Illness: " << p.illness
            << " | Doctor: " << (p.doctor.empty() ? "None" : p.doctor)
            << " | Priority: " << p.priority << endl;
    }
}

void assignDoctor() {
    int pid;
    string doctor;
    cout << "Enter patient ID: "; cin >> pid;
    cin.ignore();
    Patient* p = findPatientByID(pid);
    if (!p) {
        cout << "Patient not found.\n";
        return;
    }
    cout << "Enter doctor name: "; getline(cin, doctor);
    p->doctor = doctor;
    doctorPatients[doctor].push_back(pid);
    actionHistory.push("Assigned doctor " + doctor + " to patient ID " + to_string(pid));
    cout << "Doctor assigned successfully.\n";
}

void searchPatient() {
    cout << "Search by (1) ID or (2) Name: ";
    int option; cin >> option;
    cin.ignore();
    if (option == 1) {
        int pid; cout << "Enter patient ID: "; cin >> pid;
        Patient* p = findPatientByID(pid);
        if (!p) { cout << "Patient not found.\n"; return; }
        cout << "Found: " << p->name << " | Age: " << p->age << " | Illness: " << p->illness
            << " | Doctor: " << (p->doctor.empty() ? "None" : p->doctor) << endl;
    }
    else if (option == 2) {
        string name; cout << "Enter name: "; getline(cin, name);
        bool found = false;
        for (auto& p : patientList) {
            if (p.name == name) {
                cout << "Found: ID " << p.id << " | Age: " << p.age << " | Illness: " << p.illness
                    << " | Doctor: " << (p.doctor.empty() ? "None" : p.doctor) << endl;
                found = true;
            }
        }
        if (!found) cout << "No patient with that name found.\n";
    }
}void deletePatient() {
    int pid; cout << "Enter patient ID to delete: "; cin >> pid;
    Patient* p = findPatientByID(pid);
    if (!p) { cout << "Patient not found.\n"; return; }
    string doc = p->doctor;
    patientByID.erase(pid);
    patientList.erase(remove_if(patientList.begin(), patientList.end(),
        [pid](Patient& x) { return x.id == pid; }), patientList.end());
    if (!doc.empty()) {
        auto& vec = doctorPatients[doc];
        vec.erase(remove(vec.begin(), vec.end(), pid), vec.end());
    }
    actionHistory.push("Deleted patient ID " + to_string(pid));
    cout << "Patient deleted successfully.\n";
}

void showStatistics() {
    cout << "--- Statistics ---\n";
    cout << "Total Patients: " << patientList.size() << endl;
    cout << "Unique Illnesses: " << uniqueIllnesses.size() << endl;
    cout << "Patients per Doctor:\n";
    for (auto& pair : doctorPatients)
        cout << "Doctor " << pair.first << ": " << pair.second.size() << " patients\n";
}

void undoLastAction() {
    if (actionHistory.empty()) { cout << "No actions to undo.\n"; return; }
    cout << "Undo: " << actionHistory.top() << endl;
    actionHistory.pop();
    cout << "Undo feature is illustrative; manually reverse changes if needed.\n";
}

void showWaitingList() {
    queue<int> temp = waitingList;
    cout << "--- Waiting List (FIFO) ---\n";
    while (!temp.empty()) {
        Patient* p = findPatientByID(temp.front());
        cout << p->id << " | " << p->name << endl;
        temp.pop();
    }
}

void showUrgentQueue() {
    priority_queue<pair<int, int>> temp = urgentQueue;
    cout << "--- Urgent Patients ---\n";
    while (!temp.empty()) {
        Patient* p = findPatientByID(temp.top().second);
        cout << "Priority: " << temp.top().first << " | " << p->id << " | " << p->name << endl;
        temp.pop();
    }
}

// --- Main Menu ---
int main() {
    int choice;
    do {
        cout << "\n--- Hospital Management System ---\n";
        cout << "1. Add Patient\n";
        cout << "2. Display All Patients\n";
        cout << "3. Search Patient\n";
        cout << "4. Assign Doctor\n";
        cout << "5. Delete Patient\n";
        cout << "6. Show Statistics\n";
        cout << "7. Show Waiting List\n";
        cout << "8. Show Urgent Queue\n";
        cout << "9. Undo Last Action\n";
        cout << "10. Exit\n";
        cout << "Enter choice: "; cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1: addPatient(); break;
        case 2: displayPatients(); break;
        case 3: searchPatient(); break;
        case 4: assignDoctor(); break;
        case 5: deletePatient(); break;
        case 6: showStatistics(); break;
        case 7: showWaitingList(); break;
        case 8: showUrgentQueue(); break;
        case 9: undoLastAction(); break;
        }
    } while (choice != 10);
    return 0;
}
