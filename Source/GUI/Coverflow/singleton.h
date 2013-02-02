#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "includes.h"

/*
	Pattern : Singleton
*/

template <class ClassName>
class Singleton {
private:
   static ClassName* singletonInstance;
   static CriticalSection singletonLock;

public:
    static ClassName* getInstance()
    {
        if (singletonInstance == 0)
        {
            const ScopedLock sl (singletonLock);
            if (singletonInstance == 0)
            {
                static bool alreadyInside = false;
                static bool createdOnceAlready = false;
                const bool problem = alreadyInside || createdOnceAlready;
                jassert (!problem);
                if (! problem)
                {
                    createdOnceAlready = true;
                    alreadyInside = true;
                    ClassName* newObject = new ClassName();  /* (use a stack variable to avoid setting the newObject value before the class has finished its constructor) */
                    alreadyInside = false;
                    singletonInstance = newObject;
                }
            }
        }

        return singletonInstance;
    }

    static inline ClassName* getInstanceWithoutCreating() throw()
    {
        return singletonInstance;
    }

    static void deleteInstance()
    {
        const ScopedLock sl (singletonLock);
        if (singletonInstance != 0)
        {
            ClassName* const old = singletonInstance;
            singletonInstance = 0;
            delete old;
        }
    }

    void clearSingletonInstance() throw()
    {
        if (singletonInstance == this)
            singletonInstance = 0;
   }
 
};

template <typename ClassName> ClassName* Singleton<ClassName>::singletonInstance = 0;
template <typename ClassName> CriticalSection Singleton<ClassName>::singletonLock; 

#endif