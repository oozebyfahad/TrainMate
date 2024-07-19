#include <iostream>
#include <cstdlib>
#include <cstring>
#include <list>
#include <limits>
#include <fstream>
#include<vector>
#include<algorithm>
using namespace std;

int shortestDistance = 10000000;

ofstream outputFile;

struct Fare
{
    string source;
    string destination;
    int fare;
    Fare *next;
    Fare *prev;
};

Fare *first = nullptr, *last = nullptr;
void insertFare(string source, string destination, int fare)
{
    Fare *newFare = new Fare;
    newFare->source = source;
    newFare->destination = destination;
    newFare->fare = fare;

    if (first == NULL)
    {
        first = last = newFare;
        newFare->next = NULL;
        newFare->prev = NULL;
    }
    else
    {
        last->next = newFare;
        newFare->prev = last;
        newFare->next = NULL;
        last = newFare;
    }
}

int searchFare(string source, string destination)
{
    Fare *current = first;
    while (current != nullptr)
    {
        if (current->source == source && current->destination == destination)
        {
            return current->fare;
        }
        current = current->next;
    }

    return 0;
}

// stack to store routes
struct Train
{
    string name;
    string source;
    string destination;
    string arrivalTime;
    string departureTime;
    int distance;
    int fare;
    int capacity;
};

const int MAX_SIZE = 100; // Maximum size of the stack

Train trainStack[MAX_SIZE]; // Stack array

int top = -1; // Variable to keep track of the top of the stack

bool isempty() {
    return (top == -1);
}

bool isfull() {
    return (top == MAX_SIZE - 1);
}

void push(const Train& train) {
    if (isfull()) {
        cout << "Stack overflow!\n";
        return;
    }
    trainStack[++top] = train;
}

void pop() {
    if (isempty()) {
        cout << "Stack underflow!\n";
        return;
    }
    --top;
}

Train& topElement() {
    return trainStack[top]; // Return a reference to the top element of the stack
}

void displayStack() {
    if (isempty()) {
        cout << "No routes currently. Kindly add new train routes\n";
        return;
    }
    
    cout << "displaying all the routes of our trains:\n";
    for (int i = 0; i <= top; ++i) {
        cout << "Train Name: " << trainStack[i].name << "\n";
        cout << "Source Station: " << trainStack[i].source << "\n";
        cout << "Destination Station: " << trainStack[i].destination << "\n";
        cout << "Arrival Time: " << trainStack[i].arrivalTime << "\n";
        cout << "Departure Time: " << trainStack[i].departureTime << "\n";
        cout << "Distance: " << trainStack[i].distance << "\n";
        cout << "Fare per ticket: " << trainStack[i].fare << "\n";
        cout << "\n";
    }
}
void popTrainBySourceAndDestination(const string& source, const string& destination) {
    if (isempty()) {
        cout << "No routes currently. Kindly add new train routes\n";
        return;
    }
    
    // Iterate through the stack from top to bottom
    for (int i = top; i >= 0; --i) {
        if (trainStack[i].source == source && trainStack[i].destination == destination) 
            
            // Shift all elements above the popped train down by one position
            for (int j = i; j < top; ++j) {
                trainStack[j] = trainStack[j + 1];
            }
            
            --top; // Decrease the top index to reflect the popped train
            return;
        }
    }
    

struct Node
{
    Train train;
    int weight; // edge weight (distance)
    Node *next;
};
const int MAX_VERTICES = 100; // Maximum number of vertices (stations)

Node *adjList[MAX_VERTICES] = {nullptr};
int V = 0; // Number of stations

void addTrain(const Train &train)
{
    int sourceIndex = -1;
    int destinationIndex = -1;

    // Check if source station already exists in the graph
    for (int i = 0; i < V; i++)
    {
        if (adjList[i] != nullptr && adjList[i]->train.source == train.source)
        {
            sourceIndex = i;
            break;
        }
    }

    // If source station does not exist, create a new node for it
    if (sourceIndex == -1)
    {
        Node *newNode = new Node;
        newNode->train = train;
        newNode->weight = 0; // Set initial weight to 0 for source node
        newNode->next = nullptr;
        sourceIndex = V;
        adjList[sourceIndex] = newNode;
        V++;
    }

    // Add an edge from source to destination (train schedule) if not already added
    bool found = false;
    Node *temp = adjList[sourceIndex];
    while (temp->next != nullptr)
    {
        temp = temp->next;
        if (temp->train.destination == train.destination)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        Node *newNode = new Node;
        newNode->train = train;
        newNode->weight = train.distance; // Assign the edge weight as train.distance
        newNode->next = nullptr;
        temp->next = newNode;
    }
}

void deleteTrain(const string &source, const string &destination)
{
    int sourceIndex = -1;

    // Check if source station already exists in the graph
    for (int i = 0; i < V; i++)
    {
        if (adjList[i] != nullptr && adjList[i]->train.source == source)
        {
            sourceIndex = i;
            break;
        }
    }

    // If source station does not exist, print a message and return
    if (sourceIndex == -1)
    {
        cout << "No such train is scheduled." << endl;
        return;
    }

    // Search for the train schedule to be deleted
    Node *prev = adjList[sourceIndex];
    Node *current = prev->next;

    while (current != nullptr)
    {
        if (current->train.destination == destination)
        {
            // Delete the train schedule
            prev->next = current->next;
            delete current;
            popTrainBySourceAndDestination(source,destination);
            cout << "Train schedule removed successfully." << endl;
            return;
        }

        prev = current;
        current = current->next;
    }

    // If the train schedule to be deleted does not exist, print a message
    cout << "No such train is scheduled." << endl;
}

void printGraph()
{
    for (int i = 0; i < V; i++)
    {
        cout << "checking details for Station: " << adjList[i]->train.source << endl;
        Node *temp = adjList[i]->next;
        while (temp != nullptr)
        {
            cout << "Train Name: " << temp->train.name << endl;
            cout << "Destination: " << temp->train.destination << endl;
            cout << "Arrival Time: " << temp->train.arrivalTime << endl;
            cout << "Departure Time: " << temp->train.departureTime << endl;
            cout << "Distance: " << temp->train.distance << endl;
            cout << "Fare per ticket: " << temp->train.fare << endl;
            cout << endl;
            temp = temp->next;
        }
        cout << endl;
        cout << "details completed" << endl;
    }
}

int getSourceIndex(const string &source)
{
    for (int i = 0; i < V; i++)
    {
        if (adjList[i] != nullptr && adjList[i]->train.destination == source)
        {
            return i;
        }
    }
    return -1;
}

int getDestinationIndex(const string &destination)
{
    for (int i = 0; i < V; i++)
    {
        if (adjList[i] != nullptr && adjList[i]->train.source == destination)
        {
            return i;
        }
    }
    return -1;
}

void dfsTraversal(int vertex, const string &destination, bool *visited, const list<Train> &path, bool &found)
{
    visited[vertex] = true;

    if (adjList[vertex] == nullptr)
    {
        visited[vertex] = false;
        return;
    }

    Node *temp = adjList[vertex]->next;

    while (temp != nullptr)
    {
        if (!visited[getDestinationIndex(temp->train.destination)])
        {
            // Adding source to destination stations along with intermediate stations' details in list
            int count = 1;
            list<Train> newPath = path;
            newPath.push_back(temp->train);

            if (temp->train.destination == destination)
            {
                cout << "Train Route:" << count << endl;
                auto it = newPath.begin();
                count = count + 1;
                // Iterate through all the stations details added in the list
                for (const auto &station : newPath)
                {
                    if (it != newPath.begin())
                    {
                        cout << "Station: " << station.source << endl;
                        cout << "Train Name: " << station.name << endl;
                        cout << "Destination: " << station.destination << endl;
                        cout << "Arrival Time: " << station.arrivalTime << endl;
                        cout << "Departure Time: " << station.departureTime << endl;
                        cout << "Distance: " << station.distance << endl;
                        cout << "Fare per ticket: " << station.fare << endl;
                        cout << endl;
                    }
                    ++it;
                }
                cout << endl;
                found = true; // Set found to true when reaching the destination via all intermediate stations
            }

            dfsTraversal(getDestinationIndex(temp->train.destination), destination, visited, newPath, found);
        }

        temp = temp->next;
    }

    visited[vertex] = false;
}

void shortest_route(int vertex, const string &destination, bool *visited, const list<Train> &path, bool &found, int &shortestDistance, list<Train> &shortestRoute)
{
    visited[vertex] = true;

    if (adjList[vertex] == nullptr)
    {
        visited[vertex] = false;
        return;
    }

    Node *temp = adjList[vertex]->next;

    while (temp != nullptr)
    {
        if (!visited[getDestinationIndex(temp->train.destination)])
        {
            // Adding source to destination stations along with intermediate stations' details in list
            list<Train> newPath = path;
            newPath.push_back(temp->train);

            if (temp->train.destination == destination)
            {
                // Calculate the total distance of the current route
                int totalDistance = 0;
                for (const auto &station : newPath)
                {
                    totalDistance += station.distance;
                }

                // Check if the current route has the shortest distance
                if (totalDistance < shortestDistance)
                {
                    shortestDistance = totalDistance;
                    shortestRoute = newPath; // Store the current route with the shortest distance
                }

                found = true; // Set found to true when reaching the destination via all intermediate stations
            }

            shortest_route(getDestinationIndex(temp->train.destination), destination, visited, newPath, found, shortestDistance, shortestRoute);
        }

        temp = temp->next;
    }

    visited[vertex] = false;
}

bool normalRoute(const string &source, const string &destination){
    for (int i = 0; i <= top; ++i) {

        if (trainStack[i].source==source && trainStack[i].destination==destination)
        {
            return true;
        }
        
    }
    return false;
}


int searchTrain(const string &source, const string &destination)
{   
    bool normal_route;
    bool found = false;
    int shortestDistance = 1000000;
    list <Train> temp;
    list<Train> shortestRoute;
    bool *visited = new bool[V];
    memset(visited, false, sizeof(bool) * V);

    // Traverse the adjacency list using DFS to find routes between source and destination
    for (int i = 0; i < V; i++)
    {
        if (adjList[i] != nullptr && adjList[i]->train.source == source)
        {
            list<Train> path;
            path.push_back(adjList[i]->train);
            dfsTraversal(i, destination, visited, path, found);
            shortest_route(i, destination, visited, path, found, shortestDistance, shortestRoute);
            
            Node *temp = adjList[i]->next;
            while (temp != nullptr)
            {
                if (temp->train.destination == destination)
                {
                    found = true;
                    break;
                }
                temp = temp->next;
            }
        }
    }

    delete[] visited;

    //check if there is direct route or not
    normal_route=normalRoute(source,destination);

    int fare = 0;
    if (normal_route){
        cout << "Do you want to take shortest route?(y/n)" << endl;
        char a;
        cin >> a;
        if (a == 'y')
        {
            if (found)
            {
                cout << "as you have seen possible routes to your destination, we are here taking you from the shortest route" << endl;
                cout << "Below is the shortest route on the basis of shortest distance from source to destination" << endl;
                cout << "Shortest Distance: " << shortestDistance << endl;
                cout << "Shortest Route: " << endl;
                auto it = shortestRoute.begin();
                int count = 1;
                for (const auto &station : shortestRoute)
                {
                    if (it != shortestRoute.begin())
                    {
                        fare = fare + searchFare(station.source, station.destination);
                        cout << "Station: " << station.source << endl;
                        cout << "Train Name: " << station.name << endl;
                        cout << "Destination: " << station.destination << endl;
                        cout << "Arrival Time: " << station.arrivalTime << endl;
                        cout << "Departure Time: " << station.departureTime << endl;
                        cout << "Distance: " << station.distance << endl;
                        cout << "Fare per ticket: " << station.fare << endl;
                        cout << endl;
                    }
                    ++it;
                    count++;
                }
                // return fare;
            }
            else if (!found)
            {
                cout << "No trains found for the given source and destination." << endl;
                return fare;
            }
        }
        else if (a == 'n')
        {
                cout<<"so normal route details are given below"<<endl;
                for (int i = 0; i <= top; ++i) {
                    if (trainStack[i].source==source && trainStack[i].destination==destination){
                        fare = fare + searchFare(trainStack[i].source, trainStack[i].destination);
                cout << "Train Name: " << trainStack[i].name << "\n";
                cout << "Source Station: " << trainStack[i].source << "\n";
                cout << "Destination Station: " << trainStack[i].destination << "\n";
                cout << "Arrival Time: " << trainStack[i].arrivalTime << "\n";
                cout << "Departure Time: " << trainStack[i].departureTime << "\n";
                cout << "Distance: " << trainStack[i].distance << "\n";
                cout << "Fare per ticket: " << trainStack[i].fare << "\n";
                cout << "\n";
                return fare;
    }}
                 
        }

    }
    else{
        if (found)
            {
                cout << "as you have seen possible routes to your destination, we are here taking you from the shortest route because there is no normal route" << endl;
                cout << "Below is the shortest route on the basis of shortest distance from source to destination" << endl;
                cout << "Shortest Distance: " << shortestDistance << endl;
                cout << "Shortest Route: " << endl;
                auto it = shortestRoute.begin();
                int count = 1;
                for (const auto &station : shortestRoute)
                {
                    if (it != shortestRoute.begin())
                    {
                        fare = fare + searchFare(station.source, station.destination);
                        cout << "Station: " << station.source << endl;
                        cout << "Train Name: " << station.name << endl;
                        cout << "Destination: " << station.destination << endl;
                        cout << "Arrival Time: " << station.arrivalTime << endl;
                        cout << "Departure Time: " << station.departureTime << endl;
                        cout << "Distance: " << station.distance << endl;
                        cout << "Fare per ticket: " << station.fare << endl;
                        cout << endl;
                    }
                    ++it;
                    count++;
                }
                // return fare;
            }
            else if (!found)
            {
                cout << "No trains found for the given source and destination." << endl;
                return fare;
            }
    }
    return fare;

}

int getShortestPathIndex(int *dist, bool *visited, int V)
{
    int minDist = numeric_limits<int>::max();
    int minIndex = -1;
    for (int i = 0; i < V; i++)
    {
        if (!visited[i] && dist[i] <= minDist)
        {
            minDist = dist[i];
            minIndex = i;
        }
    }

    return minIndex;
}

struct Ticket
{
    string passenger_name;
    string source;
    string destination;
    int numTickets;
    bool isRoundTrip;
    int price;
    char classType;
};

struct node
{
    Ticket data;
    node *next;
};

node *front = nullptr;
node *rear = nullptr;

bool isEmpty()
{
    return (front == nullptr);
}

void enqueue(Ticket ticket)
{
    node *newNode = new node;
    newNode->data = ticket;
    newNode->next = nullptr;

    if (rear == nullptr)
    {
        front = rear = newNode;
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
    }
}

void dequeue()
{
    if (isEmpty())
    {
        cout << "Queue is empty." <<endl;
    }
    else
    {
        node *temp = front;
        front = front->next;

        if (front == nullptr)
        {
            rear = nullptr;
        }

        delete temp;
    }
}

void bookTicket()
{
    Ticket ticket;

    cout << "enter your name :";
    cin >> ticket.passenger_name;
    cout << "Enter source: ";
    cin >> ticket.source;

    cout << "Enter destination: ";
    cin >> ticket.destination;

    ticket.price = searchTrain(ticket.source, ticket.destination);
    if (ticket.price == 0)
    {
        cout << "kindly check schedule train and then book your tickets";
        cout << endl;
    }
    else
    {

        cout << "ticket price per person is : " << ticket.price << endl;
        cout << "Enter number of tickets: ";
        cin >> ticket.numTickets;

        cout << "Is it a round trip? (1 for Yes, 0 for No): ";
        cin >> ticket.isRoundTrip;
        if (ticket.isRoundTrip == 1)
        {
            // ticket_price = ticket_price + ticket_price;
            ticket.price = ticket.price + ticket.price;
        }
        cout << "Enter ticket class type.Enter b. for business class/ Enter e. for economy\n";
        cin >> ticket.classType;

        if (ticket.classType == 'e')
        {
            int fare = ticket.price * ticket.numTickets;
            ticket.price = fare;
        }
        else if (ticket.classType == 'b')
        {

            int fare = ((ticket.price * ticket.numTickets * 0.3) + ticket.price * ticket.numTickets);
            ticket.price = fare;
        }

        enqueue(ticket);

        cout << "Ticket booked successfully!" <<endl;
    }
}
void processTickets()
{
    while (!isEmpty())
    {
        Ticket ticket = front->data;
        dequeue();
        /// Write ticket information to the file
        outputFile << "Passenger name: " << ticket.passenger_name << endl;
        outputFile << "Source: " << ticket.source << endl;
        outputFile << "Destination: " << ticket.destination << endl;
        outputFile << "Number of tickets: " << ticket.numTickets << endl;
        outputFile << "Round trip: " << (ticket.isRoundTrip ? "Yes" : "No") << endl;
        outputFile << "Total fare: $" << ticket.price << endl;
        outputFile << endl;

        cout << "Passenger name:" << ticket.passenger_name << endl;
        cout << "Source: " << ticket.source << endl;
        cout << "Destination: " << ticket.destination << endl;
        cout << "Number of tickets: " << ticket.numTickets << endl;
        cout << "Round trip: " << (ticket.isRoundTrip ? "Yes" : "No") << endl;
        cout << "Total fare: $" << ticket.price << endl;
        cout << endl;
    }
}
class TrainHeap {
public:
    void heapify();
    void push(Train train);
    Train pop();
    int size();
    bool isEmpty() const {
        return trains.empty();
    }
    
private:
    std::vector<Train> trains;
    int left(int i) { return 2*i + 1; }
    int right(int i) { return 2*i + 2; }
    
    void maxHeapify(int i);  
};

void TrainHeap::heapify() {
    for (int i = trains.size()/2 - 1; i >= 0; i--) {
        maxHeapify(i);
    }
}

void TrainHeap::maxHeapify(int i) {
    int l = left(i);
    int r = right(i); 
    int largest = i;
    if (l < trains.size() && trains[l].capacity > trains[i].capacity) 
        largest = l;
    if (r < trains.size() && trains[r].capacity > trains[largest].capacity)
        largest = r;  
    if (largest != i) {
        std::swap(trains[i], trains[largest]);
        maxHeapify(largest);
    }
}

void TrainHeap::push(Train train) {
    trains.push_back(train);
    int current = trains.size() - 1;  
    while (current > 0 && trains[(current - 1)/2].capacity < trains[current].capacity) {
        std::swap(trains[(current - 1)/2], trains[current]); 
        current = (current - 1) / 2;  
    }
}

Train TrainHeap::pop() {
    Train root = trains[0];  
    trains[0] = trains[trains.size()-1];
    trains.pop_back();      
    maxHeapify(0);
    return root;  
}
int TrainHeap::size(){
	int size;
	for(int i = 0; i<trains.size();i++){
		size++;
	}
	return size;
}

int main()
{
    Train train1;
    train1.name = "Train 1";
    train1.source = "rawalpindi";
    train1.destination = "lahore";
    train1.arrivalTime = "4pm";
    train1.departureTime = "11am";
    train1.distance = 450;
    train1.fare = 47;
    addTrain(train1);
    insertFare(train1.source, train1.destination, train1.fare);
    push(train1); //adding to stack
    train1.capacity = 100;

    Train train2;
    train2.name = "Train 2";
    train2.source = "lahore";
    train2.destination = "karachi";
    train2.arrivalTime = "11:00 AM";
    train2.departureTime = "11:30 AM";
    train2.distance = 500;
    train2.fare = 30;
    addTrain(train2);
    insertFare(train2.source, train2.destination, train2.fare);
    push(train2); //adding to stack
    train2.capacity = 200;
  
    Train train3;
    train3.name = "Train 3";
    train3.source = "rawalpindi";
    train3.destination = "multan";
    train3.arrivalTime = "12:00 PM";
    train3.departureTime = "12:30 PM";
    train3.distance = 350;
    train3.fare = 40;
    addTrain(train3);
    insertFare(train3.source, train3.destination, train3.fare);
    push(train3); //adding to stack
    train3.capacity = 300;


    Train train4;
    train4.name = "Train 4";
    train4.source = "multan";
    train4.destination = "quetta";
    train4.arrivalTime = "1:00 PM";
    train4.departureTime = "6:30 PM";
    train4.distance = 340;
    train4.fare = 25;
    addTrain(train4);
    insertFare(train4.source, train4.destination, train4.fare);
    push(train4); //adding to stack
    train4.capacity = 400;


    Train train5;
    train5.name = "Train 4";
    train5.source = "rawalpindi";
    train5.destination = "karachi";
    train5.arrivalTime = "1:00 PM";
    train5.departureTime = "6:30 PM";
    train5.distance = 1000;
    train5.fare = 90;
    train5.capacity = 400;
    
    addTrain(train5);
    insertFare(train5.source, train5.destination, train5.fare);
    push(train5); //adding to stack
    
    TrainHeap trainHeap; 
    trainHeap.push(train1);
trainHeap.push(train2);
trainHeap.push(train3);
trainHeap.push(train4);
trainHeap.push(train5);
trainHeap.heapify();

    string source, destination;
    // Create an array of linked lists (adj

    int choice;
    bool access = true;
    string userType;
    outputFile.open("ticket_information.txt", ios::out);

    while (true)
    {
        cout << "Welcome To Train!" << endl;
        cout << "Are you a client or an admin?" << endl;
        cout << "Enter 0 for client/ 1. for admin" << endl;
        cout << endl;
        cin >> userType;

        if (userType == "0")
        {
            while (true)
            {
                cout << "Please choose an option:" << endl;
                cout << "1. View train schedule" << endl;
                cout << "2. Book a ticket" << endl;
                cout << "3. process a ticket" << endl;
                cout << "4. Return to main menu" << endl;
                cout << "5. Exit" << endl;

                cin >> choice;
                cout << endl;

                if (choice == 1)
                {
                    printGraph();
                }
                else if (choice == 2)
                {
                    bookTicket();
                }
                else if (choice == 3)
                {
                    processTickets();
                }
                else if (choice == 4)
                {
                    cout << "Returning to main menu." << endl;
                    break;
                }
                else if (choice == 5)
                {
                    cout << "Exiting program." << endl;
                    return 0;
                }
                else
                {
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
        }
        else if (userType == "1")
        {
            string pword;
            string password = "12345";
            cout << "Enter the password: ";
            cin >> pword;

            if (pword == password)
            {
                cout << "Admin access granted." << endl;
                access = true;
            }
            else
            {
                cout << "Invalid password. Access denied." << endl;
                // Handle the case when the password is incorrect
                access = false;
            }
            if (access != false)
            {
                while (true)
                {
                    cout << "Please choose an option:" << endl;
                    cout << "1. Train scheduling system" << endl;
                    cout << "2. View train schedule" << endl;
                    cout << "3. Delete train route" << endl;
                    cout<<"4. Show Train Capacities"<<endl;
                    cout << "5. Return to main menu" << endl;
                    cout << "6. Exit" << endl;

                    cin >> choice;
                    cout << endl;

                    if (choice == 1)
                    {
                        Train train;
                        cout << "Enter Train Name: ";
                        cin >> train.name;
                        cout << "Enter Source Station: ";
                        cin >> train.source;
                        cout << "Enter Destination Station: ";
                        cin >> train.destination;
                        cout << "Enter Arrival Time: ";
                        cin >> train.arrivalTime;
                        cout << "Enter Departure Time: ";
                        cin >> train.departureTime;
                        cout << "Enter Distance: ";
                        cin >> train.distance;
                        cout << "Enter Fare per ticket: ";
                        cin >> train.fare;
                        addTrain(train);
                        insertFare(train.source, train.destination, train.fare);
                        push(train);
                        cout << "Train added successfully!" << endl;
                    }
                    else if (choice == 2)
                    {
                        displayStack();
                    }
                    else if (choice == 3)
                    {
                        cout << "Enter source: ";
                        cin >> source;

                        cout << "Enter destination: ";
                        cin >> destination;
                        deleteTrain(source, destination);
                    }
                    else if(choice == 4){
                    	 while (!trainHeap.isEmpty()) {
                             Train train = trainHeap.pop();
                              cout << "Train: " << train.name << ", Capacity: " << train.capacity <<endl;
					}
				}

                    else if (choice == 5)
                    {
                        cout << "Returning to main menu." << endl;
                        break;
                    }
                    else if (choice == 6)
                    {
                        cout << "Exiting program." << endl;
                        return 0;
                    }
                    else
                    {
                        cout << "Invalid choice. Please try again." << endl;
                    }
                }
            }
            else
            {
                cout << "Invalid user type. Please try again." << endl;
            }
        }

        cout << endl;
    }

    int option;
    do
    {
        cout << "Train Management System" << endl;
        cout << "1. Add Train" << endl;            // this will be handled by admin only
        cout << "2. Print Train Schedule" << endl; // can be displayed to user the complete train schedule
        cout << "3. Search Train" << endl;         // ok this can be helpful when we use queue to book tickets we will tell them which route they can use to reach there destination etc.
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> option;

        switch (option)
        {
        case 1:
        {
            Train train;
            cout << "Enter Train Name: ";
            cin >> train.name;
            cout << "Enter Source Station: ";
            cin >> train.source;
            cout << "Enter Destination Station: ";
            cin >> train.destination;
            cout << "Enter Arrival Time: ";
            cin >> train.arrivalTime;
            cout << "Enter Departure Time: ";
            cin >> train.departureTime;
            cout << "Enter Distance: ";
            cin >> train.distance;
            cout << "Enter Fare per ticket: ";
            cin >> train.fare;
            addTrain(train);

            cout << "Train added successfully!" << endl;

            break;
        }
        case 2:
            cout << "Train Schedule:" << endl;
            printGraph();
            break;
        case 3:
            cout << "Enter Source Station: ";
            cin >> source;
            cout << "Enter Destination Station: ";
            cin >> destination;
            cout << "Matching Trains:" << endl;
            searchTrain(source, destination);
            cout << "done all paths " << endl;
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
        cout << endl;
    } while (option != 4);

    outputFile.close();
    return 0;
}

