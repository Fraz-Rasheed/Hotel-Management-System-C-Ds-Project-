#include <iostream>
using namespace std;

const int MAX_FLOORS = 5;
const int MAX_ROOMS_PER_FLOOR = 10;
const int MAX_AVAILABILITY_DAYS = 30;

string intToString(int num) {
    if (num == 0) {
        return "0";
    }
    string result = "";
    bool isNegative = num < 0;

    if (isNegative) {
        num = -num;
    }
    while (num > 0) {
        result = char('0' + (num % 10)) + result;
        num /= 10;
    }

    if (isNegative) {
        result = "-" + result;
    }
    return result;
}

class Queue {
    struct Node {
        string customerName;
        string roomType;
        int nights;
        Node* next;

        Node(string name, string type, int n) {
            customerName = name;
            roomType = type;
            nights = n;
            next = NULL;
        }
    };
    Node* front;
    Node* rear;
    int size;

public:
    Queue() {
        front = NULL;
        rear = NULL;
        size = 0;
    }

    void enqueue(string customerName, string roomType, int nights) {
        Node* newNode = new Node(customerName, roomType, nights);
        if (rear != NULL) {
            rear->next = newNode;
        }
        rear = newNode;
        if (!front) {
            front = rear;
        }
        size++;
    }

    bool dequeue(string& customerName, string& roomType, int& nights) {
        if (isEmpty()) {
            return false;
        }
        Node* temp = front;
        customerName = temp->customerName;
        roomType = temp->roomType;
        nights = temp->nights;
        front = front->next;
        if (!front) {
            rear = NULL;
        }
        delete temp;
        size--;
        return true;
    }

    bool isEmpty() const {
        return size == 0;
    }

    ~Queue() {
        string dummyName, dummyType;
        int dummyNights;
        while (dequeue(dummyName, dummyType, dummyNights));
    }
};

class Stack {
    struct Node {
        string customerName;
        string roomID;
        int nights;
        Node* next;

        Node(string name, string id, int n) {
            customerName = name;
            roomID = id;
            nights = n;
            next = NULL;
        }
    };
    Node* top;

public:
    Stack() {
        top = NULL;
    }

    void push(string customerName, string roomID, int nights) {
        Node* newNode = new Node(customerName, roomID, nights);
        newNode->next = top;
        top = newNode;
    }

    bool pop(string& customerName, string& roomID, int& nights) {
        if (isEmpty()) {
            return false;
        }
        Node* temp = top;
        customerName = temp->customerName;
        roomID = temp->roomID;
        nights = temp->nights;
        top = top->next;
        delete temp;
        return true;
    }

    bool isEmpty() {
        return top == NULL;
    }

    void display() {
        Node* current = top;
        while (current != NULL) {
            cout << "Customer: " << current->customerName
                 << ", Room ID: " << current->roomID
                 << ", Nights: " << current->nights << endl;
            current = current->next;
        }
    }

    ~Stack() {
        string dummyName, dummyID;
        int dummyNights;
        while (pop(dummyName, dummyID, dummyNights));
    }
};

class Room {
public:
    string roomID;
    string roomType;
    string status;
    string availability[MAX_AVAILABILITY_DAYS];

    Room() {
        roomID = "";
        roomType = "";
        status = "Ready";
        for (int i = 0; i < MAX_AVAILABILITY_DAYS; ++i) {
            availability[i] = "Available";
        }
    }

    Room(string id, string type) {
        roomID = id;
        roomType = type;
        status = "Ready";
        for (int i = 0; i < MAX_AVAILABILITY_DAYS; ++i) {
            availability[i] = "Available";
        }
    }
};

class TreeNode {
public:
    string floorID;
    Room rooms[MAX_ROOMS_PER_FLOOR];
    TreeNode* nextFloor;

    TreeNode(string id)  {
	floorID=id;
	nextFloor=NULL;
	}

    void addRoom( Room room, int index) {
        rooms[index] = room;
    }
};

class Hotel {
    TreeNode* root;
    Stack bookingHistory;
    Queue bookingRequests;

public:
    Hotel() {
        root = NULL;
        createHotel();
    }

    void createHotel() {
        for (int i = 0; i < MAX_FLOORS; ++i) {
            string floorID = "Floor " + intToString(i + 1);
            TreeNode* newFloor = new TreeNode(floorID);

            for (int j = 0; j < MAX_ROOMS_PER_FLOOR; ++j) {
                string roomID = "F" + intToString(i + 1) + "R" + intToString(j + 1);
                string roomType = (j % 3 == 0) ? "Single" : (j % 3 == 1) ? "Double" : "Suite";
                Room room(roomID, roomType);
                newFloor->addRoom(room, j);
            }

            if (!root) {
                root = newFloor;
            } else {
                TreeNode* current = root;
                while (current->nextFloor) {
                    current = current->nextFloor;
                }
                current->nextFloor = newFloor;
            }
        }
    }

    void displayRooms()  {
        TreeNode* currentFloor = root;
        while (currentFloor) {
            cout << currentFloor->floorID << ":\n";
            for (int i = 0; i < MAX_ROOMS_PER_FLOOR; ++i) {
                cout << "  Room ID: " << currentFloor->rooms[i].roomID
                     << ", Type: " << currentFloor->rooms[i].roomType
                     << ", Status: " << currentFloor->rooms[i].status << endl;
            }
            currentFloor = currentFloor->nextFloor;
        }
    }

    void bookRoom( string& customerName,  string& roomType, int nights) {
        if (nights > MAX_AVAILABILITY_DAYS) {
            cout << "Cannot book a room for more than " << MAX_AVAILABILITY_DAYS << " days!" << endl;
            return;
        }

        TreeNode* currentFloor = root;
        while (currentFloor) {
            for (int i = 0; i < MAX_ROOMS_PER_FLOOR; ++i) {
                if (currentFloor->rooms[i].roomType == roomType && currentFloor->rooms[i].status == "Ready") {
                    
                    bool isAvailable = true;
                    for (int k = 0; k < nights; ++k) {
                        if (currentFloor->rooms[i].availability[k] == "Booked") {
                            isAvailable = false;
                            break;
                        }
                    }

                    if (isAvailable) {
                        currentFloor->rooms[i].status = "Booked";
                        for (int k = 0; k < nights; ++k) {
                            currentFloor->rooms[i].availability[k] = "Booked";
                        }
                        bookingHistory.push(customerName, currentFloor->rooms[i].roomID, nights);
                        cout << "Room " << currentFloor->rooms[i].roomID << " booked successfully for " << customerName
                             << " for " << nights << " nights." << endl;
                        return;
                    }
                }
            }
            currentFloor = currentFloor->nextFloor;
        }
        cout << "No available rooms of type " << roomType << " for " << nights << " nights!" << endl;
    }

    void cancelBooking() {
        string customerName, roomID;
        int nights;
        if (!bookingHistory.pop(customerName, roomID, nights)) {
            cout << "No booking to cancel!" << endl;
            return;
        }

        TreeNode* currentFloor = root;
        while (currentFloor) {
            for (int i = 0; i < MAX_ROOMS_PER_FLOOR; ++i) {
                if (currentFloor->rooms[i].roomID == roomID) {
                    currentFloor->rooms[i].status = "Ready";
                    for (int k = 0; k < nights; ++k) {
                        currentFloor->rooms[i].availability[k] = "Available";
                    }
                    cout << "Booking for room " << roomID << " has been canceled." << endl;
                    return;
                }
            }
            currentFloor = currentFloor->nextFloor;
        }

        cout << "Error: Room " << roomID << " not found in the records!" << endl;
    }

    void displayBookingHistory() {
        if (bookingHistory.isEmpty()) {
            cout << "No booking history available!" << endl;
        } else {
            bookingHistory.display();
        }
    }
};

int main() {
    Hotel hotel;
    int choice;

    do {
        cout << "\n1. Display Rooms\n2. Book Room\n3. Cancel Booking\n4. Display Booking History\n5. Exit\nChoose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                hotel.displayRooms();
                break;
            case 2: {
                string customerName, roomType;
                int nights;
                cin.ignore();
                cout << "Enter Customer Name: ";
                getline(cin, customerName);
                cout << "Enter Room Type (Single/Double/Suite): ";
                cin >> roomType;
                cout << "Enter Nights: ";
                cin >> nights;
                hotel.bookRoom(customerName, roomType, nights);
                break;
            }
            case 3:
                hotel.cancelBooking();
                break;
            case 4:
                hotel.displayBookingHistory();
                break;
            case 5:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
