/// @file scpi.h
///
/// Functions that handle SCPI parsing state-machine, invoking events,
/// invoke store and query of configuration and current state.
///
/// Author: Nathan Poppleton
///
/// Copyright: University of Utah, College of Engineering
///

#ifndef INC_SCPI_H_
#define INC_SCPI_H_

#include "stddef.h"
#include "stdint.h"

#ifdef    __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE              1
#endif


#ifndef FALSE
#define FALSE             0
#endif

#define SCPI_RX_MIN_LEN   2             //all SCPI segments are at least this long
#define SCPI_RX_BFR_SZ  256
#define SCPI_TX_BFR_SZ  256

typedef enum scpi_menu_string_e
{
    k_scpi_str_unknown = 0,
    k_scpi_str_opc,
    k_scpi_str_idn,
    k_scpi_str_rst,
    k_scpi_str_input,
    k_scpi_str_position,
    k_scpi_str_a0,
    k_scpi_str_a1,
    k_scpi_str_a2,
    k_scpi_str_a3,
    k_scpi_str_angle,
    k_scpi_str_immediate,
    k_scpi_str_initiate,
    k_scpi_str_direction,
    k_scpi_str_limit,
    k_scpi_str_low,
    k_scpi_str_high,
    k_scpi_str_state,
    k_scpi_str_sense
}   scpi_menu_string_t;

const char *                            scpi_str_short(scpi_menu_string_t item);
const char *                            scpi_str_long(scpi_menu_string_t item);
size_t                                  scpi_str_len_short(scpi_menu_string_t item);
size_t                                  scpi_str_len_long(scpi_menu_string_t item);

// returns 1 on TRUE
// returns 0 on FALSE
int                                     scpi_is_menu_match(const char * menu, size_t len, scpi_menu_string_t item);

// ***********************************************
/// This is the top level root menu for the SCPI parser
///
/// Each value defines the starting value for the SCPI sub-menu enumerations it encompasses
typedef enum scpi_menu_root_e
{
    k_scpi_root_none                    = 0,
    k_scpi_root_q_idn                   = 0x2000,
    k_scpi_root_rst                     = 0x4000,
    k_scpi_root_opc                     = 0x6000,
    k_scpi_root_q_opc                   = 0x8000,
    k_scpi_root_input                   = 0xA000,
    k_scpi_root_initiate                = 0xC000,
    k_scpi_root_sense                   = 0xE000
}   scpi_menu_root_t;

// ***********************************************
/// This is the input sub-menu and some simple children
///
///
typedef enum scpi_input_e
{
    k_scpi_input_none                    = 0,
    k_scpi_input                         = k_scpi_root_input,           //0xA000
    k_scpi_input_position                = 0x100 + k_scpi_root_input,
    k_scpi_input_position_a0             = 0x200 + k_scpi_root_input,
    k_scpi_input_position_a0_angle       = 0x220 + k_scpi_root_input,
    k_scpi_input_position_a1             = 0x400 + k_scpi_root_input,
    k_scpi_input_position_a1_angle       = 0x420 + k_scpi_root_input,
    k_scpi_input_position_a2             = 0x600 + k_scpi_root_input,
    k_scpi_input_position_a2_angle       = 0x620 + k_scpi_root_input,
    k_scpi_input_position_a3             = 0x800 + k_scpi_root_input,
    k_scpi_input_position_a3_angle       = 0x820 + k_scpi_root_input
} scpi_menu_input_t;


// ***********************************************
/// This is the INPut:POSition:__axis__::angle: sub-menu(s)
///
///
typedef enum scpi_input_position_axis_angle_e
{
    k_scpi_input_position_axis_none     = 0,

    k_scpi_input_position_a0_axis        =        k_scpi_input_position_a0_angle,
    k_scpi_input_position_a0_immediate   = 0x1  + k_scpi_input_position_a0_axis,
    k_scpi_input_position_a0_limit       = 0x20 + k_scpi_input_position_a0_axis,
    k_scpi_input_position_a0_limit_low   = 0x1  + k_scpi_input_position_a0_limit,
    k_scpi_input_position_a0_limit_high  = 0x2  + k_scpi_input_position_a0_limit,
    k_scpi_input_position_a0_limit_state = 0x3  + k_scpi_input_position_a0_limit,
    k_scpi_input_position_a0_dir         = 0x40 + k_scpi_input_position_a0_axis,

    k_scpi_input_position_a1_axis        =        k_scpi_input_position_a1_angle,
    k_scpi_input_position_a1_immediate   = 0x1  + k_scpi_input_position_a1_axis,
    k_scpi_input_position_a1_limit       = 0x20 + k_scpi_input_position_a1_axis,
    k_scpi_input_position_a1_limit_low   = 0x1  + k_scpi_input_position_a1_limit,
    k_scpi_input_position_a1_limit_high  = 0x2  + k_scpi_input_position_a1_limit,
    k_scpi_input_position_a1_limit_state = 0x3  + k_scpi_input_position_a1_limit,
    k_scpi_input_position_a1_dir         = 0x40 + k_scpi_input_position_a1_axis,

    k_scpi_input_position_a2_axis        =        k_scpi_input_position_a2_angle,
    k_scpi_input_position_a2_immediate   = 0x1  + k_scpi_input_position_a2_axis,
    k_scpi_input_position_a2_limit       = 0x20 + k_scpi_input_position_a2_axis,
    k_scpi_input_position_a2_limit_low   = 0x1  + k_scpi_input_position_a2_limit,
    k_scpi_input_position_a2_limit_high  = 0x2  + k_scpi_input_position_a2_limit,
    k_scpi_input_position_a2_limit_state = 0x3  + k_scpi_input_position_a2_limit,
    k_scpi_input_position_a2_dir         = 0x40  + k_scpi_input_position_a2_axis,

    k_scpi_input_position_a3_axis        =        k_scpi_input_position_a3_angle,
    k_scpi_input_position_a3_immediate   = 0x1  + k_scpi_input_position_a3_axis,
    k_scpi_input_position_a3_limit       = 0x20 + k_scpi_input_position_a3_axis,
    k_scpi_input_position_a3_limit_low   = 0x1  + k_scpi_input_position_a3_limit,
    k_scpi_input_position_a3_limit_high  = 0x2  + k_scpi_input_position_a3_limit,
    k_scpi_input_position_a3_limit_state = 0x3  + k_scpi_input_position_a3_limit,
    k_scpi_input_position_a3_dir         = 0x40  + k_scpi_input_position_a3_axis
} scpi_menu_input_position_axis_angle_t;

typedef enum scpi_initiate_e
{
    k_scpi_initiate_none                = 0,
    k_scpi_initiate                     = 0xC00,
    k_scpi_initiate_immediate           = 0xC01
} scpi_menu_initiate_t;

int                                     scpi_find_level(char * buffer, size_t len, int level, char ** found, size_t * found_len);

// *********************************************************************
/// Raise an event after a scpi command has been parsed successfully / error if not
///
/// @param evt[in]      - This is the SCPI enumeration value cast to uint32_t
///
/// @returns            -   0 for successfully processed an event
///                     - < 0 for errors
///
int                                     scpi_query_event_handler(uint32_t evt);
int                                     scpi_write_event_handler(uint32_t evt);
int                                     scpi_error_event_handler();
int                                     scpi_error_partial_event_handler();

// ***********************************************
/// Handles a scpi menu item parsed from input string
///
/// @param str*[in]     - pointer to a string
/// @param str_len[in]  - length of the string
///
/// @returns            -   0 for successfully processed
///                     - < 0 for errors
///
int                                     scpi_handle_menu(const char * str, size_t str_len);


// ***********************************************
/// Handles a top-level SCPI string being input from TCP
///
/// @param p_buf*[in]           - pointer to a Rx buffer
/// @param len[in]              - length of the Rx buffer
/// @param p_reply**[i/o]       - pointer to a string with a reply message
/// @param p_reply_len*[i/o]    - pointer used to return the length of the string
/// @param evt*[i/o]            - pointer used to return the event_id (for a successful parsed command)
///
/// @returns            -  0 proceed to next sub-menu
///                     -  2 indicates finished processing, with a command type
///                     -  1 indicates finished processing, with a query type
///                     - < 0 for errors
///
int                                     scpi_input(const uint8_t * p_buf, size_t len, uint8_t ** p_reply, size_t * p_reply_len, uint32_t * evt);


int                                     scpi_menu_root_sm(scpi_menu_root_t *state, const char * str, size_t str_len );

int                                     scpi_menu_input_sm(scpi_menu_input_t *state, const char * str, size_t str_len );

int                                     scpi_menu_input_pos_sm(scpi_menu_input_position_axis_angle_t *state, const char * str, size_t str_len );

int                                     scpi_menu_initiate_sm(scpi_menu_initiate_t *state, const char * str, size_t str_len );

#ifdef  __cplusplus
}
#endif

#endif /* INC_SCPI_H_ */
