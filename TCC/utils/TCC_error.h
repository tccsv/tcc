#ifndef TCC__ERROR_H
#define TCC__ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum TCC_Error
{
    TCC_ERROR_MISSING = 0,
    TCC_ERROR_MEMORY_ALLOCATION = 1,
    TCC_ERROR_PROFITS_LOSES_LOAD_ERROR = 2,
    TCC_ERROR_EMPLOYEES_LOAD_ERROR = 3,
    TCC_ERROR_OWNERSHIP_LOAD_ERROR = 4,
    TCC_ERROR_IMMOVABLES_LOAD_ERROR = 5
} TCC_Error;



#ifdef __cplusplus
}
#endif


#endif
