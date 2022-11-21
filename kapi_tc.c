/*
 * Copyright (c) 2022, The Puppy RTOS Authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include <puppy.h>

static void kapi_err_tc(void)
{
    int err;
    const char *errstr;
    
    p_set_errno(P_EBUSY);
    err = p_get_errno();
    errstr = p_errno_str(err);
    if (err == P_EBUSY)
    {
        P_TC_LOG("err is %s", errstr);
        P_TC_PASS();
    }
    else
    {
        P_TC_LOG("err is %s", errstr);
        P_TC_FAIL();
    }
}

static p_obj_t t1;
static void k_tc_entry(void *parameter)
{
    if (t1 != p_thread_self())
    {
        P_TC_FAIL();
        return;
    }
    p_thread_sleep(10);
    p_thread_yield();
    P_TC_PASS();
}

static void kapi_thread_tc(void)
{
    p_thread_attr_t attr;
    p_thread_getattr(p_thread_self(), &attr);
    t1 = p_thread_create("t1", k_tc_entry, NULL, 512, attr.priority + 1);
    if (!t1)
    {
        P_TC_LOG("err is %s", p_errno_str(p_get_errno()));
        P_TC_FAIL();
    }

    p_thread_start(t1);
    
    p_thread_sleep(5);
    P_TC_PASS();
}


/* convenience macro - return either 64-bit or 32-bit value */
#define ATOMIC_WORD(val_if_64, val_if_32)                                           \
    ((p_atomic_t)((sizeof(void *) == sizeof(uint64_t)) ? (val_if_64) : (val_if_32)))

void test_atomic_api(void)
{
    p_atomic_t base;

    /* p_atomic_t */
    if(sizeof(p_atomic_t) != ATOMIC_WORD(sizeof(uint64_t), sizeof(uint32_t)))
    {
        P_TC_FAIL();
    }

    /* arch_atomic_add */
    base = 0;
    arch_atomic_add(&base, 10);
    if(base != 10)
    {
        P_TC_FAIL();
    }
    /* arch_atomic_add negative */
    base = 2;
    arch_atomic_add(&base, -4);
    if(base != -2)
    {
        P_TC_FAIL();
    }

    /* arch_atomic_sub */
    base = 11;
    arch_atomic_sub(&base, 10);
    if(base != 1)
    {
        P_TC_FAIL();
    }
    /* arch_atomic_sub negative */
    base = 2;
    arch_atomic_sub(&base, -5);
    if(base != 7)
    {
        P_TC_FAIL();
    }

    /* arch_atomic_or */
    base = 0xFF00;
    arch_atomic_or(&base, 0x0F0F);
    if(base != 0xFF0F)
    {
        P_TC_FAIL();
    }

    /* arch_atomic_xor */
    base = 0xFF00;
    arch_atomic_xor(&base, 0x0F0F);
    if(base != 0xF00F)
    {
        P_TC_FAIL();
    }
    /* arch_atomic_and */
    base = 0xFF00;
    arch_atomic_and(&base, 0x0F0F);
    if(base != 0x0F00)
    {
        P_TC_FAIL();
    }

    /* arch_atomic_nand */
    base = 0xFF00;
    arch_atomic_nand(&base, 0x0F0F);
    if(base != ATOMIC_WORD(0xFFFFFFFFFFFFF0FF, 0xFFFFF0FF))
    {
        P_TC_FAIL();
    }

    /* arch_atomic_cas */
    base = 10;
    if(arch_atomic_cas(&base, 10, 11) != 10)
    {
        P_TC_FAIL();
    }
    if(base != 11)
    {
        P_TC_FAIL();
    }
    P_TC_PASS();
}
