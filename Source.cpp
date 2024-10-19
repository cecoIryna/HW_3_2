#include <iostream>
#include <Windows.h>
#pragma warning(disable : 4996)
#include <cstring>
#include <fstream>

using namespace std;

class Contact {
    char* fullName;
    char homePhone[15];
    char workPhone[15];
    char mobilePhone[15];
    char additionalInfo[255];

public:
    Contact() {
        fullName = nullptr;
        homePhone[0] = '\0';
        workPhone[0] = '\0';
        mobilePhone[0] = '\0';
        additionalInfo[0] = '\0';
    }

    Contact(const char* name, const char* home, const char* work, const char* mobile, const char* info) {
        fullName = new char[strlen(name) + 1];
        strcpy(fullName, name);
        strcpy(homePhone, home);
        strcpy(workPhone, work);
        strcpy(mobilePhone, mobile);
        strcpy(additionalInfo, info);
    }

    Contact(const Contact& other) {
        if (other.fullName != nullptr) {
            fullName = new char[strlen(other.fullName) + 1];
            strcpy(fullName, other.fullName);
        }
        else {
            fullName = nullptr;
        }
        strcpy(homePhone, other.homePhone);
        strcpy(workPhone, other.workPhone);
        strcpy(mobilePhone, other.mobilePhone);
        strcpy(additionalInfo, other.additionalInfo);
    }

    Contact& operator=(const Contact& other) {
        if (this == &other) return *this;
        delete[] fullName;

        if (other.fullName != nullptr) {
            fullName = new char[strlen(other.fullName) + 1];
            strcpy(fullName, other.fullName);
        }
        else {
            fullName = nullptr;
        }
        strcpy(homePhone, other.homePhone);
        strcpy(workPhone, other.workPhone);
        strcpy(mobilePhone, other.mobilePhone);
        strcpy(additionalInfo, other.additionalInfo);

        return *this;
    }

    ~Contact() {
        delete[] fullName;
    }

    const char* getFullName() const {
        return fullName;
    }

    void setContact(const char* name, const char* home, const char* work, const char* mobile, const char* info) {
        if (fullName != nullptr) {
            delete[] fullName;
        }
        fullName = new char[strlen(name) + 1];
        strcpy(fullName, name);
        strcpy(homePhone, home);
        strcpy(workPhone, work);
        strcpy(mobilePhone, mobile);
        strcpy(additionalInfo, info);
    }

    void display() const {
        cout << "Full name: " << fullName << endl;
        cout << "Home Phone: " << homePhone << endl;
        cout << "Work Phone: " << workPhone << endl;
        cout << "Mobile Phone: " << mobilePhone << endl;
        cout << "Additional Info: " << additionalInfo << endl;
    }

    void saveToFile(FILE* file) const {
        fprintf(file, "Full name: %s\n", fullName);
        fprintf(file, "Home Phone: %s\n", homePhone);
        fprintf(file, "Work Phone: %s\n", workPhone);
        fprintf(file, "Mobile Phone: %s\n", mobilePhone);
        fprintf(file, "Additional Info: %s\n", additionalInfo);
    }

    void loadFromFile(FILE* file) {
        char buffer[255];
        fscanf(file, "Full name: %[^\n]\n", buffer);
        setContact(buffer, "", "", "", "");

        fscanf(file, "Home Phone: %[^\n]\n", buffer);
        strcpy(homePhone, buffer);

        fscanf(file, "Work Phone: %[^\n]\n", buffer);
        strcpy(workPhone, buffer);

        fscanf(file, "Mobile Phone: %[^\n]\n", buffer);
        strcpy(mobilePhone, buffer);

        fscanf(file, "Additional Info: %[^\n]\n", buffer);
        strcpy(additionalInfo, buffer);
    }
};

class PhoneBook {
    Contact* contacts;
    int count;

public:
    PhoneBook() : contacts(nullptr), count(0) {}

    ~PhoneBook() {
        delete[] contacts;
    }

    void addContact(const Contact& contact) {
        Contact* newContacts = new Contact[count + 1];
        for (int i = 0; i < count; ++i) {
            newContacts[i] = contacts[i]; 
        }
        newContacts[count] = contact;
        delete[] contacts;
        contacts = newContacts;
        count++;
    }

    void removeContact(const char* name) {
        int index = -1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(contacts[i].getFullName(), name) == 0) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            cout << "Contact " << name << " not found." << endl;
            return;
        }

        Contact* newContacts = new Contact[count - 1];
        for (int i = 0, j = 0; i < count; ++i) {
            if (i != index) {
                newContacts[j++] = contacts[i];
            }
        }
        delete[] contacts;
        contacts = newContacts;
        count--;
    }

    void searchContact(const char* name) const {
        for (int i = 0; i < count; ++i) {
            if (strcmp(contacts[i].getFullName(), name) == 0) {
                contacts[i].display();
                return;
            }
        }
        cout << "Contact " << name << " not found." << endl;
    }

    void displayAll() const {
        if (count == 0) {
            cout << "Phone Book empty." << endl;
            return;
        }
        for (int i = 0; i < count; ++i) {
            cout << "Contact #" << i + 1 << ":" << endl;
            contacts[i].display();
            cout << endl;
        }
    }

    void saveContactsToFile(const char* filename) const {
        FILE* file = fopen(filename, "w");
        if (file) {
            for (int i = 0; i < count; ++i) {
                contacts[i].saveToFile(file);
            }
            fclose(file);
            cout << "The information has been successfully saved to file." << endl;
        }
        else {
            cout << "Error opening file for writing." << endl;
        }
    }

    void loadContactsFromFile(const char* filename) {
        FILE* file = fopen(filename, "r");
        if (file) {
            while (!feof(file)) {
                Contact tempContact;
                tempContact.loadFromFile(file);
                addContact(tempContact);
            }
            fclose(file);
            cout << "Information successfully loaded from file." << endl;
        }
        else {
            cout << "Error opening file for reading." << endl;
        }
    }

};

int main() {
    PhoneBook phoneBook;

    Contact contact1("John Doe", "123-456", "789-101", "111-222", "Friend from work");
    Contact contact2("Jane Smith", "987-654", "321-432", "333-444", "Work colleague");

    phoneBook.addContact(contact1);
    phoneBook.addContact(contact2);

    cout << "All Contacts:" << endl;
    phoneBook.displayAll();

    cout << "\nSearching for John Doe:" << endl;
    phoneBook.searchContact("John Doe");

    cout << "\nRemoving John Doe..." << endl;
    phoneBook.removeContact("John Doe");

    cout << "\nAll Contacts after removal:" << endl;
    phoneBook.displayAll();

    phoneBook.saveContactsToFile("phonebook.txt");

    PhoneBook newPhoneBook;

    newPhoneBook.loadContactsFromFile("phonebook.txt");

    newPhoneBook.displayAll();

    return 0;
}