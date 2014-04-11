#ifndef SKEW_H
#define SKEW_H

#include "Node.h"

#include <exception>
#include <cstring>
#include <string>

/***ONLY FOR VERIFICATION. THIS IS NOT USED IN THE ACTUAL IMPLEMENTATION***/
#include <queue>
/***ONLY FOR VERIFICATION. THIS IS NOT USED IN THE ACTUAL IMPLEMENTATION***/

class skew_heap
{
  public:

  public:
    skew_heap();
    virtual ~skew_heap();

    void Insert(int val);
    void DeleteMin();
    void Merge(skew_heap& rhs);
    void levelOrder();

  private:
    Node* merge(Node* h1, Node* h2);
    Node* merge1(Node* h1, Node* h2);
    void swapChildren(Node* h1);

  private:
    Node* root;
    struct DeleteException : std::exception
    {
      std::string msg;
      DeleteException(std::string ss) : msg(ss) { }
      ~DeleteException() throw() {}
      const char* what() const throw() {return msg.c_str();}
    };
};
#endif
