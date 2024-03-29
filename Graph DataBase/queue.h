#include "movies.h"

namespace queue {
    typedef movies::VertexNode *Elem;

    struct Node {
        Elem e;
        Node *next;
    };

    struct Queue {
        Node *head;
        Node *tail;
    };

    Queue createEmpty();

    bool isEmpty(const Queue &);

    void addBack(Queue &, Elem);

    Elem popFront(Queue &);
}