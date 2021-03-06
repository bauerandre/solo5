/* 
 * Copyright (c) 2015-2017 Contributors as noted in the AUTHORS file
 *
 * This file is part of Solo5, a unikernel base layer.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "kernel.h"

/* ukvm net interface */
int solo5_net_write_sync(uint8_t *data, int n)
{
    volatile struct ukvm_netwrite wr;

    wr.data = data;
    wr.len = n;
    wr.ret = 0;

    ukvm_do_hypercall(UKVM_HYPERCALL_NETWRITE, &wr);

    return wr.ret;
}

int solo5_net_read_sync(uint8_t *data, int *n)
{
    volatile struct ukvm_netread rd;

    rd.data = data;
    rd.len = *n;
    rd.ret = 0;

    ukvm_do_hypercall(UKVM_HYPERCALL_NETREAD, &rd);

    *n = rd.len;
    return rd.ret;
}

static char mac_str[18];
char *solo5_net_mac_str(void)
{
    volatile struct ukvm_netinfo info;

    ukvm_do_hypercall(UKVM_HYPERCALL_NETINFO, &info);

    memcpy(mac_str, (void *)&info, 18);
    return mac_str;
}

/* ukvm block interface */
int solo5_blk_write_sync(uint64_t sec, uint8_t *data, int n)
{
    volatile struct ukvm_blkwrite wr;

    wr.sector = sec;
    wr.data = data;
    wr.len = n;
    wr.ret = 0;

    ukvm_do_hypercall(UKVM_HYPERCALL_BLKWRITE, &wr);

    return wr.ret;
}

int solo5_blk_read_sync(uint64_t sec, uint8_t *data, int *n)
{
    volatile struct ukvm_blkread rd;

    rd.sector = sec;
    rd.data = data;
    rd.len = *n;
    rd.ret = 0;

    ukvm_do_hypercall(UKVM_HYPERCALL_BLKREAD, &rd);

    *n = rd.len;
    return rd.ret;
}

int solo5_blk_sector_size(void)
{
    volatile struct ukvm_blkinfo info;

    ukvm_do_hypercall(UKVM_HYPERCALL_BLKINFO, &info);

    return info.sector_size;
}

uint64_t solo5_blk_sectors(void)
{
    volatile struct ukvm_blkinfo info;

    ukvm_do_hypercall(UKVM_HYPERCALL_BLKINFO, &info);

    return info.num_sectors;
}

int solo5_blk_rw(void)
{
    volatile struct ukvm_blkinfo info;

    ukvm_do_hypercall(UKVM_HYPERCALL_BLKINFO, &info);

    return info.rw;
}
