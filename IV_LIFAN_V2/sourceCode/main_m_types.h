//
// File: main_m_types.h
//
// Code generated for Simulink model 'main_m'.
//
// Model version                  : 1.32
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sun Sep  3 07:59:46 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_main_m_types_h_
#define RTW_HEADER_main_m_types_h_
#include "rtwtypes.h"
#include "model_reference_types.h"
#ifndef _DEFINED_TYPEDEF_FOR_VAR_PLAT_
#define _DEFINED_TYPEDEF_FOR_VAR_PLAT_

typedef struct {
  real_T longitude;
  real_T latitude;
  real32_T center_point_x;
  real32_T center_point_y;
  real32_T obj_size_x;
  real32_T obj_size_y;
  uint16_T obj_kind;
  uint8_T auto_manual;
  uint8_T launch_stop;
  uint8_T vehicle_speed;
} VAR_PLAT;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_VAR_INPUT_
#define _DEFINED_TYPEDEF_FOR_VAR_INPUT_

typedef struct {
  real_T longitude;
  real_T latitude;
  real32_T center_point_x;
  real32_T center_point_y;
  real32_T obj_size_x;
  real32_T obj_size_y;
  uint16_T obj_kind;
  uint8_T auto_manual;
  uint8_T launch_stop;
  uint8_T vehicle_speed;
} VAR_INPUT;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_VAR_OUTPUT_
#define _DEFINED_TYPEDEF_FOR_VAR_OUTPUT_

typedef struct {
  uint8_T gear_sts;
  uint8_T gear_enable;
  real32_T brake_pressure;
  uint8_T brake_enable;
  real32_T gas_pedal_open;
  uint8_T gas_pedal_enable;
  real32_T steering_angle;
  uint8_T steering_angle_enable;
  uint8_T left_lamp;
  uint8_T right_lamp;
  uint8_T horn;
} VAR_OUTPUT;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_VAR_ACTUATOR_
#define _DEFINED_TYPEDEF_FOR_VAR_ACTUATOR_

typedef struct {
  uint8_T gear_sts;
  uint8_T gear_enable;
  real32_T brake_pressure;
  uint8_T brake_enable;
  real32_T gas_pedal_open;
  uint8_T gas_pedal_enable;
  real32_T steering_angle;
  uint8_T steering_angle_enable;
  uint8_T left_lamp;
  uint8_T right_lamp;
  uint8_T horn;
} VAR_ACTUATOR;

#endif

// Forward declaration for rtModel
typedef struct tag_RTM_main_m_T RT_MODEL_main_m_T;

#endif                                 // RTW_HEADER_main_m_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
