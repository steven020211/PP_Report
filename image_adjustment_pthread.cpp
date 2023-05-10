#include "image_adjustment.h"
#include <algorithm>
#include <pthread.h>
#include <chrono>

using namespace std;

#define NUM_THREADS 4

typedef struct
{
public:
    uint8_t* data;

    int width;
    int height;

    int thread_index;

    int start_row;
    int end_row;

    pthread_t thread;

    image_attributes attributes;
}  thread_args;


void* threadProc(void* param)
{
    thread_args *args = (thread_args*)param;

    uint8_t* data = args->data;
    for(int y = args->start_row; y < args->end_row; y++)
    {
        for(int x = 0; x < args->width; x++) {
            uint8_t* pixel = data + (y * args->width + x) * 3;
            adjust_pixel(pixel, args->attributes);
        }
    }

    return NULL;
}

void startThreads(unsigned char* data, int width, int height, image_attributes& attributes)
{
    thread_args all_args[NUM_THREADS];
    int num_rows = (height + NUM_THREADS - 1) / NUM_THREADS;

    for(int i = 0;i < NUM_THREADS; i++)
    {
        thread_args* args = &all_args[i];
        args->data = data;
        args->width = width;
        args->height = height;
        args->attributes = attributes;

        args->thread_index = i;
        args->start_row = i * num_rows;
        args->end_row = min((i+1)* num_rows, height);

        pthread_create(&args->thread, NULL, &threadProc, args);
    }

    for(int i = 0;i < NUM_THREADS; i++)
        pthread_join(all_args[i].thread, NULL);
}


void adjust_attributes(uint8_t* data, int width, int height, image_attributes& attributes)
{
    startThreads(data, width, height, attributes);
}