#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include <stdio.h>
#include "misc.h"

uint8_t ADCoku()
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_56Cycles); //ADC KANAL 0 SECILDI VE 56 CYCLES KONUMUNDA
	ADC_SoftwareStartConv(ADC1); //DONUSUME BASLA DEDÝK
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET); //HER DÖNUSUM SONRASI BAYRAGI SIFIRLAYALIM
	return ADC_GetConversionValue(ADC1);

}


int main(void)
{
	uint8_t value=0; //8 bitlik bir degiþken atadýk gerilimin digital degerini saklamak ýcýn

	// port yönlendirmelerini yapalým
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	//SECTIGIMIZ BIRIMLERIN CLOCK PALSLERINI VERELIM

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	//A VE D PORTLARININ ÖZELLÝKLERNÝ BELIRLEYELIM
	/*D PORTU*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	/*A PORTU ANALOG OKUMAYI BU PORTTAN YAPIYORUZ*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN; // ANALOG GÝRÝÞ MODU
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*ADC-1 AYARLARINI YAPALIM */

	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
    ADC_InitStructure.ADC_ScanConvMode=DISABLE;
    ADC_InitStructure.ADC_Resolution=ADC_Resolution_8b;
    ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion=1;
    ADC_Init(ADC1,&ADC_InitStructure);

    ADC_Cmd(ADC1,ENABLE);



    while(1)
    {
     	value=ADCoku(); // deger dondur
    	double voltaj,voltaj1; //
    	voltaj=3*value; //bölücü köprü parametresi
    	voltaj1=voltaj/241;

    	/*LED DURUMLARININ KONTROLLERÝ*/

    	if(voltaj1<1 && voltaj1>=0)           /* 0 ÝLE 1 ARASINDA*/
    	{
    		GPIO_SetBits(GPIOD,GPIO_Pin_12);
    		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
    		GPIO_ResetBits(GPIOD,GPIO_Pin_14);
    		GPIO_ResetBits(GPIOD,GPIO_Pin_15);

    	}

    	else if(voltaj1>=1 && voltaj1<2)      /*1 ÝLE 2 ARASINDA*/
    	{
    		GPIO_SetBits(GPIOD,GPIO_Pin_12);
    		GPIO_SetBits(GPIOD,GPIO_Pin_13);
    		GPIO_ResetBits(GPIOD,GPIO_Pin_14);
    		GPIO_ResetBits(GPIOD,GPIO_Pin_15);

    	}
    	else if (voltaj1>=2 && voltaj1<3)      /*2 ÝLE 3 ARASINDA*/
    	{
    		GPIO_SetBits(GPIOD,GPIO_Pin_12);
    		GPIO_SetBits(GPIOD,GPIO_Pin_13);
    		GPIO_SetBits(GPIOD,GPIO_Pin_14);
    		GPIO_ResetBits(GPIOD,GPIO_Pin_15);

    	}

    	else if(voltaj1>=3 && voltaj1<4)       /* 3 ÝLE 4 ARASINDA*/
    	{
    		GPIO_SetBits(GPIOD,GPIO_Pin_12);
    		GPIO_SetBits(GPIOD,GPIO_Pin_13);
    		GPIO_SetBits(GPIOD,GPIO_Pin_14);
    		GPIO_SetBits(GPIOD,GPIO_Pin_15);

   	}
  }

}


uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
    return 0; // ses çipini kullanýyorsanýz tek sample veriyi burada return ile döndürün.
}
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
    return; // ses çipini kullanýyorsanýz  burada çipe veri aktarýmý DMA sona ermiþ
}

