extern "C" {
    unsigned long* kmalloc(int size);
};

template<class T>
class CircularLinkedList
{
private:
    struct Node
    {
        T data;
        Node* next;
    };


public:
    Node *startNode;
    Node *currentNode;

    Node *counterNode;

    CircularLinkedList(T node)
    {
        startNode = (Node*)kmalloc(sizeof(Node));
        startNode->data = node;
        startNode->next = startNode;
        currentNode = startNode;

        counterNode = startNode;
    }
    CircularLinkedList(){}
    void addStartNode(T node)
    {
        startNode = (Node*)kmalloc(sizeof(Node));
        startNode->data = node;
        startNode->next = startNode;
    }
    void addNode(T node)
    {
        currentNode->next = (Node*)kmalloc(sizeof(Node));
        currentNode = currentNode->next;
        currentNode->data = node;
        currentNode->next = startNode;
    }
    void IncramentNode()
    {
        counterNode = counterNode->next;
    }
};