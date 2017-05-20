#ifndef MULTIPLATFORM_H
#define MULTIPLATFORM_H

#ifdef WINDOWS
#define THREAD HANDLE
#define THREADFUNC DWORD WINAPI
#define THREADRETURN 0
#define THREAD_CREATE(handle, function, data) *handle = CreateThread(NULL, 0, function, data, 0, NULL)
#define THREAD_WAIT(handle) WaitForSingleObject(*handle, INFINITE)
#define MUTEX HANDLE
#define MUTEX_CREATE(handle) *handle = CreateMutex(NULL, FALSE, NULL)
#define MUTEX_LOCK(handle) WaitForSingleObject(*handle, INFINITE)
#define MUTEX_UNLOCK(handle) ReleaseMutex(*handle)
#define MUTEX_DESTROY(handle) CloseHandle(*handle)
#define SOCKET SOCKET
#define socklen_t int
#define CLOSESOCKET closesocket
#define DELAY Sleep
#else
#define THREAD pthread_t
#define THREADFUNC void*
#define THREADRETURN NULL
#define THREAD_CREATE(handle, function, data) pthread_create(handle, NULL, function, data)
#define THREAD_WAIT(handle) pthread_join(*handle, NULL)
#define MUTEX pthread_mutex_t
#define MUTEX_CREATE(handle) pthread_mutex_init(handle, NULL)
#define MUTEX_LOCK(handle) pthread_mutex_lock(handle)
#define MUTEX_UNLOCK(handle) pthread_mutex_unlock(handle)
#define MUTEX_DESTROY(handle) pthread_mutex_destroy(handle)
#define SOCKET int
#define CLOSESOCKET close
#define DELAY delay
#endif

#endif
