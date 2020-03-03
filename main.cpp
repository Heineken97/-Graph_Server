#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

#include <list>

using namespace std;

/*!
 * @brief This class represents a directed graph using adjacency list representation
 */
class Graph
{
    int V;      // Number of vertices
    list<int> *adj; // Pointer to an array containing adjacency lists
public:
    Graph(int V);
    void addEdge(int v, int w); // add an edge to graph
    bool isReachable(int source, int destiny); // returns true if there is a path from source to destiny
};
Graph::Graph(int V){
    this->V = V;
    adj = new list<int> [V];
}
void Graph::addEdge(int v, int w){
    adj[v].push_back(w); // Add w to v’s list.
}

/*!
 * @brief Dijkstra
 * @param int s, int d
 */
//
bool Graph::isReachable(int s, int d){
    // Base case
    if (s == d)
        return true;
    bool *visited = new bool[V]; // Mark all the vertices as not visited
    for (int i = 0; i < V; i++)
        visited[i] = false;
    // Create a queue for BFS
    list<int> queue;
    visited[s] = true;  // Mark the current node as visited and enqueue it
    queue.push_back(s);
    // it will be used to get all adjacent vertices of a vertex
    list<int>::iterator i;
    while (!queue.empty()){
        // Dequeue a vertex from queue and print it
        s = queue.front();
        queue.pop_front();
        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i){
            // If this adjacent node is the destination node, then return true
            if (*i == d)
                return true;
            // Else, continue to do BFS
            if (!visited[*i]){
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }

    return false;

}
/*!
 * @brief error server socket
 */
void error(const char *msg){
    perror(msg);
    exit(1);
}

/*!
 * @brief Server socket and calculing djkstra
 */
int main(int argc, char *argv[]) {
    int socketON, portnumber, newsocketON, thereading;
    struct sockaddr_in server_addr{},client_addr{};
    socklen_t connectionLen;
    char buffer[256];

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    socketON=socket(AF_INET,SOCK_STREAM,0); //Creando socket
    if(socketON<0){
        error("ERROR. Comprobar creacion del socket");
    }

    bzero((char*) &server_addr, sizeof(server_addr)); // Cleaning server address structure
    portnumber=atoi(argv[1]); //portnumber by input

    //Filling the structure
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY; //current Host's IPaddress
    server_addr.sin_port=htons(portnumber);// adding & convert to Network host byte

    //Binding the socket to the current IP address, passing the file descriptor
    if(bind(socketON,(struct sockaddr*) &server_addr, sizeof(server_addr))<0) error ("ERROR. Enlazar el socket");

    //Listening incoming connections with a MAXSIZE backlog queue
    listen(socketON, 5);

    //Accepting the connection from the client
    newsocketON= accept(socketON,(struct sockaddr*) &client_addr, &connectionLen);
    if(newsocketON<0)error("ERROR. Comprobar aceptacion del nuevo socket");

    printf("Checking SERVER connection: got connection from %s port %d \n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    //Sending data
    Graph g(4);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

    send(newsocketON, "Result djkstra be here",100,0);

    //Reading data
    bzero(buffer,256);
    thereading= read(newsocketON,buffer,255);
    if(thereading<0)error("ERROR. La data no llega");

    //Checking
    printf("Heres is the Data: %s\n", buffer);

    //closing
    close(newsocketON);
    close(socketON);
}