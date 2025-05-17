#ifndef TCC__CONFIG_H
#define TCC__CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

// Режим обработки в одиночном режиме
 #define TCC_SINGLE_MODE
// Режим обработки в параллельном режиме
//	#define TCC_PARALLEL_MODE

#define TCC_MIN_MONEYS_PER_THREAD 10000u

#ifdef __cplusplus
}
#endif


#endif