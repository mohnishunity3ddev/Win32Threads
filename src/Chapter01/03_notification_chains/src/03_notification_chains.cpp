#include <windows.h>

#include <GlueThread/glthread.h>
#include <Logger/Win32Logger.h>

typedef struct _person
{
    int age;
    int weight;
    glthread_t glthread;
} person_t;

int
senior_citizen(void *p1, void *p2)
{
    person_t *person1 = (person_t *)p1;
    person_t *person2 = (person_t *)p2;
    
    if (person1->age == person2->age)
        return 0;
    if (person1->age < person2->age)
        return 1;
    return -1;
}

GLTHREAD_TO_STRUCT(thread_to_person, person_t, glthread)

DWORD
GetNumberOfPages(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize)
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    MEMORY_BASIC_INFORMATION memInfo;
    VirtualQueryEx(hProcess, lpAddress, &memInfo, sizeof(memInfo));

    if (memInfo.State != MEM_COMMIT)
    {
        return 0;
    }

    DWORD pages = dwSize / sysInfo.dwPageSize;
    if (dwSize % sysInfo.dwPageSize > 0)
    {
        pages++;
    }

    return pages;
}

i32
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
          i32 CmdShow)
{
#if 0
    person_t person[5];
    memset(person, 0, sizeof(person_t) * 5);
    person[0].age = 1;
    person[0].weight = 2;
    person[1].age = 3;
    person[1].weight = 4;
    person[2].age = 5;
    person[2].weight = 6;
    person[3].age = 7;
    person[3].weight = 8;
    person[4].age = 9;
    person[4].weight = 10;
    
    glthread_t base_glthread;
    init_glthread(&base_glthread);
    
    glthread_priority_insert(&base_glthread, &person[4].glthread, senior_citizen, OFFSETOF(person_t, glthread));
    glthread_priority_insert(&base_glthread, &person[3].glthread, senior_citizen, OFFSETOF(person_t, glthread));
    glthread_priority_insert(&base_glthread, &person[2].glthread, senior_citizen, OFFSETOF(person_t, glthread));
    glthread_priority_insert(&base_glthread, &person[1].glthread, senior_citizen, OFFSETOF(person_t, glthread));
    glthread_priority_insert(&base_glthread, &person[0].glthread, senior_citizen, OFFSETOF(person_t, glthread));
    
    glthread_t *curr = NULL;
    ITERATE_GLTHREAD_BEGIN(&base_glthread, curr)
    {
        
        person_t *p = thread_to_person(curr);
        Win32Logger::LogInfo("Age = %d\n", p->age);
    }
    ITERATE_GLTHREAD_END(&base_glthread, curr);
    DWORD pages;
    
    pause();
    FreeConsole();
    return 0;
#else
    // Allocate memory using VirtualAlloc
    HANDLE hProcess = GetCurrentProcess();
    LPVOID lpAddress =
        VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
    
    // Get the number of pages allocated
    DWORD pages = GetNumberOfPages(hProcess, lpAddress, 4096);
    
    // Print the number of pages
    printf("Number of pages allocated: %d\n", pages);
    
    // Free the allocated memory
    VirtualFreeEx(hProcess, lpAddress, 4096, MEM_RELEASE);
    
    return 0;
#endif
}
