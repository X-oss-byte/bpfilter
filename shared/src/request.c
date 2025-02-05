/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 Meta Platforms, Inc. and affiliates.
 */

#include "shared/request.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared/helper.h"

int bf_request_new(struct bf_request **request, const void *data,
                   size_t data_len)
{
    _cleanup_bf_request_ struct bf_request *_request = NULL;

    assert(request);
    assert(data);

    _request = calloc(1, sizeof(*_request) + data_len);
    if (!_request)
        return -ENOMEM;

    memcpy(_request->data, data, data_len);
    _request->data_len = data_len;

    *request = TAKE_PTR(_request);

    return 0;
}

int bf_request_copy(struct bf_request **dest, const struct bf_request *src)
{
    _cleanup_bf_request_ struct bf_request *_request = NULL;

    assert(dest);
    assert(src);

    _request = bf_memdup(src, bf_request_size(src));
    if (!_request)
        return -ENOMEM;

    *dest = TAKE_PTR(_request);

    return 0;
}

void bf_request_free(struct bf_request **request)
{
    free(*request);
    *request = NULL;
}
