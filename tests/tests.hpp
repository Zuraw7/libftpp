#ifndef TESTS_HPP
#define TESTS_HPP

// Data structures

void data_buffer_tests();
void pool_tests();

// Design patterns

void memento_tests();
void observable_value_tests();
void observer_tests();
void singleton_tests();
void state_machine_tests();

// IOStream
void thread_safe_iostream_tests();

// Mathematics
void ivector2_tests();
void ivector3_tests();
void perlin_noise_2D_tests();
void random_2D_coordinate_generator_tests();

// Network
void message_tests();
void client_tests();
void server_tests();

// Threading
void thread_tests();
void persistent_worker_tests();
void thread_safe_queue_tests();
void worker_pool_tests();

#endif
