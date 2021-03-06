/*
 * =====================================================================================
 *
 *       Filename:  notif.h
 *
 *    Description:  This file defines the Structure definition required to implement Notification Chains
 *
 *        Version:  1.0
 *        Created:  08/23/2020 08:05:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Juniper Networks (https://csepracticals.wixsite.com/csepracticals), sachinites@gmail.com
 *        Company:  Juniper Networks
 *
 *        This file is part of the NotificationChains distribution (https://github.com/sachinites) 
 *        Copynext (c) 2019 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify it under the terms of the GNU General 
 *        Public License as published by the Free Software Foundation, version 3.
 *        
 *        This program is distributed in the hope that it will be useful, but
 *        WITHOUT ANY WARRANTY; without even the implied warranty of
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *        General Public License for more details.
 *
 *        visit website : https://csepracticals.wixsite.com/csepracticals for more courses and projects
 *                                  
 * =====================================================================================
 */

#ifndef __NOTIF_H__
#define __NOTIF_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

typedef struct notif_chain_elem_ notif_chain_elem_t;
typedef struct notif_chain_ notif_chain_t;
typedef int (*notif_chain_comp_cb)(void *data1, uint32_t data_size1,
                                   void *data2, uint32_t data_size2);
typedef void (*notif_chain_app_cb)(notif_chain_elem_t *notif_chain_elem);
typedef char * (*app_key_data_print_cb)(void *data,
                        uint32_t size,
                        char *outbuffer,
                        uint32_t outbuffer_size);

#define NOTIF_NAME_SIZE 32

typedef enum notif_ch_type_{

    NOTIF_C_CALLBACKS,
    NOTIF_C_NOT_KNOWN
} notif_ch_type_t;

static inline char *
notif_chain_get_str_notif_ch_type(
    notif_ch_type_t notif_ch_type){

    switch(notif_ch_type){

        case NOTIF_C_CALLBACKS:
            return "NOTIF_C_CALLBACKS";
        case NOTIF_C_NOT_KNOWN:
            return "NOTIF_C_NOT_KNOWN";
        default:
            assert(0);
    }
}

typedef enum notif_ch_notify_opcode_{

    /* Used by Publisher to tell 
     * subscriber the nature of 
     * Update*/
    PUB_TO_SUBS_NOTIF_C_CREATE,
    PUB_TO_SUBS_NOTIF_C_UPDATE,
    PUB_TO_SUBS_NOTIF_C_DELETE,
    /* Used by Subscriber to tell
     * publisher about the type
     * of subscription*/
    SUBS_TO_PUB_NOTIF_C_SUBSCRIBE,
    SUBS_TO_PUB_NOTIF_C_UNSUBSCRIBE,
    SUBS_TO_PUB_NOTIFY_C_NOTIFY_ALL,
    SUBS_TO_PUB_NOTIFY_C_CLIENT_UNSUBSCRIBE_ALL,
    NOTIF_C_UNKNOWN
} notif_ch_notify_opcode_t;

static inline char *
notif_chain_get_str_notify_opcode(
    notif_ch_notify_opcode_t notif_opcode){

    switch(notif_opcode){

        case PUB_TO_SUBS_NOTIF_C_CREATE:
            return "PUB_TO_SUBS_NOTIF_C_CREATE";
        case PUB_TO_SUBS_NOTIF_C_UPDATE:
            return "PUB_TO_SUBS_NOTIF_C_UPDATE";
        case PUB_TO_SUBS_NOTIF_C_DELETE:
            return "PUB_TO_SUBS_NOTIF_C_DELETE";
        case SUBS_TO_PUB_NOTIF_C_SUBSCRIBE:
            return "SUBS_TO_PUB_NOTIF_C_SUBSCRIBE";
        case SUBS_TO_PUB_NOTIF_C_UNSUBSCRIBE:
            return "SUBS_TO_PUB_NOTIF_C_UNSUBSCRIBE";
        case SUBS_TO_PUB_NOTIFY_C_NOTIFY_ALL:
            return "SUBS_TO_PUB_NOTIFY_C_NOTIFY_ALL";
        case SUBS_TO_PUB_NOTIFY_C_CLIENT_UNSUBSCRIBE_ALL:
            return "SUBS_TO_PUB_NOTIFY_C_CLIENT_UNSUBSCRIBE_ALL";
        case NOTIF_C_UNKNOWN:
            return "NOTIF_C_UNKNOWN";
        default:
            assert(0);
    }
}

typedef struct notif_chain_comm_channel_{
    
    notif_ch_type_t notif_ch_type;
    union {
        /*Via Callbacks*/
        notif_chain_app_cb app_cb;
    }u;
} notif_chain_comm_channel_t;

#define NOTIF_CHAIN_ELEM_APP_CB(notif_chain_comm_channel_ptr)           \
    (notif_chain_comm_channel_ptr->u.app_cb)

struct notif_chain_elem_{

    notif_chain_elem_t *prev;
    notif_chain_elem_t *next;
    
    uint32_t client_id;
    notif_ch_notify_opcode_t notif_code;
    
    struct {
        /* Key data to decide which 
         * subscriber to notify
         * */
        void *app_key_data;
        uint32_t app_key_data_size;

        /* Once the subscriber is chosen, 
         * NCM must send notif to subscriber 
         * with this data*/ 
        bool is_alloc_app_data_to_notify;
        void *app_data_to_notify;
        uint32_t app_data_to_notify_size;
    } data;

    notif_chain_comm_channel_t 
        notif_chain_comm_channel;
};

#define NOTIF_CHAIN_ELEM_TYPE(notif_chain_elem_ptr)                     \
    (notif_chain_elem_ptr->notif_chain_comm_channel.notif_ch_type)

struct notif_chain_{

    char name[NOTIF_NAME_SIZE];
    /* Comparison fn to compare app_key_data 
     * present in notif_chain_elem_t objects
     * present in chain. This can be NULL if
     * comparison is not required
     * */
    notif_chain_comp_cb comp_cb;
    app_key_data_print_cb print_cb;
    /* Head of the linked list containing
     * notif_chain_elem_t
     * */
    notif_chain_elem_t *head;
    struct notif_chain_ *prev;
    struct notif_chain_ *next;

};

typedef struct notif_chain_db_ {

    notif_chain_t *notif_chain_head;
} notif_chain_db_t;

void
notif_chain_free_notif_chain_elem(
                notif_chain_elem_t *notif_chain_elem);
void
notif_chain_free_notif_chain_elem_internals(
                notif_chain_elem_t *notif_chain_elem);

void
notif_chain_init(notif_chain_t *notif_chain,
                 char *chain_name,
                 notif_chain_comp_cb comp_cb,
                 app_key_data_print_cb print_cb);

void
notif_chain_delete(notif_chain_t *notif_chain);

bool
notif_chain_register_chain_element(notif_chain_t *notif_chain,
                notif_chain_elem_t *notif_chain_elem);

bool
notif_chain_deregister_chain_element(notif_chain_t *notif_chain,
                notif_chain_elem_t *notif_chain_elem);

void
notif_chain_invoke(notif_chain_t *notif_chain,
                notif_chain_elem_t *notif_chain_elem);

void
notif_chain_dump(notif_chain_t *notif_chain);

bool
notif_chain_communication_channel_match(
                notif_chain_comm_channel_t *channel1,
                notif_chain_comm_channel_t *channel2);

void
notif_chain_release_communication_channel_resources(
                notif_chain_comm_channel_t *channel);

void
notif_chain_elem_remove(notif_chain_t *notif_chain,
                notif_chain_elem_t *notif_chain_elem);

bool
notif_chain_subscribe(char *notif_name, 
                      notif_chain_elem_t *notif_chain_elem);

bool
notif_chain_unsubscribe(char *notif_name, 
                      notif_chain_elem_t *notif_chain_elem);

#define ITERTAE_NOTIF_CHAIN_BEGIN(notif_chain_ptr, notif_chain_elem_ptr)  \
{                                                                         \
    notif_chain_elem_t *_next_notif_chain_elem;                           \
    for(notif_chain_elem_ptr = notif_chain_ptr->head;                     \
        notif_chain_elem_ptr;                                             \
        notif_chain_elem_ptr = _next_notif_chain_elem) {                  \
        _next_notif_chain_elem = notif_chain_elem_ptr->next;

#define ITERTAE_NOTIF_CHAIN_END(notif_chain_ptr, notif_chain_elem_ptr)  }}

bool
notif_chain_subscribe_by_callback(
        char *notif_chain_name,
        void *key,
        uint32_t key_size,
        uint32_t client_id,
        notif_chain_app_cb cb);


#define NOTIF_C_CLIENT_ID_VALUE_LEN         (FIELD_SIZE(notif_chain_elem_t, client_id))
#define NOTIF_C_COMM_CHANNEL_TYPE_VALUE_LEN \
	(FIELD_SIZE(notif_chain_comm_channel_t, notif_ch_type)) 
#define NOTIF_C_NOTIF_CODE_VALUE_LEN        (FIELD_SIZE(notif_chain_elem_t, notif_code)) 

#endif /* __NOTIF_H__ */
