#include <iostream>
#include <vector>
#include <string>
#include <iomanip>  // For output formatting
#include <limits>
#include <ctime>    // For logging visit dates

using namespace std;

// Store the data of Hospital
class Hospital {
public:
    string H_name;
    string location;
    int private_beds, semi_private_beds, general_beds;
    float rating;
    string contact;
    string doctor_name;
    int private_bed_price, semi_private_bed_price, general_bed_price;
    int consultation_fee, surgery_cost;

    bool hasAvailableBeds(string room_type) {
        if (room_type == "private" && private_beds > 0) return true;
        if (room_type == "semi-private" && semi_private_beds > 0) return true;
        if (room_type == "general" && general_beds > 0) return true;
        return false;
    }
    
    void allocateBed(string room_type) {
        if (room_type == "private") private_beds--;
        else if (room_type == "semi-private") semi_private_beds--;
        else if (room_type == "general") general_beds--;
    }
    
    void releaseBed(string room_type) {
        if (room_type == "private") private_beds++;
        else if (room_type == "semi-private") semi_private_beds++;
        else if (room_type == "general") general_beds++;
    }
};

// Stores the data of Patient and their history
class Patient {
public:
    string P_name;
    int P_id;
    string contact;
    string assigned_hospital;
    string room_type;  // Room type selected (private, semi-private, general)
    int days_stayed;
    int bed_price;
    int consultation_fee;
    int medication_cost;
    int surgery_cost;
    int total_bill;
    string diagnosis;

    struct VisitLog {
        string hospital_name;
        time_t admission_date;
        time_t discharge_date;
        int bill_amount;
    };

    vector<VisitLog> visit_history;

    void addVisitLog(string hospital_name, time_t admission, time_t discharge, int bill) {
        VisitLog log = { hospital_name, admission, discharge, bill };
        visit_history.push_back(log);
    }

    void printVisitHistory() const{
        cout << "\nPatient Visit History:\n";
        for (const auto& log : visit_history) {
            cout << "Hospital: " << log.hospital_name
                 << ", Admission Date: " << ctime(&log.admission_date)
                 << "Discharge Date: " << ctime(&log.discharge_date)
                 << "Total Bill: $" << log.bill_amount << endl;
        }
    }
};

// Function to print hospital data
void PrintHospitalData(const vector<Hospital>& hospitals) {
    if (hospitals.empty()) {
        cout << "No hospitals to display.\n";
        return;
    }

    for (size_t i = 0; i < hospitals.size(); ++i) {
        cout << i + 1 << ". " << hospitals[i].H_name << endl;
        cout << "   Location: " << hospitals[i].location << endl;
        cout << "   Private Beds: " << hospitals[i].private_beds << ", Price: $" << hospitals[i].private_bed_price << endl;
        cout << "   Semi-Private Beds: " << hospitals[i].semi_private_beds << ", Price: $" << hospitals[i].semi_private_bed_price << endl;
        cout << "   General Beds: " << hospitals[i].general_beds << ", Price: $" << hospitals[i].general_bed_price << endl;
        cout << "   Consultation Fee: $" << hospitals[i].consultation_fee << endl;
        cout << "   Surgery Cost: $" << hospitals[i].surgery_cost << endl;
        cout << "   Rating: " << hospitals[i].rating << endl;
        cout << endl;  // Add extra line break for readability
    }
}

// Function to print patient data
void PrintPatientData(const vector<Patient>& patients) {
    if (patients.empty()) {
        cout << "No patients to display.\n";
        return;
    }

    for (size_t i = 0; i < patients.size(); ++i) {
        cout << i + 1 << ". " << patients[i].P_name << endl;
        cout << "   Patient ID: " << patients[i].P_id << endl;
        cout << "   Contact: " << patients[i].contact << endl;
        cout << "   Allotted Hospital: " << patients[i].assigned_hospital << endl;
        cout << "   Room Type: " << patients[i].room_type << endl;
        cout << "   Days Stayed: " << patients[i].days_stayed << endl;
        cout << "   Total Bill: $" << patients[i].total_bill << endl;
        patients[i].printVisitHistory();
        cout << endl;  // Add extra line break for readability
    }
}

// Function to manually allocate a hospital to a patient based on user input
void AllocateHospitalToPatient(Patient& patient, vector<Hospital>& hospitals) {
    if (hospitals.empty()) {
        cout << "No hospitals available for allocation.\n";
        return;
    }

    cout << "Select a hospital for patient " << patient.P_name << ":\n";
    PrintHospitalData(hospitals);
    int hospital_choice;
    cout << "Enter the number of the hospital (1-" << hospitals.size() << "): ";
    cin >> hospital_choice;

    if (hospital_choice < 1 || hospital_choice > hospitals.size()) {
        cout << "Invalid choice. Allocation failed.\n";
        return;
    }

    Hospital& selected_hospital = hospitals[hospital_choice - 1];

    string room_type;
    cout << "Enter room type (private, semi-private, general): ";
    cin >> room_type;

    if (!selected_hospital.hasAvailableBeds(room_type)) {
        cout << "No available beds in the selected room type.\n";
        return;
    }

    selected_hospital.allocateBed(room_type);
    patient.assigned_hospital = selected_hospital.H_name;
    patient.room_type = room_type;

    // Set bed price based on room type
    if (room_type == "private") {
        patient.bed_price = selected_hospital.private_bed_price;
    } else if (room_type == "semi-private") {
        patient.bed_price = selected_hospital.semi_private_bed_price;
    } else if (room_type == "general") {
        patient.bed_price = selected_hospital.general_bed_price;
    }

    patient.consultation_fee = selected_hospital.consultation_fee;
    patient.surgery_cost = selected_hospital.surgery_cost;
    cout << "Patient " << patient.P_name << " allocated to hospital " << selected_hospital.H_name << " in a " << room_type << " room.\n";
}

// Function to discharge a patient and generate a detailed bill
void DischargePatient(Patient& patient, vector<Hospital>& hospitals) {
    if (patient.assigned_hospital.empty()) {
        cout << "Patient " << patient.P_name << " is not assigned to any hospital.\n";
        return;
    }

    int days_stayed;
    cout << "Enter the number of days the patient stayed in the hospital: ";
    cin >> days_stayed;
    patient.days_stayed = days_stayed;

    int medication_cost;
    cout << "Enter medication costs for the patient: $";
    cin >> medication_cost;
    patient.medication_cost = medication_cost;

    int total_bill = (patient.bed_price * days_stayed) + patient.consultation_fee + patient.medication_cost + patient.surgery_cost;
    patient.total_bill = total_bill;

    time_t now = time(0);

    for (auto& hospital : hospitals) {
        if (hospital.H_name == patient.assigned_hospital) {
            hospital.releaseBed(patient.room_type);
            patient.addVisitLog(hospital.H_name, now - (days_stayed * 86400), now, total_bill);  // Simplified date handling
            cout << "Patient " << patient.P_name << " discharged from hospital " << hospital.H_name << ".\n";
            cout << "\nDetailed Bill:\n";
            cout << "Room Charges ($" << patient.bed_price << " per day): $" << patient.bed_price * days_stayed << endl;
            cout << "Consultation Fee: $" << patient.consultation_fee << endl;
            cout << "Medication Cost: $" << patient.medication_cost << endl;
            cout << "Surgery Cost: $" << patient.surgery_cost << endl;
            cout << "Total Bill: $" << total_bill << endl;
            patient.assigned_hospital = "";  // Clear the hospital assignment
            break;
        }
    }
}

// Menu function
void Menu() {
    vector<Hospital> hospitals;
    vector<Patient> patients;
    int choice;

    while (true) {
        cout << "\nHospital Management System Menu:\n";
        cout << "1. Add Hospital\n";
        cout << "2. View All Hospitals\n";
        cout << "3. Add Patient\n";
        cout << "4. View All Patients\n";
        cout << "5. Allocate Hospital to Patient\n";
        cout << "6. Discharge Patient\n";
        cout << "7. Exit\n";
        cin >> choice;

        if (choice == 1) {
            Hospital h;
            cout << "Enter hospital details:\n";
            cout << "Name: "; cin >> h.H_name;
            cout << "Location: "; cin >> h.location;
            cout << "Private Beds: "; cin >> h.private_beds;
            cout << "Semi-Private Beds: "; cin >> h.semi_private_beds;
            cout << "General Beds: "; cin >> h.general_beds;
            cout << "Private Bed Price: $"; cin >> h.private_bed_price;
            cout << "Semi-Private Bed Price: $"; cin >> h.semi_private_bed_price;
            cout << "General Bed Price: $"; cin >> h.general_bed_price;
            cout << "Consultation Fee: $"; cin >> h.consultation_fee;
            cout << "Surgery Cost: $"; cin >> h.surgery_cost;
            cout << "Rating: "; cin >> h.rating;
            hospitals.push_back(h);
        } else if (choice == 2) {
            PrintHospitalData(hospitals);
        } else if (choice == 3) {
            Patient p;
            cout << "Enter patient details:\n";
            cout << "Name: "; cin >> p.P_name;
            cout << "Patient ID: "; cin >> p.P_id;
            cout << "Contact: "; cin >> p.contact;
            patients.push_back(p);
        } else if (choice == 4) {
            PrintPatientData(patients);
        } else if (choice == 5) {
            if (patients.empty()) {
                cout << "No patients to allocate.\n";
            } else if (hospitals.empty()) {
                cout << "No hospitals available for allocation.\n";
            } else {
                for (auto& patient : patients) {
                    if (patient.assigned_hospital.empty()) {
                        AllocateHospitalToPatient(patient, hospitals);
                    }
                }
            }
        } else if (choice == 6) {
            if (patients.empty()) {
                cout << "No patients to discharge.\n";
            } else {
                int patientId;
                cout << "Enter patient ID to discharge: ";
                cin >> patientId;
                bool patientFound = false;
                for (auto& patient : patients) {
                    if (patient.P_id == patientId && !patient.assigned_hospital.empty()) {
                        DischargePatient(patient, hospitals);
                        patientFound = true;
                        break;
                    }
                }
                if (!patientFound) {
                    cout << "Patient with ID " << patientId << " not found or not assigned to any hospital.\n";
                }
            }
        } else if (choice == 7) {
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Driver function
int main() {
    Menu();
    return 0;
}
