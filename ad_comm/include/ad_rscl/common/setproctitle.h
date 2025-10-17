/*
 * Copyright (C) 2002-2021 Igor Sysoev
 * Copyright (C) 2011-2022 Nginx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Reference:
 * https://github.com/nginx/nginx/blob/master/src/os/unix/ngx_setproctitle.c
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ad_base/common/project_env.h"

#define NGX_OK 0
#define NGX_ERROR -1
#define NGX_SETPROCTITLE_PAD '\0'

typedef int ngx_int_t;
typedef unsigned int ngx_uint_t;

extern char **environ;

static char *ngx_os_argv_last;

int ngx_argc;
char **ngx_os_argv;

u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n) {
    if (n == 0) {
        return dst;
    }

    while (--n) {
        *dst = *src;

        if (*dst == '\0') {
            return dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    return dst;
}

ngx_int_t ngx_init_setproctitle() {
    u_char *p;
    size_t size;
    ngx_uint_t i;

    size = 0;

    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }

    p = (u_char *)malloc(size);  // NOLINT
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_os_argv_last = ngx_os_argv[0];

    for (i = 0; ngx_os_argv[i]; i++) {
        if (ngx_os_argv_last == ngx_os_argv[i]) {
            ngx_os_argv_last = ngx_os_argv[i] + strlen(ngx_os_argv[i]) + 1;
        }
    }

    for (i = 0; environ[i]; i++) {
        if (ngx_os_argv_last == environ[i]) {
            size = strlen(environ[i]) + 1;
            ngx_os_argv_last = environ[i] + size;

            ngx_cpystrn(p, (u_char *)environ[i], size);  // NOLINT
            environ[i] = (char *)p;                      // NOLINT
            p += size;
        }
    }

    ngx_os_argv_last--;

    return NGX_OK;
}

void ngx_setproctitle(char *title) {
    u_char *p;

    ngx_os_argv[1] = NULL;

    p = ngx_cpystrn((u_char *)ngx_os_argv[0],               // NOLINT
                    senseAD::base::common::kProcessPrefix,  // NOLINT
                    ngx_os_argv_last - ngx_os_argv[0]);

    p = ngx_cpystrn(p, (u_char *)title,             // NOLINT
                    ngx_os_argv_last - (char *)p);  // NOLINT

    if (ngx_os_argv_last - (char *)p) {  // NOLINT
        memset(p, NGX_SETPROCTITLE_PAD,
               ngx_os_argv_last - (char *)p);  // NOLINT
    }
}

static ngx_int_t ngx_save_argv(int argc, char *const *argv) {  // NOLINT
    ngx_os_argv = (char **)argv;                               // NOLINT
    ngx_argc = argc;

    return NGX_OK;
}
