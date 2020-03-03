#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define N_NODES 6

typedef struct _node _node;
typedef struct _edge _edge;

struct _node{
    _edge *edge;
    _node *destiny;
    double distance;
    char name[8];
    int heap_idx;  //heap position on cost updating
};

struct _edge{
    _edge *sibling;
    _node *nd;
    int cost;

};

#define BLOCK_SIZE 15
_edge *edge_root = 0, *edge_next = 0;

void add_edge(_node *node1, _node *node2, double c ){
    if(edge_next == edge_root){
        edge_root = (_edge*)malloc(sizeof(_edge)*(BLOCK_SIZE+1));
        edge_root[BLOCK_SIZE].sibling = edge_next;
        edge_next = edge_root+BLOCK_SIZE;
    }
    --edge_next;

    edge_next->nd = node2;
    edge_next->cost = c;
    edge_next->sibling = node1->edge;
    node1->edge = edge_next;
}

void free_edges(){
    for( ; edge_root; edge_root = edge_next){
        edge_next = edge_root[BLOCK_SIZE].sibling;
        free(edge_root);
    }
}

_node **heap;
int heap_len;

void set_distance(_node *nd, _node *destiny, double d){
    int i,j;

    if(nd->destiny && d >= nd->distance)return;

    //Crear o buscar entrada al heap
    nd->distance = d;
    nd->destiny = destiny;
    i = nd->heap_idx;
    if(!i)i = ++heap_len;

    for(; i>1 && nd->distance < heap[j=i/2]->distance; i=j)
        (heap[i]=heap[j])->heap_idx=i;

    heap[i]=nd;
    nd->heap_idx=i;
}

_node *pop_queue(){
    _node *nd, *tmp;
    int i,j;

    if(!heap_len)return 0;
    //remove lead element, pull tail on lead and downheap
    nd=heap[1];
    tmp=heap[heap_len--];

    for(i=1; i<heap_len && (j=i*2)<= heap_len; i=j){
        if( j< heap_len && heap[j]->distance > heap[j+1]->distance) j++;
        if( heap[j]->distance >= tmp->distance) break;

        (heap[i]=heap[j])->heap_idx = i;
    }

    heap[i]=tmp;
    tmp-> heap_idx=i;

    return nd;
}

void calc_dijkstra(_node *start){
    _node *lead;
    _edge *edgie;

    set_distance(start,start,0);
    while((lead=pop_queue()))
        for (edgie = lead->edge; edgie; edgie= edgie->sibling)
            set_distance(edgie->nd, lead, lead->distance + edgie->cost);

}

void showme_path(_node *nd){
    if(nd->destiny == nd)
        printf("%s", nd->name);
    else if(!nd->destiny)
        printf("%s(unreached node) ", nd->name);
    else{
        showme_path(nd->destiny);
        printf("-> %s(%g) ", nd->name, nd->distance );
    }
}

void error(const char *msg){
    perror(msg);
    exit(1);
}

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

    //Dojkstra
    _node *nodes = (_node*) calloc(sizeof(_node), N_NODES);
    for (int i = 0; i < N_NODES; i++)
        sprintf(nodes[i].name, "%c", 'a' + i);

    add_edge(nodes,   nodes+1,  7);   //a-b
    add_edge(nodes,   nodes+2,  9);   //a-c
    add_edge(nodes,   nodes+5, 14);   //a-f
    add_edge(nodes+1, nodes+2, 10);   //b-c
    add_edge(nodes+1, nodes+3, 15);   //b-d
    add_edge(nodes+2, nodes+3, 11);   //c-d
    add_edge(nodes+2, nodes+5,  2);   //c-f
    add_edge(nodes+3, nodes+4,  6);   //d-e
    add_edge(nodes+4, nodes+5,  9);   //e-f

    heap = (_node**)calloc(sizeof(_node), N_NODES + 1);
    heap_len = 0;

    calc_dijkstra(nodes);

    //Sending data
    send(newsocketON, nodes,14636,0);

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