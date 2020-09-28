
//LAB4 Complete Header File
//Chase Austin Deitner
//ECE 474 SP2020

//Lists all used register addressess and some GPIO clock run values for LAB 4 
//Includes:	GPIO E, N, F, and J
//		Timer 0, 1, and 2
//		Interupt Enable 0 and 1
//		ADC0


#ifndef LAB4
#define LAB4



//*****************************************************************
// Enable registers


#define SYSCTL_RCGCGPIO_R5      0x00000020  // GPIO Port F Run Mode Clock
#define SYSCTL_RCGCGPIO_R8      0x00000100  // GPIO Port J Run Mode Clock
#define SYSCTL_RCGCGPIO_R12     0x00001000  // GPIO Port N Run Mode Clock

//Timer and GPIO Run modes
#define SYSCTL_RCGCTIMER_R      (*((volatile uint32_t *)0x400FE604))
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_RCGCADC_R        (*((volatile uint32_t *)0x400FE638))

//Interupt enables
#define NVIC_EN0_R              (*((volatile uint32_t *)0xE000E100))
#define NVIC_EN1_R              (*((volatile uint32_t *)0xE000E104))

//*****************************************************************
// GPIO registers

#define GPIO_PORTE_DATA_R       (*((volatile uint32_t *)0x4005C3FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x4005C400))
#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x4005C420))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4005C51C))
#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x4005C528))

#define GPIO_PORTF_DATA_R       (*((volatile uint32_t *)0x4005D3FC))
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x4005D400))
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4005D51C))

#define GPIO_PORTN_DATA_R       (*((volatile uint32_t *)0x400643FC))
#define GPIO_PORTN_DIR_R        (*((volatile uint32_t *)0x40064400))
#define GPIO_PORTN_DEN_R        (*((volatile uint32_t *)0x4006451C))

#define GPIO_PORTJ_DATA_R       (*((volatile uint32_t *)0x400603FC))
#define GPIO_PORTJ_DIR_R        (*((volatile uint32_t *)0x40060400))
#define GPIO_PORTJ_PUR_R        (*((volatile uint32_t *)0x40060510))
#define GPIO_PORTJ_DEN_R        (*((volatile uint32_t *)0x4006051C))
//GPIO interupts
#define GPIO_PORTJ_IS_R         (*((volatile uint32_t *)0x40060404))
#define GPIO_PORTJ_IM_R         (*((volatile uint32_t *)0x40060410))


//*****************************************************************
// Timer registers

#define TIMER0_CFG_R            (*((volatile uint32_t *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile uint32_t *)0x40030004))
#define TIMER0_CTL_R            (*((volatile uint32_t *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile uint32_t *)0x40030018))
#define TIMER0_TAILR_R          (*((volatile uint32_t *)0x40030028))
#define TIMER0_ICR_R            (*((volatile uint32_t *)0x40030024))
#define TIMER0_RIS_R            (*((volatile uint32_t *)0x4003001C))
#define TIMER0_ADCEV_R          (*((volatile uint32_t *)0x40030070))

#define TIMER1_CFG_R            (*((volatile uint32_t *)0x40031000))
#define TIMER1_TAMR_R           (*((volatile uint32_t *)0x40031004))
#define TIMER1_CTL_R            (*((volatile uint32_t *)0x4003100C))
#define TIMER1_IMR_R            (*((volatile uint32_t *)0x40031018))
#define TIMER1_TAILR_R          (*((volatile uint32_t *)0x40031028))
#define TIMER1_ICR_R            (*((volatile uint32_t *)0x40031024))
#define TIMER1_RIS_R            (*((volatile uint32_t *)0x4003101C))

#define TIMER2_CFG_R            (*((volatile uint32_t *)0x40032000))
#define TIMER2_TAMR_R           (*((volatile uint32_t *)0x40032004))
#define TIMER2_CTL_R            (*((volatile uint32_t *)0x4003200C))
#define TIMER2_IMR_R            (*((volatile uint32_t *)0x40032018))
#define TIMER2_TAILR_R          (*((volatile uint32_t *)0x40032028))
#define TIMER2_ICR_R            (*((volatile uint32_t *)0x40032024))
#define TIMER2_RIS_R            (*((volatile uint32_t *)0x4003201C))



//*****************************************************************
// ADC registers
#define ADC0_ACTSS_R            (*((volatile uint32_t *)0x40038000))
#define ADC0_RIS_R              (*((volatile uint32_t *)0x40038004))
#define ADC0_ISC_R              (*((volatile uint32_t *)0x4003800C))
#define ADC0_SSCTL3_R           (*((volatile uint32_t *)0x400380A4))
#define ADC0_EMUX_R             (*((volatile uint32_t *)0x40038014))
#define ADC0_IM_R               (*((volatile uint32_t *)0x40038008))
#define ADC0_CC_R               (*((volatile uint32_t *)0x40038FC8))
#define ADC0_SSTSH3_R           (*((volatile uint32_t *)0x400380BC))
#define ADC0_SSFIFO3_R          (*((volatile uint32_t *)0x400380A8))


#endif