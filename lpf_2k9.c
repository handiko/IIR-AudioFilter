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

#include "lpf_2k9.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

float lpf_2k9_coefficients[10] = 
{
// Scaled for floating point

    0.6034331798066227, 1.186800073467511, 0.6034331798066226, -0.40183795017214957, -0.20446076266765573,// b0, b1, b2, a1, a2
    0.5, 0.9149508749066326, 0.5, -1.2052352772784476, -0.7985666612458371// b0, b1, b2, a1, a2

};


lpf_2k9Type *lpf_2k9_create( void )
{
    lpf_2k9Type *result = (lpf_2k9Type *)malloc( sizeof( lpf_2k9Type ) ); // Allocate memory for the object
    lpf_2k9_init( result );                                               // Initialize it
    return result;                                                        // Return the result
}

void lpf_2k9_destroy( lpf_2k9Type *pObject )
{
    free( pObject );
}

void lpf_2k9_init( lpf_2k9Type * pThis )
{
    lpf_2k9_reset( pThis );
}

void lpf_2k9_reset( lpf_2k9Type * pThis )
{
    memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
    pThis->output = 0;                                    // Reset output
}

int lpf_2k9_filterBlock( lpf_2k9Type * pThis, float * pInput, float * pOutput, unsigned int count )
{
    lpf_2k9_executionState executionState;          // The executionState structure holds call data, minimizing stack reads and writes 
    if( ! count ) return 0;                         // If there are no input samples, return immediately
    executionState.pInput = pInput;                 // Pointers to the input and output buffers that each call to filterBiquad() will use
    executionState.pOutput = pOutput;               // - pInput and pOutput can be equal, allowing reuse of the same memory.
    executionState.count = count;                   // The number of samples to be processed
    executionState.pState = pThis->state;                   // Pointer to the biquad's internal state and coefficients. 
    executionState.pCoefficients = lpf_2k9_coefficients;    // Each call to filterBiquad() will advance pState and pCoefficients to the next biquad

    // The 1st call to filter1_filterBiquad() reads from the caller supplied input buffer and writes to the output buffer.
    // The remaining calls to filterBiquad() recycle the same output buffer, so that multiple intermediate buffers are not required.

    lpf_2k9_filterBiquad( &executionState );		// Run biquad #0
    executionState.pInput = executionState.pOutput;         // The remaining biquads will now re-use the same output buffer.

    lpf_2k9_filterBiquad( &executionState );		// Run biquad #1

    // At this point, the caller-supplied output buffer will contain the filtered samples and the input buffer will contain the unmodified input samples.  
    return count;		// Return the number of samples processed, the same as the number of input samples
}

void lpf_2k9_filterBiquad( lpf_2k9_executionState * pExecState )
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


