#ifndef VFIO_DEBUG_H
#define VFIO_DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

static FILE *vfio_debug_init(void)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static FILE *dbg_fp = NULL;

    pthread_mutex_lock(&mutex);
    if( dbg_fp == NULL ){
        char *ptr = getenv("DEBUG_FP");
        if(ptr) {
            sscanf(ptr, "%p", (void**)&dbg_fp);
        } else {
            char fname[256];
            sprintf(fname, "/tmp/qemu_vfio_log.%d", getpid());
            dbg_fp = fopen(fname, "w");
            assert(dbg_fp);
            char fp_s[64];
            sprintf(fp_s, "%p", dbg_fp);
            setenv("DEBUG_FP", fp_s, 1);
        }
    }
    pthread_mutex_unlock(&mutex);
    return dbg_fp;
}

static int vfio_debug_print(const char *fmt, ...)
{
    char fmt_new[1024];
    FILE *fp = vfio_debug_init();
    va_list args;
    va_start (args, fmt);
    sprintf(fmt_new, "%s\n", fmt);
    vfprintf (fp, fmt_new, args);
    va_end (args);
    return 0;
}

#endif
