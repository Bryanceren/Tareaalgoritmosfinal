
#include <iostream> 
#include <queue> 
using namespace std; 
  
enum COLOR { RED, BLACK }; 
  
class Node { 
public: 
  int val; 
  COLOR color; 
  Node *left, *right, *parent; 
  
  Node(int val) : val(val) { 
    parent = left = right = NULL; 
    color = RED; 
  } 
  
  Node *uncle() { 
    if (parent == NULL or parent->parent == NULL) 
      return NULL; 
  
    if (parent->isOnLeft()) 
      return parent->parent->right; 
    else
      return parent->parent->left; 
  } 
   
  bool isOnLeft() { return this == parent->left; } 

  Node *sibling() { 
    if (parent == NULL) 
      return NULL; 
  
    if (isOnLeft()) 
      return parent->right; 
  
    return parent->left; 
  } 
  
  void moveDown(Node *nParent) { 
    if (parent != NULL) { 
      if (isOnLeft()) { 
        parent->left = nParent; 
      } else { 
        parent->right = nParent; 
      } 
    } 
    nParent->parent = parent; 
    parent = nParent; 
  } 
  
  bool hasRedChild() { 
    return (left != NULL and left->color == RED) or 
           (right != NULL and right->color == RED); 
  } 
}; 
  
class RBTree { 
  Node *root; 
  void leftRotate(Node *x) { 
    Node *nParent = x->right; 
      if (x == root) 
      root = nParent; 
  
    x->moveDown(nParent); 
    x->right = nParent->left; 

    if (nParent->left != NULL) 
      nParent->left->parent = x; 
      nParent->left = x; 
  } 
  
  void rightRotate(Node *x) { 
    Node *nParent = x->left; 
  
    if (x == root) 
      root = nParent; 
  
    x->moveDown(nParent); 

    x->left = nParent->right; 
   
    if (nParent->right != NULL) 
      nParent->right->parent = x; 
  
    nParent->right = x; 
  } 
  
  void swapColors(Node *x1, Node *x2) { 
    COLOR temp; 
    temp = x1->color; 
    x1->color = x2->color; 
    x2->color = temp; 
  } 
  
  void swapValues(Node *u, Node *v) { 
    int temp; 
    temp = u->val; 
    u->val = v->val; 
    v->val = temp; 
  } 
  
  void fixRedRed(Node *x) { 
    if (x == root) { 
      x->color = BLACK; 
      return; 
    } 
  
    Node *parent = x->parent, *grandparent = parent->parent, 
         *uncle = x->uncle(); 
  
    if (parent->color != BLACK) { 
      if (uncle != NULL && uncle->color == RED) { 
        
        parent->color = BLACK; 
        uncle->color = BLACK; 
        grandparent->color = RED; 
        fixRedRed(grandparent); 
      } else { 
        
        if (parent->isOnLeft()) { 
          if (x->isOnLeft()) { 
          
            swapColors(parent, grandparent); 
          } else { 
            leftRotate(parent); 
            swapColors(x, grandparent); 
          } 
          
          rightRotate(grandparent); 
        } else { 
          if (x->isOnLeft()) { 
            
            rightRotate(parent); 
            swapColors(x, grandparent); 
          } else { 
            swapColors(parent, grandparent); 
          } 
  
         
          leftRotate(grandparent); 
        } 
      } 
    } 
  } 
  

  Node *successor(Node *x) { 
    Node *temp = x; 
  
    while (temp->left != NULL) 
      temp = temp->left; 
  
    return temp; 
  } 
  
  // encuentra nodo que reemplaza nodo eliminado bst
  Node *BSTreplace(Node *x) { 
    if (x->left != NULL and x->right != NULL) 
      return successor(x->right); 
  
    if (x->left == NULL and x->right == NULL) 
      return NULL; 
 
    if (x->left != NULL) 
      return x->left; 
    else
      return x->right; 
  } 
  
  // deletes the given node 
  void deleteNode(Node *v) { 
    Node *u = BSTreplace(v); 
  
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK)); 
    Node *parent = v->parent; 
  
    if (u == NULL) { 
      // u is NULL therefore v is leaf 
      if (v == root) { 
        // v is root, making root null 
        root = NULL; 
      } else { 
        if (uvBlack) { 
          // u and v both black 
          // v is leaf, fix double black at v 
          fixDoubleBlack(v); 
        } else { 
          // u or v is red 
          if (v->sibling() != NULL) 
            // sibling is not null, make it red" 
            v->sibling()->color = RED; 
        } 
  
        // delete v from the tree 
        if (v->isOnLeft()) { 
          parent->left = NULL; 
        } else { 
          parent->right = NULL; 
        } 
      } 
      delete v; 
      return; 
    } 
  
    if (v->left == NULL or v->right == NULL) { 
      // v has 1 child 
      if (v == root) { 
        // v is root, assign the value of u to v, and delete u 
        v->val = u->val; 
        v->left = v->right = NULL; 
        delete u; 
      } else { 
        // Detach v from tree and move u up 
        if (v->isOnLeft()) { 
          parent->left = u; 
        } else { 
          parent->right = u; 
        } 
        delete v; 
        u->parent = parent; 
        if (uvBlack) { 
          // u and v both black, fix double black at u 
          fixDoubleBlack(u); 
        } else { 
          // u or v red, color u black 
          u->color = BLACK; 
        } 
      } 
      return; 
    } 
  
    swapValues(u, v); 
    deleteNode(u); 
  } 
  
  
  void inorder(Node *x) { 
    if (x == NULL) 
      return; 
    inorder(x->left); 
    cout << x->val << " "; 
    inorder(x->right); 
  }
  void fixDoubleBlack(Node *x) { 
    if (x == root) 
      // Reached root 
      return; 
  
    Node *sibling = x->sibling(), *parent = x->parent; 
    if (sibling == NULL) { 
      // No sibiling, double black pushed up 
      fixDoubleBlack(parent); 
    } else { 
      if (sibling->color == RED) { 
        // Sibling red 
        parent->color = RED; 
        sibling->color = BLACK; 
        if (sibling->isOnLeft()) { 
          // left case 
          rightRotate(parent); 
        } else { 
          // right case 
          leftRotate(parent); 
        } 
        fixDoubleBlack(x); 
      } else { 
        // Sibling black 
        if (sibling->hasRedChild()) { 
          // at least 1 red children 
          if (sibling->left != NULL and sibling->left->color == RED) { 
            if (sibling->isOnLeft()) { 
              // left left 
              sibling->left->color = sibling->color; 
              sibling->color = parent->color; 
              rightRotate(parent); 
            } else { 
              // right left 
              sibling->left->color = parent->color; 
              rightRotate(sibling); 
              leftRotate(parent); 
            } 
          } else { 
            if (sibling->isOnLeft()) { 
              // left right 
              sibling->right->color = parent->color; 
              leftRotate(sibling); 
              rightRotate(parent); 
            } else { 
              // right right 
              sibling->right->color = sibling->color; 
              sibling->color = parent->color; 
              leftRotate(parent); 
            } 
          } 
          parent->color = BLACK; 
        } else { 
          // 2 black children 
          sibling->color = RED; 
          if (parent->color == BLACK) 
            fixDoubleBlack(parent); 
          else
            parent->color = BLACK; 
        } 
      } 
    } 
  } 
  
   
  
public: 
  
  RBTree() { root = NULL; } 
  
  Node *getRoot() { return root; } 
  
 
  Node *search(int n) { 
    Node *temp = root; 
    while (temp != NULL) { 
      if (n < temp->val) { 
        if (temp->left == NULL) 
          break; 
        else
          temp = temp->left; 
      } else if (n == temp->val) { 
        break; 
      } else { 
        if (temp->right == NULL) 
          break; 
        else
          temp = temp->right; 
      } 
    } 
  
    return temp; 
  } 
  
  void insertAndFix(int n) { 
    Node *newNode = new Node(n); 
    if (root == NULL) { 
      newNode->color = BLACK; 
      root = newNode; 
    } else { 
      Node *temp = search(n); 
      if (temp->val == n) { 
        return; 
      } 
      newNode->parent = temp; 
      if (n < temp->val) 
        temp->left = newNode; 
      else
        temp->right = newNode; 
  
      
      fixRedRed(newNode); 
    } 
  } 
  
  // utility function that deletes the node with given value 
  void deleteByVal(int n) { 
    if (root == NULL) 
      return; 
  
    Node *v = search(n), *u; 
  
    if (v->val != n) { 
      cout << "no se encontro nodo: " << n << endl; 
      return; 
    } 
  
    deleteNode(v); 
  } 
  
  
  void printInOrder() { 
    cout << "Inorder: " << endl; 
    if (root == NULL) 
      cout << "vacio" << endl; 
    else
      inorder(root); 
    cout << endl; 
  } 
  
}; 
  
int main() { 
  RBTree tree; 
  
  tree.insertAndFix(7); 
  tree.insertAndFix(6); 
  tree.insertAndFix(5); 
  tree.insertAndFix(4); 
  tree.insertAndFix(3); 
  tree.insertAndFix(2); 
  tree.insertAndFix(1); 
  
  tree.printInOrder(); 
  
  cout<<endl<<"Eliminando 1"<<endl; 
  
  tree.deleteByVal(1); 
  
  tree.printInOrder(); 
  return 0; 
} 
