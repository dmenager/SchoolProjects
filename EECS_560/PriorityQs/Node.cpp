#include "Node.h"

using namespace std;

Node::Node()
{
  left = NULL;
  right = NULL;
  parent = NULL;
}

Node::Node(int key)
{
  left = NULL;
  right = NULL;
  parent = NULL;

  val = key;
}

Node::~Node()
{/*
  delete left;
  delete right;*/
  left = NULL;
  right = NULL;
  parent = NULL;
  val = 0;
}
