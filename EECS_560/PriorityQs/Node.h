#ifndef NODE_H
#define NODE_H

#include <cstddef>

class Node
{
  public:
    Node* left;
    Node* right;
    Node* parent;
  
    int val;

  public:
    Node();
    Node(int key);
    virtual ~Node();
};
#endif
