#include "skew_heap.h"
#include <iostream>

using namespace std;

skew_heap::skew_heap()
{
  root = NULL;
}

skew_heap::~skew_heap()
{
  delete root->left;
  delete root->right;
}

void skew_heap::Insert(int val)
{
   root = merge(new Node(val), root);
}

void skew_heap::Merge(skew_heap& rhs)
{
  if(this == &rhs)
    return;

  root = merge(root, rhs.root);
  rhs.root = NULL;
}

Node* skew_heap::merge(Node* h1, Node* h2)
{
  if(h1 == NULL)
    return h2;
  if(h2 == NULL)
    return h1;

  if(h1->val < h2->val)
  {
    return merge1(h1, h2);
  }
  else
  {
    return merge1(h2, h1);
  }
}

Node* skew_heap::merge1(Node* h1, Node* h2)
{
  if(h1->left == NULL)
  {
    h1->left = h2;
  }
  else
  {
    h1->right = merge(h1->right, h2);
    swapChildren(h1);
  }

  return h1;
}

void skew_heap::DeleteMin()
{
  if(root == NULL)
  {
    throw DeleteException("INVALD HEAP OPERATION: Heap already empty.\n");
  }

  Node* oldRoot = root;

  root = merge(root->left, root->right);
  delete oldRoot;
}

void skew_heap::swapChildren(Node* h1)
{
  Node* temp = h1->left;

  h1->left = h1->right;
  h1->right = temp;
}

void skew_heap::levelOrder()
{
  //q is used only for verification!!!!
  queue<Node*> q;
  //q is used only for verification!!!!
  
  if(root)
  {
    q.push(root);
    cout << "| " << root->val;
  }
  else
  {
    cout << "NULL\n";
  }
  
  while(!q.empty())
  {
    const Node* const temp_node = q.front();
    q.pop();

    if(temp_node->left)
    {
      q.push(temp_node->left);
      cout << " | " << temp_node->left->val;
    }

    if(temp_node->right)
    {
      q.push(temp_node->right);
      cout << " | " << temp_node->right->val;
    }
  }
  cout << "|\n";
}
