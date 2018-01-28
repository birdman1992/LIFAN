//
// File: ert_main.cpp
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
#include <stddef.h>
#include <stdio.h>                     // This ert_main.c example uses printf/fflush 
#include "main_m.h"                    // Model's header file
#include "rtwtypes.h"
#include "model_reference_types.h"

 main_mModelClass main_m_Obj;    // Instance of model class

// '<Root>/VAR_PLAT'
 VAR_PLAT arg_VAR_PLAT = {
  0.0,                                 // longitude
  0.0,                                 // latitude
  0.0F,                                // center_point_x
  0.0F,                                // center_point_y
  0.0F,                                // obj_size_x
  0.0F,                                // obj_size_y
  0U,                                  // obj_kind
  0U,                                  // auto_manual
  0U,                                  // launch_stop
  0U                                   // vehicle_speed
} ;

// '<Root>/VAR_OUTPUT'
 VAR_OUTPUT arg_VAR_OUTPUT;

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(main_m_Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model
  main_m_Obj.step(&arg_VAR_PLAT, &arg_VAR_OUTPUT);

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

//
// The example "main" function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific.  This example
// illustates how you do this relative to initializing the model.
//
/*
int_T main_0(int_T argc, const char *argv[])
{
  // Unused arguments
  (void)(argc);
  (void)(argv);

  // Initialize model
  main_m_Obj.initialize();

  // Attach rt_OneStep to a timer or interrupt service routine with
  //  period 0.005 seconds (the model's base sample time) here.  The
  //  call syntax for rt_OneStep is
  //
  //   rt_OneStep();

  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (rtmGetErrorStatus(main_m_Obj.getRTM()) == (NULL)) {
    //  Perform other application tasks here
  }

  // Disable rt_OneStep() here

  // Terminate model
  main_m_Obj.terminate();
  return 0;
}*/

//
// File trailer for generated code.
//
// [EOF]
//
