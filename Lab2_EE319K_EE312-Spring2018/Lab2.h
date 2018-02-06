// ****************** Lab2.h ***************
// Program written by: put your names here
// Date Created: 1/18/2017 
// Last Modified: 1/17/2018 
// Brief description of the Lab; 
// This version is for the combined EE319K (Valvano) EE312 (Gligoric) sections
// An embedded system is capturing temperature data from a
// sensor and performing analysis on the captured data.
// The controller part of the system is periodically capturing size
// readings of the temperature sensor. Your task is to write three
// analysis routines to help the controller perform its function
//   The three analysis subroutines are:
//    1. Calculate the mean of the temperature readings 
//       rounded down to the nearest integer
//    2. Calculate the range of the temperature readings, 
//       defined as the difference between the largest and smallest reading 
//    3. Check if the captured readings are a non-increasing montonic series
//       This simply means that the readings are sorted in non-increasing order.
//       We do not say "increasing" because it is possible for consecutive values
//       to be the same, hence the term "non-increasing". The controller performs 
//       some remedial operation and the desired effect of the operation is to 
//       lower the the temperature of the sensed system. This routine helps 
//       verify whether this has indeed happened


// Return the computed Mean
// Readings is an array of length N
// N is the length of the array
uint8_t Find_Mean(uint8_t Readings[],uint32_t N);

// Return the computed Range
// Readings is an array of length N
// N is the length of the array
uint8_t Find_Range(uint8_t Readings[],uint32_t N);

// Return True of False based on whether the readings
// a non-increasing montonic series
// Readings is an array of length N
// N is the length of the array
uint8_t IsMonotonic(uint8_t Readings[],uint32_t N);
