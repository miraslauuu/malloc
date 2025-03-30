/*
 * Unit Test Bootstrap
 * Autor: Tomasz Jaworski, 2018-2020
 *
 * Test dla zadania Projekt: Ocena 3,0
 * Autor testowanej odpowiedzi: Miraslau Alkhovik
 * Test wygenerowano automatycznie o 2024-12-15 20:47:52.873002
 *
 * Debug: 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>
#include <assert.h>

#if !defined(__clang__) && !defined(__GNUC__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na swoim kompilatorze C.
#error System testow jednostkowych jest przeznaczony dla kompilatorów GCC/Clang.
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na platformie Windows.
#error System testow jednostkowych NIE jest przeznaczony dla testów uruchamianych na platformach Windows.
#endif

#define _RLDEBUG_API_
#include "unit_helper_v2.h"
#include "rdebug.h"

#include "tested_declarations.h"
#include "rdebug.h"

//
// Elementy globalne dla całego testu
//




//
//  Test 1: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST1(void)
{
    // informacje o teście
    test_start(1, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);
                
                void *ptr = heap_malloc(0);
                
                test_error(ptr == NULL, "Funkcja heap_malloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 2: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST2(void)
{
    // informacje o teście
    test_start(2, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(67108830);

                test_error(ptr == NULL, "Funkcja heap_malloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 3: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST3(void)
{
    // informacje o teście
    test_start(3, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(775);

                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 4: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST4(void)
{
    // informacje o teście
    test_start(4, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(48);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(111);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 5: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST5(void)
{
    // informacje o teście
    test_start(5, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(89);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(15199081);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 6: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST6(void)
{
    // informacje o teście
    test_start(6, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(18);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(67137378);
                test_error(ptr2 == NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 7: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST7(void)
{
    // informacje o teście
    test_start(7, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(42);
                void *ptr2 = heap_malloc(138);
                void *ptr3 = heap_malloc(248);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(107);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 8: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST8(void)
{
    // informacje o teście
    test_start(8, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(74);
                void *ptr2 = heap_malloc(133);
                void *ptr3 = heap_malloc(238);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(133);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 9: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST9(void)
{
    // informacje o teście
    test_start(9, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {376, 387, 119, 634, 966, 413, 741, 208, 121, 921, 589, 983, 199, 446, 231, 165, 587, 603, 509, 848, 507, 698, 397, 937, 356, 481, 149, 739, 852, 172, 333, 813, 369, 945, 277, 228};
                void *ptrs[36];

                for (int i = 0; i < 36; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[23]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[23]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[23]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(913);
                test_error(ptrs[23] == ptr, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptrs[23] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 10: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
//
void UTEST10(void)
{
    // informacje o teście
    test_start(10, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {367, 787, 655, 973, 952, 956, 742, 703, 514, 423, 489, 167, 377, 573, 445, 478, 371, 718, 801, 923, 827, 138, 492, 537, 223, 407, 510, 945, 736, 552, 188, 206, 626, 427, 519, 267, 534, 262};
                void *ptrs[38];

                for (int i = 0; i < 38; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[6]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[6]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[6]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(742);
                test_error(ptrs[6] == ptr, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptrs[6] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 11: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST11(void)
{
    // informacje o teście
    test_start(11, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(68);
                void *ptr2 = heap_malloc(160);
                void *ptr3 = heap_malloc(228);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(170);
                
                test_error(ptr2 != ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 12: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST12(void)
{
    // informacje o teście
    test_start(12, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(99);
                void *ptr2 = heap_malloc(3);
                void *ptr3 = heap_malloc(254);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(11);

                test_error(ptr2 != ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 13: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST13(void)
{
    // informacje o teście
    test_start(13, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(0, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 14: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST14(void)
{
    // informacje o teście
    test_start(14, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(12, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 15: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST15(void)
{
    // informacje o teście
    test_start(15, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(0, 2);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 16: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST16(void)
{
    // informacje o teście
    test_start(16, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(67108847, 1);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 17: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST17(void)
{
    // informacje o teście
    test_start(17, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(16777216, 4);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 18: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST18(void)
{
    // informacje o teście
    test_start(18, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(371, 1);

                test_error(ptr != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 0; i < 371; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 19: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST19(void)
{
    // informacje o teście
    test_start(19, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(442, 2);

                test_error(ptr != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 0; i < 884; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 20: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST20(void)
{
    // informacje o teście
    test_start(20, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(66, 49);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 3234; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(173, 49);
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 8477; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 21: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST21(void)
{
    // informacje o teście
    test_start(21, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(18, 3);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 54; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(15278493, 3);
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 45835479; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 22: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST22(void)
{
    // informacje o teście
    test_start(22, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(30, 3);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 90; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(67687776, 3);
                test_error(ptr2 == NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 23: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST23(void)
{
    // informacje o teście
    test_start(23, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(94, 3);
                char *ptr2 = heap_calloc(127, 3);
                char *ptr3 = heap_calloc(234, 3);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                for (int i = 0; i < 282; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 381; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 702; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(108, 3);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 324; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 24: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST24(void)
{
    // informacje o teście
    test_start(24, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(13, 3);
                char *ptr2 = heap_calloc(142, 3);
                char *ptr3 = heap_calloc(236, 3);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                for (int i = 0; i < 39; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 426; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 708; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(142, 3);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 426; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 25: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST25(void)
{
    // informacje o teście
    test_start(25, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {605, 397, 341, 205, 337, 261, 645, 927, 741, 927, 581, 132, 442, 122, 164, 913, 382, 712, 341, 933, 638, 224, 610, 971, 844, 541, 790, 649, 118};
                size_t mem_units[] = {5, 3, 4, 4, 3, 4, 5, 5, 5, 3, 4, 2, 4, 2, 4, 5, 3, 2, 5, 2, 5, 3, 4, 1, 4, 5, 3, 5, 2};
                char *ptrs[29];

                for (int i = 0; i < 29; ++i)
                {
                    ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                        test_error(ptrs[i][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }

                heap_free(ptrs[28]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[28]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[28]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(109, 2);
                test_error(ptrs[28] == ptr, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptrs[28] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (size_t j = 0; j < 109 * 2; ++j)
                    test_error(ptrs[28][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 26: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST26(void)
{
    // informacje o teście
    test_start(26, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {455, 315, 785, 909, 800, 591, 632, 143, 517, 576, 976, 521, 311, 414, 228, 233, 269, 596, 360, 196, 594, 468, 692, 772, 502, 283};
                 size_t mem_units[] = {2, 5, 4, 2, 2, 5, 5, 4, 5, 1, 2, 2, 2, 3, 2, 2, 1, 2, 5, 5, 5, 3, 1, 1, 5, 5};
                 char *ptrs[26];

                 for (int i = 0; i < 26; ++i)
                 {
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                     test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                     test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                     for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                         test_error(ptrs[i][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                 }

                 heap_free(ptrs[14]);

                 test_error(pointer_unallocated == get_pointer_type(ptrs[14]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[14]));

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 void *ptr = heap_calloc(228, 2);
                 test_error(ptrs[14] == ptr, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptrs[14] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                 for (size_t j = 0; j < 228 * 2; ++j)
                     test_error(ptrs[14][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 27: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST27(void)
{
    // informacje o teście
    test_start(27, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(45, 1);
                char *ptr2 = heap_calloc(115, 1);
                char *ptr3 = heap_calloc(228, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                for (int i = 0; i < 45; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 115; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 228; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(125, 1);

                test_error(ptr2 != ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                for (int i = 0; i < 125; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 28: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST28(void)
{
    // informacje o teście
    test_start(28, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 29: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST29(void)
{
    // informacje o teście
    test_start(29, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 30: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST30(void)
{
    // informacje o teście
    test_start(30, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {520, 202, 571, 541, 736, 245, 721, 480, 289, 844, 919, 966, 638, 171, 225, 358, 430, 683, 888, 810, 806, 924, 913, 409, 556, 843, 650, 594, 391, 160, 749, 218, 429};
                void *ptrs[33];

                for (int i = 0; i < 33; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           


                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 966, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 966, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 31: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST31(void)
{
    // informacje o teście
    test_start(31, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {776, 969, 804, 600, 363, 804, 423, 821, 127, 747, 631, 333, 207, 263, 907, 228, 966, 739, 195, 742, 385, 765, 927, 504, 299, 402, 970, 776, 157, 765, 554, 219, 791, 238, 887, 514, 381};
                 size_t mem_units[] = {1, 5, 3, 3, 4, 2, 5, 1, 5, 5, 5, 5, 3, 1, 1, 3, 3, 3, 1, 5, 5, 3, 2, 1, 4, 2, 5, 5, 3, 1, 5, 2, 3, 5, 3, 3, 1};
                 char *ptrs[37];

                 for (int i = 0; i < 37; ++i)
                 {
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                     test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                 }

                 size_t block_size = heap_get_largest_used_block_size();
                 test_error(block_size == 4850, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 4850, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 32: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST32(void)
{
    // informacje o teście
    test_start(32, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {487, 410, 523, 581, 575, 782, 636, 437, 314, 123, 552, 356, 981, 330, 782, 868, 337, 128, 862, 714, 945, 220, 895, 227, 926};
                char *ptrs[25];

                for (int i = 0; i < 25; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                ptrs[9][-1] += 9 ;

                status = heap_validate();
                test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 33: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty
//
void UTEST33(void)
{
    // informacje o teście
    test_start(33, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                char *ptr = (char *)(intptr_t)rand();
                
                heap_free(ptr);
                
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);
            
                heap_free(NULL);
                
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 34: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST34(void)
{
    // informacje o teście
    test_start(34, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(177);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr + 1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 177, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 177, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr - 1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 177, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 177, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr + 99);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 177, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 177, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr - 24);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 177, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 177, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 35: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST35(void)
{
    // informacje o teście
    test_start(35, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(375);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(89);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 375, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 375, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 89, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 89, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 36: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST36(void)
{
    // informacje o teście
    test_start(36, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(365);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 365, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 365, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 37: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST37(void)
{
    // informacje o teście
    test_start(37, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(360);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(96);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 360, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 360, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 96, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 96, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 38: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
//
void UTEST38(void)
{
    // informacje o teście
    test_start(38, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(262);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(892);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(93);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 892, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 892, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 262, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 262, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 93, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 93, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 39: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST39(void)
{
    // informacje o teście
    test_start(39, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(394);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(934);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(66);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 934, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 934, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 394, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 394, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 66, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 66, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr4 = heap_malloc(1338);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error(ptr1 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 1338, "Funkcja heap_validate() powinna zwrócić wartość 1338, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 40: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
//
void UTEST40(void)
{
    // informacje o teście
    test_start(40, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(297);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(9525);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(979);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(57);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 9525, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 9525, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 979, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 979, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 297, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 297, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 41: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST41(void)
{
    // informacje o teście
    test_start(41, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(308);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(9261);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(990);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(83);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 9261, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 9261, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 990, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 990, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 308, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 308, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(10267);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr2 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 10267, "Funkcja heap_validate() powinna zwrócić wartość 10267, a zwróciła na %llu", heap_get_largest_used_block_size());


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 42: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą
//
void UTEST42(void)
{
    // informacje o teście
    test_start(42, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(8162);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(283);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(745);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(89);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 8162, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 8162, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 745, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 745, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 283, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 283, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 89, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 89, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 43: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST43(void)
{
    // informacje o teście
    test_start(43, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(5767);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(357);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(829);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(85);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 5767, "Funkcja heap_validate() powinna zwrócić wartość 5767, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 829, "Funkcja heap_validate() powinna zwrócić wartość 829, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 357, "Funkcja heap_validate() powinna zwrócić wartość 357, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 85, "Funkcja heap_validate() powinna zwrócić wartość 85, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(6961);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr1 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 6961, "Funkcja heap_validate() powinna zwrócić wartość 6961, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 44: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku
//
void UTEST44(void)
{
    // informacje o teście
    test_start(44, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(390);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(50);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 390, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 390, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 50, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 50, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr3 = heap_malloc(293);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 == ptr1, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(heap_get_largest_used_block_size() == 293, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 293, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                void *ptr4 = heap_malloc(390);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr4 == ptr1, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                test_error(heap_get_largest_used_block_size() == 390, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 390, a zwróciła na %llu", heap_get_largest_used_block_size());


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 45: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST45(void)
{
    // informacje o teście
    test_start(45, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(829);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(276);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(6858);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(50);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6858, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 6858, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 829, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 829, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr5 = heap_malloc(4319);
                test_error(ptr5 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr5 == ptr3, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));

                test_error(heap_get_largest_used_block_size() == 4319, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 4319, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr5);
                heap_free(ptr2);
                
                void *ptr6 = heap_malloc(1121);
                test_error(ptr6 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr6 == ptr2, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr6), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr6));

                test_error(heap_get_largest_used_block_size() == 1121, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 1121, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 46: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST46(void)
{
    // informacje o teście
    test_start(46, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(826);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(299);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_malloc(6970);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(54);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6970, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 6970, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 826, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 826, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr5 = heap_malloc(3294);
                test_error(ptr5 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr5 == ptr3, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));

                test_error(heap_get_largest_used_block_size() == 3294, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 3294, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);
                heap_free(ptr5);

                void *ptr6 = heap_malloc(1137);
                test_error(ptr6 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr6 == ptr2, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr6), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr6));

                test_error(heap_get_largest_used_block_size() == 1137, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 1137, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 47: Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST47(void)
{
    // informacje o teście
    test_start(47, "Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(9258);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr2 = heap_malloc(321);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                void *ptr3 = heap_calloc(913, 1);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr4 = heap_malloc(84);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 9258, "Funkcja heap_validate() powinna zwrócić wartość 9258, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 913, "Funkcja heap_validate() powinna zwrócić wartość 913, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 321, "Funkcja heap_validate() powinna zwrócić wartość 321, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 84, "Funkcja heap_validate() powinna zwrócić wartość 84, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(10503);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr1 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 10503, "Funkcja heap_validate() powinna zwrócić wartość 10503, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 48: Sprawdzanie poprawności działania funkcji heap_malloc i heap_free
//
void UTEST48(void)
{
    // informacje o teście
    test_start(48, "Sprawdzanie poprawności działania funkcji heap_malloc i heap_free", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  srand (time(NULL));

                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  char *ptr[2000];
                  int ptr_state[2000] = {0};

                  int is_allocated = 0;

                  for (int i = 0; i < 2000; ++i)
                  {
                    if (rand() % 100 < 66)
                    {
                      for (int j = 0; j < 2000; ++j)
                        if (ptr_state[j] == 0)
                        {
                          ptr_state[j] = 1;
                          ptr[j] = heap_malloc(rand() % 100 + 50);
                          is_allocated++;
                          break;
                        }
                    }
                    else
                    {
                      if (is_allocated)
                      {
                        int to_free = rand() % is_allocated;
                         for (int j = 0; j < 2000; ++j)
                          {
                              if (ptr_state[j] == 1 && !to_free)
                              {
                                ptr_state[j] = 0;
                                is_allocated--;
                                heap_free(ptr[j]);
                                break;
                              }        
                              to_free--;
                          }
                      }
                    }
                  }

                  for (int j = 0; j < 2000; ++j)
                    if (ptr_state[j] == 1)
                      heap_free(ptr[j]);

                  test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 49: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST49(void)
{
    // informacje o teście
    test_start(49, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                test_error(get_pointer_type(NULL) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(NULL));

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 50: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST50(void)
{
    // informacje o teście
    test_start(50, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                test_error(get_pointer_type(NULL) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(NULL));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 51: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST51(void)
{
    // informacje o teście
    test_start(51, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(170);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 52: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST52(void)
{
    // informacje o teście
    test_start(52, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(178);
                char *ptr1 = heap_malloc(1408);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr);

                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr + 36) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 36));
                test_error(get_pointer_type(ptr + 178) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 178));
                test_error(get_pointer_type(ptr - 1) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 53: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST53(void)
{
    // informacje o teście
    test_start(53, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1264);
                char *ptr1 = heap_malloc(1224);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error(get_pointer_type(ptr + 1264) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 1264));
                test_error(get_pointer_type(ptr - 1) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 54: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST54(void)
{
    // informacje o teście
    test_start(54, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1360);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 1; i < 1360; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_inside_data_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_data_block, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 55: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST55(void)
{
    // informacje o teście
    test_start(55, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1496);
                char *ptr1 = heap_malloc(1184);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr1 + i) == pointer_control_block) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 56: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST56(void)
{
    // informacje o teście
    test_start(56, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1544);
                char *ptr1 = heap_malloc(1024);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = 1544 + 1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr + i) == pointer_control_block) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 57: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST57(void)
{
    // informacje o teście
    test_start(57, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(872);
                char *ptr1 = heap_malloc(1344);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1, fences_size_before = 0, fences_size_after = 0;

                while (get_pointer_type(ptr + i) == pointer_inside_fences) 
                {
                    fences_size_before++;
                    --i;
                }

                i = 872 + 1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)             
                {
                    fences_size_after++;
                    ++i;
                }

                heap_free(ptr);

                for (int i = -fences_size_before; i < 872 + fences_size_after; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 58: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST58(void)
{
    // informacje o teście
    test_start(58, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(101, 140);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 59: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST59(void)
{
    // informacje o teście
    test_start(59, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1312, 19);
                char *ptr1 = heap_calloc(1072, 19);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr);

                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr + 35) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 35));
                test_error(get_pointer_type(ptr + 24928) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 1312));
                test_error(get_pointer_type(ptr - 1) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 60: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST60(void)
{
    // informacje o teście
    test_start(60, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1456, 20);
                char *ptr1 = heap_calloc(1192, 20);


                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error(get_pointer_type(ptr + 29120) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 1456));
                test_error(get_pointer_type(ptr - 1) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 61: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST61(void)
{
    // informacje o teście
    test_start(61, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1592, 12);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 1; i < 19104; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_inside_data_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_data_block, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 62: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST62(void)
{
    // informacje o teście
    test_start(62, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1256, 19);
                char *ptr1 = heap_calloc(1536, 19);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr1 + i) == pointer_control_block) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 63: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST63(void)
{
    // informacje o teście
    test_start(63, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_validate();
                test_error(status == 2, "Funkcja heap_validate() powinna zwrócić wartość 2, a zwróciła na %d", status);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 64: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST64(void)
{
    // informacje o teście
    test_start(64, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(166);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
     
                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 65: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST65(void)
{
    // informacje o teście
    test_start(65, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(148);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                int i = -1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)
                {
                    char temp = ptr[i];
                    ptr[i] += 12;

                    status = heap_validate();
                    test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                    ptr[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    --i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 66: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST66(void)
{
    // informacje o teście
    test_start(66, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(193);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                int i = 193;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)
                {
                    char temp = ptr[i];
                    ptr[i] += 20;

                    status = heap_validate();
                    test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                    ptr[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    ++i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 67: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST67(void)
{
    // informacje o teście
    test_start(67, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(113);
                char *ptr1 = heap_malloc(113);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                while (get_pointer_type(ptr1 + i) == pointer_control_block)
                {
                    char temp = ptr1[i];
                    ptr1[i] += 16;

                    status = heap_validate();
                    test_error(status == 3, "Funkcja heap_validate() powinna zwrócić wartość 3, a zwróciła na %d", status);

                    ptr1[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    --i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 68: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST68(void)
{
    // informacje o teście
    test_start(68, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {355, 756, 847, 641, 608, 604, 670, 203, 613, 905, 761, 887, 743, 278, 383, 627, 352, 690, 647, 933, 990, 785, 760, 589, 447, 769, 258, 297, 493, 281, 751};
                 size_t mem_units[] = {4, 1, 4, 1, 2, 5, 3, 4, 3, 4, 5, 2, 5, 2, 3, 4, 5, 4, 3, 4, 4, 3, 2, 4, 4, 3, 3, 2, 4, 4, 1};
                 char *ptrs[31];

                 for (int i = 0; i < 31; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 ptrs[30][-1] += 7;
                 
                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 69: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST69(void)
{
    // informacje o teście
    test_start(69, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {748, 146, 960, 235, 281, 622, 748, 713, 980, 975, 802, 472, 104, 566, 780, 635, 251, 541, 986, 894, 863, 272, 801, 216, 195, 260, 400, 486, 671, 325, 852, 862, 539, 689, 790, 469, 467, 881};
                 size_t mem_units[] = {4, 4, 5, 3, 5, 3, 1, 5, 3, 4, 3, 3, 3, 1, 5, 3, 3, 1, 5, 5, 5, 5, 2, 1, 5, 5, 4, 3, 1, 4, 3, 1, 3, 2, 5, 2, 1, 5};
                 char *ptrs[38];

                 for (int i = 0; i < 38; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 ptrs[21][mem_sizes[21] * mem_units[21]] += 6;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 70: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST70(void)
{
    // informacje o teście
    test_start(70, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {689, 649, 915, 384, 280, 360, 852, 848, 625, 163, 750, 656, 372, 183, 632, 593, 166, 532, 899, 699, 377, 217, 583, 979, 660, 921, 848, 298, 422, 440, 576, 369, 989, 416, 374, 801, 504, 556, 450};
                 size_t mem_units[] = {4, 5, 2, 1, 1, 3, 5, 3, 4, 2, 4, 1, 4, 1, 2, 3, 3, 1, 1, 4, 3, 1, 5, 1, 3, 1, 3, 3, 1, 3, 5, 4, 2, 2, 3, 1, 4, 5, 2};
                 char *ptrs[39];

                 for (int i = 0; i < 39; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 int i = -1;

                 while (get_pointer_type(ptrs[25] + i) == pointer_inside_fences) --i;

                 ptrs[25][i - 12] += 8;

                 status = heap_validate();
                 test_error(status == 3, "Funkcja heap_validate() powinna zwrócić wartość 3, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 71: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST71(void)
{
    // informacje o teście
    test_start(71, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 0);

                test_error(ptr == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 72: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
//
void UTEST72(void)
{
    // informacje o teście
    test_start(72, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                void *ptr = heap_realloc(NULL, 200);

                test_error(ptr == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 73: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
//
void UTEST73(void)
{
    // informacje o teście
    test_start(73, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 178);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 74: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST74(void)
{
    // informacje o teście
    test_start(74, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 102);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                void *ptr1 = heap_realloc(ptr, 0);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 75: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
//
void UTEST75(void)
{
    // informacje o teście
    test_start(75, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_realloc(NULL, 170);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                void *ptr1 = heap_realloc(ptr + 79, 170);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
        
                ptr1 = heap_realloc(ptr - 90, 170);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 76: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
//
void UTEST76(void)
{
    // informacje o teście
    test_start(76, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                unsigned char values[158];
                
                for (int i = 0; i < 158; ++i)
                    values[i] = rand() % 255;

                char *ptr = heap_realloc(NULL, 158);
                char *ptr2 = heap_realloc(NULL, 158);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error(ptr2 != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                memcpy(ptr, values, 158);

                void *ptr1 = heap_realloc(ptr, 87);

                test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                test_error(memcmp(ptr, values, 87) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                test_error(get_pointer_type(ptr + 87) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 87));

                 int i = 87;

                char temp = ptr[i];
                ptr[i] += 16;

                status = heap_validate();
                test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                ptr[i] = temp;

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                while (get_pointer_type(ptr + i) == pointer_inside_fences) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 77: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST77(void)
{
    // informacje o teście
    test_start(77, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[126];

                 for (int i = 0; i < 126; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 126);
                 char *ptr2 = heap_realloc(NULL, 126);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 test_error(ptr2 != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                 memcpy(ptr, values, 126);

                 void *ptr1 = heap_realloc(ptr, 126);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 126) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 126) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 126));

                  int i = 126;

                 char temp = ptr[i];
                 ptr[i] += 16;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 78: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST78(void)
{
    // informacje o teście
    test_start(78, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[144];

                 for (int i = 0; i < 144; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 144);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 memcpy(ptr, values, 144);

                 void *ptr1 = heap_realloc(ptr, 772);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 144) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 772) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 126));

                  int i = 772;

                 char temp = ptr[i];
                 ptr[i] += 16;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 79: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST79(void)
{
    // informacje o teście
    test_start(79, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[199];

                 for (int i = 0; i < 199; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 199);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 memcpy(ptr, values, 199);

                 void *ptr1 = heap_realloc(ptr, 12408839);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 199) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 12408839) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 126));

                  int i = 12408839;

                 char temp = ptr[i];
                 ptr[i] += 16;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 80: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST80(void)
{
    // informacje o teście
    test_start(80, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[176];

                  for (int i = 0; i < 176; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 176);
                  char *ptr2 = heap_realloc(NULL, 1310);
                  char *ptr3 = heap_realloc(NULL, 132);
                  char *ptr4 = heap_realloc(NULL, 197);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);
                
                  ptr2 = heap_realloc(NULL, 176);
                
                  heap_free(ptr3);

                  memcpy(ptr2, values, 176);

                  char *ptr5 = heap_realloc(ptr2, 1476);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 176) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 1476) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 1476));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 81: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST81(void)
{
    // informacje o teście
    test_start(81, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[156];

                  for (int i = 0; i < 156; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 156);
                  char *ptr2 = heap_realloc(NULL, 1805);
                  char *ptr3 = heap_realloc(NULL, 131);
                  char *ptr4 = heap_realloc(NULL, 111);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 156);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 156);

                  char *ptr5 = heap_realloc(ptr2, 1850);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 156) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 1850) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 1850));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 82: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST82(void)
{
    // informacje o teście
    test_start(82, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[132];

                  for (int i = 0; i < 132; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 132);
                  char *ptr2 = heap_realloc(NULL, 1152);
                  char *ptr3 = heap_realloc(NULL, 142);
                  char *ptr4 = heap_realloc(NULL, 127);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 132);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 132);

                  char *ptr5 = heap_realloc(ptr2, 4103);

                  test_error(ptr5 != ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr5, values, 132) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr5 + 4103) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr5 + 4103));
                  test_error(get_pointer_type(ptr2) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 83: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST83(void)
{
    // informacje o teście
    test_start(83, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[138];

                  for (int i = 0; i < 138; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 138);
                  char *ptr2 = heap_realloc(NULL, 1897);
                  char *ptr3 = heap_realloc(NULL, 100);
                  char *ptr4 = heap_realloc(NULL, 169);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 138);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 138);

                  char *ptr5 = heap_realloc(ptr2, 67627765);

                  test_error(ptr5 != ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 138) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 84: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST84(void)
{
    // informacje o teście
    test_start(84, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[175];

                  for (int i = 0; i < 175; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 151);
                  char *ptr2 = heap_realloc(NULL, 1186);
                  char *ptr3 = heap_realloc(NULL, 117);
                  char *ptr4 = heap_realloc(NULL, 175);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  memcpy(ptr4, values, 175);

                  char *ptr5 = heap_realloc(ptr4, 67678696);

                  test_error(ptr5 == NULL, "Funkcja heap_realloc() powinna zwrócić NULL");
                  test_error(memcmp(ptr4, values, 175) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  test_error(get_pointer_type(ptr4 + 175) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr4 + 175));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 85: Sprawdzanie poprawności działania funkcji heap_realloc
//
void UTEST85(void)
{
    // informacje o teście
    test_start(85, "Sprawdzanie poprawności działania funkcji heap_realloc", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                   srand (time(NULL));

                   int status = heap_setup();
                   test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                   char *ptr[2235];
                   int ptr_state[2235] = {0};

                   int is_allocated = 0;

                   for (int i = 0; i < 2235; ++i)
                   {
                     int rand_value = rand() % 100;
                     if (rand_value < 33)
                     {
                       for (int j = 0; j < 2235; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc(NULL, rand() % 1000 + 500);  
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 66)
                     {
                       if (is_allocated)
                       {
                          int to_reallocate = rand() % is_allocated;
                          for (int j = 0; j < 2235; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_reallocate)
                               {
                                   if (rand() % 100 < 50)
                                       ptr[j] = heap_realloc(ptr[j], rand() % 1000 + 500);
                                   else
                                       ptr[j] = heap_realloc(ptr[j], rand() % 10000 + 5000);    
                                   break;
                               }
                               to_reallocate--;
                           }
                       }
                     }
                     else
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 2235; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc(ptr[j], 0);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);           
                   }

                   for (int j = 0; j < 2235; ++j)
                     if (ptr_state[j] == 1)
                         heap_realloc(ptr[j], 0);

                   test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 86: Sprawdzanie poprawności działania funkcji heap_realloc, heap_malloc, heap_calloc i heap_free
//
void UTEST86(void)
{
    // informacje o teście
    test_start(86, "Sprawdzanie poprawności działania funkcji heap_realloc, heap_malloc, heap_calloc i heap_free", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                   srand (time(NULL));

                   int status = heap_setup();
                   test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                   char *ptr[983];
                   int ptr_state[983] = {0};

                   int is_allocated = 0;

                   for (int i = 0; i < 983; ++i)
                   {
                     int rand_value = rand() % 100;
                     if (rand_value < 16)
                     {
                       for (int j = 0; j < 983; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc(NULL, rand() % 100 + 50);  
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 33)
                     {
                       if (is_allocated)
                       {
                          int to_reallocate = rand() % is_allocated;
                          for (int j = 0; j < 983; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_reallocate)
                               {
                                   if (rand() % 100 < 50)
                                       ptr[j] = heap_realloc(ptr[j], rand() % 100 + 50);
                                   else
                                       ptr[j] = heap_realloc(ptr[j], rand() % 1000 + 500);    
                                   break;
                               }
                               to_reallocate--;
                           }
                       }
                     }
                     else if (rand_value < 50)
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 983; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc(ptr[j], 0);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     else if (rand_value < 66)
                     {
                       for (int j = 0; j < 983; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_malloc(rand() % 100 + 50);  
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 83)
                     {
                       for (int j = 0; j < 983; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_calloc(rand() % 100 + 50, rand() % 10 + 1);  
                           is_allocated++;
                           break;
                         }
                     }     
                     else
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 983; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_free(ptr[j]);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła %d", status);           
                   }

                   for (int j = 0; j < 983; ++j)
                     if (ptr_state[j] == 1)
                         heap_realloc(ptr[j], 0);

                   test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}




enum run_mode_t { rm_normal_with_rld = 0, rm_unit_test = 1, rm_main_test = 2 };

int __wrap_main(volatile int _argc, char** _argv, char** _envp)
{
    int volatile vargc = _argc;
    char ** volatile vargv = _argv, ** volatile venvp = _envp;
	volatile enum run_mode_t run_mode = rm_unit_test; // -1
	volatile int selected_test = -1;

    if (vargc > 1)
	{
	    char* smode = strtok(vargv[1], ",");
	    char* stest = strtok(NULL, "");
		char *errptr = NULL;
		run_mode = (enum run_mode_t)strtol(smode, &errptr, 10);
		if (*errptr == '\x0')
		{
			memmove(vargv + 1, vargv + 2, sizeof(char*) * (vargc - 1));
			vargc--;

			if (stest != NULL)
			{
			    int val = (int)strtol(stest, &errptr, 10);
			    if (*errptr == '\x0')
			        selected_test = val;
			}
		}
	}

    // printf("runmode=%d; selected_test=%d\n", run_mode, selected_test);

    // inicjuj testy jednostkowe
    unit_test_init(run_mode, "unit_test_v2.c");
    test_limit_init();
    rldebug_set_reported_severity_level(MSL_FAILURE);

    if (run_mode == rm_normal_with_rld)
    {
        // konfiguracja ograniczników
        rldebug_reset_limits();
        

        // uruchom funkcję main Studenta a potem wyświetl podsumowanie sterty i zasobów
        volatile int ret_code = rdebug_call_main(tested_main, vargc, vargv, venvp);

        rldebug_reset_limits();
        

        int leaks_detected = rldebug_show_leaked_resources(0);
        if (leaks_detected)
            raise(SIGHEAP);

        return ret_code;
    }

    
    if (run_mode == rm_unit_test)
    {
        test_title("Testy jednostkowe");

        void (*pfcn[])(void) =
        { 
            UTEST1, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST2, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST3, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST4, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST5, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST6, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST7, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST8, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST9, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
            UTEST10, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
            UTEST11, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST12, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST13, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST14, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST15, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST16, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST17, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST18, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST19, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST20, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST21, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST22, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST23, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST24, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST25, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
            UTEST26, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST27, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST28, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST29, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST30, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST31, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST32, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST33, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty
            UTEST34, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST35, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST36, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST37, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST38, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
            UTEST39, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST40, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
            UTEST41, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST42, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą
            UTEST43, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST44, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku
            UTEST45, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST46, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST47, // Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST48, // Sprawdzanie poprawności działania funkcji heap_malloc i heap_free
            UTEST49, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST50, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST51, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST52, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST53, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST54, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST55, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST56, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST57, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST58, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST59, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST60, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST61, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST62, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST63, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST64, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST65, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST66, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST67, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST68, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST69, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST70, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST71, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST72, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
            UTEST73, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
            UTEST74, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST75, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
            UTEST76, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
            UTEST77, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
            UTEST78, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST79, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST80, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST81, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST82, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST83, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST84, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST85, // Sprawdzanie poprawności działania funkcji heap_realloc
            UTEST86, // Sprawdzanie poprawności działania funkcji heap_realloc, heap_malloc, heap_calloc i heap_free
            NULL
        };

        for (int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx]();

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(86); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem
        return EXIT_SUCCESS;
    }
    

    if (run_mode == rm_main_test)
    {
        test_title("Testy funkcji main()");

        void (*pfcn[])(int, char**, char**) =
        { 
            NULL
        };

        for (volatile int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx](vargc, vargv, venvp);

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(0); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem

        return EXIT_SUCCESS;
    }

    printf("*** Nieznana wartość RunMode: %d", (int)run_mode);
    abort();
}