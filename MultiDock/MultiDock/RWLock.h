/******************************************************************************
Module:  SWMRG.h
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#pragma once

#ifdef _DEBUG_RWLOCK
#include <map>
using namespace std;
#endif


///////////////////////////////////////////////////////////////////////////////

#ifdef _CATCORE_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class DLL_EXPORT CRWLock {
public:
   CRWLock();                 // Constructor
   ~CRWLock();                // Destructor

   VOID WaitToRead();        // Call this to gain shared read access
   VOID WaitToWrite();       // Call this to gain exclusive write access
   VOID Done();              // Call this when done accessing the resource

public:
   CRITICAL_SECTION m_cs;    // Permits exclusive access to other members
   HANDLE m_hsemReaders;     // Readers wait on this if a writer has access
   HANDLE m_hsemWriters;     // Writers wait on this if a reader has access
   int    m_nWaitingReaders; // Number of readers waiting for access
   int    m_nWaitingWriters; // Number of writers waiting for access
   int    m_nActive;         // Number of threads currently with access
                             //   (0=no threads, >0=# of readers, -1=1 writer)
#ifdef _DEBUG_RWLOCK
   map<int,int>   m_mapActive;
#endif
};


class DLL_EXPORT CRWReadLock
{
   CRWReadLock(CRWLock& lock)
      : m_Lock(lock)
   {
      m_Lock.WaitToRead();
   }

   ~CRWReadLock()
   {
      m_Lock.Done();
   }

   CRWLock& m_Lock;
};

class DLL_EXPORT CRWWriteLock
{
   CRWWriteLock(CRWLock& lock)
      : m_Lock(lock)
   {
      m_Lock.WaitToWrite();
   }

   ~CRWWriteLock()
   {
      m_Lock.Done();
   }

   CRWLock& m_Lock;
};


//////////////////////////////////////////////////////////////////////////
//Extended
class DLL_EXPORT CRWReader
{
public:
   CRWReader(CRWLock& lock)
      :m_lock(lock)
   {
      m_lock.WaitToRead();
      m_bLocked  = true;
   }

   ~CRWReader()
   {
      m_lock.Done();
      m_bLocked = false;
   }

   void Lock(){
      if(!m_bLocked){
         m_lock.WaitToRead();
         m_bLocked = true;
      }
   }

   void Unlock(){
      if(m_bLocked){
         m_lock.Done();
         m_bLocked = false;
      }
   }

   CRWLock& m_lock;
   bool m_bLocked;
};

class DLL_EXPORT CRWWriter
{
public:
   CRWWriter(CRWLock& lock)
      :m_lock(lock)
   {
      m_lock.WaitToWrite();
      m_bLocked = true;
   }

   ~CRWWriter()
   {
      m_lock.Done();
      m_bLocked = false;
   }

   void Lock(){
      if(!m_bLocked){
         m_lock.WaitToWrite();
         m_bLocked = true;
      }
   }

   void Unlock(){
      if(m_bLocked){
         m_lock.Done();
         m_bLocked = false;
      }
   }

   CRWLock& m_lock;
   bool m_bLocked;
};
