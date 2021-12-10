/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Alexander Dohms, Stephen Costigan, Shaun Crook, Jonathan Colwell 
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

namespace custom
{

    /**************************************************
     * LIST
     * Just like std::list
     **************************************************/
    template <typename T>
    class list
    {
    public:
        //
        // Construct
        //

        list();
        list(list <T>& rhs);
        list(list <T>&& rhs);
        list(size_t num, const T& t);
        list(size_t num);
        list(const std::initializer_list<T>& il);
        template <class Iterator>
        list(Iterator first, Iterator last);
        ~list() { clear(); }

        //
        // Assign
        //

        list <T>& operator = (list& rhs);
        list <T>& operator = (list&& rhs);
        list <T>& operator = (const std::initializer_list<T>& il);

        //
        // Iterator
        //

        class iterator;
        iterator begin() { return iterator(pHead); }
        iterator rbegin() { return iterator(pTail); }
        iterator end() { return iterator(nullptr); }

        //
        // Access
        //

        T& front();
        T& back();

        //
        // Insert
        //

        void push_front(const T& data);
        void push_front(T&& data);
        void push_back(const T& data);
        void push_back(T&& data);
        iterator insert(iterator it, const T& data);
        iterator insert(iterator it, T&& data);

        iterator find(const T& data);
        //
        // Remove
        //

        void pop_back();
        void pop_front();
        void clear();
        iterator erase(const iterator& it);

        //
        // Status - Finished
        //

        bool empty()  const { return size() == 0; }
        size_t size() const { return numElements; }


#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif
        // nested linked list class
        class Node;

        // member variables
        size_t numElements; // though we could count, it is faster to keep a variable
        Node* pHead;    // pointer to the beginning of the list
        Node* pTail;    // pointer to the ending of the list
    };

    /*************************************************
     * NODE - Finished
     * the node class.  Since we do not validate any
     * of the setters, there is no point in making them
     * private.  This is the case because only the
     * List class can make validation decisions
     *************************************************/
    template <typename T>
    class list <T> ::Node
    {
    public:
        //
        // Construct
        //
        Node() : pNext(nullptr), pPrev(nullptr), data() { } // Default constructor
        Node(const T& data) : pNext(nullptr), pPrev(nullptr), data(data) { } // Copy Constructor
        Node(T&& data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { } // Move Constructor

       //
       // Data
       //

        T data;                 // user data 
        Node* pNext;       // pointer to next node
        Node* pPrev;       // pointer to previous node
    };

    /*************************************************
     * LIST ITERATOR - Finished
     * Iterate through a List, non-constant version
     ************************************************/
    template <typename T>
    class list <T> ::iterator
    {
    public:
        iterator() { p = nullptr; }
        iterator(Node* pRHS) { p = pRHS; }
        iterator(const iterator& rhs) { p = rhs.p; }
        iterator& operator = (const iterator& rhs)
        {
            this->p = rhs.p;
            return *this;
        }

        // equals, not equals operator
        bool operator != (const iterator& rhs) const { return rhs.p != p; }
        bool operator == (const iterator& rhs) const { return rhs.p == p; }

        // dereference operator, fetch a node
        T& operator * ()
        {
            return p->data;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
            iterator it(*this);
            if (p->pNext)
                p = p->pNext;
            return it;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            if (p->pNext)
                p = p->pNext;
            else
                p = nullptr;

            return *this;
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
            iterator it(*this);
            if (p->pPrev)
                p = p->pPrev;

            return it;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            if (p->pPrev)
                p = p->pPrev;
            return *this;
        }

        // two friends who need to access p directly
        friend iterator list <T> ::insert(iterator it, const T& data);
        friend iterator list <T> ::insert(iterator it, T&& data);
        friend iterator list <T> ::erase(const iterator& it);

#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif

        typename list <T> ::Node* p;
    };

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T>
    list <T> ::list(size_t num, const T& t)
    {
        if (num) {
            Node* pPrevious = pHead = pTail = new Node(T(t));
            pHead->pPrev = nullptr;

            for (int i = 1; i < num; i++) {
                Node* pNew = new Node(T(t));
                pNew->pPrev = pPrevious;
                pNew->pPrev->pNext = pNew;
                pPrevious = pNew;
            }
            pPrevious->pNext = nullptr;
            pTail = pPrevious;
        }
        numElements = num;
    }

    /*****************************************
     * LIST :: ITERATOR constructors
     * Create a list initialized to a set of values
     ****************************************/
    template <typename T>
    template <class Iterator>
    list <T> ::list(Iterator first, Iterator last)
    {
        /*pHead = pTail = nullptr;
        numElements = 0;*/
        auto it = first;
        while (it != last)
        {
            push_back(*it);
            it++;
        }
    }

    /*****************************************
     * LIST :: INITIALIZER constructors
     * Create a list initialized to a set of values
     ****************************************/
    template <typename T>
    list <T> ::list(const std::initializer_list<T>& il)
    {
        /*numElements = 0;
        pHead = pTail = nullptr;*/

        if (il.size() > 0)
        {
            for (T const item : il)
            {
                push_back(item);
            }
        }
    }

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T>
    list <T> ::list(size_t num)
    {
        if (num) {
            Node* pPrevious = pHead = pTail = new Node(0);
            pHead->pPrev = nullptr;

            for (int i = 1; i < num; i++) {
                Node* pNew = new Node(T(0));
                pNew->pPrev = pPrevious;
                pNew->pPrev->pNext = pNew;
                pPrevious = pNew;
            }
            pPrevious->pNext = nullptr;
            pTail = pPrevious;
        }
        numElements = num;
    }

    /*****************************************
     * LIST :: DEFAULT constructors - Finished | Alexander
     ****************************************/
    template <typename T>
    list <T> ::list()
    {
        numElements = 0;
        pHead = pTail = nullptr;
    }

    /*****************************************
     * LIST :: COPY constructors - Alexander
     ****************************************/
    template <typename T>
    list <T> ::list(list& rhs)
    {
        pHead = pTail = nullptr;
        numElements = 0;
        *this = rhs;
        
    }

    /*****************************************
     * LIST :: MOVE constructors - Finished
     * Steal the values from the RHS
     ****************************************/
    template <typename T>
    list <T> ::list(list <T>&& rhs)
    {
        pHead = rhs.pHead;
        pTail = rhs.pTail;
        numElements = rhs.numElements;

        rhs.pHead = nullptr;
        rhs.pTail = nullptr;
        rhs.numElements = 0;
    }

    /**********************************************
     * LIST :: assignment operator - Finished
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (list <T>& rhs)
    {
        iterator itRHS = rhs.begin();
        iterator itLHS = begin();
        while (itRHS != rhs.end() && itLHS != end())
        {
            *itLHS = *itRHS;
            ++itRHS;
            ++itLHS;
        }
        if (itRHS != rhs.end()) {
            while (itRHS != rhs.end()) {
                push_back(*itRHS);
                ++itRHS;
            }
        }
        else if (rhs.size() == 0)
        {
            clear();
        }
        else if (itLHS != end())
        {
            Node* p = itLHS.p;
            pTail = p->pPrev;
            Node* pNext = p->pNext;
            while (p) {
                pNext = p->pNext;
                delete p;
                p = pNext;
                numElements--;
            }
            pTail->pNext = NULL;
        }

        return *this;
    }

    /**********************************************
     * LIST :: assignment operator - MOVE - Alexander
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (list <T>&& rhs)
    {
        clear();
        swap(*this,rhs);

        return *this;
    }

    /**********************************************
     * LIST :: assignment operator - Steve
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
    {
        if (rhs.size() == 0)
            return *this;

        const int* itRHS = rhs.begin();
        iterator itLHS = begin();
        while (itRHS != rhs.end() && itLHS != end())
        {
            *itLHS = *itRHS;
            ++itRHS;
            ++itLHS;
        }
        if (itRHS != rhs.end()) {
            while (itRHS != rhs.end()) {
                push_back(*itRHS);
                ++itRHS;
            }
        }
        else if (rhs.size() == 0)
        {
            clear();
        }
        else if (itLHS != end())
        {
            Node* p = itLHS.p;
            pTail = p->pPrev;
            Node* pNext = p->pNext;
            while (p) {
                pNext = p->pNext;
                delete p;
                p = pNext;
                numElements--;
            }
            pTail->pNext = NULL;
        }


        return *this;
    }

    /**********************************************
     * LIST :: CLEAR - Finished
     * Remove all the items currently in the linked list
     *     INPUT  :
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    void list <T> ::clear()
    {
        while (pHead != NULL)
        {
            Node* pDelete = pHead;
            pHead = pHead->pNext;
            delete pDelete;
        }
        pTail = nullptr;
        numElements = 0;
    }

    /*********************************************
     * LIST :: PUSH BACK - Finished (check the other one below for the error)
     * add an item to the end of the list
     *    INPUT  : data to be added to the list
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::push_back(const T& data)
    {
        Node* pNew = new Node(data);
        pNew->pPrev = pTail;
        if (pTail)
            pTail->pNext = pNew;
        else
            pHead = pNew;

        pTail = pNew;
        numElements++;
    }

    template <typename T>
    void list <T> ::push_back(T&& data)
    {
        Node* pNew = new Node(data);
        pNew->pPrev = pTail;
        if (pTail)
            pTail->pNext = pNew;
        else
            pHead = pNew;

        pTail = pNew;
        numElements++;

    }

    /*********************************************
     * LIST :: PUSH FRONT - Finished
     * add an item to the head of the list
     *     INPUT  : data to be added to the list
     *     OUTPUT :
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::push_front(const T& data)
    {
        Node* pNew = new Node(data);
        if (pNew != nullptr) {
            if (numElements == 0)
            {
                pHead = pTail = pNew;
            }
            else
            {
                pHead->pPrev = pNew;
                pNew->pNext = pHead;
                pHead = pNew;
            }
            numElements++;
        }
    }

    template <typename T>
    void list <T> ::push_front(T&& data)
    {
        Node* pNew = new Node(data);
        if (pNew != nullptr) {
            if (numElements == 0)
            {
                pHead = pTail = pNew;
            }
            else
            {
                pHead->pPrev = pNew;
                pNew->pNext = pHead;
                pHead = pNew;
            }
            numElements++;
        }
    }


    /*********************************************
     * LIST :: POP BACK
     * remove an item from the end of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::pop_back()
    {
        if (!empty())
        {
            if (numElements <= 1) {
                pTail = pHead = nullptr;
            }
            else {
                Node* pNew = pHead;

                while (pNew != nullptr) {
                    if (pNew->pNext == pTail)
                        pNew->pNext = nullptr;
                    pNew = pNew->pNext;
                }
                pTail = pTail->pPrev;
            }
            numElements--;
        }
    }

    /*********************************************
     * LIST :: POP FRONT
     * remove an item from the front of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::pop_front()
    {
        if (!empty())
        {
            if (numElements == 1 || numElements == NULL) {
                pTail = pHead = nullptr;
            }
            else {
                Node* pNew = pTail;

                while (pNew != nullptr) {
                    if (pNew->pPrev == pHead)
                        pNew->pPrev = nullptr;
                    pNew = pNew->pPrev;
                }
                pHead = pHead->pNext;
            }
            numElements > 0 ? numElements-- : 0;
        }
    }

    /*********************************************
     * LIST :: FRONT - Finished
     * retrieves the first element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    T& list <T> ::front()
    {
        size_t empty = 0;
        if (numElements == empty)
            return *(new T);
        return pHead->data;
    }

    /*********************************************
     * LIST :: BACK - Finished
     * retrieves the last element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    T& list <T> ::back()
    {
        size_t empty = 0;
        if (numElements == empty)
            return *(new T);
        return pTail->data;
    }

    /******************************************
     * LIST :: REMOVE
     * remove an item from the middle of the list
     *     INPUT  : an iterator to the item being removed
     *     OUTPUT : iterator to the new location
     *     COST   : O(1)
     ******************************************/
    template <typename T>
    typename list <T> ::iterator  list <T> ::erase(const list <T> ::iterator& it)
    {
        iterator itNext = end();
        if (it.p != nullptr)
        {
            if (it.p->pNext)
            {
                it.p->pNext->pPrev = it.p->pPrev;
                itNext = it.p->pNext;
            }
            else
            {
                /*pTail->pNext = nullptr;*/
                pTail = it.p->pPrev;
            }

            if (it.p->pPrev)
            {
                it.p->pPrev->pNext = it.p->pNext;
            }
            else
            {
                /*pHead->pPrev = nullptr;*/
                pHead = it.p->pNext;
            }
            delete it.p;
            numElements--;
        }
        return itNext;
    }

    /******************************************
     * LIST :: INSERT
     * add an item to the middle of the list
     *     INPUT  : data to be added to the list
     *              an iterator to the location where it is to be inserted
     *     OUTPUT : iterator to the new item
     *     COST   : O(1)
     ******************************************/
    template <typename T>
    typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
        const T& data)
    {
        Node* pNew = new Node(data);

        if (numElements == 0)
        {
            numElements = 1;
            pHead = pTail = pNew;
            return begin();
        }

        if (it.p)
        {
            pNew->pNext = it.p;
            pNew->pPrev = it.p->pPrev;
            it.p->pPrev = pNew;

            if (pNew->pPrev)
                pNew->pPrev->pNext = pNew;

            if (it.p == pHead)
                pHead = pNew;
        }
        else
        {
            pTail->pNext = pNew;
            pNew->pPrev = pTail;
            pTail = pNew;
        }

        numElements++;
        return iterator(pNew);
        /*return end();*/
    }

    template <typename T>
    typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
        T&& data)
    {
        Node* pNew = new Node(data);

        if (numElements == 0)
        {
            numElements = 1;
            pHead = pTail = pNew;
            return begin();
        }

        if (it.p)
        {
            pNew->pNext = it.p;
            pNew->pPrev = it.p->pPrev;
            it.p->pPrev = pNew;

            if (pNew->pPrev)
                pNew->pPrev->pNext = pNew;

            if (it.p == pHead)
                pHead = pNew;
        }
        else
        {
            pTail->pNext = pNew;
            pNew->pPrev = pTail;
            pTail = pNew;
        }

        numElements++;
        return iterator(pNew);
        /*return end();*/
    }
    
    template <typename T>
    typename list <T> ::iterator list <T> ::find(const T& data)
    {
        for (auto it = begin(); it != end(); ++it)
            if (*it == data)
                return it;
        return end();
    }

    /**********************************************
     * LIST :: assignment operator - MOVE
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS
     *********************************************/
    template <typename T>
    void swap(list <T>& lhs, list <T>& rhs)
    {
        list <T> tempHead = rhs;
        rhs = lhs;
        lhs = tempHead;
    }


    //#endif
}; // namespace custom