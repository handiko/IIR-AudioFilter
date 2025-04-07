/******************************* SOURCE LICENSE *********************************
Copyright (c) 2021 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to 
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at https://www.micromodeler.com/launch.jsp

#include "lpf_2k85.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

float lpf_2k85_coefficients[10] = 
{
// Scaled for floating point

    0.5701150200902853, 1.1192625539114465, 0.570115020090285, -0.34438242211158304, -0.19688306510881998,// b0, b1, b2, a1, a2
    0.5, 0.9062802677440678, 0.4999999999999999, -1.1492373424300801, -0.7924500585076442// b0, b1, b2, a1, a2

};


lpf_2k85Type *lpf_2k85_create( void )
{
    lpf_2k85Type *result = (lpf_2k85Type *)malloc( sizeof( lpf_2k85Type ) ); // Allocate memory for the object
    lpf_2k85_init( result );                                               // Initialize it
    return result;                                                        // Return the result
}

void lpf_2k85_destroy( lpf_2k85Type *pObject )
{
    free( pObject );
}

void lpf_2k85_init( lpf_2k85Type * pThis )
{
    lpf_2k85_reset( pThis );
}

void lpf_2k85_reset( lpf_2k85Type * pThis )
{
    memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
    pThis->output = 0;                                    // Reset output
}

int lpf_2k85_filterBlock( lpf_2k85Type * pThis, float * pInput, float * pOutput, unsigned int count )
{
    lpf_2k85_executionState executionState;          // The executionState structure holds call data, minimizing stack reads and writes 
    if( ! count ) return 0;                         // If there are no input samples, return immediately
    executionState.pInput = pInput;                 // Pointers to the input and output buffers that each call to filterBiquad() will use
    executionState.pOutput = pOutput;               // - pInput and pOutput can be equal, allowing reuse of the same memory.
    executionState.count = count;                   // The number of samples to be processed
    executionState.pState = pThis->state;                   // Pointer to the biquad's internal state and coefficients. 
    executionState.pCoefficients = lpf_2k85_coefficients;    // Each call to filterBiquad() will advance pState and pCoefficients to the next biquad

    // The 1st call to filter1_filterBiquad() reads from the caller supplied input buffer and writes to the output buffer.
    // The remaining calls to filterBiquad() recycle the same output buffer, so that multiple intermediate buffers are not required.

    lpf_2k85_filterBiquad( &executionState );		// Run biquad #0
    executionState.pInput = executionState.pOutput;         // The remaining biquads will now re-use the same output buffer.

    lpf_2k85_filterBiquad( &executionState );		// Run biquad #1

    // At this point, the caller-supplied output buffer will contain the filtered samples and the input buffer will contain the unmodified input samples.  
    return count;		// Return the number of samples processed, the same as the number of input samples
}

void lpf_2k85_filterBiquad( lpf_2k85_executionState * pExecState )
{
    // Read state variables
    float w0, x0;
    float w1 = pExecState->pState[0];
    float w2 = pExecState->pState[1];

    // Read coefficients into work registers
    float b0 = *(pExecState->pCoefficients++);
    float b1 = *(pExecState->pCoefficients++);
    float b2 = *(pExecState->pCoefficients++);
    float a1 = *(pExecState->pCoefficients++);
    float a2 = *(pExecState->pCoefficients++);

    // Read source and target pointers
    float *pInput  = pExecState->pInput;
    float *pOutput = pExecState->pOutput;
    short count = pExecState->count;
    float accumulator;

    // Loop for all samples in the input buffer
    while( count-- )
    {
        // Read input sample
        x0 = *(pInput++);

        // Run feedback part of filter
        accumulator  = w2 * a2;
        accumulator += w1 * a1;
        accumulator += x0 ;

        w0 = accumulator ;

        // Run feedforward part of filter
        accumulator  = w0 * b0;
        accumulator += w1 * b1;
        accumulator += w2 * b2;

        w2 = w1;        // Shuffle history buffer
        w1 = w0;

        // Write output
        *(pOutput++) = accumulator ;
    }

    // Write state variables
    *(pExecState->pState++) = w1;
    *(pExecState->pState++) = w2;
}


