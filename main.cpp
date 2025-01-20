#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <fstream>
#include <vector>
#include <string>

struct Contact {
    std::string name;
    std::string phone;
    std::string email;
};

std::vector<Contact> contacts;
const std::string filename = "contacts.txt";

void loadContactsFromFile() {
    std::ifstream file(filename);
    if (file.is_open()) {
        Contact contact;
        while (std::getline(file, contact.name)) {
            std::getline(file, contact.phone);
            std::getline(file, contact.email);
            contacts.push_back(contact);
        }
        file.close();
    }
}

void saveContactsToFile() {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto &contact : contacts) {
            file << contact.name << "\n";
            file << contact.phone << "\n";
            file << contact.email << "\n";
        }
        file.close();
    }
}

class ContactManager : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout *mainLayout;
    QListWidget *contactList;
    QLineEdit *nameInput;
    QLineEdit *phoneInput;
    QLineEdit *emailInput;

public:
    ContactManager() {
        mainLayout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel("Contact Management System", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);

        contactList = new QListWidget(this);
        mainLayout->addWidget(contactList);
        loadContactList();

        QHBoxLayout *inputLayout = new QHBoxLayout();
        nameInput = new QLineEdit(this);
        nameInput->setPlaceholderText("Name");
        phoneInput = new QLineEdit(this);
        phoneInput->setPlaceholderText("Phone");
        emailInput = new QLineEdit(this);
        emailInput->setPlaceholderText("Email");

        inputLayout->addWidget(nameInput);
        inputLayout->addWidget(phoneInput);
        inputLayout->addWidget(emailInput);
        mainLayout->addLayout(inputLayout);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *addButton = new QPushButton("Add", this);
        QPushButton *editButton = new QPushButton("Edit", this);
        QPushButton *deleteButton = new QPushButton("Delete", this);

        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(deleteButton);
        mainLayout->addLayout(buttonLayout);

        connect(addButton, &QPushButton::clicked, this, &ContactManager::addContact);
        connect(editButton, &QPushButton::clicked, this, &ContactManager::editContact);
        connect(deleteButton, &QPushButton::clicked, this, &ContactManager::deleteContact);
    }

    void loadContactList() {
        contactList->clear();
        for (const auto &contact : contacts) {
            contactList->addItem(QString::fromStdString(contact.name + " - " + contact.phone + " - " + contact.email));
        }
    }

    void addContact() {
        std::string name = nameInput->text().toStdString();
        std::string phone = phoneInput->text().toStdString();
        std::string email = emailInput->text().toStdString();

        if (name.empty() || phone.empty() || email.empty()) {
            QMessageBox::warning(this, "Input Error", "All fields must be filled!");
            return;
        }

        contacts.push_back({name, phone, email});
        loadContactList();
        clearInputs();
        QMessageBox::information(this, "Success", "Contact added successfully!");
    }

    void editContact() {
        int currentIndex = contactList->currentRow();
        if (currentIndex == -1) {
            QMessageBox::warning(this, "Selection Error", "No contact selected!");
            return;
        }

        std::string name = nameInput->text().toStdString();
        std::string phone = phoneInput->text().toStdString();
        std::string email = emailInput->text().toStdString();

        if (name.empty() || phone.empty() || email.empty()) {
            QMessageBox::warning(this, "Input Error", "All fields must be filled!");
            return;
        }

        contacts[currentIndex] = {name, phone, email};
        loadContactList();
        clearInputs();
        QMessageBox::information(this, "Success", "Contact updated successfully!");
    }

    void deleteContact() {
        int currentIndex = contactList->currentRow();
        if (currentIndex == -1) {
            QMessageBox::warning(this, "Selection Error", "No contact selected!");
            return;
        }

        contacts.erase(contacts.begin() + currentIndex);
        loadContactList();
        QMessageBox::information(this, "Success", "Contact deleted successfully!");
    }

    void clearInputs() {
        nameInput->clear();
        phoneInput->clear();
        emailInput->clear();
    }

    ~ContactManager() {
        saveContactsToFile();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    loadContactsFromFile();

    ContactManager manager;
    manager.setWindowTitle("Contact Management System");
    manager.resize(600, 400);
    manager.show();

    return app.exec();
}
