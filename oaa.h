/*
    oaa.h
    Kevin Perez
    7/3/15
    
    OAA: Ordered Asociative Array
    OAA is being implemented using left-leaning red-black (balanced) binary trees as coined
    by Robert Sedgewick of Stanford University. RB tree structure is best used when a lot of
		insertion and deletions will be performed. For lots of searching, using AVL trees.

  Concept of LLRBT (1-4 are properies of RBTs as well)
  ----------------
  The depth of nodes are managed using coloration.
    1. Every node is either red of black.
    2. Root is black.
    3. If a node is red, all its children are black.
    4. All root to NULL paths have the same number of black nodes.
    5. If a node is red, all its children are black. (hence left-leaning)

  The "black height" of a node is (b(x)<=log(n)) = the number of black nodes between x and a null
  descendant of x(count down tree until a null descendant of x is found). The
  subtree rooted at x contains at least 2^((b(x)-1) nodes, not counting x.
  This implies that the height of the tree is always <= 2log(n) = O(log(n)).

  OAA Public Methods
  ------------------
    The initial development of this class is missing the implementation of:
        1) Object comparison operators == and !=
        2) Iterators and iterator support
        3) Remove
    These will be implemented some time in the future. 
    The "mutator" portion of the OAA API consists of Get, Put, Clear,
    Erase and Rehash -- arguably the minimal necessary for a useful general
    purpose container.
    
    -operator[] the AA bracket operator is in the interface and is implemented
    in-line above with a single call to Get. Also note that Put is implemented
    with a single call to Get. Operator[], and thus Get(k), is supposed to return the data
    associated with the key. 
    
    - Get(key) returns the data associated with the key. It ensures that the
      key is in the set and it also retrieves the value when it is found. In
      other words, it ensures that the key is in the set and returns a reference
      to the data already stored, thus allowing modification.

  OAA Private Methods
  -------------------
  RGet(node,k,location) is called in Get(key) and in RInsert(node,k,d) and works
  recursively. In Get(key), RGet is always initially called as
  RGet(root_,k,location) where it begins searching from the root of the
  tree. The Node pointer for root_ in the function eventually will point to  

  Notice that OAA is a friend class of class Node which consists of only private
  data and functions, thus not accessible by a client program or other classes.

*/

#ifndef _OAA_H
#define _OAA_H

#include <cstddef>    // size_t
#include <iostream>
#include <iomanip>
#include <compare.h>  // LessThan
#include <queue.h>    // used in Dump()
#include <ansicodes.h>

namespace fsu
{
  template < typename K , typename D , class P >
  class OAA;

  template < typename K , typename D , class P = LessThan<K> >
  class OAA
  {
  public:
    typedef K    KeyType;
    typedef D    DataType;
    typedef P    PredicateType;

             OAA  ();
    explicit OAA  (P p);
             OAA  (const OAA& a);
             ~OAA ();
    OAA& operator=(const OAA& a);

    DataType& operator [] (const KeyType& k)        { return Get(k); }

    void Put (const KeyType& k , const DataType& d) { Get(k) = d; }
    D&   Get (const KeyType& k);

    void Erase(const KeyType& k);
    void Clear();
    void Rehash();

    bool   Empty    () const { return root_ == nullptr; }
    size_t Size     () const { return RSize(root_); }     // counts alive nodes
    size_t NumNodes () const { return RNumNodes(root_); } // counts nodes
    int    Height   () const { return RHeight(root_); }

    template <class F>  //F is a function object
    void   Traverse(F f) const { RTraverse(root_,f); }

    void   Display (std::ostream& os, int kw, int dw,     // key, data widths
                    std::ios_base::fmtflags kf = std::ios_base::right, // key flag
                    std::ios_base::fmtflags df = std::ios_base::right // data flag
                   ) const;

    void   DumpBW (std::ostream& os) const;
    void   Dump (std::ostream& os) const;
    void   Dump (std::ostream& os, int kw) const;
    void   Dump (std::ostream& os, int kw, char fill) const;

  private: // definitions and relationships

    enum Flags { ZERO = 0x00 , DEAD = 0x01, RED = 0x02 , DEFAULT = RED }; 
    static const char* ColorMap (unsigned char flags)
    {
      switch(flags)
      {
        case 0x00: return ANSI_BOLD_BLUE;        // bits 00
        case 0x01: return ANSI_BOLD_BLUE_SHADED; // bits 01
        case 0x02: return ANSI_BOLD_RED;         // bits 10
        case 0x03: return ANSI_BOLD_RED_SHADED;  // bits 11
        default: return "unknown color";   // unknown flags
      }
    }

    static char BWMap (uint8_t flags)
    {
      switch(flags)
      {
        case 0x00: return 'B'; // bits 00 black alive
        case 0x01: return 'b'; // bits 01 black dead
        case 0x02: return 'R'; // bits 10 red alive
        case 0x03: return 'r'; // bits 11 red dead
        default:   return 'U'; // unknown flags
      }
    }

    class Node
    {
      const KeyType   key_;
            DataType  data_;
      Node * lchild_, * rchild_;
      unsigned char flags_;
      Node (const KeyType& k, const DataType& d, Flags flags = DEFAULT)
        : key_(k), data_(d), lchild_(nullptr), rchild_(nullptr), flags_(flags)
      {}
      friend class OAA<K,D,P>;
      bool IsRed    () const { return 0 != (RED & flags_); }
      bool IsBlack  () const { return !IsRed(); }
      bool IsDead   () const { return 0 != (DEAD & flags_); }
      bool IsAlive  () const { return !IsDead(); }
      void SetRed   ()       { flags_ |= RED; }
      void SetBlack ()       { flags_ &= ~RED; }
      void SetDead  ()       { flags_ |= DEAD; }
      void SetAlive ()       { flags_ &= ~DEAD; }
    }; //Class Node

    class PrintNode
    {
     public:
      PrintNode (std::ostream& os, int kw, int dw,
                 std::ios_base::fmtflags kf, std::ios_base::fmtflags df )
        : os_(os), kw_(kw), dw_(dw), kf_(kf), df_(df) {}
      void operator() (const Node * n) const
      {
        if (n->IsAlive())
        {
          os_.setf(kf_,std::ios_base::adjustfield);
          os_ << std::setw(kw_) << n->key_;
          os_.setf(df_,std::ios_base::adjustfield);
          os_ << std::setw(dw_) << n->data_;
          os_ << '\n';
        }
      }
     private:
      std::ostream& os_;
      int kw_, dw_;      // key and data column widths
      std::ios_base::fmtflags kf_, df_; // column adjustment flags for output stream
    }; //class PrintNode

    class CopyNode
    {
     public:
      CopyNode (Node*& newroot, OAA<K,D>* oaa) : newroot_(newroot), oldtree_(oaa) {}
      void operator() (const Node * n) const
      {
        if (n->IsAlive())
        {
          newroot_ = oldtree_->RInsert(newroot_,n->key_, n->data_);
          newroot_->SetBlack();
        }
      }
     private:
      Node *&    newroot_; 
      OAA<K,D> * oldtree_;
    }; //class CopyNode
    
  private: // data
    Node *         root_;
    PredicateType  pred_;  //Default is LessThan<K>

  private: // methods
    static Node * NewNode     (const K& k, const D& d, Flags flags = DEFAULT);
    static void   RRelease    (Node* n); // deletes all descendants of n
    static Node * RClone      (const Node* n); // returns deep copy of n
    static size_t RSize       (Node * n);
    static size_t RNumNodes   (Node * n);
    static int    RHeight     (Node * n);

    // rotations
    static Node * RotateLeft  (Node * n);
    static Node * RotateRight (Node * n);

    template < class F >
    static void   RTraverse (Node * n, F f);

    // recursive left-leaning get
    Node * RGet(Node* nptr, const K& kval, Node*& location);

    // recursive left-leaning insert
    Node * RInsert(Node* nptr, const K& key, const D& data);

  }; // class OAA<>


  // API
  /*
    Returns data (in Wordbench it is size_t) associated with the key IN THE TREE.
    In terms of location: Get declares "location" as a local variable, calls
    RGet, and then returns "location->data_".
  */
  template < typename K , typename D , class P >
  D& OAA<K,D,P>::Get (const KeyType& k)
  {
    Node* location;
    root_ = RGet(root_, k, location); // RGet() returns a Node pointer
    root_->SetBlack();  // RGet() returns root_ as red if tree is empty
    return location->data_;  
  }

  template < typename K , typename D , class P >
  void OAA<K,D,P>::Erase(const KeyType& k)
  {
    std::cout << "In OAA::Erase(key = " << k << ")" << std::endl;
		
	}
  
	
  template < typename K , typename D , class P >
  void OAA<K,D,P>::Clear()
  {
    RRelease(root_);
    root_ = nullptr;
    delete root_;
  }

  template < typename K , typename D , class P >
  void OAA<K,D,P>::Rehash()
  /*
    CopyNode constructor makes the following changes: 
      CopyNode::Node *& newroot_ = newRoot
      NopyNode::OAA<K,D> * oldtree_ = this
    NOTE: the address of newRoot is changed in CopyNode.
    Traverse(cn) inline calls RTraverse(root_,cn)
   */
  { // this is complete!
    Node* newRoot = nullptr;    
    CopyNode cn(newRoot,this);  
    Traverse(cn);
    Clear();
    root_ = newRoot;
  }

  template < typename K , typename D , class P >
  void  OAA<K,D,P>::Display (std::ostream& os, int kw, int dw, std::ios_base::fmtflags kf, std::ios_base::  fmtflags df) const
  // Displays tree as inorder traversal
  {
    PrintNode print(os, kw, dw, kf, df);  // print(node) will only print alive nodes
		Traverse(print);    
  } // Display

  template < typename K , typename D , class P >
  typename OAA<K,D,P>::Node * OAA<K,D,P>::RGet(Node* nptr, const K& kval, Node*& location)
  // recursive left-leaning get
  /*
    RGet() is based on the table semantics:
    1. If k is in the table, Get returns a reference to the data associated with k
    2. If k is not in the table, Get inserts the pair (k, DataType()) into the table
    and returns a reference to the created data location
  */
  {   
    if (nptr == nullptr)    //add new node at bottom of tree
    {
      location = NewNode(kval,D());  // new node is alive and red
      return location;
    }

    // inserting recursively
    if (pred_(kval,nptr->key_))      // go down left branch
    {
      nptr->lchild_ = RGet(nptr->lchild_, kval, location);
    }
    else if (pred_(nptr->key_,kval))  // go down right branch
		
    {
      nptr->rchild_ = RGet(nptr->rchild_, kval, location);
    }
    else  // if key already exists
    {
      nptr->SetAlive();  //this revives a dead node
      location = nptr;
    }

    // repair on the way back up tree
    if (nptr->rchild_ != 0 && nptr->rchild_->IsRed() && !(nptr->lchild_ != 0 && nptr->lchild_->IsRed()))
    {
      nptr = RotateLeft(nptr);
    }
    if (nptr->lchild_ != 0 && nptr->lchild_->IsRed() && nptr->lchild_->lchild_ != 0 && nptr->lchild_->lchild_->IsRed())
    {
      nptr = RotateRight(nptr);
    }
    if (nptr->lchild_ != 0 && nptr->lchild_->IsRed() && nptr->rchild_ != 0 && nptr->rchild_->IsRed())
    {
      nptr->lchild_->SetBlack();
      nptr->rchild_->SetBlack();
      nptr->SetRed();
    }
    return nptr;
  }

  template < typename K , typename D , class P >
  typename OAA<K,D,P>::Node * OAA<K,D,P>::RInsert(Node* nptr, const K& key, const D& data)
  /* 
     recursive left-leaning insert
     RInsert, unlike RGet, itself has the ability to change the data of an
     existing key.
   */
  {
    //if(nptr != 0)
      //std::cout << "key: " << nptr->key_ << " data: " << nptr->data_ << std::endl;
    // else
      //    std::cout << "NULL" << std::endl;
    // invariant: number of black nodes in root->null paths has not changed
    // This means the only place the black node count goes up is at the top:
    // if the node returned by RInsert is red, its color changes to black in Insert
    // if a key is not found, then RGet will return a new node with the key
    // nptr is set alive/ black in RGet()
    Node* location;
    nptr = RGet(nptr,key,location);
    if (nptr->key_ == key)  // node found (and now alive)
    {
      nptr->data_ = data;                // data is changed
    }
    else                    // node does not exist
    {                       // key was added in RGet()
      nptr->data_ = data;
    }
    nptr->SetBlack();
    return nptr;     
  }
         
         
  /* everyting below here is complete */

  // proper type
  
  template < typename K , typename D , class P >
  OAA<K,D,P>::OAA  () : root_(nullptr), pred_()
  {}

  template < typename K , typename D , class P >
  OAA<K,D,P>::OAA  (P p) : root_(nullptr), pred_(p)
  {}

  template < typename K , typename D , class P >
  OAA<K,D,P>::~OAA ()
  {
    Clear();
  }

  template < typename K , typename D , class P >
  OAA<K,D,P>::OAA( const OAA& tree ) : root_(nullptr), pred_(tree.pred_)
  {
    root_ = RClone(tree.root_);
  }

  template < typename K , typename D , class P >
  OAA<K,D,P>& OAA<K,D,P>::operator=( const OAA& that )
  {
    if (this != &that)
    {
      Clear();
      this->root_ = RClone(that.root_);
    }
    return *this;
  }

  // rotations
  template < typename K , typename D , class P >
  typename OAA<K,D,P>::Node * OAA<K,D,P>::RotateLeft(Node * n)
  {
    if (nullptr == n || n->rchild_ == nullptr) return n;
    if (!n->rchild_->IsRed())
    {
      std::cerr << " ** RotateLeft called with black right child\n";
      return n;
    }
    Node * p = n->rchild_;
    n->rchild_ = p->lchild_;
    p->lchild_ = n;  

    n->IsRed()? p->SetRed() : p->SetBlack();
    n->SetRed();
    return p;
  }

  template < typename K , typename D , class P >
  typename OAA<K,D,P>::Node * OAA<K,D,P>::RotateRight(Node * n)
  {
    if (n == nullptr || n->lchild_ == nullptr) return n;
    if (!n->lchild_->IsRed())
    {
      std::cerr << " ** RotateRight called with black left child\n";
      return n;
    }

    Node * p = n->lchild_;
    n->lchild_ = p->rchild_;
    p->rchild_ = n;  

    n->IsRed()? p->SetRed() : p->SetBlack();
    n->SetRed();
    return p;
  }

  // private static recursive methods

  template < typename K , typename D , class P >
  size_t OAA<K,D,P>::RSize(Node * n)
  {
    if (n == nullptr) return 0;
    return (size_t)(n->IsAlive()) + RSize(n->lchild_) + RSize(n->rchild_);
  }

  template < typename K , typename D , class P >
  size_t OAA<K,D,P>::RNumNodes(Node * n)
  {
    if (n == nullptr) return 0;
    return 1 + RNumNodes(n->lchild_) + RNumNodes(n->rchild_);
  }

  template < typename K , typename D , class P >
  int OAA<K,D,P>::RHeight(Node * n)
  {
    if (n == nullptr) return -1;
    int lh = RHeight(n->lchild_);
    int rh = RHeight(n->rchild_);
    if (lh < rh) return 1 + rh;
    return 1 + lh;
  }

  template < typename K , typename D , class P >
  template < class F >
  void OAA<K,D,P>::RTraverse (Node * n, F f)
  /*
    As of 8/5/15:
    n is always root_.
    f is of type CopyNode. Member data includes:
      Node *& newroot_ and OAA<K,D> * oldtree_
    f(n) represents:
      void operator() (const Node * n) const
      {
        if (n->IsAlive())
        {
          newroot_ = oldtree_->RInsert(newroot_,n->key_, n->data_);
          newroot_->SetBlack();
        }
      }

      PROOF (Assume tree root is alive and has left and right children)
        The first recursive call to RTraverse can be viewed as going all the way 
      down the left branch of the tree until a nullptr is found. Then f(n) is
      called starting with the smallest key which should always pass as
      n->IsAlive() in f(n) if root_ is not null. Then that node is checked to
      see if it has a right child. IF it does then it travels all the way down to the left
      (i.e. RTraverse(n->lchild_,f) all over again. ELSE n points to A.
        Ending condition is when the memory stack is empty of calls to RTraverse
      (i.e. all nodes have been traversed). All the dead nodes in the old tree
      were eliminated. Thus, all the nodes now only point to other alive nodes.  

      Traverse, thus RTraverse(root_,n) is called in OAA<K,D,P>::Rehash()
   */
  {
    if (n == nullptr) return;
    RTraverse(n->lchild_,f); 
    f(n);
    RTraverse(n->rchild_,f);
  }

  template < typename K , typename D , class P >
  void OAA<K,D,P>::RRelease(Node* n)
  // post:  all descendants of n have been deleted
  {
    if (n != nullptr)
    {
      if (n->lchild_ != nullptr)
      {
        OAA<K,D,P>::RRelease(n->lchild_);
        delete n->lchild_;
        n->lchild_ = nullptr;
      }
      if (n->rchild_ != nullptr)
      {
        OAA<K,D,P>::RRelease(n->rchild_);
        delete n->rchild_;
        n->rchild_ = nullptr;
      }
    }
  } // OAA<K,D,P>::RRelease()

  template < typename K , typename D , class P >
  typename OAA<K,D,P>::Node* OAA<K,D,P>::RClone(const OAA<K,D,P>::Node* n)
  // returns a pointer to a deep copy of n
  {
    if (n == nullptr)
      return 0;
    typename OAA<K,D,P>::Node* newN = NewNode (n->key_,n->data_);
    newN->flags_ = n->flags_;
    newN->lchild_ = OAA<K,D,P>::RClone(n->lchild_);
    newN->rchild_ = OAA<K,D,P>::RClone(n->rchild_);
    return newN;
  } // end OAA<K,D,P>::RClone() */


  // private node allocator
  template < typename K , typename D , class P >
  typename OAA<K,D,P>::Node * OAA<K,D,P>::NewNode(const K& k, const D& d, Flags flags) 
  {
    Node * nPtr = new(std::nothrow) Node(k,d,flags);
    if (nPtr == nullptr)
    {
      std::cerr << "** OAA memory allocation failure\n";
      // handle exception in-class here
    }
    return nPtr;
  }

  // development assistants

  template < typename K , typename D , class P >
  void OAA<K,D,P>::DumpBW (std::ostream& os) const
  {
    // fsu::debug ("DumpBW(1)");
    // This is the same as "Dump(1)" except it uses a character map instead of a
    // color map for the display: B/b = black_alive/black_dead, R/r = red_alive/red_dead

    if (root_ == nullptr)
      return;

    // we will use "root" as the placeholder node; test for dummy by address
    Queue < Node * , Deque < Node * > > Que;
    char nullFill = '-';
    Node * fillNode = root_;
    Node * current;
  
    size_t currLayerSize, nextLayerSize, j, k;
    currLayerSize = 1;
    k = 1;  // 2^LayerNumber
    Que.Push(root_);

    // execute body once for "real" root:
    os << ' '; // 1-space left margin for graphic
    nextLayerSize = 0;
    current = Que.Front();
    Que.Pop();
    os << BWMap(current->flags_);
    if (current->lchild_ != nullptr)
    {
      Que.Push(current->lchild_);
      ++nextLayerSize;
    }
    else
    {
      Que.Push(fillNode);
    }
    if (current->rchild_ != nullptr)
    {
      Que.Push(current->rchild_);
      ++nextLayerSize;
    }
    else
    {
      Que.Push(fillNode);
    }
    os << '\n';
    currLayerSize = nextLayerSize;
    k *= 2;

    // now finish off, using address to detect fillNode.
    while (currLayerSize > 0)
    {
      nextLayerSize = 0;
      os << ' '; // 1-space left margin for graphic
      for (j = 0; j < k; ++j)
      {
        current = Que.Front();
        Que.Pop();
        if (current == fillNode) // an empty position in the tree
          os << nullFill;
        else 
          os << BWMap(current->flags_);
        if (current != fillNode && current->lchild_ != nullptr)
        {
          Que.Push(current->lchild_);
          ++nextLayerSize;
        }
        else
        {
          Que.Push(fillNode);
        }
        if (current != fillNode && current->rchild_ != nullptr)
        {
          Que.Push(current->rchild_);
          ++nextLayerSize;
        }
        else
        {
          Que.Push(fillNode);
        }
      } // end for
      os << '\n';
      currLayerSize = nextLayerSize;
      k *= 2;
    } // end while
    Que.Clear();
  } // DumpBW(os)

  template < typename K , typename D , class P >
  void OAA<K,D,P>::Dump (std::ostream& os) const
  {
    // fsu::debug ("Dump(1)");

    // This implementation is very like that for Dump(os, kw, fill), except that
    // there is no supplied fill object or output format character. The trick is
    // to use the root of a non-empty tree as the fill. To make that work, we have
    // to take care of the real root case before entering the main loop. 

    if (root_ == nullptr)
      return;

    // we will use "root" as the placeholder node; test for dummy by address
    Queue < Node * , Deque < Node * > > Que;
    char nullFill = '-';
    char nodeFill = '*';
    Node * fillNode = root_;
    Node * current;
  
    size_t currLayerSize, nextLayerSize, j, k;
    currLayerSize = 1;
    k = 1;  // 2^LayerNumber
    Que.Push(root_);

    // execute body once for "real" root:
    os << ' '; // 1-space left margin for graphic
    nextLayerSize = 0;
    current = Que.Front();
    Que.Pop();
    os << ColorMap(current->flags_) << nodeFill << ANSI_RESET_ALL;
    if (current->lchild_ != nullptr)
    {
      Que.Push(current->lchild_);
      ++nextLayerSize;
    }
    else
    {
      Que.Push(fillNode);
    }
    if (current->rchild_ != nullptr)
    {
      Que.Push(current->rchild_);
      ++nextLayerSize;
    }
    else
    {
      Que.Push(fillNode);
    }
    os << '\n';
    currLayerSize = nextLayerSize;
    k *= 2;

    // now finish off, using address to detect fillNode.
    while (currLayerSize > 0)
    {
      nextLayerSize = 0;
      os << ' '; // 1-space left margin for graphic
      for (j = 0; j < k; ++j)
      {
        current = Que.Front();
        Que.Pop();
        if (current == fillNode) // an empty position in the tree
          os << nullFill;
        else 
          os << ColorMap(current->flags_) << nodeFill << ANSI_RESET_ALL;
        if (current != fillNode && current->lchild_ != nullptr)
        {
          Que.Push(current->lchild_);
          ++nextLayerSize;
        }
        else
        {
          Que.Push(fillNode);
        }
        if (current != fillNode && current->rchild_ != nullptr)
        {
          Que.Push(current->rchild_);
          ++nextLayerSize;
        }
        else
        {
          Que.Push(fillNode);
        }
      } // end for
      os << '\n';
      currLayerSize = nextLayerSize;
      k *= 2;
    } // end while
    Que.Clear();
  } // Dump(os)

  template < typename K , typename D , class P >
  void OAA<K,D,P>::Dump (std::ostream& os, int kw) const
  {
    // fsu::debug ("Dump(2)");
    if (root_ == nullptr)
      return;
    Queue < Node * , Deque < Node * > > Que;
    Node * current;
    size_t currLayerSize, nextLayerSize, j;
    Que.Push(root_);
    currLayerSize = 1;
    while (!Que.Empty())
    {
      nextLayerSize = 0; //will determine how many keys will print
      if (kw == 1) os << ' '; // indent picture 1 space
      for (j = 0; j < currLayerSize; ++j)
      {
        current = Que.Front();
        Que.Pop();
        if (kw > 1) os << ' '; // indent each column 1 space
        os << ColorMap(current->flags_) << std::setw(kw) << current->key_<< ANSI_RESET_ALL;
        if (current->lchild_ != nullptr)
        {
          Que.Push(current->lchild_);
          ++nextLayerSize;
        }
        if (current->rchild_ != nullptr)
        {
          Que.Push(current->rchild_);
          ++nextLayerSize;
        }
      }
      os << '\n';
      currLayerSize = nextLayerSize;
    } // end while
    if (currLayerSize > 0)
      std::cerr << "** OAA<K,D,P>::Dump() inconsistency\n";
  } // Dump(os, kw)

  template < typename K , typename D , class P >
  void OAA<K,D,P>::Dump (std::ostream& os, int kw, char fill) const
  {
    // fsu::debug ("Dump(3)");
    if (root_ == nullptr)
      return;

    Node* fillNode = NewNode(K(),D());
    Queue < Node * , Deque < Node * > > Que;
    Node * current;
    size_t currLayerSize, nextLayerSize, j, k;
    Que.Push(root_);
    currLayerSize = 1;
    k = 1;  // 2^LayerNumber
    while (currLayerSize > 0)
    {
      nextLayerSize = 0;
      if (kw == 1) os << ' '; // indent picture 1 space
      for (j = 0; j < k; ++j)
      {
        current = Que.Front();
        Que.Pop();
        if (kw > 1) os << ' '; // indent each column 1 space
        if (current == fillNode)
        {
          os <<std::setw(kw) << fill;
        }
        else
        {
          os << ColorMap(current->flags_) << std::setw(kw) << current->key_<< ANSI_RESET_ALL;
        }

        if (current->lchild_ != nullptr)
        {
          Que.Push(current->lchild_);
          ++nextLayerSize;
        }
        else
        {
          Que.Push(fillNode);
        }
        if (current->rchild_ != nullptr)
        {
          Que.Push(current->rchild_);
          ++nextLayerSize;
        }
        else
        {
          Que.Push(fillNode);
        }
      }
      os << '\n';
      currLayerSize = nextLayerSize;
      k *= 2;
    } // end while
    Que.Clear();
    delete fillNode;
  } // Dump(os, kw, fill) */

} // namespace fsu 

#endif

