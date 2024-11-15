#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Node class for chaining in case of overflow
class Node {
public:
    string key;
    string value;
    Node* next;

    Node(const string& key, const string& value) : key(key), value(value), next(nullptr) {}
};

// HashBucket class representing a single bucket
class HashBucket {
    vector<pair<string, string>> entries; // Fixed-size bucket
    int maxSize; // Max size of the bucket
    Node* overflowHead; // Head of the linked list for overflow entries

public:
    HashBucket(int maxSize) : maxSize(maxSize), overflowHead(nullptr) {}

    // Insert a key-value pair
    void insert(const string& key, const string& value) {
        if ((int) entries.size() < maxSize) {
            entries.emplace_back(key, value);
        } else {
            // Use chaining for overflow
            Node* newNode = new Node(key, value);
            newNode->next = overflowHead;
            overflowHead = newNode;
        }
    }

    // Retrieve the value for a given key
    string get(const string& key) const {
        // Check in the fixed-size bucket
        for (const auto& entry : entries) {
            if (entry.first == key) {
                return entry.second;
            }
        }

        // Traverse the linked list if not found in the bucket
        Node* current = overflowHead;
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return ""; // Return empty string if not found
    }

    // Delete the key-value pair for a given key
    bool remove(const string& key) {
        // Check in the fixed-size bucket
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->first == key) {
                entries.erase(it);
                return true;
            }
        }

        // Traverse the linked list for overflow
        Node* current = overflowHead;
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    overflowHead = current->next;
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }

        return false;
    }

    // Destructor to clean up linked list
    ~HashBucket() {
        Node* current = overflowHead;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // Display contents of the bucket for debugging
    void display() const {
        for (const auto& entry : entries) {
            cout << entry.first << " => " << entry.second << " | ";
        }
        Node* current = overflowHead;
        while (current != nullptr) {
            cout << current->key << " => " << current->value << " (chained) | ";
            current = current->next;
        }
    }
};

// HashMap class using buckets with chaining
class HashMap {
    vector<HashBucket*> table;
    int capacity;
    int bucketSize=0;

    // Hash function for strings
    int hashCode(const string& key) const {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % capacity;
    }

public:
    HashMap(int capacity, int bucketSize) : capacity(capacity), bucketSize(bucketSize) {
        table.resize(capacity);
        for (int i = 0; i < capacity; i++) {
            table[i] = new HashBucket(bucketSize);
        }
    }

    ~HashMap() {
        for (auto bucket : table) {
            delete bucket;
        }
    }

    // Insert key-value pair
    void insert(const string& key, const string& value) {
        int index = hashCode(key);
        table[index]->insert(key, value);
    }

    // Retrieve the value for a given key
    string get(const string& key) const {
        int index = hashCode(key);
        return table[index]->get(key);
    }

    // Delete a key-value pair
    bool remove(const string& key) {
        int index = hashCode(key);
        return table[index]->remove(key);
    }

    // Display the hash map
    void display() const {
        for (int i = 0; i < capacity; i++) {
            cout << "Bucket " << i << ": ";
            table[i]->display();
            cout << endl;
        }
    }
};

// Driver function to test the HashMap
int main() {
    int bucketSize;
    cout << "Enter bucket size: ";
    cin >> bucketSize;

    HashMap hashMap(10, bucketSize); // Create a HashMap with 10 buckets

    int choice;
    string key, value;

    while (true) {
        cout << "\n1. Insert, 2. Retrieve, 3. Delete, 4. Display, 5. Exit: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter key (string): ";
                cin >> key;
                cout << "Enter value (string): ";
                cin >> value;
                hashMap.insert(key, value);
                break;
            case 2:
                cout << "Enter key to retrieve (string): ";
                cin >> key;
                value = hashMap.get(key);
                if (!value.empty()) {
                    cout << "Value: " << value << endl;
                } else {
                    cout << "Key not found!" << endl;
                }
                break;
            case 3:
                cout << "Enter key to delete (string): ";
                cin >> key;
                if (hashMap.remove(key)) {
                    cout << "Key deleted successfully." << endl;
                } else {
                    cout << "Key not found!" << endl;
                }
                break;
            case 4:
                hashMap.display();
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
