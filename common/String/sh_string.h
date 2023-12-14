#if !defined(SH_STRING_H)

void
StringCopy(char *Destination, const char *Source)
{
    while (*Source != '\0')
    {
        *Destination++ = *Source++;
    }
    *Destination = '\0';
}

size_t
StringLength(const char *String)
{
    size_t Result = 0;
    
    while(*String++ != '\0')
    {
        ++Result;
    }
    
    return Result;
}

#define SH_STRING_H
#endif // SH_STRING_H