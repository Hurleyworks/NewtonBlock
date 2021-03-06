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


dgMutexThread::dgMutexThread (const char * const name, dgInt32 id, dgInt32 stackSize)
   : dgThread (name, id)
   , m_isBusy (0)
   , m_myMutex()
   , m_callerMutex()
{
   Init (stackSize);
}

dgMutexThread::~dgMutexThread (void)
{
   Terminate();
}

void dgMutexThread::Terminate()
{
   if (StillBusy())
   {
      m_terminate.exchange (1);
      m_callerMutex.Release();
      m_myMutex.Release();
      Close();
   }
}


void dgMutexThread::Execute (dgInt32 threadID)
{
   // suspend this tread until the call thread decide to
   dgAssert (threadID == m_id);
   while (!m_terminate)
   {
      // wait for the main thread to signal an update
      m_isBusy.exchange (0);
      SuspendExecution (m_myMutex);
      m_isBusy.exchange (1);
      if (!m_terminate)
      {
         TickCallback (threadID);
         // let main thread resume execution
         m_callerMutex.Release();
      }
   }
   m_isBusy.exchange (0);
}

bool dgMutexThread::IsBusy() const
{
   return m_isBusy ? true : false;
}

void dgMutexThread::Tick()
{
   // let the thread run until the update function return
   m_myMutex.Release();
   // wait for the thread function to return
   SuspendExecution (m_callerMutex);
}

