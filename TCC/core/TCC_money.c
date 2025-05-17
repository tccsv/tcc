#include "TCC/TCC_money.h"
#include "TCC/TCC_thr_config.h"
#include <pthread.h>
#include <stdlib.h>



typedef struct TCC_MoneyNotes
{
    size_t length;
    size_t capacity;
    TCC_Money* moneyArr;
    TCC_Date* dateArr;
} TCC_MoneyNotes;

typedef struct TCC_MoneyNotesStep_p
{
    TCC_Money* moneyPtr; 
    size_t startIndex;
    size_t endIndex;
    TCC_Money res;
} TCC_MoneyNotesStep_p;

TCC_Error TCC_moneyNotesCreate(TCC_MoneyNotes** moneyNotesPP, const size_t capacity)
{ 
    *moneyNotesPP = (TCC_MoneyNotes*) malloc(sizeof(TCC_MoneyNotes) 
        + capacity*(sizeof(TCC_Money) + sizeof(TCC_Date))); 
    if(*moneyNotesPP == NULL)
    {
        return TCC_ERROR_MEMORY_ALLOCATION;
    }
    (*moneyNotesPP)->capacity = capacity;
    (*moneyNotesPP)->length = 0; 
    (*moneyNotesPP)->moneyArr = (size_t*)  ((char *) *moneyNotesPP 
         + sizeof(TCC_MoneyNotes) );
    (*moneyNotesPP)->dateArr = (TCC_Date*) ((char *) (*moneyNotesPP)->moneyArr 
         + (*moneyNotesPP)->capacity * sizeof(*(*moneyNotesPP)->moneyArr));
    return TCC_ERROR_MISSING;
}

void TCC_moneyNotesDel(TCC_MoneyNotes** moneyNoterPP)
{
    free(*moneyNoterPP);
}

TCC_Error TCC_moneyNotesInsert(TCC_MoneyNotes *moneyNotesP, const TCC_Money money,
                               const TCC_Date date)
{
    ++moneyNotesP->length;
    moneyNotesP->dateArr[moneyNotesP->length-1] = date;
    moneyNotesP->moneyArr[moneyNotesP->length-1] = money;
}

size_t TCC_moneyNotesGetCapacity(const TCC_MoneyNotes* moneyNotesP)
{
    return moneyNotesP->capacity;
}

static size_t TCC_moneyYearFindFirst(const TCC_MoneyNotes * historyPtr, const TCC_DateYear year)
{
    size_t i = 0;
    while(i < historyPtr->length && historyPtr->dateArr[i].year != year)
    {
        ++i;
    }
    return i;
}

static size_t TCC_moneyYearFindLast(const TCC_MoneyNotes * historyPtr, const TCC_DateYear year)
{
    size_t i = historyPtr->length - 1;
    while(i > 0 && historyPtr->dateArr[i].year != year)
    {
        --i;
    }
    return i;
}

static TCC_Money TCC_moneyRangeSum_s(const TCC_MoneyNotes * notesPtr, 
                            const size_t startIndex, const size_t endIndex)
{
    TCC_Money res = 0;
    for(size_t i = startIndex; i < endIndex+1; ++i)
    {
        res += notesPtr->moneyArr[i];
    }
    return res;
}

// Измените функции, использующие threads.h на аналоги pthread
static int TCC_moneyRangeSum_p_step(void* data)
{    
    TCC_MoneyNotesStep_p* notesStepPtr = (TCC_MoneyNotesStep_p*) data;
    notesStepPtr->res = 0;
    for(size_t i = notesStepPtr->startIndex; i < notesStepPtr->endIndex+1; ++i)
    {
        notesStepPtr->res += (notesStepPtr->moneyPtr[i]);
    }
    return 0;
}

// Замените thrd_t на pthread_t
static void TCC_moneyRangeSum_p_init(const TCC_MoneyNotes * notesPtr, 
                                    const size_t startIndex, const size_t endIndex,
                                    pthread_t** threads, TCC_MoneyNotesStep_p** threadsData,
                                    const int numThreads)
{
    *threads = (pthread_t*) malloc(numThreads*sizeof(pthread_t));
    *threadsData = (TCC_MoneyNotesStep_p*) malloc(numThreads*sizeof(TCC_MoneyNotesStep_p));
    const size_t indicesPerThread = (endIndex - startIndex) / numThreads;    
    for(size_t i = 0; i < numThreads-1; ++i)
    {
        (*threadsData)[i].startIndex = startIndex + i*indicesPerThread;
        (*threadsData)[i].endIndex = startIndex + (i+1) * indicesPerThread - 1;
        (*threadsData)[i].moneyPtr = notesPtr->moneyArr;        
    }
    (*threadsData)[numThreads-1].startIndex = startIndex + (numThreads-1) * indicesPerThread;
    (*threadsData)[numThreads-1].endIndex = endIndex;
    (*threadsData)[numThreads-1].moneyPtr = notesPtr->moneyArr;  
}

static TCC_Money TCC_moneyRangeSum_p_proc(pthread_t* threads, TCC_MoneyNotesStep_p* threadsData, const int numThreads)
{
    TCC_Money res = 0;
    for(size_t i = 0; i < numThreads; ++i)
    {
        pthread_create(&threads[i], NULL, (void *(*)(void *))TCC_moneyRangeSum_p_step, (void*) &threadsData[i]);
    }
    for(size_t i = 0; i < numThreads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    for(size_t i = 0; i < numThreads; ++i)
    {
        res += threadsData[i].res; 
    }
    return res;
}

static TCC_Money TCC_moneyRangeSum_p(const TCC_MoneyNotes * notesPtr, 
                                    const size_t startIndex, const size_t endIndex)
{
    // int numThreads = TCC_getLogicalCPUCount();
    int numThreads = 8;
    if(endIndex-startIndex < numThreads*TCC_MIN_MONEYS_PER_THREAD)
    {
        return TCC_moneyRangeSum_s(notesPtr, startIndex, endIndex);
    } 
    TCC_Money res = 0;
    pthread_t* threads;
    TCC_MoneyNotesStep_p* threadsData;
    TCC_moneyRangeSum_p_init(notesPtr, startIndex, endIndex, 
        &threads, &threadsData, numThreads);
    return TCC_moneyRangeSum_p_proc(threads, threadsData, numThreads);
}

static TCC_Money TCC_moneyRangeSum(const TCC_MoneyNotes * notesPtr, 
                            const size_t startIndex, const size_t endIndex)
{    
    #ifdef TCC_SINGLE_MODE
        #undef TCC_PARALLEL_MODE
        return TCC_moneyRangeSum_s(notesPtr, startIndex, endIndex);
    #endif
    #ifdef TCC_PARALLEL_MODE
        return TCC_moneyRangeSum_p(notesPtr, startIndex, endIndex);
    #endif
    return TCC_moneyRangeSum_s(notesPtr, startIndex, endIndex);
}

TCC_Money TCC_moneyYearSum(const TCC_MoneyNotes * notesPtr, const TCC_DateYear year)
{
    size_t startIndex = TCC_moneyYearFindFirst(notesPtr, year);
    if(startIndex == notesPtr->length)
    {
        return 0;
    }
    size_t endIndex = TCC_moneyYearFindLast(notesPtr, year); 
    return TCC_moneyRangeSum(notesPtr, startIndex, endIndex);
}