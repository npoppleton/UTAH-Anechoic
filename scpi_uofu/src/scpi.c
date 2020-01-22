/// @file scpi.c
///
/// Functions that handle SCPI parsing state-machine, invoking events,
/// invoke store and query of configuration and current state.
///
/// Author: Nathan Poppleton
///
/// Copyright: University of Utah, College of Engineering
///

#include "scpi.h"

#include <stdio.h>
#include <ctype.h>
#include "string.h"
#include "stdint.h"

//#define SCPI_DBG


const char * STR_INP        = "inp";           //input shorthand
const char * STR_INPUT      = "input";
const char * STR_POS        = "pos";           //position shorthand
const char * STR_POSITION   = "position";
const char * STR_A0         = "a0";
const char * STR_A1         = "a1";
const char * STR_A2         = "a2";
const char * STR_A3         = "a3";
const char * STR_ANGL       = "angl";          //angle shorthand
const char * STR_ANGLE      = "angle";
const char * STR_INIT       = "init";          //initiate shorthand
const char * STR_INITIATE   = "initiate";
const char * STR_IMM        = "imm";           //immediate shorthand
const char * STR_IMMEDIATE  = "immediate";
const char * STR_LIM        = "lim";           //limit shorthand
const char * STR_LIMIT      = "limit";
const char * STR_DIR        = "dir";           //direction shorthand
const char * STR_DIRECTION  = "direction";
const char * STR_LOW        = "low";
const char * STR_HIGH       = "high";
const char * STR_STAT       = "stat";          //state shorthand
const char * STR_STATE      = "state";
const char * STR_SENS       = "sens";          //sense shorthand
const char * STR_SENSE      = "sense";
const char * STR_OPC        = "*opc";
const char * STR_IDN        = "*idn";
const char * STR_RST        = "*rst";

//Replies
const char * STR_REPLY_OK1              = "OK_QUERY";
const char * STR_REPLY_OK2              = "OK_CMD";
const char * STR_REPLY_ERR              = "ERROR";
const char * STR_REPLY_ERR_PARTIAL      = "ERROR_PARTIAL";
const char * STR_REPLY_IDN              = "Antenna Rotator Controller v0.1; University of Utah; Nov. 2019";

static uint8_t s_reply[SCPI_TX_BFR_SZ];


inline
static int menu_len_match(size_t len, scpi_menu_string_t str)
{
    return (len == scpi_str_len_short(str) || len == scpi_str_len_long(str)) ? TRUE : FALSE;
}

// *********************************************************************
//
//
const char *  scpi_str_short(scpi_menu_string_t item)
{
    switch (item)
    {
    case k_scpi_str_opc:
        return STR_OPC;
    case k_scpi_str_idn:
        return STR_IDN;
    case k_scpi_str_rst:
        return STR_RST;
    case k_scpi_str_input:
        return STR_INP;
    case k_scpi_str_position:
        return STR_POS;
    case k_scpi_str_a0:
        return STR_A0;
    case k_scpi_str_a1:
        return STR_A1;
    case k_scpi_str_a2:
        return STR_A2;
    case k_scpi_str_a3:
        return STR_A3;
    case k_scpi_str_angle:
        return STR_ANGL;
    case k_scpi_str_immediate:
        return STR_IMM;
    case k_scpi_str_initiate:
        return STR_INIT;
    case k_scpi_str_direction:
        return STR_DIR;
    case k_scpi_str_limit:
        return STR_LIM;
    case k_scpi_str_low:
        return STR_LOW;
    case k_scpi_str_high:
        return STR_HIGH;
    case k_scpi_str_state:
        return STR_STAT;
    case k_scpi_str_sense:
        return STR_SENS;
    case k_scpi_str_unknown:
    default:
        return 0;
    }
}

// *********************************************************************
//
//
const char *  scpi_str_long(scpi_menu_string_t item)
{

    switch (item)
    {
    case k_scpi_str_opc:
        return STR_OPC;
    case k_scpi_str_idn:
        return STR_IDN;
    case k_scpi_str_rst:
        return STR_RST;
    case k_scpi_str_input:
        return STR_INPUT;
    case k_scpi_str_position:
        return STR_POSITION;
    case k_scpi_str_a0:
        return STR_A0;
    case k_scpi_str_a1:
        return STR_A1;
    case k_scpi_str_a2:
        return STR_A2;
    case k_scpi_str_a3:
        return STR_A3;
    case k_scpi_str_angle:
        return STR_ANGLE;
    case k_scpi_str_immediate:
        return STR_IMMEDIATE;
    case k_scpi_str_initiate:
        return STR_INITIATE;
    case k_scpi_str_direction:
        return STR_DIRECTION;
    case k_scpi_str_limit:
        return STR_LIMIT;
    case k_scpi_str_low:
        return STR_LOW;
    case k_scpi_str_high:
        return STR_HIGH;
    case k_scpi_str_state:
        return STR_STATE;
    case k_scpi_str_sense:
        return STR_SENSE;
    case k_scpi_str_unknown:
    default:
        return 0;
    }
}

// *********************************************************************
//
//
size_t  scpi_str_len_short(scpi_menu_string_t item)
{
    switch (item)
    {
    case k_scpi_str_opc:
        return 4;
    case k_scpi_str_idn:
        return 4;
    case k_scpi_str_rst:
        return 4;
    case k_scpi_str_input:
        return 3;
    case k_scpi_str_position:
        return 3;
    case k_scpi_str_a0:
    case k_scpi_str_a1:
    case k_scpi_str_a2:
    case k_scpi_str_a3:
        return 2;
    case k_scpi_str_angle:
        return 4;
    case k_scpi_str_immediate:
        return 3;
    case k_scpi_str_initiate:
        return 4;
    case k_scpi_str_direction:
        return 3;
    case k_scpi_str_limit:
        return 3;
    case k_scpi_str_low:
        return 3;
    case k_scpi_str_high:
        return 4;
    case k_scpi_str_state:
        return 4;
    case k_scpi_str_sense:
        return 4;
    case k_scpi_str_unknown:
    default:
        return 0;
    }
}


// *********************************************************************
//
//
size_t  scpi_str_len_long(scpi_menu_string_t item)
{
    switch (item)
    {
    case k_scpi_str_opc:
        return 4;
    case k_scpi_str_idn:
        return 4;
    case k_scpi_str_rst:
        return 4;
    case k_scpi_str_input:
        return 5;
    case k_scpi_str_position:
        return 8;
    case k_scpi_str_a0:
    case k_scpi_str_a1:
    case k_scpi_str_a2:
    case k_scpi_str_a3:
        return 2;
    case k_scpi_str_angle:
        return 5;
    case k_scpi_str_immediate:
        return 9;
    case k_scpi_str_initiate:
        return 8;
    case k_scpi_str_direction:
        return 9;
    case k_scpi_str_limit:
        return 5;
    case k_scpi_str_low:
        return 3;
    case k_scpi_str_high:
        return 4;
    case k_scpi_str_state:
        return 5;
    case k_scpi_str_sense:
        return 5;
    case k_scpi_str_unknown:
    default:
        return 0;
    }
}

// *********************************************************************
//
//
int scpi_is_menu_match(const char * buffer, size_t len, scpi_menu_string_t item)
{
    //get long and short form lengths
    size_t llen = scpi_str_len_long(item);
    size_t slen = scpi_str_len_short(item);
    const char * stxt = scpi_str_short(item);
    const char * ltxt = scpi_str_long(item);
    int rc = 0;

    //perform a valid length check and compare if length is correct menu_item being checked
    if (len < slen)
    {
        //unable to test anything
        rc = TRUE;  //This will force a failure to match since strncmp should return 0
    }
    else if (len == slen)
    {
        //only test for shortform...
        rc = strncmp(buffer, stxt, slen);
    }
    else if (len == llen)
    {
        rc = strncmp(buffer, ltxt, llen);
    }
    else
    {
        rc = TRUE;  //This will force a failure to match since strncmp should return 0
    }

    return (0 == rc ) ? TRUE : FALSE;
}


// *********************************************************************
//
//
int scpi_find_level(char * buffer, size_t len, int level, char ** found, size_t * found_len)
{
    if (   !buffer
        || (len < SCPI_RX_MIN_LEN))   //Test parameters
        return -1;

    char * seek = buffer;
    size_t remain = len;
    char * needle1 = 0;
    char * needle2 = 0;
    char * needle3 = 0;

    if (   ':' == buffer[0] )
    {
        seek++;
        remain--;
    }

    //if seeking the 0-th menu level this won't even run
    while (level && remain)
    {
        if (':' == *seek)
        {
            level--;
        }

        seek++;
        remain--;
    }

    if (!level)
    {
        //try to find the next delimiter if it exists:
        needle1 = strchr(seek, ':');
        needle2 = strchr(seek, ' ');
        needle3 = strchr(seek, '\0');

        if (needle1 > seek)
        {
            *found_len = (needle1 - seek);
        }
        else if (needle2 > seek)
        {
            *found_len = (needle2 - seek);
        }
        else if (needle3 > seek)
        {
            *found_len = (needle3 - seek);
        }
        else
        {
            *found_len = remain;
        }

        *found = seek;
        return 0;
    }

    //unable to find menu level(x)
    return -2;

}


// *********************************************************************
//
//
int scpi_query_event_handler(uint32_t evt)
{
    switch(evt)
    {
    case k_scpi_root_q_idn:
        strncpy((char *)s_reply, STR_REPLY_IDN, strlen(STR_REPLY_IDN) + 1);
        break;
    case k_scpi_root_q_opc:
        strncpy((char *)s_reply, STR_REPLY_OK1, strlen(STR_REPLY_OK1) + 1);
        break;
    default:
        break;
    }

    return 0;
}

int                                     scpi_write_event_handler(uint32_t evt)
{

    strncpy((char *)s_reply, STR_REPLY_OK2, strlen(STR_REPLY_OK2) + 1);

//    switch(evt)
//    {
//    case k_scpi_root_rst:
//        strncpy((char *)s_reply, STR_REPLY_OK2, strlen(STR_REPLY_OK2) + 1);
//        break;
//    case k_scpi_root_opc:
//        strncpy((char *)s_reply, STR_REPLY_OK2, strlen(STR_REPLY_OK2) + 1);
//        break;
//    default:
//        break;
//    }

    return 0;
}

int                                     scpi_error_event_handler()
{
    strncpy((char *)s_reply, STR_REPLY_ERR, strlen(STR_REPLY_ERR) + 1);

    return 0;
}

int                                     scpi_error_partial_event_handler()
{
    strncpy((char *)s_reply, STR_REPLY_ERR_PARTIAL, strlen(STR_REPLY_ERR_PARTIAL) + 1);

    return 0;
}

// *********************************************************************
//
//
int scpi_input(const uint8_t * p_buf, size_t len, uint8_t ** p_reply, size_t * p_reply_len, uint32_t * event)
{
    static char       c_buffer[SCPI_RX_BFR_SZ];     //create a copy so we can manipulate the buffer
    int level         = 0;                          //scpi menu level

    size_t remain = len;

    scpi_menu_root_t    root_state = k_scpi_root_none;
    scpi_menu_input_t   input_state = k_scpi_input_none;

    scpi_menu_input_position_axis_angle_t
                        angle_state = k_scpi_input_position_axis_none;

    uint32_t last_state = 0;

    size_t cpy_len = len;

    //clip as needed to prevent segfault
    if (SCPI_RX_BFR_SZ < (len+1)) {
        cpy_len = SCPI_RX_BFR_SZ;
    }

    memcpy(c_buffer, p_buf, cpy_len);

    //put all characters in lowercase form
    int i=0;

    for (i = 0; i < cpy_len; i++)
        c_buffer[i] = tolower(c_buffer[i]);

    //parse the root level command / menu
    char * p_menu = 0;
    size_t menu_len;

    int rc = scpi_find_level(c_buffer, len, level, &p_menu, &menu_len);
    if (0 > rc || 0 == p_menu)
    {
        scpi_error_event_handler();
        *p_reply = s_reply;
        *p_reply_len = strlen((char *)s_reply);
        return -1;
    }

    remain -= menu_len;

#ifdef SCPI_DBG
    printf("scpi_find_level() done; root_state:0x%x p_menu:0x%x str: %s menu_len:0x%x\n", root_state, p_menu, p_menu, menu_len);
#endif

    rc = scpi_menu_root_sm(&root_state, p_menu, menu_len);
    last_state = (uint32_t)root_state;

    // 2   - indicates finished processing, with a command type
    // 1   - indicates finished processing, with a query type
    // 0   - continue down submenus
    // < 0 - error
    switch (rc)
    {
    case 0:
        level++;
        break;
    case 1:
        scpi_query_event_handler(last_state);
        *event = last_state;
        *p_reply = s_reply;
        *p_reply_len = strlen((char *)s_reply);
        return 1;
    case 2:
        scpi_write_event_handler(last_state);
        *event = last_state;
        *p_reply = s_reply;
        *p_reply_len = strlen((char *)s_reply);
        return 2;
    default:
        scpi_error_event_handler();
        *event = last_state;
        *p_reply = s_reply;
        *p_reply_len = strlen((char *)s_reply);
        return -2;
    }

    // dive into lower sub-menus until the final command is resolved
    while (!rc)
    {

#ifdef SCPI_DBG
        printf("scpi_find_level(%d) last_state:0x%x\n", level, last_state);
#endif
        // find next level! e.g. level(1)
        rc = scpi_find_level(c_buffer, len, level, &p_menu, &menu_len);
        if (0 > rc)
        {
            scpi_error_partial_event_handler();
            *event = last_state;          //return the partial command match (root menu)
            *p_reply = s_reply;
            *p_reply_len = strlen((char *)s_reply);
#ifdef SCPI_DBG
            printf("scpi_find_level() - failed last_state:0x%x\n", *event);
#endif
            return -3;
        }

#ifdef SCPI_DBG
        printf("scpi_find_level() done; last_state:0x%x p_menu:0x%x str: %s menu_len:0x%x\n", last_state, p_menu, p_menu, menu_len);
#endif
        remain -= menu_len;


        switch (last_state)
        {
        case k_scpi_root_input:
        case k_scpi_input_position:
        case k_scpi_input_position_a0:
        case k_scpi_input_position_a1:
        case k_scpi_input_position_a2:
        case k_scpi_input_position_a3:
            rc = scpi_menu_input_sm(((scpi_menu_input_t *)(&last_state)), p_menu, menu_len);
            break;
        case k_scpi_root_initiate:
            rc = scpi_menu_initiate_sm(((scpi_menu_initiate_t *)(&last_state)), p_menu, menu_len);
            break;
        case k_scpi_root_sense:
            rc = scpi_menu_input_sm(((scpi_menu_input_t *)(&last_state)), p_menu, menu_len);
            break;
            //input submenus

        case k_scpi_input_position_a0_angle:
        case k_scpi_input_position_a1_angle:
        case k_scpi_input_position_a2_angle:
        case k_scpi_input_position_a3_angle:
            rc = scpi_menu_input_pos_sm((scpi_menu_input_position_axis_angle_t *)&last_state, p_menu, menu_len);
            break;
        }

        //continue seeking?
        if (!rc)
        {
            level++;
        }
    }


    // 2   - indicates finished processing, with a command type
    // 1   - indicates finished processing, with a query type
    // 0   - continue down submenus
    // < 0 - error
    switch (rc)
    {
    case 0:
        level++;
        break;
    case 1:
        scpi_query_event_handler(last_state);
        *event = last_state;
        *p_reply = s_reply;
        *p_reply_len = strlen((char *)s_reply);
        return 1;
    case 2:
        scpi_write_event_handler(last_state);
        *event = last_state;
        *p_reply = s_reply;
        *p_reply_len = strlen((char *)s_reply);
        return 2;
    default:
        scpi_error_event_handler();
        *event = last_state;
        *p_reply = s_reply;
        *p_reply_len = strlen((char *)s_reply);
        return -2;
    }



//    // level should be "1"...
//    //root_state should already be a valid sub-menu
//    switch(root_state)
//    {
//    case k_scpi_root_input:
//
//        break;
//    case k_scpi_root_initiate:
//        break;
//    case k_scpi_root_sense:
//        break;
//    default:
//        scpi_error_event_handler();
//        *event = last_state;          //return the partial command match (root menu)
//        *p_reply = s_reply;
//        *p_reply_len = strlen((char *)s_reply);
//        return -4;
//    }
//
//    rc = scpi_find_level(p_menu, menu_len, level, &p_menu, &menu_len);
//    if (0 > rc)
//    {
//        scpi_error_event_handler();
//        *event = last_state;          //return the partial command match (root menu)
//        *p_reply = s_reply;
//        *p_reply_len = strlen((char *)s_reply);
//        return -3;
//    }

    return -5;
}

// **********************************************************************************
// Menu state machine handlers
//
int scpi_menu_root_sm(scpi_menu_root_t *state, const char * str, size_t str_len )
{
    if (!str || !str_len)
        return -1;

    char filter = 0;

    //pre-filter on the first character so we don't test every string under the sun when parsing for a match
    if (str_len > 1)
    {
        if ('*' == str[0])
        {
            filter = str[1];
        }
        else
        {
            filter = str[0];
        }
    }

    switch (filter)
    {
    case 'i':
        if (   scpi_is_menu_match(str, str_len-1, k_scpi_str_idn)
            && '?' == str[str_len-1]                              )
        {
            *state = k_scpi_root_q_idn;
            return 1;
        }
        else if (scpi_is_menu_match(str, str_len, k_scpi_str_input))
        {
            *state = k_scpi_root_input;
            return 0;
        }
        else if (scpi_is_menu_match(str, str_len, k_scpi_str_initiate))
        {
            *state = k_scpi_root_initiate;
            return 0;
        }
        break;
    case 'o':
        if (scpi_is_menu_match(str, str_len-1, k_scpi_str_opc)
                && '?' == str[str_len-1]                              )
        {
            *state = k_scpi_root_q_opc;
            return 1;

        }
        else if (scpi_is_menu_match(str, str_len, k_scpi_str_opc))
        {
            *state = k_scpi_root_opc;
            return 2;
        }
        break;
    case 'r':
        if (scpi_is_menu_match(str, str_len, k_scpi_str_rst))
        {
            *state = k_scpi_root_rst;
            return 2;
        }
        break;
    case 's':
        if (scpi_is_menu_match(str, str_len, k_scpi_str_sense))
        {
            *state = k_scpi_root_sense;
            return 0;
        }
        break;
    }



    return -1;
}

// *********************************************************************
//
//
int scpi_menu_input_sm(scpi_menu_input_t *state, const char * str, size_t str_len )
{
    if (!str || !str_len)
        return -1;

    const int expect_sz  = 4;
    scpi_menu_string_t expect[expect_sz];
    scpi_menu_string_t matched = k_scpi_str_unknown;

    for (int i = 0; i < expect_sz; i++)
    {
        expect[i] = k_scpi_str_unknown;
    }

    switch (*state)
    {

    case k_scpi_input:
        expect[0] = k_scpi_str_position;
        break;
    case k_scpi_input_position:
        expect[0] = k_scpi_str_a0;
        expect[1] = k_scpi_str_a1;
        expect[2] = k_scpi_str_a2;
        expect[3] = k_scpi_str_a3;
        break;
    case k_scpi_input_position_a0:
    case k_scpi_input_position_a1:
    case k_scpi_input_position_a2:
    case k_scpi_input_position_a3:
        expect[0] = k_scpi_str_angle;
        break;
    }

    for (int i = 0; i < expect_sz; i++)
    {
        if (k_scpi_str_unknown == expect[i])
        {
            break;
        }
        else if ( scpi_is_menu_match(str, str_len, expect[i]) )
        {
            matched = expect[i];
            break;
        }
    }

    if (!(int)matched)
    {
        return -2;  //failed to match a valid string, exit
    }

    switch (matched)
    {
    case k_scpi_str_position:
        *state = k_scpi_input_position;
        return 0;   //continue seek
    case k_scpi_str_a0:
        *state = k_scpi_input_position_a0;
        return 0;
    case k_scpi_str_a1:
        *state = k_scpi_input_position_a1;
        return 0;
    case k_scpi_str_a2:
        *state = k_scpi_input_position_a2;
        return 0;
    case k_scpi_str_a3:
        *state = k_scpi_input_position_a3;
        return 0;
    case k_scpi_str_angle:

        switch (*state)
        {
        case k_scpi_input_position_a0:
            *state = k_scpi_input_position_a0_angle;
            return 0;
        case k_scpi_input_position_a1:
            *state = k_scpi_input_position_a1_angle;
            return 0;
        case k_scpi_input_position_a2:
            *state = k_scpi_input_position_a2_angle;
            return 0;
        case k_scpi_input_position_a3:
            *state = k_scpi_input_position_a3_angle;
            return 0;
        }

        break;
    }

    return -3;
}

// *********************************************************************
//
//
int scpi_menu_initiate_sm(scpi_menu_initiate_t *state, const char * str, size_t str_len )
{
    if (!str || !str_len)
        return -1;

    scpi_menu_string_t expect = k_scpi_str_unknown;
    scpi_menu_string_t matched = k_scpi_str_unknown;

    switch (*state)
    {

    case k_scpi_initiate:
        expect = k_scpi_str_immediate;
        break;
    }


    if (k_scpi_str_unknown == expect)
    {
        return -2;
    }
    else if (   scpi_is_menu_match(str, str_len, expect) )
    {
        matched = expect;
    }


    if (!(int)matched)
    {
        return -3;  //failed to match a valid string, exit
    }

    switch (matched)
    {
    case k_scpi_str_immediate:
        *state = k_scpi_initiate_immediate;
        return 1;   //Accept command
    }

    return -4;
}

// *********************************************************************
//
//
int scpi_menu_input_pos_sm(scpi_menu_input_position_axis_angle_t *state, const char * str, size_t str_len )
{
    if (!str || !str_len)
        return -1;

    const int expect_sz  = 4;
    scpi_menu_string_t expect[expect_sz];
    scpi_menu_string_t matched = k_scpi_str_unknown;

    for (int i = 0; i < expect_sz; i++)
    {
        expect[i] = k_scpi_str_unknown;
    }

    switch (*state)
    {
        case k_scpi_input_position_axis_none:
            break;
        case k_scpi_input_position_a0_axis:
        case k_scpi_input_position_a1_axis:
        case k_scpi_input_position_a2_axis:
        case k_scpi_input_position_a3_axis:
            expect[0] = k_scpi_str_limit;
            expect[1] = k_scpi_str_direction;
            expect[2] = k_scpi_str_immediate;
            break;
        case k_scpi_input_position_a0_limit:
        case k_scpi_input_position_a1_limit:
        case k_scpi_input_position_a2_limit:
        case k_scpi_input_position_a3_limit:
            expect[0] = k_scpi_str_low;
            expect[1] = k_scpi_str_high;
            expect[2] = k_scpi_str_state;
            break;
        default:
            break;
    }

    for (int i = 0; i < expect_sz; i++)
    {
        if (k_scpi_str_unknown == expect[i])
        {
            break;
        }
        else if ( scpi_is_menu_match(str, str_len, expect[i]) )
        {
            matched = expect[i];
            break;
        }
    }

    if (!(int)matched)
    {
        return -2;  //failed to match a valid string, exit
    }

    switch (matched)
    {
    case k_scpi_str_immediate:
        switch (*state)
        {
        case k_scpi_input_position_a0_axis:
            *state = k_scpi_input_position_a0_immediate;
            return 2;
        case k_scpi_input_position_a1_axis:
            *state = k_scpi_input_position_a1_immediate;
            return 2;
        case k_scpi_input_position_a2_axis:
            *state = k_scpi_input_position_a2_immediate;
            return 2;
        case k_scpi_input_position_a3_axis:
            *state = k_scpi_input_position_a3_immediate;
            return 2;
        default:
            break;
        }
        break;
    case k_scpi_str_limit:
        switch (*state)
        {
        case k_scpi_input_position_a0_axis:
            *state = k_scpi_input_position_a0_limit;
            return 0;
        case k_scpi_input_position_a1_axis:
            *state = k_scpi_input_position_a1_limit;
            return 0;
        case k_scpi_input_position_a2_axis:
            *state = k_scpi_input_position_a2_limit;
            return 0;
        case k_scpi_input_position_a3_axis:
            *state = k_scpi_input_position_a3_limit;
            return 0;
        default:
            break;
        }
        break;
    case k_scpi_str_direction:
        switch (*state)
        {
        case k_scpi_input_position_a0_axis:
            *state = k_scpi_input_position_a0_dir;
            return 2;
        case k_scpi_input_position_a1_axis:
            *state = k_scpi_input_position_a1_dir;
            return 2;
        case k_scpi_input_position_a2_axis:
            *state = k_scpi_input_position_a2_dir;
            return 2;
        case k_scpi_input_position_a3_axis:
            *state = k_scpi_input_position_a3_dir;
            return 2;
        default:
            break;
        }
        break;
    case k_scpi_str_low:
        switch (*state)
        {
        case k_scpi_input_position_a0_limit:
            *state = k_scpi_input_position_a0_limit_low;
            return 2;
        case k_scpi_input_position_a1_limit:
            *state = k_scpi_input_position_a1_limit_low;
            return 2;
        case k_scpi_input_position_a2_limit:
            *state = k_scpi_input_position_a2_limit_low;
            return 2;
        case k_scpi_input_position_a3_limit:
            *state = k_scpi_input_position_a3_limit_low;
            return 2;
        default:
            break;
        }
        break;
    case k_scpi_str_high:
        switch (*state)
        {
        case k_scpi_input_position_a0_limit:
            *state = k_scpi_input_position_a0_limit_high;
            return 2;
        case k_scpi_input_position_a1_limit:
            *state = k_scpi_input_position_a1_limit_high;
            return 2;
        case k_scpi_input_position_a2_limit:
            *state = k_scpi_input_position_a2_limit_high;
            return 2;
        case k_scpi_input_position_a3_limit:
            *state = k_scpi_input_position_a3_limit_high;
            return 2;
        default:
            break;
        }
        break;
    case k_scpi_str_state:
        switch (*state)
        {
        case k_scpi_input_position_a0_limit:
            *state = k_scpi_input_position_a0_limit_state;
            return 2;
        case k_scpi_input_position_a1_limit:
            *state = k_scpi_input_position_a1_limit_state;
            return 2;
        case k_scpi_input_position_a2_limit:
            *state = k_scpi_input_position_a2_limit_state;
            return 2;
        case k_scpi_input_position_a3_limit:
            *state = k_scpi_input_position_a3_limit_state;
            return 2;
        default:
            break;
        }
        break;
    }

    return -3;
}



//    switch (root_state)
//    {
//        case k_scpi_root_q_idn:
//            break;
//        case k_scpi_root_rst:
//            break;
//        case k_scpi_root_opc:
//            break;
//        case k_scpi_root_q_opc:
//            break;
//        case k_scpi_root_input:
//            input_state =
//            break;
//        case k_scpi_root_initiate:
//            break;
//        case k_scpi_root_sense:
//            break;
//        case k_scpi_root_none:
//        default:
//            break;
//    }

//    switch (state)
//    {
//        case strstr() k_scpi_input_position_axis_none:
//            break;
//        case k_scpi_input_position_x_immediate:
//            break;
//        case k_scpi_input_position_x_limit_low:
//            break;
//        case k_scpi_input_position_x_limit_high:
//            break;
//        case k_scpi_input_position_x_limit_state:
//            break;
//        case k_scpi_input_position_x_limit_dir:
//            break;
//        case k_scpi_input_position_y_immediate:
//            break;
//        case k_scpi_input_position_y_limit_low:
//            break;
//        case k_scpi_input_position_y_limit_high:
//            break;
//        case k_scpi_input_position_y_limit_state:
//            break;
//        case k_scpi_input_position_y_limit_dir:
//            break;
//        case k_scpi_input_position_z_immediate:
//            break;
//        case k_scpi_input_position_z_limit_low:
//            break;
//        case k_scpi_input_position_z_limit_high:
//            break;
//        case k_scpi_input_position_z_limit_state:
//            break;
//        case k_scpi_input_position_z_limit_dir:
//            break;
//        default:
//            break;
//    }

