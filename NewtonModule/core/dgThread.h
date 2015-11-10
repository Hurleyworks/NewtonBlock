/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __DG_THREAD_API_H__
#define __DG_THREAD_API_H__

// by default newton is run on a separate thread, optionally concurrent with the calling thread, it also uses a thread job pool for multi core systems.
// define DG_USE_THREAD_EMULATION on the command line for platform that do not support hardware multi threading or if multi threading is not stable
//#define DG_USE_THREAD_EMULATION

//
//#define DG_USE_MUTEX_CRITICAL_SECTION

class dgThread
{
   public:
      class dgCriticalSection
      {
         public:
            dgCriticalSection();
            ~dgCriticalSection();
            void Lock(bool yield);
            void Unlock();

         private:
#if (defined (DG_USE_THREAD_EMULATION) || !defined (DG_USE_MUTEX_CRITICAL_SECTION))
            std::atomic<dgInt32> m_mutex;
#else
            std::mutex  m_mutex;
#endif
      };

      class dgSemaphore
      {
            // http://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
         private:
            std::mutex mtx;
            std::condition_variable cv;
            int count;

         public:
            dgSemaphore (int count_ = 0): count (count_)
            {
               ;
            }
            void Release()
            {
               std::unique_lock<std::mutex> lck (mtx);
               ++count;
               cv.notify_one();
            }
            void Wait()
            {
               std::unique_lock<std::mutex> lck (mtx);
               while (count == 0)
                  cv.wait (lck);
               count--;
            }
            friend class dgThread;
      };

      dgThread ();
      dgThread (const char * const name, dgInt32 id);
      virtual ~dgThread ();

      virtual void Execute (dgInt32 threadId) = 0;

      bool StillBusy() const;
      void SuspendExecution (dgSemaphore & mutex);
      void SuspendExecution (dgInt32 count, dgSemaphore * const mutexes);

      dgInt32 GetPriority() const;
      void SetPriority (int priority);

   protected:
      void Init (dgInt32 stacksize = 0);
      void Init (const char * const name, dgInt32 id, dgInt32 stacksize);
      void Close ();

      static void * dgThreadSystemCallback (void * threadData);

      std::thread m_handle;
      dgInt32 m_id;
      std::atomic<dgInt32> m_terminate;
      std::atomic<dgInt32> m_threadRunning;

      char m_name[32];
};



inline void dgThread::dgCriticalSection::Lock(bool yield)
{
#ifndef DG_USE_THREAD_EMULATION
#ifdef DG_USE_MUTEX_CRITICAL_SECTION
   m_mutex.lock():
#else
   while (m_mutex.exchange (1))
      std::this_thread::yield();
#endif
#endif
   }

   inline void dgThread::dgCriticalSection::Unlock()
{
#ifndef DG_USE_THREAD_EMULATION
#ifdef DG_USE_MUTEX_CRITICAL_SECTION
   m_mutex.unlock();
#else
   m_mutex.exchange (0);
#endif
#endif
}


#endif
