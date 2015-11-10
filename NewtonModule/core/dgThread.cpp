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

dgThread::dgThread ()
   : m_id (0)
   , m_terminate (0)
   , m_threadRunning (0)
{
   m_name[0] = 0;
}

dgThread::dgThread (const char * const name, dgInt32 id)
   : m_id (id)
   , m_terminate (0)
   , m_threadRunning (0)
{
   strncpy (m_name, name, sizeof (m_name) - 1);
}


void dgThread::Init (const char * const name, dgInt32 id, dgInt32  stacksize)
{
   m_id = id;
   strncpy (m_name, name, sizeof (m_name) - 1);
   Init(stacksize);
}


bool dgThread::StillBusy() const
{
   return m_threadRunning ? true : false;
}



#ifdef DG_USE_THREAD_EMULATION

dgThread::dgSemaphore::dgSemaphore ()
{
   //m_sem = NULL;
}

dgThread::dgSemaphore::~dgSemaphore ()
{
}

void dgThread::dgSemaphore::Release ()
{
}

dgThread::dgCriticalSection::dgCriticalSection()
{
}

dgThread::dgCriticalSection::~dgCriticalSection()
{
}


dgThread::~dgThread ()
{
}

void dgThread::Init ()
{
}

void dgThread::Close ()
{
}

void dgThread::SuspendExecution (dgSemaphore & mutex)
{
}


void dgThread::SuspendExecution (dgInt32 count, dgSemaphore * const semArray)
{
}


int dgThread::GetPriority() const
{
   return 0;
}

void dgThread::SetPriority (int priority)
{
}


void * dgThread::dgThreadSystemCallback (void * threadData)
{
   return 0;
}

#else


dgThread::dgCriticalSection::dgCriticalSection()
#ifdef DG_USE_MUTEX_CRITICAL_SECTION
   : m_mutex (PTHREAD_MUTEX_INITIALIZER)
#else
   : m_mutex (0)
#endif
{
}

dgThread::dgCriticalSection::~dgCriticalSection()
{
};


dgThread::~dgThread ()
{
}

void dgThread::Init (dgInt32  stacksize)
{
   m_handle = std::thread (dgThreadSystemCallback, this);
}

void dgThread::Close ()
{
   if (m_handle.joinable() )
      m_handle.join();
}


dgInt32 dgThread::GetPriority() const
{
   // i think range is 0-100
   return 50;
}

void dgThread::SetPriority (int priority)
{
   // not supprted in c++c111
}


void dgThread::SuspendExecution (dgSemaphore & mutex)
{
   mutex.Wait();
}


void dgThread::SuspendExecution (dgInt32 count, dgSemaphore * const semArray)
{
   for (dgInt32 i = 0; i < count; i ++)
      SuspendExecution (semArray[i]);
}


void * dgThread::dgThreadSystemCallback (void * threadData)
{
   dgFloatExceptions exception;
   dgSetPrecisionDouble precision;
   dgThread * const me = (dgThread *) threadData;
   me->m_threadRunning.exchange (1);
   me->Execute (me->m_id);
   me->m_threadRunning.exchange (0);
   std::this_thread::yield();
   return 0;
}


#endif



