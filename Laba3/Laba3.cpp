#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct User {
    string name;
    int password_length = 0;
    string password;
    bool blocked = false;
    bool restrictions = true;
    bool is_admin = false;
};
void scan_file(vector<User>& users);
void start_work();
void print_users(const vector<User>& users);
void create_user(vector<User>& users);
bool check_requirements(const string password);
void change_password(User* user);
void first_account(vector<User>& users);
void log_in(vector<User>& users, User* current_user);
User* find_account(vector<User>& users, const string name);
void work_admin(vector<User>& users, User* current_user);
void work_user(vector<User>& users, User* current_user);
void block_user(vector<User>& users);
void switch_restrictions(vector<User>& users);
void write_to_file(vector<User>& users);


int main()
{
    start_work();
}


void scan_file(vector<User>& users) {
    ifstream file;
    file.open("Users.txt");
    if (!file) {
        cout << "No such file here" << endl;
        return;
    }
    users.clear();

    string line;
    while (getline(file, line)) {
        User user;
        user.name = line;
        getline(file, line);
        user.password = line;
        user.password_length = line.length();
        getline(file, line);
        getline(file, line);
        user.blocked = (line == "true" || line == "True");
        getline(file, line);
        user.restrictions = (line == "true" || line=="True");
        getline(file, line);
        user.is_admin = (line == "true" || line == "True");

        users.push_back(user);
    }
    file.close();
}

void start_work()
{
    vector<User> users;
    scan_file(users);
    if (users.size() == 0) {
        first_account(users);
        cout << "It seems there is no active accounts. Basic admin account with \"11\" password created" << endl;
    }
    bool active_menu = true;
    while (active_menu) {
        User* current_user = nullptr;
        cout << "Press 1 to log in account, press 2 to create new account, press 3 to display users, press * to exit: ";
        char choise;
        cin >> choise;
        switch (choise)
        {
        case '1':
            log_in(users, current_user);
            break;
        case '2':
            create_user(users);
            break;
        case '3':
            print_users(users);
            break;
        case '*':
            cout << "Exiting menu, goodbye!" << endl;
            active_menu = false;
            break;
        default:
            cout << "There is no such option" << endl;
            break;
        }
    }
    write_to_file(users);
}

void print_users(const vector<User>& users)
{
    cout << "------------------------" << endl;
    for (const User& user : users) {
        cout << user.name << " is ";
        cout << (user.is_admin ? "admin, " : "common user, ");
        cout << (user.blocked ? "blocked, " : "");
        cout << (user.restrictions ? "has password restrictions" : "has no password restrictions");
        cout << endl;
    }
    cout << "------------------------" << endl;
}

void create_user(vector<User>& users)
{
    User* user = new User;
    cout << "------------------------" << endl;
    cout << "Enter name of the user: ";
    cin >> user->name;
    change_password(user);
    cout << "You want this user to be administrator? Print \"Yes\" if so, otherwise this user will be default user: ";
    string answer;
    cin >> answer;
    user->is_admin = (answer == "Yes" || answer == "yes");
    users.push_back(*user);
    cout << "User was created succesfully" << endl;
    cout << "------------------------" << endl;
    delete user;
}

bool check_requirements(const string password)
{
    bool has_letter = false;
    bool has_operator = false;
    for (char c : password) {
        if (isalpha(c)) {
            has_letter = true;
        }
        else if (c == '*' || c == '+' || c == '-' || c == '/') {
            has_operator = true;
        }
    }
    return has_letter&&has_operator;
}

void change_password(User* user)
{
    while (true) {
        if (user->password.length() != 0) {
            cout << "Enter your old password: ";
            string old_password;
            cin >> old_password;
            if (old_password == user->password) {
            }
            else {
                cout << "Passwords doesn't match" << endl;
                continue;
            }
        }
                cout << "Enter password of the user ";
                if (user->restrictions) {
                    cout << "(letters and math operators required)";
                }
                cout << ": ";
                string password;
                cin >> password;
                if (check_requirements(password) || !user->restrictions) {
                    user->password = password;
                    user->password_length = user->password.length();
                    cout << "Succesfully changed password!" << endl;
                    break;
                }
                cout << "Your password didn't met requirements. Try again using math operators and letters" << endl;
            }
            

}

void first_account(vector<User>& users)
{
    User admin;
    admin.name = "Admin";
    admin.password = "11";
    admin.password_length = admin.password.length();
    admin.is_admin = true;
    users.push_back(admin);
}

void log_in(vector<User>& users, User* current_user)
{
    cout << "------------------------" << endl;
    cout << "Enter account name: ";
    string name;
    cin >> name;
    current_user = find_account(users, name);
    if (current_user==nullptr) {
        cout << "There is no user with such name" << endl;
        return;
    }
    else {
        while (true) {
            cout << "Enter password or enter * to escape: ";
            string password;
            cin >> password;
            if (password == current_user->password) {
                cout << "Succesfully logged in" << endl;
                cout << "------------------------" << endl;
                if (current_user->is_admin) {
                    work_admin(users, current_user);
                }
                else {
                    work_user(users, current_user);
                }
                return;
            }
            else if (password == "*") {
                cout << "------------------------" << endl;
                return;
            }
            else {
                cout << "Wrong password" << endl;
            }
        }
    }
}

User* find_account(vector<User>& users, const string name)
{
    for (User& user : users) {
        if (user.name == name) {
            return &user;
        }
    }
    return nullptr;
}

void work_admin(vector<User>& users, User* current_user)
{
    bool active_menu = true;
    cout << "Greetings, " << current_user->name << endl;
    while (active_menu) {
        cout << "Press 1 to change password, press 2 to display all users, press 3 to block user, press 4 to change user's password restrictions press * to exit: ";
        char choise;
        cin >> choise;
        switch (choise)
        {
        case '1':
            change_password(current_user);
            break;
        case '2':
            print_users(users);
            break;
        case '3':
            block_user(users);
            break;
        case '4':
            switch_restrictions(users);
            break;
        case '*':
            cout << "Exiting account, goodbye!" << endl;
            active_menu = false;
            break;
        default:
            cout << "There is no such option" << endl;
            break;
        }
    }
}

void work_user(vector<User>& users, User* current_user)
{
    bool active_menu = true;
    cout << "Greetings, " << current_user->name << endl;
    if (current_user->blocked) {
        cout << "You are blocked, you can't do nothing" << endl;
        return;
    }
    while (active_menu) {
        cout << "Press 1 to change password, press 2 to display all users, press * to exit: ";
        char choise;
        cin >> choise;
        switch (choise)
        {
        case '1':
            change_password(current_user);
            break;
        case '2':
            print_users(users);
            break;
        case '*':
            cout << "Exiting account, goodbye!" << endl;
            active_menu = false;
            break;
        default:
            cout << "There is no such option" << endl;
            break;
        }
    }
}

void block_user(vector<User>& users)
{
    string name;
    cout << "Enter name of user: ";
    cin >> name;
    User* user = find_account(users, name);
    if (user != nullptr && !user->is_admin) {
        cout << "This user is " << (user->blocked ? "blocked" : "not blocked") << ". Enter 1 if you want to change it: ";
        char choice;
        cin >> choice;
        if (choice == '1') {
            user->blocked = !user->blocked;
        }
        
    }
    else if (user==nullptr){
        
        cout << "There is no users with such name to block" << endl;
    }
    else {
        cout << "You can't block admins!" << endl;
    }
}

void switch_restrictions(vector<User>& users)
{
    string name;
    cout << "Enter name of user: ";
    cin >> name;
    User* user = find_account(users, name);
    if (user != nullptr) {
        cout << "This user is " << (user->restrictions ? "restricted" : "not restricted") << ". Enter 1 if you want to change it: ";
        char choice;
        cin >> choice;
        if (choice == '1') {
            user->restrictions = !user->restrictions;
        }
    }
    else {
        cout << "There is no users with such name to restrict" << endl;
    }
}

void write_to_file(vector<User>& users)
{
    ofstream file("Users.txt");
    if (!file) {
        cout << "Can't write to file" << endl;;
        return;
    }
    for (const User& user : users) {
        file << user.name << endl;
        file << user.password << endl;
        file << user.password_length << endl;
        file << (user.blocked ? "true" : "false") << endl;
        file << (user.restrictions ? "true" : "false") << endl;
        file << (user.is_admin ? "true" : "false") << endl;
    }
    file.close();
}
