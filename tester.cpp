/*
 * Name: Jonathan Lemon
 * Date Submitted: 3/22/2021
 * Lab Section: 002
 * Assignment Name: Lab 7
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

using namespace std;

struct Node {
  int key;
  int size;
  Node *left;
  Node *right;
  Node (int k) { key = k; size = 1; left = right = nullptr; }
};

void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

// insert key k into tree T, returning a pointer to the resulting tree
Node *insert(Node *T, int k)
{
  if (T == nullptr) return new Node(k);
  if (k < T->key) T->left = insert(T->left, k);
  else T->right = insert(T->right, k);
  fix_size(T);
  return T;
}


// returns a vector of key values corresponding to the inorder traversal of T (i.e., the contents of T in sorted order)
vector<int> inorder_traversal(Node *T)
{
  vector<int> inorder;
  vector<int> rhs;
  if (T == nullptr) return inorder;
  inorder=inorder_traversal(T->left);
  inorder.push_back(T->key);
  rhs=inorder_traversal(T->right);
  inorder.insert(inorder.end(), rhs.begin(), rhs.end());
  return inorder;
}

// return a pointer to the node with key k in tree T, or nullptr if it doesn't exist
Node *find(Node *T, int k)
{
  //Implement Node *find(Node *T, int k)
  if(T==nullptr){ //Edge case
      return nullptr;
  }
  //Traverse the tree based on where it should appear, until you reach the value or a nullptr
  if(T->key==k){
      return T;
  }
  if(k<T->key){
      return find(T->left, k);
  }
  else{
      return find(T->right, k);
  }
}

// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)

Node *select(Node *T, int r)
{
  assert(T!=nullptr && r>=0 && r<T->size);
    int sizeOfLeft;//Size of the left subtree
    if(T->left==nullptr){
        sizeOfLeft=0;
    }
    else{
        sizeOfLeft=T->left->size;
    }
    if(r==sizeOfLeft){//If r is the number of elements in left subtree, the parent node is the rth value
        return T;
    }
    else if(r<sizeOfLeft){//If r is in left subtree, recursively call it
        return select(T->left, r);
    }
    else{//If r is in right subtree, recursively call it and subtract sizeOfLeft+1 from the value of r to account for the left subtree and parent node
        return select(T->right, (r-(sizeOfLeft+1)));
    }
  //Implement Node *select(Node *T, int r)
}

// Join trees L and R (with L containing keys all <= the keys in R)
// Return a pointer to the joined tree.  

Node *join(Node *L, Node *R)
{
    bool useLeft;
    //Edgecases
    if(L==nullptr&&R==nullptr){
        return nullptr;
    }
    if(L==nullptr){
        return R;
    }
    if(R==nullptr){
        return L;
    }
    //Whichever side is bigger, make that the head
    if(L->size>=R->size){
        useLeft=true;
    }
    else{
        useLeft=false;
    }
    int c=0;
    vector<int> vals;
    //Based on whichever side is the head, store the values of the other side
    if(useLeft){
        vals=inorder_traversal(R);
    }
    else{
        vals=inorder_traversal(L);
    }
    //Loop through the values of the other side and insert them into the head.
    while(c<vals.size()){
        if(useLeft){
            insert(L, vals[c]);
        }
        else{
            insert(R, vals[c]);
        }
        c++;
    }
    if(useLeft){
        return L;
    }
    else{
        return R;
    }
  // choose either the root of L or the root of R to be the root of the joined tree
  // (where we choose with probabilities proportional to the sizes of L and R)
  
  //Implement Node *join(Node *L, Node *R)
}

// remove key k from T, returning a pointer to the resulting tree.
// it is required that k be present in T
Node *remove(Node *T, int k)
{
  assert(T != nullptr);
  //If only 1 element
  if(T->size==1){
      return nullptr;
  }
  T->size--;
  //Remove by joining the subtrees
  if(T->key==k){
      return join(T->left, T->right);
  }
  else if(k<T->key){
      if(k==T->left->key){
          T->left=join(T->left->left, T->left->right);
      }
      else{
          remove(T->left, k);
      }
  }
  else if(k>T->key){
      if(k==T->right->key){
          T->right=join(T->right->left, T->right->right);
      }
      else{
          remove(T->right, k);
      }
  }
  return T;
  //Implement Node *remove(Node *T, int k)
}

// Split tree T on key k into tree L (containing keys <= k) and a tree R (containing keys > k)
void split(Node *T, int k, Node **L, Node **R)
{
    vector<int> list=inorder_traversal(T);
    vector<int> left;
    vector<int> right;
    int index;
    //Create two vectors for the values on the left and on the right of k
    for(int i=0;k>=list[i];i++){
        left.push_back(list[i]);
        index=i;
    }
    index++;
    for(int i=index;i<list.size();i++){
        right.push_back(list[i]);
    }
    Node* l=nullptr;
    Node* r=nullptr;
    //Insert these values into two trees
    for(int i=0;i<left.size();i++){
        l=insert(l, left[i]);
    }
    for(int i=0;i<right.size();i++){
        r=insert(r, right[i]);
    }
    //"Return" the trees
    *L=l;
    *R=r;
  //Implement void split(Node *T, int k, Node **L, Node **R)
}

// insert key k into the tree T, returning a pointer to the resulting tree

vector<int> insertionPath(Node* T, int k, vector<int> path){
    if(T==nullptr){
        return path;
    }
    if(k>=T->key){
        path.push_back(1);
        return insertionPath(T->right, k, path);
    }
    else{
        path.push_back(-1);
        return insertionPath(T->left, k, path);
    }
}

Node* findParent(Node* child, Node* root){
    if(root==nullptr){
        return nullptr;
    }
    if(root->key==child->key){
        return nullptr;
    }
    if(root->left!=nullptr){
        if(root->left->key==child->key){
            return root;
        }
    }
    if(root->right!=nullptr){
        if(root->right->key==child->key){
            return root;
        }
    }
    if(child->key>=root->key){
        return findParent(child, root->right);
    }
    else{
        return findParent(child, root->left);
    }
}

void printVector(vector<int> v)
{
    for (int i=0; i<v.size(); i++)
    {
        cout << v[i] << " ";
    }
}

Node* rotateRight(Node* left, Node* oldHead, Node* root){
    Node* grandparent=findParent(oldHead, root);
    int oS=0;
    int orS=0;
    int lrS=0;
    oS=oldHead->size;
    if(left->right!=nullptr){
        lrS=left->right->size;
    }
    if(oldHead->right!=nullptr){
        orS=oldHead->right->size;
    }
    oldHead->left=left->right;
    left->right=oldHead;
    if(grandparent!=nullptr){
        if(left->key>=grandparent->key){
            grandparent->right=left;
        }
        else{
            grandparent->left=left;
        }
    }
    oldHead->size=orS+lrS+1;
    left->size=oS;
    return left;
}

Node* rotateLeft(Node* right, Node* oldHead, Node* root){
    Node* grandparent=findParent(oldHead, root);
    int oS=0;
    int olS=0;
    int rlS=0;
    oS=oldHead->size;
    if(right->left!=nullptr){
        rlS=right->left->size;
    }
    if(oldHead->left!=nullptr){
        olS=oldHead->left->size;
    }
    oldHead->right=right->left;
    right->left=oldHead;
    if(grandparent!=nullptr){
        if(right->key>=grandparent->key){
            grandparent->right=right;
        }
        else{
            grandparent->left=right;
        }
    }
    oldHead->size=olS+rlS+1;
    right->size=oS;
    return right;
    
}

int height=0;

Node *insert_random(Node *T, int k)
{
    //Random seed
    if(find(T,k)==nullptr){
    if(T==nullptr){
        //If 1 element, always make it head
        return insert(T,k);//This function has been implemented previously, and works fine
    }
    else{
        //Based on size of T, determine odds of making this the head
        float percent=(1.0/T->size);
        int num=(rand()%1000);//Random 1-1000
        //This line basically randomly chooses if the insertion should be the head
        if(num<(int)(1000)){
            vector<int> path=insertionPath(T, k, vector<int>());
            insert(T, k);
            Node* newRoot=find(T, k);
            if(path.size()>height){
                height=path.size();
            }
            while(path.size()>0){
                if(path[path.size()-1]==1){
                    newRoot=rotateLeft(newRoot, findParent(newRoot, T), T);
                }
                if(path[path.size()-1]==-1){
                    newRoot=rotateRight(newRoot, findParent(newRoot, T), T);
                }
                path.pop_back();
            }
            return newRoot;
        }
        else{
            //If not chosen to be the head, insert recursively
            T->size++;
            if(k>=T->key){
                T->right=insert_random(T->right,k);
            }
            else{
                T->left=insert_random(T->left, k);
            }
        }
    }
    }
    return T;
  // If k is the Nth node inserted into T, then:
  // with probability 1/N, insert k at the root of T
  // otherwise, insert_random k recursively left or right of the root of T
  //Implement Node *insert_random(Node *T, int k)
}

int main(void)
{
    srand(time(0));
    Node* T = nullptr;
    int num[1000000];
    for(int i=0;i<1000000;i++){
        num[i]=i;
    }
    for(int i=0;i<1000000;i++){
        int index=(rand()^2)%1000000;
        int temp=num[i];
        num[i]=num[index];
        num[index]=temp;
        cout<<num[i]<<" ";
    }
    cout<<"SORTED:"<<endl;
    for(int i=0;i<1000000;i++){
        T=insert_random(T, num[i]);
    }
    cout<<T->size<<endl;
    vector<int> inorder=inorder_traversal(T);
    printVector(inorder);
    cout<<"Height: "<<height<<endl;

  return 0;
}
