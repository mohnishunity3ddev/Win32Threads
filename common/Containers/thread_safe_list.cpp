#include "thread_safe_list.h"
#include <memory>

void
list::cleanup()
{
    _mutex.lock();
    node *curr = head;
    while (curr != nullptr)
    {
        node *t = curr;
        curr = curr->next;
        free(t);
    }
    _mutex.unlock();
}

i32
list::insert(i32 key)
{
    node *n = (node *)malloc(sizeof(node));
    if (n == nullptr)
    {
        Logger::LogFatalUnformatted("Problem getting memory for node\n");
        return -1;
    }
    n->next = nullptr;
    n->key = key;

    _mutex.lock();
    n->next = this->head;
    this->head = n;
    _mutex.unlock();

    return 0;
}

i32
list::lookup(i32 key)
{
    i32 result = -1;

    _mutex.lock();
    node *curr = head;
    while (curr != nullptr)
    {
        if (curr->key == key)
        {
            result = 0;
            break;
        }
        curr = curr->next;
    }
    _mutex.unlock();

    return result;
}

i32
list::del(i32 key)
{
    i32 result = -1;
    _mutex.lock();

    node *curr = head;
    node *prev = curr;

    while (curr != nullptr)
    {
        if (curr->key == key)
        {
            prev->next = curr->next;
            curr->next = nullptr;
            free(curr);
            result = 0;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
    _mutex.unlock();

    return result;
}

void
list::print()
{
    _mutex.lock();

    Logger::LogDebugUnformatted("This list is as follows:\n");
    node *curr = head;
    while (curr != nullptr)
    {
        Logger::LogInfo("%d -> ", curr->key);
        curr = curr->next;
    }
    Logger::LogInfoUnformatted("NULL\n");

    _mutex.unlock();
}

i32
list::list::getCount()
{
    i32 result = 0;

    _mutex.lock();
    node *curr = head;
    while (curr != nullptr)
    {
        ++result;
        curr = curr->next;
    }
    _mutex.unlock();

    return result;
}
