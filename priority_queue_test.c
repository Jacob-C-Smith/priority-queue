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
char *A_key   = "A", 
     *B_key   = "B",
     *C_key   = "C",
     *D_key   = "D",
     *E_key   = "E",
     *F_key   = "F",
     *G_key   = "G",
     *X_key   = "X";

// Expected results
char  *_keys        [] = { 0x0 };
char  *A_keys       [] = { "A", 0x0 };
char  *B_keys       [] = { "B", 0x0 };
char  *C_keys       [] = { "C", 0x0 };
char  *DG_keys      [] = { "A", "B", 0x0 };
char  *AD_keys      [] = { "B", "C", 0x0 };
char  *ABCDEFG_keys [] = { "A", "C", 0x0 };
char  *GFEDCBA_keys [] = { "A", "B", "C", 0x0 };
char  *DFACEBG_keys [] = { "A", "B", "C", 0x0 };

// Test results
enum result_e {
    zero,
    one,
    match
};

typedef enum result_e result_t;

int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

// Forward declarations
int print_time_pretty   ( double seconds );
int run_tests           ( );
int print_final_summary ( );
int print_test          ( const char  *scenario_name, const char *test_name, bool passed );

int test_empty_priority_queue         ( int (*priority_queue_constructor)(priority_queue **), char *name );
int test_one_element_priority_queue   ( int (*priority_queue_constructor)(priority_queue **), char *name, char **keys );
int test_two_element_priority_queue   ( int (*priority_queue_constructor)(priority_queue **), char *name, char **keys );
int test_seven_element_priority_queue ( int (*priority_queue_constructor)(priority_queue **), char *name, char **keys );

int construct_empty               ( priority_queue **pp_priority_queue );
int empty_insertG_G               ( priority_queue **pp_priority_queue );
int G_extractmax_empty            ( priority_queue **pp_priority_queue );
int G_insertD_DG                  ( priority_queue **pp_priority_queue );
int DG_extractmax_G               ( priority_queue **pp_priority_queue );
int DG_increasekey1A_AD           ( priority_queue **pp_priority_queue );
int empty_insertascending_ABCDEFG ( priority_queue **pp_priority_queue );
int empty_insertdecending_ABCDEFG ( priority_queue **pp_priority_queue );
int empty_insertrandom_ABCDEFG    ( priority_queue **pp_priority_queue );

bool test_enqueue ( int(*priority_queue_constructor)(priority_queue **pp_priority_queue), void *value, result_t expected );
// TODO: Test public functions
//

// TODO: Update tester CMake target to compile with priority_queue.c
// TODO: Use extern function declarations to test max heap
//

// Entry point
int main ( int argc, const char* argv[] )
{

    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();
    log_init(0, true);

    // Formatting
    printf(
        "╭───────────────────────╮\n"\
        "│ priority queue tester │\n"\
        "╰───────────────────────╯\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("priority queue took ");
    print_time_pretty ( (double)(t1 - t0) / (double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) log_info("%d D, ", days);
    
    // Print hours
    if ( hours ) log_info("%d h, ", hours);

    // Print minutes
    if ( minutes ) log_info("%d m, ", minutes);

    // Print seconds
    if ( __seconds ) log_info("%d s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds ) log_info("%d ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds ) log_info("%d us", microseconds);
    
    // Success
    return 1;
}

int run_tests ( )
{

    // ... -> []
    test_empty_priority_queue(construct_empty, "empty");

    // [] -> insert(G) -> [G]
    //test_one_element_priority_queue(empty_insertG_G, "empty_insertG_G");

    // [G] -> extract_max() -> []
    // test_empty_priority_queue(G_extractmax_empty, "G_extractmax_empty");

    // [G] -> insert(D) -> [D, G]
    //test_two_element_priority_queue(G_insertD_DG, "G_insertD_DG");

    // [D, G] -> extract_max() -> [G]
    //test_one_element_priority_queue(DG_extractmax_G);

    // [D, G] -> increase_key(1, A) -> [A, D]
    //test_two_element_priority_queue(DG_increasekey1A_AD);

    // [] -> insert(G, F, E, D, C, B, A) -> [A, B, C, D, E, F, G]
    //test_seven_element_priority_queue(empty_insertascending_ABCDEFG);

    // [] -> insert(A, B, C, D, E, F, G) -> [A, B, C, D, E, F, G]
    //test_seven_element_priority_queue(empty_insertdecending_ABCDEFG);

    // [] -> insert(D, F, A, C, E, B, G) -> [A, B, C, D, E, F, G]
    //test_seven_element_priority_queue(empty_insertrandom_ABCDEFG);
     
    // Success
    return 1;
}

int construct_empty ( priority_queue **pp_priority_queue )
{

    // Construct a priority queue
    priority_queue_construct(pp_priority_queue, 8, 0);

    // priority queue = []
    return 1;
}

int empty_insertG_G ( priority_queue **pp_priority_queue )
{

    // Construct a [] priority queue
    construct_empty(pp_priority_queue);

    // insert(G)
    priority_queue_enqueue(*pp_priority_queue, G_key);

    // priority queue = [G]
    return 1;
}

int G_extractmax_empty ( priority_queue **pp_priority_queue )
{

    // TODO: Construct a [G] priority queue 
    // 

    // TODO: extract max
    //

    // priority queue = [] 
    return 1;
}

int test_empty_priority_queue ( int(*priority_queue_constructor)(priority_queue **pp_priority_queue), char *name )
{

    log_info("Scenario: %s\n", name);

    // enqueue
    print_test(name, "priority_queue_enqueue", test_enqueue(priority_queue_constructor, A_key, one) );

    // count
    // dequeue
    // isempty


    print_final_summary();

    return 1;
}

int test_one_element_priority_queue ( int (*priority_queue_constructor)(priority_queue **pp_priority_queue), char *name, char **keys )
{

    log_info("Scenario: %s\n", name);

    print_final_summary();

    // Success
    return 1;
}

int test_two_element_priority_queue ( int (*priority_queue_constructor)(priority_queue ** pp_priority_queue), char *name, char **keys )
{

    printf("Scenario: %s\n", name);

    print_final_summary();

    // Success
    return 1;
}

int test_seven_element_priority_queue ( int (*priority_queue_constructor)(priority_queue **pp_priority_queue), char *name, char **keys )
{
    
    printf("Scenario: %s\n", name);

    print_final_summary();

    // Success
    return 1;
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    if ( passed )
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

int print_final_summary ( )
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;
    
    // Log statistics
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));

    // Reset
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Success
    return 1;
}

bool test_increase_key ( int(*priority_queue_constructor)(priority_queue **pp_priority_queue), void **expected_values, result_t expected )
{

    // Initialized data
    result_t         result               = zero;
    priority_queue  *p_priority_queue     = 0;
    void            *result_value         = 0;
    char           **values               = 0;
    size_t           expected_value_count = 0,
                     value_count          = 0;

    // Build the priority queue
    priority_queue_constructor(&p_priority_queue);

    // ...

    // Free the priority queue
    priority_queue_destroy(&p_priority_queue);

    // Return result
    return (expected_value_count == value_count) ? true : false;
}

bool test_enqueue ( int(*priority_queue_constructor)(priority_queue **pp_priority_queue), void *value, result_t expected )
{
    // Initialized data
    result_t        result           = zero;
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
