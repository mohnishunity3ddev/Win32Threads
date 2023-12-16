#if !defined(CONTAINERS_H)

#include <windows.h>
#include "defines.h"
#include "Thread/thread_api.h"

struct node
{
    i32 key;
    struct node *next;
};

class concurrent_list
{
  private:
    node *head;
    mutex _mutex;

   public:
    concurrent_list() : _mutex{}, head(nullptr){}
    ~concurrent_list()
    {
        cleanup();
    }

    concurrent_list(const concurrent_list &other) = delete;
    concurrent_list(concurrent_list &&other) = delete;
    concurrent_list &operator=(const concurrent_list &other) = delete;
    concurrent_list &operator=(concurrent_list &&other) = delete;

    void cleanup();
    i32 insert(i32 key);
    i32 lookup(i32 key);
    i32 del(i32 key);
    void print();
    i32 getCount();
};

class concurrent_queue
{
  private:
    node *head;
    node *tail;
    mutex headLock, tailLock;

  public:
    concurrent_queue();
    ~concurrent_queue();
    void enqueue(i32 val);
    i32 dequeue(i32 *val);
    i32 count();
    void cleanup();
};



#define CONTAINERS_H
#endif // CONTAINERS_H