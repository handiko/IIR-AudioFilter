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

#include "lpf_3k.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

float lpf_3k_coefficients[10] = 
{
// Scaled for floating point

    0.6750074996209356, 1.3318649591971754, 0.6750074996209358, -0.5202951093607447, -0.2238627552570286,// b0, b1, b2, a1, a2
    0.5, 0.9307725691486297, 0.5, -1.3133702755871792, -0.811705329715734// b0, b1, b2, a1, a2

};


lpf_3kType *lpf_3k_create( void )
{
    lpf_3kType *result = (lpf_3kType *)malloc( sizeof( lpf_3kType ) ); // Allocate memory for the object
    lpf_3k_init( result );                                               // Initialize it
    return result;                                                        // Return the result
}

void lpf_3k_destroy( lpf_3kType *pObject )
{
    free( pObject );
}

void lpf_3k_init( lpf_3kType * pThis )
{
    lpf_3k_reset( pThis );
}

void lpf_3k_reset( lpf_3kType * pThis )
{
    memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
    pThis->output = 0;                                    // Reset output
}

int lpf_3k_filterBlock( lpf_3kType * pThis, float * pInput, float * pOutput, unsigned int count )
{
    lpf_3k_executionState executionState;          // The executionState structure holds call data, minimizing stack reads and writes 
    if( ! count ) return 0;                         // If there are no input samples, return immediately
    executionState.pInput = pInput;                 // Pointers to the input and output buffers that each call to filterBiquad() will use
    executionState.pOutput = pOutput;               // - pInput and pOutput can be equal, allowing reuse of the same memory.
    executionState.count = count;                   // The number of samples to be processed
    executionState.pState = pThis->state;                   // Pointer to the biquad's internal state and coefficients. 
    executionState.pCoefficients = lpf_3k_coefficients;    // Each call to filterBiquad() will advance pState and pCoefficients to the next biquad

    // The 1st call to filter1_filterBiquad() reads from the caller supplied input buffer and writes to the output buffer.
    // The remaining calls to filterBiquad() recycle the same output buffer, so that multiple intermediate buffers are not required.

    lpf_3k_filterBiquad( &executionState );		// Run biquad #0
    executionState.pInput = executionState.pOutput;         // The remaining biquads will now re-use the same output buffer.

    lpf_3k_filterBiquad( &executionState );		// Run biquad #1

    // At this point, the caller-supplied output buffer will contain the filtered samples and the input buffer will contain the unmodified input samples.  
    return count;		// Return the number of samples processed, the same as the number of input samples
}

void lpf_3k_filterBiquad( lpf_3k_executionState * pExecState )
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


