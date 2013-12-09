#pragma once

#include <cstdlib>

void funk_memory_init();
void* funk_malloc(std::size_t size, const char* file, std::size_t line);
void funk_free(void* p, const char* file, std::size_t line);

#define malloc(s) funk_malloc(s, __FILE__, __LINE__)
#define free(s) funk_free(s, __FILE__, __LINE__)
