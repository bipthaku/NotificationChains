/*
 * =====================================================================================
 *
 *       Filename:  enums.h
 *
 *    Description:  This file contains the decalaration of all enumerations used in this file
 *
 *        Version:  1.0
 *        Created:  Wednesday 18 September 2019 02:38:12  IST
 *       Revision:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Networking Developer (AS), sachinites@gmail.com
 *        Company:  Brocade Communications(Jul 2012- Mar 2016), Current : Juniper Networks(Apr 2017 - Present)
 *        
 *        This file is part of the NetworkGraph distribution (https://github.com/sachinites).
 *        Copyright (c) 2017 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify
 *        it under the terms of the GNU General Public License as published by  
 *        the Free Software Foundation, version 3.
 *
 *        This program is distributed in the hope that it will be useful, but 
 *        WITHOUT ANY WARRANTY; without even the implied warranty of 
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *        General Public License for more details.
 *
 *        You should have received a copy of the GNU General Public License 
 *        along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * =====================================================================================
 */
/* Visit my Website for more wonderful assignments and projects :
 * https://csepracticals.wixsite.com/csepracticals
 * if above URL dont work, then try visit : https://csepracticals.com*/

#ifndef __UTILS__
#define __UTILS__

#include <stdint.h>

#define TLV_OVERHEAD_SIZE  2 /* 1 Bytes for TYPE, 1 Byte for value len*/

#define FIELD_SIZE(struct_name, field_name) \
    (sizeof(((struct_name *)0)->field_name))

#define MAX(a, b) (a > b ? a : b)

#define offsetof(structure, field)	\
	((size_t)&(((structure *)0)->field))

/*Macro to Type Length Value reply
 * uint8_t * - start_ptr, IN
 * uint8_t - type, OUT
 * uint8_t - length, OUT
 * uint8_t * - tlv_ptr, OUT
 * unsigned int - total_size(excluding first 8 bytes), IN
 * */
#define ITERATE_TLV_BEGIN(start_ptr, type, length, tlv_ptr, tlv_size)           \
{                                                                               \
    unsigned int _len = 0; uint8_t _tlv_value_size = 0;                         \
    type = 0; length = 0; tlv_ptr = NULL;                                       \
    for(tlv_ptr = (uint8_t *)start_ptr +                                        \
             TLV_OVERHEAD_SIZE; _len < tlv_size;                                \
            _len += _tlv_value_size + TLV_OVERHEAD_SIZE,                        \
             tlv_ptr = (tlv_ptr + TLV_OVERHEAD_SIZE + length)){                 \
        type = *(uint8_t *)(tlv_ptr - TLV_OVERHEAD_SIZE);                       \
        _tlv_value_size = (uint8_t)(*(tlv_ptr - sizeof(uint8_t)));              \
        length = _tlv_value_size;

#define ITERATE_TLV_END(start_ptr, type, length, tlv_ptr, tlv_size)             \
    }}

char *
tlv_buffer_get_particular_tlv(char *tlv_buff, /*Input TLV Buffer*/
                              uint32_t tlv_buff_size, /*Input TLV Buffer Total Size*/
                              uint8_t tlv_no, /*Input TLV Number*/
                              uint8_t *tlv_data_len); /*Output TLV Data len*/

char *
tlv_buffer_insert_tlv(char *tlv_buff, uint8_t tlv_no, 
                     uint8_t data_len, char *data);

char *
tcp_ip_covert_ip_n_to_p(uint32_t ip_addr, 
                        char *output_buffer);

uint32_t
tcp_ip_covert_ip_p_to_n(char *ip_addr);
#endif /* __UTILS__ */
