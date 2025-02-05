/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 Meta Platforms, Inc. and affiliates.
 */

#pragma once

#include <assert.h>
#include <bpfilter/shared/front.h>
#include <sys/types.h>

#define _cleanup_bf_request_ __attribute__((cleanup(bf_request_free)))

/**
 * @enum bf_request_cmd
 *
 * Defines a request type, so bpfilter can understand the client-specific
 * data contained in the request, and call the proper handler.
 *
 * @var bf_request_cmd::BF_REQ_CUSTOM
 *  Custom request: only the front this request is targeted to is able to
 *  understand what is the actual command. Allows for fronts to implement
 *  new commands.
 */
enum bf_request_cmd
{
    BF_REQ_SET_RULES,
    BF_REQ_GET_RULES,
    BF_REQ_SET_COUNTERS,
    BF_REQ_GET_COUNTERS,
    BF_REQ_CUSTOM,
    _BF_REQ_CMD_MAX,
};

/**
 * @struct bf_request
 *
 * Generic request format sent by the client to the daemon.
 *
 * @var bf_request::front
 *  Front this request is targeted to.
 * @var bf_request::cmd
 *  Command.
 * @var bf_request::ipt_cmd
 *  Custom command for the IPT front.
 * @var bf_request::data_len
 *  Length of the client-specific data.
 * @var bf_request::data
 *  Client-specific data.
 */
struct bf_request
{
    enum bf_front front;
    enum bf_request_cmd cmd;

    union
    {
        struct
        {
            int ipt_cmd;
        };
    };

    size_t data_len;
    char data[];
};

/**
 * @brief Allocate and initialise a new request.
 *
 * @param request Pointer to the request to allocate. Must be non-NULL.
 * @param data Client-specific data.
 * @param data_len Length of the client-specific data.
 * @return 0 on success or negative errno code on failure.
 */
int bf_request_new(struct bf_request **request, const void *data,
                   size_t data_len);

/**
 * @brief Free a request.
 *
 * If @p request points to a NULL pointer, this function does nothing. Once the
 * function returns, @p request points to a NULL pointer.
 *
 * @param request Request to free. Can't be NULL.
 */
void bf_request_free(struct bf_request **request);

/**
 * @brief Copy a request.
 *
 * @param dest The destination request. It will be allocated during the call.
 *  Can't be NULL.
 * @param src The source request, to copy. Can't be NULL.
 * @return 0 on success, negative error code on failure.
 */
int bf_request_copy(struct bf_request **dest, const struct bf_request *src);

/**
 * @brief Get the total size of the request: request structure and data.
 *
 * @param request Request to get the size of. Can't be NULL.
 * @return Total size of the request.
 */
static inline size_t bf_request_size(const struct bf_request *request)
{
    assert(request);

    return sizeof(struct bf_request) + request->data_len;
}
