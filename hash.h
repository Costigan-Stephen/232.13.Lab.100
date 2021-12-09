/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Stephen Costigan, Alexander Dohms, Shaun Crook
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   
namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T>
class unordered_set
{
public:
   //
   // Construct - Shaun
   //
   unordered_set()
   {
       numElements = 0;
       maxLoadFactor = 1.0;
   }
   unordered_set(unordered_set&  rhs) 
   {
       *this = rhs;
   }
   unordered_set(unordered_set&& rhs) 
   {
       *this = std::move(rhs);
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last)
   {
       numElements = 0;
       auto it = first;
       while (it != last)
       {
           insert(*it);
           it++;
       }
   }

   //
   // Assign - Alex
   //
   unordered_set& operator = (unordered_set& rhs) 
   {
      numElements = rhs.numElements;
      for (int i = 0; i < 10; i++)
         buckets[i] = rhs.buckets[i];
      
      return *this;
   }
   unordered_set& operator = (unordered_set&& rhs)
   {
       numElements = std::move(rhs.numElements);
       rhs.numElements = NULL;
       for (int i = 0; i < 10; i++)
           buckets[i] = std::move(rhs.buckets[i]);

       return *this;
   }
   unordered_set& operator = (const std::initializer_list<T>& il) // Initializer List Assign and Fill Assignment
   {
      //clear();
      reserve(il.size());
      for (T t : il)      
         insert(t);
      
      return *this;
   }
   void swap(unordered_set& rhs)
   {
       std::swap(this->numElements, rhs.numElements);
       std::swap(this->buckets, rhs.buckets);
   }

   // 
   // Hash
   //

   size_t hash(const T& value) const {
       if(numElements > 0)
            return value % numElements;
       return 0;
   }


   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
       /*
       FOR itBucket = buckets.begin() … buckets.end()
         IF NOT itBucket.empty()
            RETURN iterator(buckets.end(), itBucket, itBucket.begin())
       RETURN end()
         */
       auto itBucket = buckets[0].begin(); 
       /*while ( itBucket < buckets[0].end()) {
           if (*itBucket.size() > 0)
               return iterator(buckets + 10, itBucket, itBucket->begin());
       }*/
      return end();
   }
   iterator end()
   {
      return iterator(buckets + 10, buckets + 10, buckets[0].end()); 
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(nullptr);
   }

   //
   // Access - Jon
   //
   size_t bucket(const T& t)
   {
      return t % 10;
   }
   iterator find(const T& t);

   //   
   // Insert - Steve
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);


   // 
   // Remove - Steve
   //
   void clear() noexcept { 
       for (auto& bucket : buckets)
            bucket.clear();
       numElements = 0; 
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const { return numElements; }
   bool empty() const  { return numElements == 0; }
   size_t bucket_count() const
   { 
      return numElements; 
   }
   size_t bucket_size(size_t i) const
   {
      return buckets[i].size(); // Steve, guessing here, but brought % up -- Nice guess bro
   }


#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   float maxLoadFactor;            // numElements / bucket_count()
   custom::list<T> buckets [10];   // exactly 10 buckets
   int numElements;                // number of elements in the Hash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::iterator
{
public:
   // 
   // Construct
   //
   iterator() { itList = nullptr; }
   iterator(typename custom::list<T>* pBucket,
            typename custom::list<T>* pBucketEnd,
            typename custom::list<T>::iterator itList)
   {
       this->itList     = itList;
       this->pBucket    = pBucket;
       this->pBucketEnd = pBucketEnd; 
   }
   iterator(const iterator& rhs) { *this = rhs; }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
       this->itList     = rhs.itList;
       this->pBucket    = rhs.pBucket;
       this->pBucketEnd = rhs.pBucketEnd; 
       return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const { return rhs.itList != itList; }
   bool operator == (const iterator& rhs) const { return rhs.itList == itList; }

   // 
   // Access
   //
   T& operator * () { return *itList; }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
       auto it = *this;
       ++(*this);
       return it;
   }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   custom::list<T> *pBucket;
   custom::list<T> *pBucketEnd;
   typename list<T>::iterator itList;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::local_iterator
{
public:
   // 
   // Construct
   //
   local_iterator() { itList = nullptr; }
   local_iterator(const typename custom::list<T>::iterator& itList) { this->itList = itList; }
   local_iterator(const local_iterator& rhs) { this = rhs; } 

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      this->itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const { return rhs.itList != itList ; }
   bool operator == (const local_iterator& rhs) const { return rhs.itList == itList ; }

   // 
   // Access
   //
   T& operator * ()
   {
      return *(this->itList);
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
       itList++;
       return *this;
   }
   local_iterator operator ++ (int postfix)
   {
       local_iterator it = *this;
       ++(*this);
       return it;
   }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::erase(const T& t)
{
   /*
   //Find the element to be erased. Return end() if the element is not present.
   itErase = find(element)
   IF itErase = end()
     RETURN itErase 
   */
    iterator itErase = find(t);
    if (itErase == end())
        return itErase;

   /*
   //Determine the return value.
   itReturn = itErase
   itReturn++
   */
    iterator itReturn = itErase;
    itReturn++;
   
   /*
   //Erase the element from the bucket.
   itErase.itVector.erase(itErase.itList)
   numElements--
   RETURN itReturn 
   */
    itErase.pBucket->erase(itErase.itList); // << -- pBucket must be itVector?
    numElements--;
    return itReturn; 
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T>
custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
{
    /*
    //Find the bucket where the new element is to reside.
    iBucket = bucket(element)
    */

    size_t iBucket = bucket(t);

    /*
    //See if the element is already there. If so, then return out.
    FOR it = buckets[iBucket].begin() … buckets[iBucket].end()
        IF *it = element
            RETURN pair(itHash, FALSE)
    */
    //std::cout << t << std::endl;
    auto it = buckets[iBucket].begin();
    std::string s = typeid(it).name(); // data type of auto
    //std::cout << s << std::endl;

    while (it != buckets[iBucket].end())
    {
        std::cout << *it << std::endl;
        /*if (*it == t)
            return custom::pair<typename custom::unordered_set<T>::iterator, bool>(hash(*it), false);*/
        it++;
    }

    /*for (auto it = buckets[iBucket].begin(); it != buckets[iBucket].end(); it++) {
        if (*it == t)
            return custom::pair<typename custom::unordered_set<T>::iterator, bool>(it, false);
    }*/
    

    /*
    //Reserve more space if we are already at the limit.
     
    IF min_buckets_required(numElements + 1) > bucket_count()
        reserve(numElements x 2)
        iBucket = bucket(element)
    */
   

    /*
    //Actually insert the new element on the back of the bucket.
    buckets[iBucket].push_back(element)
    numElements++
    RETURN pair(itHash, TRUE)
   */

    buckets[iBucket].push_back(t);
    numElements++;

    return custom::pair<custom::unordered_set<T>::iterator, bool>(iterator(), true);
}
template <typename T>
void unordered_set<T>::insert(const std::initializer_list<T> & il)
{
    // REHASH

    // don't know if this helps, intent was to catch empty il's
    if (il.size() == 0) {
        clear();
        return;
    }
    /*
    If the current bucket count is sufficient, then do nothing.
    IF numBuckets <= bucket_count()
        RETURN
    */
   
    /*if (bucket_count() <= bucket_count())
        return;*/
    /*
    //Create a new hash bucket.
    bucketNew = ALLOCATE(numBuckets)
    */
    
    custom::list<T> bucketNew = ALLOC(bucket_count());

    /*
    //Insert the elmements into the new hash table, one at a time.
    FOREACH element IN hash
        bucketsNew[hash(element) % numBuckets].push_back(element)
    */
    for (T e : il) {
        bucketNew[hash(e) % bucket_count()].push_back(e);
    }


    /*
    //Swap the old bucket for the new.
    swap(buckets, bucketsNew)
    */
    swap(bucketNew);
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::find(const T& t)
{
    auto iBucket = bucket(t);
    /*auto it = buckets[iBucket].begin();
    if (t) {
        while (it != buckets[iBucket].end())
        {
            if (*it == t)
                return it;
            it++;
        }
    }*/
        
    return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator & unordered_set<T>::iterator::operator ++ ()
{
    if (pBucket == pBucketEnd)
        return *this;

    ++itList;
    if (itList != pBucket->end())
        return *this;

    ++pBucket;
    while (pBucket != pBucketEnd && pBucket->empty())
        ++pBucket;

    if (pBucket != pBucketEnd)
        itList = pBucket->begin();

    return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T>
void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
{
    std::swap(lhs.numElements, rhs.numElements);
    std::swap(lhs.buckets, rhs.buckets);  
}

}
