//==========================================================================
// Slide Clock - stepper.cpp
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#include <SpeedyStepper.h>
#include "hardware.h"
#include "menu.h"
#include "stepper.h"
#include "ClockCoordinates.h"

//=====================================
// Global Variables
//=====================================
SpeedyStepper minsStepper; // x-axis motor
SpeedyStepper tomStepper; // y-axis motor
SpeedyStepper hoursStepper; // z-axis motor
SpeedyStepper tohStepper; // a-axis motor

int iAcceleration;


//=====================================
// E n a b l e S t e p p e r s
//=====================================
void EnableSteppers() {
    digitalWrite(NOT_ENABLE_PIN, LOW);
}
//=====================================
// D i s a b l e S t e p p e r s
//=====================================
void DisableSteppers() {
    digitalWrite(NOT_ENABLE_PIN, HIGH);
}
//=====================================
// I n i t S t e p p e r s
//=====================================
void InitSteppers() {
    
    minsStepper.connectToPins(MINS_STEP_PIN, MINS_DIR_PIN);
    minsStepper.setStepsPerRevolution(FULL_STEPS_PER_REVOLUTION);
    minsStepper.setSpeedInStepsPerSecond(MAXSPEED);
    minsStepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION);

    tomStepper.connectToPins(TOM_STEP_PIN, TOM_DIR_PIN);
    tomStepper.setStepsPerRevolution(FULL_STEPS_PER_REVOLUTION);
    tomStepper.setSpeedInStepsPerSecond(MAXSPEED);
    tomStepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION);

    hoursStepper.connectToPins(HOURS_STEP_PIN, HOURS_DIR_PIN);
    hoursStepper.setStepsPerRevolution(FULL_STEPS_PER_REVOLUTION);
    hoursStepper.setSpeedInStepsPerSecond(MAXSPEED);
    hoursStepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION);

    tohStepper.connectToPins(TOH_STEP_PIN, TOH_DIR_PIN);
    tohStepper.setStepsPerRevolution(FULL_STEPS_PER_REVOLUTION);
    tohStepper.setSpeedInStepsPerSecond(MAXSPEED);
    tohStepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION);

    //Default to Steppers disabled
    DisableSteppers();
}
//=====================================
// M o v e R e l a t i v e
//=====================================
void MoveRelative(int iMotor, long lSteps) {

    EnableSteppers();
    switch (iMotor) {
      case MINUTES :
          minsStepper.moveRelativeInSteps(lSteps);
          break;
      case TOM :
          tomStepper.moveRelativeInSteps(lSteps);
          break;
      case HOURS :
          hoursStepper.moveRelativeInSteps(lSteps);
          break;
      case TOH :
          tohStepper.moveRelativeInSteps(lSteps);
          break;
    }

    DisableSteppers();
}
//=====================================
// M o v e A b s o l u t e
//=====================================
void MoveAbsolute(int iMotor, long lSteps) {

    EnableSteppers();

    switch (iMotor) {
      case MINUTES :
          minsStepper.moveToPositionInSteps(lSteps);
          break;
      case TOM :
          tomStepper.moveToPositionInSteps(lSteps);
          break;
      case HOURS :
          hoursStepper.moveToPositionInSteps(lSteps);
          break;
      case TOH :
          tohStepper.moveToPositionInSteps(lSteps);
          break;
    }

    DisableSteppers();
}
//=====================================
// G e t C u r r e n t P o s i t i o n
//=====================================
long GetCurrentPosition(int iMotor) {
   
   switch (iMotor) {
      case MINUTES :
          return minsStepper.getCurrentPositionInSteps();
          break;
      case TOM :
          return tomStepper.getCurrentPositionInSteps();
          break;
      case HOURS :
          return hoursStepper.getCurrentPositionInSteps();
          break;
      case TOH :
          return tohStepper.getCurrentPositionInSteps();
          break;
    }  
}
//=====================================
// Z e r o P o s i t i o n
//=====================================
void ZeroPosition(int iMotor) {

    switch (iMotor) {
      case MINUTES :
          minsStepper.setCurrentPositionInSteps(0);
          break;
      case TOM :
          tomStepper.setCurrentPositionInSteps(0);
          break;
      case HOURS :
          hoursStepper.setCurrentPositionInSteps(0);
          break;
      case TOH :
          tohStepper.setCurrentPositionInSteps(0);
          break;
    }
}
//=====================================
// M o v e A l l S t e p p e r s
//=====================================
void MoveAllSteppers(long lTOHSteps, long lHoursSteps, long lTOMSteps, long lMinutesSteps) {

    EnableSteppers();

    tohStepper.setupMoveInSteps(lTOHSteps);
    hoursStepper.setupMoveInSteps(lHoursSteps);
    tomStepper.setupMoveInSteps(lTOMSteps);
    minsStepper.setupMoveInSteps(lMinutesSteps);

    while((!tohStepper.motionComplete()) || (!hoursStepper.motionComplete()) || (!tomStepper.motionComplete()) || (!minsStepper.motionComplete()))
    {
        tohStepper.processMovement();
        hoursStepper.processMovement();
        tomStepper.processMovement();
        minsStepper.processMovement();
    
    }

    DisableSteppers();
}
//===============================================================================
// F i n d H o m e P o s i t i o n
//
// I originally was going to add hall effect sensors and a magnet to each slide
// to detect the home position. I then realized that I can just run the slide
// all the way to the top for the max number of steps and that was just
// as effective. 
//===============================================================================
void FindHomePosition() {

    EnableSteppers();

    //Go up the maximum number of steps
    tohStepper.setupRelativeMoveInSteps(-MAX_TOH_STEPS);
    hoursStepper.setupRelativeMoveInSteps(-MAX_HOURS_STEPS);
    tomStepper.setupRelativeMoveInSteps(-MAX_TOM_STEPS);
    minsStepper.setupRelativeMoveInSteps(-MAX_MINUTES_STEPS);

    while((!tohStepper.motionComplete()) || (!hoursStepper.motionComplete()) || (!tomStepper.motionComplete()) || (!minsStepper.motionComplete())) {
        tohStepper.processMovement();
        hoursStepper.processMovement();
        tomStepper.processMovement();
        minsStepper.processMovement();
        }

    DisableSteppers();

    // Now at home position so set zero
    tohStepper.setCurrentPositionInSteps(0);
    hoursStepper.setCurrentPositionInSteps(0);
    tomStepper.setCurrentPositionInSteps(0);
    minsStepper.setCurrentPositionInSteps(0);
}
