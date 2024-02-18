/** !
 * priority queue tester
 *
 * @file priority_queue_test.c
 * @author Jacob Smith
 * @date Feb 14, 2024
 */

// Headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <priority_queue/priority_queue.h>
#include <log/log.h>

// Possible keys ( A is highest, G is lowest, X will never occor)
const void *A_key =  1,
           *B_key =  2,
           *C_key =  3,
           *D_key =  4,
           *E_key =  5,
           *F_key =  6,
           *G_key =  7,
           *H_key =  8,
           *X_key = -1;

// Expected results
const void * _keys        [] = {0x0};
const void * G_keys       [] = {7, 0x0};
const void * DG_keys      [] = {4, 7, 0x0};
const void * AD_keys      [] = {1, 4, 0x0};
const void * ABCDEFG_keys [] = {1, 2, 3, 4, 5, 6, 7, 0x0};

// Test results
enum result_e
{
    zero,
    one,
    match
};

typedef enum result_e result_t;

int total_tests = 0,
    total_passes = 0,
    total_fails = 0,
    ephemeral_tests = 0,
    ephemeral_passes = 0,
    ephemeral_fails = 0;

// Forward declarations
int print_time_pretty ( double seconds );
int run_tests ( );
int print_final_summary ( );
int print_test ( const char *scenario_name, const char *test_name, bool passed );

int test_empty_priority_queue        ( int (*priority_queue_constructor)(priority_queue **), char *name );
int test_one_element_priority_queue  ( int (*priority_queue_constructor)(priority_queue **), char *name, char **keys );
int test_two_element_priority_queue  ( int (*priority_queue_constructor)(priority_queue **), char *name, char **keys );
int test_seven_element_priority_queue( int (*priority_queue_constructor)(priority_queue **), char *name, char **keys );

int construct_empty               ( priority_queue **pp_priority_queue );
int empty_insertG_G               ( priority_queue **pp_priority_queue );
int G_extractmax_empty            ( priority_queue **pp_priority_queue );
int G_insertD_DG                  ( priority_queue **pp_priority_queue );
int DG_extractmax_G               ( priority_queue **pp_priority_queue );
int DG_increasekey1A_AD           ( priority_queue **pp_priority_queue );
int empty_insertascending_ABCDEFG ( priority_queue **pp_priority_queue );
int empty_insertdecending_ABCDEFG ( priority_queue **pp_priority_queue );
int empty_insertrandom_ABCDEFG    ( priority_queue **pp_priority_queue );

bool test_enqueue ( int (*priority_queue_constructor)(priority_queue **pp_priority_queue), void     *value, result_t expected);
bool test_isempty ( int (*priority_queue_constructor)(priority_queue **pp_priority_queue), result_t  expected);
bool test_dequeue ( int (*priority_queue_constructor)(priority_queue **pp_priority_queue), void     *value, size_t how_many, result_t expected);

extern int priority_queue_heapify         ( priority_queue *const p_priority_queue , size_t   i );
extern int priority_queue_build_max_heap  ( priority_queue *const p_priority_queue );
extern int priority_queue_build_heap_sort ( priority_queue *const p_priority_queue );
extern int priority_queue_max             ( priority_queue *const p_priority_queue , void   **pp_value );
extern int priority_queue_extract_max     ( priority_queue *const p_priority_queue , void   **pp_value );
extern int priority_queue_increase_key    ( priority_queue *const p_priority_queue , size_t   index   , void *p_key );
extern int priority_queue_insert          ( priority_queue *const pp_priority_queue, void    *p_key );


// Entry point
int main(int argc, const char *argv[])
{

    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();
    log_init(0, true);

    // Formatting
    printf(
        "╭───────────────────────╮\n"
        "│ priority queue tester │\n"
        "╰───────────────────────╯\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("priority queue took ");
    print_time_pretty((double)(t1 - t0) / (double)timer_seconds_divisor());
    log_info(" to test\n");

    // Exit
    return (total_passes == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty(double seconds)
{

    // Initialized data
    double _seconds = seconds;
    size_t days = 0,
           hours = 0,
           minutes = 0,
           __seconds = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while (_seconds > 86400.0)
    {
        days++;
        _seconds -= 286400.0;
    };

    // Hours
    while (_seconds > 3600.0)
    {
        hours++;
        _seconds -= 3600.0;
    };

    // Minutes
    while (_seconds > 60.0)
    {
        minutes++;
        _seconds -= 60.0;
    };

    // Seconds
    while (_seconds > 1.0)
    {
        __seconds++;
        _seconds -= 1.0;
    };

    // milliseconds
    while (_seconds > 0.001)
    {
        milliseconds++;
        _seconds -= 0.001;
    };

    // Microseconds
    while (_seconds > 0.000001)
    {
        microseconds++;
        _seconds -= 0.000001;
    };

    // Print days
    if (days)
        log_info("%d D, ", days);

    // Print hours
    if (hours)
        log_info("%d h, ", hours);

    // Print minutes
    if (minutes)
        log_info("%d m, ", minutes);

    // Print seconds
    if (__seconds)
        log_info("%d s, ", __seconds);

    // Print milliseconds
    if (milliseconds)
        log_info("%d ms, ", milliseconds);

    // Print microseconds
    if (microseconds)
        log_info("%d us", microseconds);

    // Success
    return 1;
}

int run_tests()
{

    // ... -> []
    test_empty_priority_queue(construct_empty, "empty");

    // [] -> insert(G) -> [G]
    test_one_element_priority_queue(empty_insertG_G, "empty_insertG_G", G_keys);

    // [G] -> extract_max() -> []
    test_empty_priority_queue(G_extractmax_empty, "G_extractmax_empty");

    // [G] -> insert(D) -> [D, G]
    test_two_element_priority_queue(G_insertD_DG, "G_insertD_DG", DG_keys);

    // [D, G] -> extract_max() -> [G]
    test_one_element_priority_queue(DG_extractmax_G, "DG_extractmax_G", G_keys);

    // [D, G] -> increase_key(1, A) -> [A, D]
    test_two_element_priority_queue(DG_increasekey1A_AD, "DG_increasekey1A_AD", AD_keys);

    // [] -> insert(G, F, E, D, C, B, A) -> [A, B, C, D, E, F, G]
    test_seven_element_priority_queue(empty_insertascending_ABCDEFG, "empty_insertascending_ABCDEFG", ABCDEFG_keys);

    // [] -> insert(A, B, C, D, E, F, G) -> [A, B, C, D, E, F, G]
    test_seven_element_priority_queue(empty_insertdecending_ABCDEFG, "empty_insertdecending_ABCDEFG", ABCDEFG_keys);

    // [] -> insert(D, F, A, C, E, B, G) -> [A, B, C, D, E, F, G]
    test_seven_element_priority_queue(empty_insertrandom_ABCDEFG, "empty_insertrandom_ABCDEFG", ABCDEFG_keys);

    // Success
    return 1;
}

int construct_empty(priority_queue **pp_priority_queue)
{

    // Construct a priority queue
    priority_queue_construct(pp_priority_queue, 10, 0);

    // priority queue = []
    return 1;
}

int empty_insertG_G(priority_queue **pp_priority_queue)
{

    // Construct a [] priority queue
    construct_empty(pp_priority_queue);

    // insert(G)
    priority_queue_insert(*pp_priority_queue, G_key);

    // priority queue = [G]
    return 1;
}

int G_extractmax_empty(priority_queue **pp_priority_queue)
{

    // Construct a [G] priority queue
    empty_insertG_G(pp_priority_queue);

    // extract max
    priority_queue_extract_max(*pp_priority_queue, (void *) 0 );

    // priority queue = []
    return 1;
}

int G_insertD_DG(priority_queue **pp_priority_queue)
{

    // Construct a [G] priority queue
    empty_insertG_G(pp_priority_queue);

    // insert(D)
    priority_queue_insert(*pp_priority_queue, D_key);

    // priority queue = [D, G]
    return 1;
}

int DG_extractmax_G(priority_queue **pp_priority_queue)
{
    
    // Construct a [D, G] priority queue
    G_insertD_DG(pp_priority_queue);

    // extractmax()
    priority_queue_extract_max(*pp_priority_queue, (void *) 0);

    // priority queue = [G]
    return 1; 
}

int DG_increasekey1A_AD(priority_queue **pp_priority_queue)
{
    
    // Construct a [D, G] priority queue
    G_insertD_DG(pp_priority_queue);

    // increasekey(1, A)
    priority_queue_increase_key(*pp_priority_queue, 1, A_key);

    // priority queue = [G]
    return 1; 
}

int empty_insertascending_ABCDEFG ( priority_queue **pp_priority_queue )
{
        
    // Construct a [] priority queue
    construct_empty(pp_priority_queue);

    // insert ascending values
    priority_queue_insert(*pp_priority_queue, A_key);
    priority_queue_insert(*pp_priority_queue, B_key);
    priority_queue_insert(*pp_priority_queue, C_key);
    priority_queue_insert(*pp_priority_queue, D_key);
    priority_queue_insert(*pp_priority_queue, E_key);
    priority_queue_insert(*pp_priority_queue, F_key);
    priority_queue_insert(*pp_priority_queue, G_key);

    // priority queue = [A, B, C, D, E, F, G]
    return 1; 
}

int empty_insertdecending_ABCDEFG ( priority_queue **pp_priority_queue )
{
        
    // Construct a [] priority queue
    construct_empty(pp_priority_queue);

    // insert decending values
    priority_queue_insert(*pp_priority_queue, G_key);
    priority_queue_insert(*pp_priority_queue, F_key);
    priority_queue_insert(*pp_priority_queue, E_key);
    priority_queue_insert(*pp_priority_queue, D_key);
    priority_queue_insert(*pp_priority_queue, C_key);
    priority_queue_insert(*pp_priority_queue, B_key);
    priority_queue_insert(*pp_priority_queue, A_key);

    // priority queue = [A, B, C, D, E, F, G]
    return 1; 
}

int empty_insertrandom_ABCDEFG ( priority_queue **pp_priority_queue )
{
        
    // Construct a [] priority queue
    construct_empty(pp_priority_queue);

    // insert random values
    priority_queue_insert(*pp_priority_queue, D_key);
    priority_queue_insert(*pp_priority_queue, F_key);
    priority_queue_insert(*pp_priority_queue, A_key);
    priority_queue_insert(*pp_priority_queue, C_key);
    priority_queue_insert(*pp_priority_queue, E_key);
    priority_queue_insert(*pp_priority_queue, B_key);
    priority_queue_insert(*pp_priority_queue, G_key);

    // priority queue = [A, B, C, D, E, F, G]
    return 1; 
}

int test_empty_priority_queue(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), char *name)
{

    // Print the scenario name
    log_info("Scenario: %s\n", name);

    // enqueue
    print_test(name, "priority_queue_enqueue", test_enqueue(priority_queue_constructor, A_key, one));

    // dequeue
    print_test(name, "priority_queue_dequeue", test_dequeue(priority_queue_constructor, (void *)0, 1, zero));

    // isempty
    print_test(name, "priority_queue_isempty", test_isempty(priority_queue_constructor, true));

    // Print the results
    print_final_summary();

    // Success
    return 1;
}

int test_one_element_priority_queue(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), char *name, char **keys)
{

    // Print the scenario name
    log_info("Scenario: %s\n", name);

    // enqueue
    print_test(name, "priority_queue_enqueue", test_enqueue(priority_queue_constructor, B_key, one));

    // dequeue
    print_test(name, "priority_queue_dequeue1", test_dequeue(priority_queue_constructor, keys[0], 1, match));
    print_test(name, "priority_queue_dequeue2", test_dequeue(priority_queue_constructor, (void *) 0, 2, zero));

    // isempty
    print_test(name, "priority_queue_isempty", test_isempty(priority_queue_constructor, false));

    // Print the results
    print_final_summary();

    // Success
    return 1;
}

int test_two_element_priority_queue(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), char *name, char **keys)
{

    // Print the scenario name
    log_info("Scenario: %s\n", name);

    // enqueue
    print_test(name, "priority_queue_enqueue", test_enqueue(priority_queue_constructor, B_key, one));

    // dequeue
    print_test(name, "priority_queue_dequeue1", test_dequeue(priority_queue_constructor, keys[0], 1, match));
    print_test(name, "priority_queue_dequeue2", test_dequeue(priority_queue_constructor, keys[1], 2, match));
    print_test(name, "priority_queue_dequeue3", test_dequeue(priority_queue_constructor, (void *) 0, 3, zero));

    // isempty
    print_test(name, "priority_queue_isempty", test_isempty(priority_queue_constructor, false));

    // Print the results
    print_final_summary();

    // Success
    return 1;
}

int test_seven_element_priority_queue(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), char *name, char **keys)
{

    // Print the scenario name
    log_info("Scenario: %s\n", name);

    // enqueue
    print_test(name, "priority_queue_enqueue", test_enqueue(priority_queue_constructor, H_key, one));

    // dequeue
    print_test(name, "priority_queue_dequeue1", test_dequeue(priority_queue_constructor, keys[0], 1, match));
    print_test(name, "priority_queue_dequeue2", test_dequeue(priority_queue_constructor, keys[1], 2, match));
    print_test(name, "priority_queue_dequeue3", test_dequeue(priority_queue_constructor, keys[2], 3, match));
    print_test(name, "priority_queue_dequeue4", test_dequeue(priority_queue_constructor, keys[3], 4, match));
    print_test(name, "priority_queue_dequeue5", test_dequeue(priority_queue_constructor, keys[4], 5, match));
    print_test(name, "priority_queue_dequeue6", test_dequeue(priority_queue_constructor, keys[5], 6, match));
    print_test(name, "priority_queue_dequeue7", test_dequeue(priority_queue_constructor, keys[6], 7, match));
    print_test(name, "priority_queue_dequeue8", test_dequeue(priority_queue_constructor, (void *) 0, 8, zero));

    // isempty
    print_test(name, "priority_queue_isempty", test_isempty(priority_queue_constructor, false));

    // Print the results
    print_final_summary();

    // Success
    return 1;
}

int print_test(const char *scenario_name, const char *test_name, bool passed)
{

    if (passed)
        log_pass("%s %s\n", scenario_name, test_name);
    else
        log_fail("%s %s\n", scenario_name, test_name);

    // Increment the counters
    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    ephemeral_tests++;

    // Success
    return 1;
}

int print_final_summary()
{

    // Accumulate
    total_tests += ephemeral_tests,
        total_passes += ephemeral_passes,
        total_fails += ephemeral_fails;

    // Log statistics
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n", ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes / (float)ephemeral_tests * 100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n", total_tests, total_passes, total_fails, ((float)total_passes / (float)total_tests * 100.f));

    // Reset
    ephemeral_tests = 0;
    ephemeral_passes = 0;
    ephemeral_fails = 0;

    // Success
    return 1;
}

bool test_increase_key(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), void **expected_values, result_t expected)
{

    // Initialized data
    result_t result = zero;
    priority_queue *p_priority_queue = 0;
    void *result_value = 0;
    char **values = 0;
    size_t expected_value_count = 0,
           value_count = 0;

    // Build the priority queue
    priority_queue_constructor(&p_priority_queue);

    // ...

    // Free the priority queue
    priority_queue_destroy(&p_priority_queue);

    // Return result
    return (expected_value_count == value_count) ? true : false;
}

bool test_enqueue(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), void *value, result_t expected)
{

    // Initialized data
    result_t result = zero;
    priority_queue *p_priority_queue = 0;

    // Build the priority queue
    priority_queue_constructor(&p_priority_queue);

    // Enqueue a value
    result = priority_queue_enqueue(p_priority_queue, value);

    // Free the priority queue
    priority_queue_destroy(&p_priority_queue);

    // Return result
    return (result == expected);
}

bool test_dequeue(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), void *value, size_t how_many, result_t expected)
{

    // Initialized data
    result_t result = zero;
    priority_queue *p_priority_queue = 0;
    void *p_value = 0;

    // Build the priority queue
    priority_queue_constructor(&p_priority_queue);

    // Dequeue N times
    for (size_t i = 0; i < how_many; i++)
        
        // Dequeue a value
        result = priority_queue_dequeue(p_priority_queue, &p_value);

    // Check for a match
    if ( result != zero ) if ( value == p_value ) result = match;

    // Free the priority queue
    priority_queue_destroy(&p_priority_queue);

    // Return result
    return (result == expected);
}

bool test_isempty(int (*priority_queue_constructor)(priority_queue **pp_priority_queue), result_t expected)
{

    // Initialized data
    result_t result = zero;
    priority_queue *p_priority_queue = 0;

    // Build the priority queue
    priority_queue_constructor(&p_priority_queue);

    // Enqueue a value
    result = priority_queue_empty(p_priority_queue);

    // Free the priority queue
    priority_queue_destroy(&p_priority_queue);

    // Return result
    return (result == expected);
}