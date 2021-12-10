/***********************************************************************
 * Header:
 *    TEST HASH
 * Summary:
 *    Unit tests for hash
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "hash.h"
#include "unitTest.h"

#include <cassert>
#include <memory>
#include <unordered_set>
#include <functional>
#include <vector>

using std::cout;
using std::endl;

// Keep our hash simple without any fancy stuff
namespace std
{
   template <> struct hash<std::size_t>
   {
      // this hash function simply returns the passed value. Nothing fancy.
      std::size_t operator()(std::size_t i) const noexcept
      {
         return i;
      }
   };
}

class TestHash : public UnitTest
{

public:
   void run()
   {
      reset();

      // Construct
      test_construct_default();
      test_constructIterator_standard();
      test_constructCopy_empty();
      test_constructCopy_standard();

      // Assign
      test_assign_emptyEmpty();
      test_assign_emptyStandard();
      test_assign_standardEmpty();
      test_assignMove_emptyEmpty();
      test_assignMove_emptyStandard();
      test_assignMove_standardEmpty();
      test_swapMember_emptyEmpty();
      test_swapMember_standardEmpty();
      test_swapMember_standardOther();
      test_swapNonMember_emptyEmpty();
      test_swapNonMember_standardEmpty();
      test_swapNonMember_standardOther();
      
      // Iterator
      test_iterator_begin_empty();
      test_iterator_begin_standard();
      test_iterator_end_empty();
      test_iterator_end_standard();
      test_iterator_increment_empty();
      test_iterator_increment_moreInBucket();
      test_iterator_increment_nextBucket();
      test_iterator_increment_toEnd();
      test_iterator_dereference();
      test_localIterator_begin_single();
      test_localIterator_begin_multiple();
      test_localIterator_begin_empty();
      test_localIterator_increment_single();
      test_localIterator_increment_multiple();
      
      // Access
      test_bucket_empty0();
      test_bucket_empty7();
      test_bucket_empty58();
      test_find_empty();
      test_find_standardFront();
      test_find_standardBack();
      test_find_standardMissingEmptyList();
      test_find_standardMissingFilledList();
      
      // Insert
      test_insert_empty0();
      test_insert_empty58();
      test_insert_standard3();
      test_insert_standard77();
      test_insert_standardDuplicate();
      
      // Remove
      test_clear_empty();
      test_clear_standard();
      test_erase_empty();
      test_erase_standardMissing();
      test_erase_standardAlone();
      test_erase_standardFront();
      test_erase_standardBack();
      test_erase_standardLast();
      
      // Status
      test_size_empty();
      test_size_standard();
      test_empty_empty();
      test_empty_standard();
      test_bucketSize_empty();
      test_bucketSize_standardEmpty();
      test_bucketSize_standardOne();
      test_bucketSize_standardTwo();

      report("Hash");
   }

   /***************************************
    * CONSTRUCTOR
    ***************************************/
   
   // create an unordered set
   void test_construct_default()
   {  // setup
      custom::unordered_set<std::size_t> us;
      std::allocator<custom::unordered_set<std::size_t>> alloc;
      us.numElements = 99;
      // exercise
      alloc.construct(&us);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us);
   }  // teardown

   // create an unordered set from a vector iterator
   void test_constructIterator_standard()
   {  // setup
      std::vector<std::size_t> v{59, 67, 31, 49};
      custom::unordered_set<std::size_t> us;
      std::allocator<custom::unordered_set<std::size_t>> alloc;
      us.numElements = 99;
      // exercise
      alloc.construct(&us, v.begin(), v.end());
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us);
   }  // teardown

   // copy an empty unordered set
   void test_constructCopy_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> usSrc;
      custom::unordered_set<std::size_t> usDes;
      std::allocator<custom::unordered_set<std::size_t>> alloc;
      usDes.numElements = 99;
      // exercise
      alloc.construct(&usDes, usSrc);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown

   // copy a standard set
   void test_constructCopy_standard()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> usSrc;
      setupStandardFixture(usSrc);
      // exercise
      custom::unordered_set<std::size_t> usDes(usSrc);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(usSrc);
      assertStandardFixture(usDes);
   }  // teardown

   /***************************************
    * ASSIGN
    ***************************************/
   
   // assign an empty set to an empty set
   void test_assign_emptyEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> usSrc;
      custom::unordered_set<std::size_t> usDes;
      // exercise
      usDes = usSrc;
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown
   
   // assign an empty set to a standard set
   void test_assign_emptyStandard()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> usSrc;
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> usDes;
      setupStandardFixture(usDes);
      // exercise
      usDes = usSrc;
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown
   
   // assign an standard set to an empty set
   void test_assign_standardEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> usSrc;
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> usDes;
      setupStandardFixture(usSrc);
      // exercise
      usDes = usSrc;
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(usSrc);
      assertStandardFixture(usDes);
      // teardown
   }
   
   // move-assign an empty set to an empty set
   void test_assignMove_emptyEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> usSrc;
      custom::unordered_set<std::size_t> usDes;
      // exercise
      usDes = std::move(usSrc);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown
   
   // move-assign an empty set to a standard set
   void test_assignMove_emptyStandard()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> usSrc;
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> usDes;
      setupStandardFixture(usDes);
      // exercise
      usDes = std::move(usSrc);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(usSrc);
      assertEmptyFixture(usDes);
   }  // teardown
   
   // move-assign an standard set to an empty set
   void test_assignMove_standardEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> usSrc;
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> usDes;
      setupStandardFixture(usSrc);
      // exercise
      usDes = std::move(usSrc);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(usSrc);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(usDes);
      // teardown
   }
   
   // swap empty hashes use member swap
   void test_swapMember_emptyEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us1;
      custom::unordered_set<std::size_t> us2;
      // exercise
      us1.swap(us2);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us1);
      assertEmptyFixture(us2);
   }  // teardown
   
   // swap standard hash with empty hash using memger functions
   void test_swapMember_standardEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us1;
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us2;
      setupStandardFixture(us1);
      // exercise
      us1.swap(us2);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us1);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us2);
   }  // teardown
   
   // swap standard hash with single-element hash
   void test_swapMember_standardOther()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us1;
      setupStandardFixture(us1);
      //      h[0] --> 20
      //      h[1] -->
      //      h[2] -->
      //      h[3] --> 23
      //      h[4] --> 24
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 27
      //      h[8] --> 28
      //      h[9] -->
      custom::unordered_set<std::size_t> us2;
      us2.buckets[0].push_back(20);
      us2.buckets[3].push_back(23);
      us2.buckets[4].push_back(24);
      us2.buckets[7].push_back(27);
      us2.buckets[8].push_back(28);
      us2.numElements = 5;
      // exercise
      us1.swap(us2);
      // verify
      //      h[0] --> 20
      //      h[1] -->
      //      h[2] -->
      //      h[3] --> 23
      //      h[4] --> 24
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 27
      //      h[8] --> 28
      //      h[9] -->
      assertUnit(us1.numElements == 5);
      assertUnit(us1.buckets[0].size() == 1); // 20
      assertUnit(us1.buckets[1].size() == 0);
      assertUnit(us1.buckets[2].size() == 0);
      assertUnit(us1.buckets[3].size() == 1); // 23
      assertUnit(us1.buckets[4].size() == 1); // 24
      assertUnit(us1.buckets[5].size() == 0);
      assertUnit(us1.buckets[6].size() == 0);
      assertUnit(us1.buckets[7].size() == 1); // 27
      assertUnit(us1.buckets[8].size() == 1); // 28
      assertUnit(us1.buckets[9].size() == 0);
      if (us1.buckets[0].size() == 1)
         assertUnit(us1.buckets[0].front() == 20);
      if (us1.buckets[3].size() == 1)
         assertUnit(us1.buckets[3].front() == 23);
      if (us1.buckets[4].size() == 1)
         assertUnit(us1.buckets[4].front() == 24);
      if (us1.buckets[7].size() == 1)
         assertUnit(us1.buckets[7].front() == 27);
      if (us1.buckets[8].size() == 1)
         assertUnit(us1.buckets[8].front() == 28);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us2);
   }  // teardown
   
   // swap empty hashs using non-member swap
   void test_swapNonMember_emptyEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us1;
      custom::unordered_set<std::size_t> us2;
      // exercise
      swap(us1, us2);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us1);
      assertEmptyFixture(us2);
   }  // teardown
   
   // swap standard hash with empty hash using non-member functions
   void test_swapNonMember_standardEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us1;
      setupStandardFixture(us1);
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us2;
      // exercise
      swap(us1, us2);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us1);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us2);
   }  // teardown
   
   // swap standard hash with single-element hash
   void test_swapNonMember_standardOther()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us1;
      setupStandardFixture(us1);
      //      h[0] --> 20
      //      h[1] -->
      //      h[2] -->
      //      h[3] --> 23
      //      h[4] --> 24
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 27
      //      h[8] --> 28
      //      h[9] -->
      custom::unordered_set<std::size_t> us2;
      us2.buckets[0].push_back(20);
      us2.buckets[3].push_back(23);
      us2.buckets[4].push_back(24);
      us2.buckets[7].push_back(27);
      us2.buckets[8].push_back(28);
      us2.numElements = 5;
      // exercise
      swap(us1, us2);
      // verify
      //      h[0] --> 20
      //      h[1] -->
      //      h[2] -->
      //      h[3] --> 23
      //      h[4] --> 24
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 27
      //      h[8] --> 28
      //      h[9] -->
      assertUnit(us1.numElements == 5);
      assertUnit(us1.buckets[0].size() == 1); // 20
      assertUnit(us1.buckets[1].size() == 0);
      assertUnit(us1.buckets[2].size() == 0);
      assertUnit(us1.buckets[3].size() == 1); // 23
      assertUnit(us1.buckets[4].size() == 1); // 24
      assertUnit(us1.buckets[5].size() == 0);
      assertUnit(us1.buckets[6].size() == 0);
      assertUnit(us1.buckets[7].size() == 1); // 27
      assertUnit(us1.buckets[8].size() == 1); // 28
      assertUnit(us1.buckets[9].size() == 0);
      if (us1.buckets[0].size() == 1)
         assertUnit(us1.buckets[0].front() == 20);
      if (us1.buckets[3].size() == 1)
         assertUnit(us1.buckets[3].front() == 23);
      if (us1.buckets[4].size() == 1)
         assertUnit(us1.buckets[4].front() == 24);
      if (us1.buckets[7].size() == 1)
         assertUnit(us1.buckets[7].front() == 27);
      if (us1.buckets[8].size() == 1)
         assertUnit(us1.buckets[8].front() == 28);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us2);
   } // teardown
   

   /***************************************
    * ITERATOR
    ***************************************/
   
   // beginning of empty hash
   void test_iterator_begin_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it;
      Spy::reset();
      // exercise
      it = us.begin();
      // verify
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertEmptyFixture(us);
   }  // teardown
   
   // beginning of standard hash
   void test_iterator_begin_standard()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it;
      // exercise
      it = us.begin();
      // verify
      assertUnit(it.pBucket == us.buckets + 1);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[1].begin());
      assertStandardFixture(us);
      // teardown
   }

   // end of empty hash
   void test_iterator_end_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it;
      // exercise
      it = us.end();
      // verify
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertEmptyFixture(us);
   }  // teardown
   
   // end of standard hash
   void test_iterator_end_standard()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it;
      // exercise
      it = us.end();
      // verify
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertStandardFixture(us);
      // teardown
   }
   
   // increment of empty hash
   void test_iterator_increment_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it = us.end();
      // exercise
      ++it;
      // verify
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertEmptyFixture(us);
   }  // teardown
   
   // increment of standard hash where there is more than one element
   void test_iterator_increment_moreInBucket()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> [59] 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it;
      it.pBucket = us.buckets + 9;
      it.pBucketEnd = us.buckets + 10;
      it.itList = us.buckets[9].begin();
      // exercise
      ++it;
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 [49]
      assertUnit(it.pBucket == us.buckets + 9);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[9].rbegin());
      assertStandardFixture(us);
      // teardown
   }
   
   // increment of standard hash where we need to go to the next bucket
   void test_iterator_increment_nextBucket()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> [67]
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it;
      it.pBucket = us.buckets + 7;
      it.pBucketEnd = us.buckets + 10;
      it.itList = us.buckets[7].begin();
      // exercise
      ++it;
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> [59] 49
      assertUnit(it.pBucket == us.buckets + 9);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[9].begin());
      assertStandardFixture(us);
      // teardown
   }
   
   // increment of standard hash where we were at the last element
   void test_iterator_increment_toEnd()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 [49]
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it;
      it.pBucket = us.buckets + 9;
      it.pBucketEnd = us.buckets + 10;
      it.itList = us.buckets[9].rbegin();
      // exercise
      ++it;
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49  []
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertStandardFixture(us);
   }
   
   // dereference the iterator
   void test_iterator_dereference()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> [67]
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it;
      it.pBucket = us.buckets + 7;
      it.pBucketEnd = us.buckets + 10;
      it.itList = us.buckets[7].begin();
      // exercise
      int s = *it;
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> [67]
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(s == 67);
      assertUnit(it.pBucket == us.buckets + 7);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[7].begin());
      assertStandardFixture(us);
      // teardown
   }
   
   // local iterator begin for single element bucket
   void test_localIterator_begin_single()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::local_iterator it;
      // exercise
      it = us.begin(1);
      // verify
      //      h[0] -->
      //      h[1] --> [31]
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(it.itList == us.buckets[1].begin());
      assertStandardFixture(us);
      // teardown
   }
   
   // local iterator begin for double element bucket
   void test_localIterator_begin_multiple()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::local_iterator it;
      // exercise
      it = us.begin(9);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> [59] 49
      assertUnit(it.itList == us.buckets[9].begin());
      assertStandardFixture(us);
   }
   
   // local iterator begin for empty element bucket
   void test_localIterator_begin_empty()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::local_iterator it;
      // exercise
      it = us.begin(8);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] --> []
      //      h[9] --> 59 49
      assertUnit(it.itList == us.buckets[8].end());
      assertStandardFixture(us);
   }
   
   // local iterator increment for single element bucket
   void test_localIterator_increment_single()
   {  // setup
      //      h[0] -->
      //      h[1] --> [31]
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::local_iterator it;
      it.itList = us.buckets[1].begin();
      // exercise
      it = us.begin(8);
      // verify
      //      h[0] -->
      //      h[1] --> 31   []
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(it.itList == us.buckets[1].end());
      assertStandardFixture(us);
   }
   
   // local iterator increment for multiple element bucket
   void test_localIterator_increment_multiple()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> [59] 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::local_iterator it;
      it.itList = us.buckets[9].begin();
      // exercise
      ++it;
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 [49]
      assertUnit(it.itList == us.buckets[9].rbegin());
      assertStandardFixture(us);
   }


   /***************************************
    * ACCESS
    ***************************************/

    // find the bucket that the value 0 will go in the empty hash
    void test_bucket_empty0()
    {  // setup
       custom::unordered_set<std::size_t> us;
       size_t iBucket = 99;
       // exercise
       iBucket = us.bucket(0);  // 0 % 10 == 0
       // verify
       assertUnit(iBucket == 0);
       assertEmptyFixture(us);
    }  // teardown

    // find the bucket that the value 7 will go in the empty hash
    void test_bucket_empty7()
    {  // setup
       custom::unordered_set<std::size_t> us;
       size_t iBucket = 99;
       // exercise
       iBucket = us.bucket(7);    // 7 % 10 == 7
       // verify
       assertUnit(iBucket == 7);
       assertEmptyFixture(us);
    }  // teardown

    // find the bucket that the value 58 will go in the empty hash
    void test_bucket_empty58()
    {  // setup
       custom::unordered_set<std::size_t> us;
       size_t iBucket = 99;
       // exercise
       iBucket = us.bucket(58);  // 58 % 10 == 8
       // verify
       assertUnit(iBucket == 8);
       assertEmptyFixture(us);
    }  // teardown

    // find something from an empty hash
   void test_find_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it = us.begin();
      // exercise
      it = us.find(99);
      // verify
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertEmptyFixture(us);
   }  // teardown

   // find something at the front of the list of a hash
   void test_find_standardFront()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it = us.begin();
      setupStandardFixture(us);
      // exercise
      it = us.find(59);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> [59] 49
      assertUnit(it.pBucket == us.buckets + 9);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[9].begin());
      assertStandardFixture(us);
      // teardown
   }

   // find something at the back of the list of a hash
   void test_find_standardBack()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it = us.begin();
      setupStandardFixture(us);
      // exercise
      it = us.find(49);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> [59] 49
      assertUnit(it.pBucket == us.buckets + 9);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[9].rbegin());
      assertStandardFixture(us);
   }

   // find something missing from an empty list
   void test_find_standardMissingEmptyList()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it = us.begin();
      setupStandardFixture(us);
      // exercise
      it = us.find(50);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertStandardFixture(us);
   }

   // find something missing from an empty list
   void test_find_standardMissingFilledList()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it = us.begin();
      setupStandardFixture(us);
      // exercise
      it = us.find(69);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
      assertStandardFixture(us);
   }
   
   /***************************************
    * INSERT
    ***************************************/
   
   // test that we can insert 0 into an empty hash
   void test_insert_empty0()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      custom::pair<custom::unordered_set<std::size_t>::iterator, bool> p;
      // exercise
      p = us.insert(0);  // 0 % 10 == 0
      // verify
      //      h[0] --> 0
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertUnit(us.numElements == 1);
      assertUnit(us.buckets[0].size() == 1); // 0
      assertUnit(us.buckets[1].size() == 0);
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 0);
      assertUnit(us.buckets[4].size() == 0);
      assertUnit(us.buckets[5].size() == 0);
      assertUnit(us.buckets[6].size() == 0);
      assertUnit(us.buckets[7].size() == 0);
      assertUnit(us.buckets[8].size() == 0);
      assertUnit(us.buckets[9].size() == 0);
      if (us.buckets[0].size() == 1)
         assertUnit(us.buckets[0].front() == 0);
      assertUnit(p.first.pBucket == us.buckets + 0);
      assertUnit(p.first.pBucketEnd == us.buckets + 10);
      assertUnit(p.first.itList == us.buckets[0].begin());
      assertUnit(p.second == true);
   }  // teardown
   
   // test that we can insert 58 into an empty hash
   void test_insert_empty58()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      custom::pair<custom::unordered_set<std::size_t>::iterator, bool> p;
      // exercise
      p = us.insert(58);  // 58 % 10 == 8
      // verify
      //      h[0] --> 0
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertUnit(us.numElements == 1);
      assertUnit(us.buckets[0].size() == 0);
      assertUnit(us.buckets[1].size() == 0);
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 0);
      assertUnit(us.buckets[4].size() == 0);
      assertUnit(us.buckets[5].size() == 0);
      assertUnit(us.buckets[6].size() == 0);
      assertUnit(us.buckets[7].size() == 0);
      assertUnit(us.buckets[8].size() == 1); // 58
      assertUnit(us.buckets[9].size() == 0);
      if (us.buckets[8].size() == 1)
         assertUnit(us.buckets[8].front() == 58);
      assertUnit(p.first.pBucket == us.buckets + 8);
      assertUnit(p.first.pBucketEnd == us.buckets + 10);
      assertUnit(p.first.itList == us.buckets[8].begin());
      assertUnit(p.second == true);
   }  // teardown
   
   // test that we can insert 3 into the standard hash
   void test_insert_standard3()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::pair<custom::unordered_set<std::size_t>::iterator, bool> p;
      // exercise
      p = us.insert(3);  // 3 % 10 == 3
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] --> 3
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(us.numElements == 5);
      assertUnit(us.buckets[0].size() == 0);
      assertUnit(us.buckets[1].size() == 1); // 31
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 1); // 3
      assertUnit(us.buckets[4].size() == 0);
      assertUnit(us.buckets[5].size() == 0);
      assertUnit(us.buckets[6].size() == 0);
      assertUnit(us.buckets[7].size() == 1); // 67
      assertUnit(us.buckets[8].size() == 0);
      assertUnit(us.buckets[9].size() == 2); // 59 49
      if (us.buckets[1].size() == 1)
         assertUnit(us.buckets[1].front() == 31);
      if (us.buckets[3].size() == 1)
         assertUnit(us.buckets[3].front() == 3);
      if (us.buckets[7].size() == 1)
         assertUnit(us.buckets[7].front() == 67);
      if (us.buckets[9].size() == 2)
      {
         assertUnit(us.buckets[9].front() == 59);
         assertUnit(us.buckets[9].back() == 49);
      }
      assertUnit(p.first.pBucket == us.buckets + 3);
      assertUnit(p.first.pBucketEnd == us.buckets + 10);
      assertUnit(p.first.itList == us.buckets[3].begin());
      assertUnit(p.second == true);
   }
   
   // test that we can insert 7 into the standard hash
   void test_insert_standard77()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::pair<custom::unordered_set<std::size_t>::iterator, bool> p;
      // exercise
      p = us.insert(77);  // 77 % 10 == 7
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67 77
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(us.numElements == 5);
      assertUnit(us.buckets[0].size() == 0);
      assertUnit(us.buckets[1].size() == 1); // 31
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 0);
      assertUnit(us.buckets[4].size() == 0);
      assertUnit(us.buckets[5].size() == 0);
      assertUnit(us.buckets[6].size() == 0);
      assertUnit(us.buckets[7].size() == 2); // 67 77
      assertUnit(us.buckets[8].size() == 0);
      assertUnit(us.buckets[9].size() == 2); // 59 49
      if (us.buckets[1].size() == 1)
         assertUnit(us.buckets[1].front() == 31);
      if (us.buckets[7].size() == 2)
      {
         assertUnit(us.buckets[7].front() == 67);
         assertUnit(us.buckets[7].back() == 77);
      }
      if (us.buckets[9].size() == 2)
      {
         assertUnit(us.buckets[9].front() == 59);
         assertUnit(us.buckets[9].back() == 49);
      }
      assertUnit(p.first.pBucket == us.buckets + 7);
      assertUnit(p.first.pBucketEnd == us.buckets + 10);
      assertUnit(p.first.itList == us.buckets[7].rbegin());
      assertUnit(p.second == true);
   }
   
   // test that we can insert 67 into the empty hash
   void test_insert_standardDuplicate()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::pair<custom::unordered_set<std::size_t>::iterator, bool> p;
      // exercise
      p = us.insert(67);  // 67 % 10 == 7
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us);
      assertUnit(p.first.pBucket == us.buckets + 7);
      assertUnit(p.first.pBucketEnd == us.buckets + 10);
      assertUnit(p.first.itList == us.buckets[7].begin());
      assertUnit(p.second == false);
   }
   
   /***************************************
    * REMOVE
    ***************************************/
   
   // clear an empty hash
   void test_clear_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      // exercise
      us.clear();
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us);
   }  // teardown
   
   // clear all the elements from the standard fixture
   void test_clear_standard()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      // exercise
      us.clear();
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us);
      // teardown
   }
   
   // erase an empty hash
   void test_erase_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      custom::unordered_set<std::size_t>::iterator it = us.end();
      // exercise
      it = us.erase(99);
      // verify
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us);
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].begin());
   }  // teardown
   
   // clear all the elements from the standard fixture
   void test_erase_standardMissing()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it = us.end();
      // exercise
      it = us.erase(99);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us);
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].begin());
      // teardown
   }
   
   // clear all the elements from the standard fixture
   void test_erase_standardAlone()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it = us.end();
      // exercise
      it = us.erase(67);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] --> 59 49
      //std::cout << us.numElements << std::endl;
      assertUnit(us.numElements == 3);
      assertUnit(us.buckets[0].size() == 0);
      assertUnit(us.buckets[1].size() == 1); // 31
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 0);
      assertUnit(us.buckets[4].size() == 0);
      assertUnit(us.buckets[5].size() == 0);
      assertUnit(us.buckets[6].size() == 0);
      assertUnit(us.buckets[7].size() == 0);
      assertUnit(us.buckets[8].size() == 0);
      assertUnit(us.buckets[9].size() == 2); // 59 49
      if (us.buckets[1].size() == 1)
         assertUnit(us.buckets[1].front() == 31);
      if (us.buckets[9].size() == 2)
      {
         assertUnit(us.buckets[9].front() == 59);
         assertUnit(us.buckets[9].back() == 49);
      }
      assertUnit(it.pBucket == us.buckets + 9);
      //std::cout << it.pBucketEnd << std::endl;
      //std::cout << us.buckets + 10 << std::endl;
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[9].begin());
   }  // teardown
   
   // erase the first element on the list
   void test_erase_standardFront()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      custom::unordered_set<std::size_t>::iterator it = us.end();
      // exercise
      it = us.erase(59);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 49
      assertUnit(us.numElements == 3);
      assertUnit(us.buckets[0].size() == 0);
      assertUnit(us.buckets[1].size() == 1); // 31
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 0);
      assertUnit(us.buckets[4].size() == 0);
      assertUnit(us.buckets[5].size() == 0);
      assertUnit(us.buckets[6].size() == 0);
      assertUnit(us.buckets[7].size() == 1); // 67
      assertUnit(us.buckets[8].size() == 0);
      assertUnit(us.buckets[9].size() == 1); // 49
      if (us.buckets[1].size() == 1)
         assertUnit(us.buckets[1].front() == 31);
      if (us.buckets[7].size() == 1)
         assertUnit(us.buckets[7].front() == 67);
      if (us.buckets[9].size() == 1)
         assertUnit(us.buckets[9].front() == 49);
      assertUnit(it.pBucket == us.buckets + 9);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[9].begin());
   }  // teardown
   
   // erase the last element on the list
   void test_erase_standardBack()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      us.buckets[7].push_back(77);
      us.numElements++;
      custom::unordered_set<std::size_t>::iterator it = us.end();
      // exercise
      it = us.erase(77);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us);
      assertUnit(it.pBucket == us.buckets + 9);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[9].begin());
   }  // teardown
   
   // erase the last element on the entire hash
   void test_erase_standardLast()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      us.buckets[1].push_back(31);
      us.buckets[7].push_back(67);
      us.numElements = 2;
      custom::unordered_set<std::size_t>::iterator it = us.end();
      // exercise
      it = us.erase(67);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertUnit(us.numElements == 1);
      assertUnit(us.buckets[0].size() == 0);
      assertUnit(us.buckets[1].size() == 1); // 31
      assertUnit(us.buckets[2].size() == 0);
      assertUnit(us.buckets[3].size() == 0);
      assertUnit(us.buckets[4].size() == 0);
      assertUnit(us.buckets[5].size() == 0);
      assertUnit(us.buckets[6].size() == 0);
      assertUnit(us.buckets[7].size() == 0);
      assertUnit(us.buckets[8].size() == 0);
      assertUnit(us.buckets[9].size() == 0);
      if (us.buckets[1].size() == 1)
         assertUnit(us.buckets[1].front() == 31);
      assertUnit(it.pBucket == us.buckets + 10);
      assertUnit(it.pBucketEnd == us.buckets + 10);
      assertUnit(it.itList == us.buckets[0].end());
   }
   
   /***************************************
    * SIZE EMPTY 
    ***************************************/

    // size of an empty hash
   void test_size_empty()
   {  // setup
      custom::unordered_set<std::size_t> us;
      // exercise
      size_t size = us.size();
      // verify
      assertUnit(0 == size);
      assertEmptyFixture(us);
   }  // teardown

   // size of the standard hash
   void test_size_standard()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      // exercise
      size_t size = us.size();
      // verify
      assertUnit(4 == size);
      assertStandardFixture(us);
      // teardown
   }

   // empty hash empty?
   void test_empty_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      // exercise
      bool empty = us.empty();
      // verify
      assertUnit(true == empty);
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us);
   }  // teardown

   // standard hash empty?
   void test_empty_standard()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      // exercise
      bool empty = us.empty();
      // verify
      assertUnit(false == empty);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us);
      // teardown
   }

   // verify the bucket size of an empty hash
   void test_bucketSize_empty()
   {  // setup
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      custom::unordered_set<std::size_t> us;
      size_t i = 0;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_size(i);
      // verify
      assertUnit(num == 0);
      //      h[0] -->
      //      h[1] -->
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] -->
      //      h[8] -->
      //      h[9] -->
      assertEmptyFixture(us);
   }  // teardown

   // verify the bucket size of standard hash, empty bucket
   void test_bucketSize_standardEmpty()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      size_t i = 3;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_size(i);
      // verify
      assertUnit(num == 0);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us);
      // teardown
   }

   // verify the bucket size of standard hash, bucket has one element
   void test_bucketSize_standardOne()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      size_t i = 1;
      size_t num = 0;
      // exercise
      num = us.bucket_size(i);
      // verify
      assertUnit(num == 1);
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertStandardFixture(us);
      // teardown
   }

   // verify the bucket size of standard hash, bucket has two elements
   void test_bucketSize_standardTwo()
   {  // setup
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      custom::unordered_set<std::size_t> us;
      setupStandardFixture(us);
      size_t i = 9;
      size_t num = 0;
      Spy::reset();
      // exercise
      num = us.bucket_size(i);
      // verify
      //      h[0] -->
      //      h[1] --> 31
      //      h[2] -->
      //      h[3] -->
      //      h[4] -->
      //      h[5] -->
      //      h[6] -->
      //      h[7] --> 67
      //      h[8] -->
      //      h[9] --> 59 49
      assertUnit(num == 2);
      assertStandardFixture(us);
      // teardown
   }


   /*************************************************************
    * SETUP STANDARD FIXTURE
    *      h[0] -->  
    *      h[1] --> 31 
    *      h[2] -->  
    *      h[3] --> 
    *      h[4] --> 
    *      h[5] -->  
    *      h[6] --> 
    *      h[7] --> 67
    *      h[8] --> 
    *      h[9] --> 59 49
    *************************************************************/
   void setupStandardFixture(custom::unordered_set<std::size_t>& us)
   {
      // clear out whatever the default constructor created
      for (int i = 0; i < 10; i++)
         us.buckets[i].clear();

      // set the values
      us.buckets[1].push_back(31);
      us.buckets[7].push_back(67);
      us.buckets[9].push_back(59);
      us.buckets[9].push_back(49);

      // set the number of elements
      us.numElements = 4;
   }


   /*************************************************************
    * VERIFY STANDARD FIXTURE
    *      h[0] -->
    *      h[1] --> 31
    *      h[2] -->
    *      h[3] -->
    *      h[4] -->
    *      h[5] -->
    *      h[6] -->
    *      h[7] --> 67
    *      h[8] -->
    *      h[9] --> 59 49
    *************************************************************/
   void assertStandardFixtureParameters(custom::unordered_set<std::size_t>& us, int line, const char* function)
   {
      assertIndirect(us.numElements == 4);

      assertIndirect(us.buckets[0].size() == 0);
      assertIndirect(us.buckets[1].size() == 1); // 31
      assertIndirect(us.buckets[2].size() == 0);
      assertIndirect(us.buckets[3].size() == 0);
      assertIndirect(us.buckets[4].size() == 0);
      assertIndirect(us.buckets[5].size() == 0);
      assertIndirect(us.buckets[6].size() == 0);
      assertIndirect(us.buckets[7].size() == 1); // 67
      assertIndirect(us.buckets[8].size() == 0);
      assertIndirect(us.buckets[9].size() == 2); // 59 49

      if (us.buckets[1].size() == 1)
         assertIndirect(us.buckets[1].front() == 31);

      if (us.buckets[7].size() == 1)
         assertIndirect(us.buckets[7].front() == 67);

      if (us.buckets[9].size() == 2)
      {
         assertIndirect(us.buckets[9].front() == 59);
         assertIndirect(us.buckets[9].back() == 49);
      }
   }

   /*************************************************************
    * VERIFY EMPTY FIXTURE
    *      h[0] -->
    *      h[1] --> 
    *      h[2] -->
    *      h[3] -->
    *      h[4] -->
    *      h[5] -->
    *      h[6] -->
    *      h[7] --> 
    *      h[8] -->
    *      h[9] -->  
    *************************************************************/
   void assertEmptyFixtureParameters(custom::unordered_set<std::size_t>& us, int line, const char* function)
   {
      assertIndirect(us.numElements == 0);

      assertIndirect(us.buckets[0].size() == 0);
      assertIndirect(us.buckets[1].size() == 0);
      assertIndirect(us.buckets[2].size() == 0);
      assertIndirect(us.buckets[3].size() == 0);
      assertIndirect(us.buckets[4].size() == 0);
      assertIndirect(us.buckets[5].size() == 0);
      assertIndirect(us.buckets[6].size() == 0);
      assertIndirect(us.buckets[7].size() == 0);
      assertIndirect(us.buckets[8].size() == 0);
      assertIndirect(us.buckets[9].size() == 0);
   }
  

};

#endif // DEBUG
