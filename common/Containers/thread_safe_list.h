#if !defined(THREAD_SAFE_LIST_H)

#include <windows.h>
#include "defines.h"
#include "Thread/mutex.h"

struct node
{
    i32 key;
    struct node *next;
};

class list
{
  private:
    node *head;
    mutex _mutex;

   public:
    list() : _mutex{}, head(nullptr){}
    ~list()
    {
        cleanup();
    }

    list(const list &other) = delete;
    list(list &&other) = delete;
    list &operator=(const list &other) = delete;
    list &operator=(list &&other) = delete;

    void cleanup();
    i32 insert(i32 key);
    i32 lookup(i32 key);
    i32 del(i32 key);
    void print();
    i32 getCount();
};



#define THREAD_SAFE_LIST_H
#endif // THREAD_SAFE_LIST_H