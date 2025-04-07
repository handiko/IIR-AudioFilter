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

// Begin header file, lpf_2k9.h

#ifndef LPF_2K9_H_ // Include guards
#define LPF_2K9_H_

/*
Generated code is based on the following filter design:
<micro.DSP.FilterDocument sampleFrequency="#8000" arithmetic="float" biquads="Direct2" classname="lpf_2k9" inputMax="#1" inputShift="#15" >
  <micro.DSP.IirEllipticFilter N="#4" bandType="l" w1="#0.3625" w2="#0.4" stopbandRipple="#0.0035836576093353756" passbandRipple="#0.05" transitionRatio="#0.49008039526061653" >
    <micro.DSP.FilterStructure coefficientBits="#0" variableBits="#0" accumulatorBits="#0" biquads="Direct2" >
      <micro.DSP.FilterSection form="Direct2" historyType="WriteBack" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct2" historyType="WriteBack" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
    </micro.DSP.FilterStructure>
    <micro.DSP.PoleOrZeroContainer >
      <micro.DSP.PoleOrZero i="#0.40356523203515393" r="#-0.9149508749066326" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.1815959395069324" r="#-0.9833732326815665" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0.6598625939138564" r="#-0.6026176386392238" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.40508311260532326" r="#-0.20091897508607479" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#0" />
    </micro.DSP.PoleOrZeroContainer>
    <micro.DSP.GenericC.CodeGenerator generateTestCases="#false" />
    <micro.DSP.GainControl magnitude="#1" frequency="#0.2294921875" peak="#true" />
  </micro.DSP.IirEllipticFilter>
</micro.DSP.FilterDocument>

*/

static const int lpf_2k9_numStages = 2;
static const int lpf_2k9_coefficientLength = 10;
extern float lpf_2k9_coefficients[10];

typedef struct
{
	float state[8];
	float output;
} lpf_2k9Type;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} lpf_2k9_executionState;


lpf_2k9Type *lpf_2k9_create( void );
void lpf_2k9_destroy( lpf_2k9Type *pObject );
void lpf_2k9_init( lpf_2k9Type * pThis );
void lpf_2k9_reset( lpf_2k9Type * pThis );
#define lpf_2k9_writeInput( pThis, input )  \
    lpf_2k9_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define lpf_2k9_readOutput( pThis )  \
    (pThis)->output

int lpf_2k9_filterBlock( lpf_2k9Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define lpf_2k9_outputToFloat( output )  \
    (output)

#define lpf_2k9_inputFromFloat( input )  \
    (input)

void lpf_2k9_filterBiquad( lpf_2k9_executionState * pExecState );
#endif // LPF_2K9_H_
	
