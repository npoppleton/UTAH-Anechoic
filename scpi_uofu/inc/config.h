/// @file config.h
///
/// Functions that handle SCPI parsing state-machine, invoking events,
/// invoke store and query of configuration and current state.
///
/// Author: Nathan Poppleton
///
/// Copyright: University of Utah, College of Engineering
///

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <stdint.h>

#ifdef    __cplusplus
extern "C" {
#endif

typedef enum axis_e
{
    k_axis_unknown      = 0xff,
    k_axis_a0           = 0,
    k_axis_a1           = 1,
    k_axis_a2           = 2,
    k_axis_a3           = 3
} axis_t;

typedef enum vna_pol_e
{
    k_vna_pol_unknown       = 0xff,
    k_vna_pol_active_low    = 0,
    k_vna_pol_active_high   = 1
} vna_pol_t;

typedef enum events_e
{
    k_unknown           = 0xff,
    k_none              = 0,
    k_halt              = 1,
    k_sweep_immediate   = 2,
    k_moveto            = 3
} events_t;

typedef struct scpi_cache_s
{
    int16_t a0_limit_min;
    int16_t a0_limit_max;
    int16_t a0_limit_angle_ref;  //home / offset
    int16_t a0_limit_angle;      //immediate_angle
    int16_t a1_limit_min;
    int16_t a1_limit_max;
    int16_t a1_limit_angle_ref;  //home / offset
    int16_t a1_limit_angle;      //immediate_angle
    int16_t a2_limit_min;
    int16_t a2_limit_max;
    int16_t a2_limit_angle_ref;  //home / offset
    int16_t a2_limit_angle;      //immediate_angle
    int16_t a3_limit_min;
    int16_t a3_limit_max;
    int16_t a3_limit_angle_ref;  //home / offset
    int16_t a3_limit_angle;      //immediate_angle
    axis_e  active_axis_sweep;   //only one axis is active for sweep functions at a time
    uint16_t ax_step_angle;      //active axis step angle

} scpi_cache_t;

typedef struct config_s
{
    axis_t   active_axis;        //active axis in use, e.g. 0|1|2|3
    vna_pol_t
             vna_rdy_pol;        //define the polarity of the VNA ready status input
    vna_pol_t
             vna_trig_pol;       //define the polarity of the VNA trigger invoke output
    uint8_t  hold_measure;       //hold for measurement when 1; continue to move pedestal during VNA sweep when 0;
    uint8_t  sweep_rate_rps;     //axis rotation rate rads/sec;
    int16_t  reference_angle;    //0-point reference for active axis
    int16_t  start_angle;        //sweep start angle
    int16_t  stop_angle;         //sweep stop  angle
    uint16_t step_angle;         //meas  step  angle
} config_t;

typedef struct state_s
{
    //azimuth needs to be implemented
    uint8_t halted;
    uint8_t sweeping;
    uint8_t vna_rdy;
    uint8_t holding_measure;
    int16_t a0_immediate;
    int16_t a1_immediate;
    int16_t a2_immediate;
    int16_t a3_immediate;
} state_t;


#ifdef  __cplusplus
}
#endif



#endif /* INC_CONFIG_H_ */
