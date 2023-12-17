#include "containers.h"
#include <memory>

void
concurrent_list::cleanup()
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
concurrent_list::insert(i32 key)
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
concurrent_list::lookup(i32 key)
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
concurrent_list::del(i32 key)
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
concurrent_list::print()
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
concurrent_list::concurrent_list::getCount()
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

concurrent_queue::concurrent_queue() : headLock{}, tailLock{}
{
    headLock.init();
    tailLock.init();
    node *temp = (node *)malloc(sizeof(node));
    temp->next = nullptr;
    head = tail = temp;
}

concurrent_queue::~concurrent_queue() { this->cleanup(); }

void
concurrent_queue::enqueue(i32 val)
{

    node *temp = (node *)malloc(sizeof(node));
    ASSERT(temp != nullptr);
    temp->key = val;
    temp->next = nullptr;

    tailLock.lock();

    this->tail->next = temp;
    this->tail = temp;

    tailLock.unlock();
}

i32
concurrent_queue::dequeue(i32 *val)
{
    headLock.lock();

    node *tmp = this->head;
    node *new_head = tmp->next;
    if (new_head == nullptr)
    {
        headLock.unlock();
        return -1; // queue is empty
    }

    *val = new_head->key;
    this->head = new_head;
    headLock.unlock();

    free(tmp);
    return 0; // success
}

i32
concurrent_queue::count()
{
    i32 result = 0;
    tailLock.lock();
    headLock.lock();

    node *curr = head->next;
    while (curr != nullptr)
    {
        ++result;
        curr = curr->next;
    }

    headLock.unlock();
    tailLock.unlock();
    return result;
}

void
concurrent_queue::cleanup()
{
    this->tailLock.lock();
    this->headLock.lock();

    node *curr = this->head;
    while (curr != nullptr)
    {
        node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    this->head = nullptr;
    this->tail = nullptr;

    this->headLock.unlock();
    this->tailLock.unlock();
}
