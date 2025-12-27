#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

class SocialNetwork {
private:
    unordered_map<string, vector<string>> friends; // adjacency list
    unordered_map<string, vector<string>> messages; // messages for each user

public:
    // Add a user
    void addUser(const string& name) {
        if (friends.count(name)) {
            cout << name << " already exists.\n";
        }
        else {
            friends[name] = {};
            messages[name] = {};
            cout << name << " added to the network.\n";
        }
    }

    // Add a friend connection
    void addFriend(const string& user1, const string& user2) {
        if (!friends.count(user1) || !friends.count(user2)) {
            cout << "One or both users do not exist.\n";
            return;
        }
        if (find(friends[user1].begin(), friends[user1].end(), user2) == friends[user1].end()) {
            friends[user1].push_back(user2);
            friends[user2].push_back(user1);
            cout << user1 << " and " << user2 << " are now friends.\n";
        }
        else {
            cout << user1 << " and " << user2 << " are already friends.\n";
        }
    }

    // Send a message
    void sendMessage(const string& sender, const string& receiver, const string& msg) {
        if (!friends.count(sender) || !friends.count(receiver)) {
            cout << "One or both users do not exist.\n";
            return;
        }
        messages[receiver].push_back(sender + ": " + msg);
        cout << "Message sent from " << sender << " to " << receiver << ".\n";
    }

    // Display messages for a user
    void viewMessages(const string& user) {
        if (!messages.count(user)) {
            cout << "User does not exist.\n";
            return;
        }
        cout << "--- Messages for " << user << " ---\n";
        for (auto& msg : messages[user]) cout << msg << endl;
        if (messages[user].empty()) cout << "No messages.\n";
    }

    // Suggest friends (friends of friends)
    void suggestFriends(const string& user) {
        if (!friends.count(user)) {
            cout << "User does not exist.\n";
            return;
        }
        unordered_map<string, bool> visited;
        for (auto& f : friends[user]) visited[f] = true;
        visited[user] = true;

        vector<string> suggestions;
        for (auto& f : friends[user]) {
            for (auto& fof : friends[f]) {
                if (!visited[fof]) suggestions.push_back(fof);
            }
        }
        if (suggestions.empty()) {
            cout << "No friend suggestions for " << user << ".\n";
        }
        else {
            cout << "Friend suggestions for " << user << ": ";
            for (auto& s : suggestions) cout << s << " ";
            cout << endl;
        }
    }

    // Display friends of a user
    void displayFriends(const string& user) {
        if (!friends.count(user)) {
            cout << "User does not exist.\n";
            return;
        }
        cout << user << "'s friends: ";
        if (friends[user].empty()) cout << "No friends yet.";
        else for (auto& f : friends[user]) cout << f << " ";
        cout << endl;
    }
};

int main() {
    SocialNetwork sn;
    int choice;
    string u1, u2, msg;
    do {
        cout << "\n--- Social Network Menu ---\n";
        cout << "1. Add User\n";
        cout << "2. Add Friend\n";
        cout << "3. Send Message\n";
        cout << "4. View Messages\n";
        cout << "5. Suggest Friends\n";
        cout << "6. Display Friends\n";
        cout << "7. Exit\n";
        cout << "Enter Your Choice: ";
        cin >> choice;
        cin.ignore(); // to handle newline

        switch (choice) {
        case 1:
            cout << "Enter user name: ";
            getline(cin, u1);
            sn.addUser(u1);
            break;
        case 2:
            cout << "Enter first user: ";
            getline(cin, u1);
            cout << "Enter second user: ";
            getline(cin, u2);
            sn.addFriend(u1, u2);
            break;
        case 3:
            cout << "Sender: "; getline(cin, u1);
            cout << "Receiver: "; getline(cin, u2);
            cout << "Message: "; getline(cin, msg);
            sn.sendMessage(u1, u2, msg);
            break;
        case 4:
            cout << "Enter user name: ";
            getline(cin, u1);
            sn.viewMessages(u1);
            break;
        case 5:
            cout << "Enter user name: ";
            getline(cin, u1);
            sn.suggestFriends(u1);
            break;
        case 6:
            cout << "Enter user name: ";
            getline(cin, u1);
            sn.displayFriends(u1);
            break;
        case 7:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 7);

    return 0;
}
