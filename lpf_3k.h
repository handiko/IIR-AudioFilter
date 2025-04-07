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

// Begin header file, lpf_3k.h

#ifndef LPF_3K_H_ // Include guards
#define LPF_3K_H_

/*
Generated code is based on the following filter design:
<micro.DSP.FilterDocument sampleFrequency="#8000" arithmetic="float" biquads="Direct2" classname="lpf_3k" inputMax="#1" inputShift="#15" >
  <micro.DSP.IirEllipticFilter N="#4" bandType="l" w1="#0.375" w2="#0.4" stopbandRipple="#0.0035836576093353756" passbandRipple="#0.05" transitionRatio="#0.49008039526061653" >
    <micro.DSP.FilterStructure coefficientBits="#0" variableBits="#0" accumulatorBits="#0" biquads="Direct2" >
      <micro.DSP.FilterSection form="Direct2" historyType="WriteBack" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct2" historyType="WriteBack" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
    </micro.DSP.FilterStructure>
    <micro.DSP.PoleOrZeroContainer >
      <micro.DSP.PoleOrZero i="#0.3655987206220219" r="#-0.9307725691486297" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.16342551697686894" r="#-0.9865556752666538" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0.6168224700160884" r="#-0.6566851377935896" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.3952037513117142" r="#-0.2601475546803724" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#0" />
    </micro.DSP.PoleOrZeroContainer>
    <micro.DSP.GenericC.CodeGenerator generateTestCases="#false" />
    <micro.DSP.GainControl magnitude="#1" frequency="#0.24609375" peak="#true" />
  </micro.DSP.IirEllipticFilter>
</micro.DSP.FilterDocument>

*/

static const int lpf_3k_numStages = 2;
static const int lpf_3k_coefficientLength = 10;
extern float lpf_3k_coefficients[10];

typedef struct
{
	float state[8];
	float output;
} lpf_3kType;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} lpf_3k_executionState;


lpf_3kType *lpf_3k_create( void );
void lpf_3k_destroy( lpf_3kType *pObject );
void lpf_3k_init( lpf_3kType * pThis );
void lpf_3k_reset( lpf_3kType * pThis );
#define lpf_3k_writeInput( pThis, input )  \
    lpf_3k_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define lpf_3k_readOutput( pThis )  \
    (pThis)->output

int lpf_3k_filterBlock( lpf_3kType * pThis, float * pInput, float * pOutput, unsigned int count );
#define lpf_3k_outputToFloat( output )  \
    (output)

#define lpf_3k_inputFromFloat( input )  \
    (input)

void lpf_3k_filterBiquad( lpf_3k_executionState * pExecState );
#endif // LPF_3K_H_
	
