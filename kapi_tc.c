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
