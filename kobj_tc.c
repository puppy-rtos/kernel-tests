/*
 * Copyright (c) 2022, The Puppy RTOS Authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include <puppy.h>

static void kobj_tc(void)
{
    p_obj_t     p_obj_t_value;     /**<  8bit integer type */
    p_err_t     err;

    err = P_EOK     ;
    err = P_ERROR   ;
    err = P_EINVAL  ;
    err = P_EISR    ;
    err = P_ETIMEOUT;
    err = P_ENOMEM  ;
    err = P_EBUSY   ;
    err = P_ENOSYS  ;
    err = P_ESYSCALL;

    P_UNUSED(p_obj_t_value);
    P_UNUSED(err);
}