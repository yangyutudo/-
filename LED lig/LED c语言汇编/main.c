
#define RCC_AP2ENR	*((unsigned volatile int*)0x40021018)

#define GPIOA_CRL	*((unsigned volatile int*)0x40010800)
#define	GPIOA_ORD	*((unsigned volatile int*)0x4001080C)

#define GPIOB_CRH	*((unsigned volatile int*)0x40010C04)
#define	GPIOB_ORD	*((unsigned volatile int*)0x40010C0C)

#define GPIOC_CRH	*((unsigned volatile int*)0x40011004)
#define	GPIOC_ORD	*((unsigned volatile int*)0x4001100C)

void  Delay_wxc( volatile  unsigned  int  t)
{
     unsigned  int  i;
     while(t--)
         for (i=0;i<800;i++);
}

int main()
{
	int j=100;
	RCC_AP2ENR|=1<<2;			
	RCC_AP2ENR|=1<<3;			
	RCC_AP2ENR|=1<<4;		
	
	GPIOA_CRL&=0x0FFFFFFF;		
	GPIOA_CRL|=0x20000000;		
	GPIOA_ORD|=1<<7;			
	
	GPIOB_CRH&=0xFFFFFF0F;	
	GPIOB_CRH|=0x00000020;		
	GPIOB_ORD|=1<<9;		
	
	GPIOC_CRH&=0x0FFFFFFF;		
	GPIOC_CRH|=0x30000000;   	
	GPIOC_ORD|=0x1<<15;			
	while(j)
	{	
		GPIOA_ORD=0x0<<0;		
		Delay_wxc(1000000);
		GPIOA_ORD=0x1<<0;	
		Delay_wxc(1000000);
		
		GPIOB_ORD=0x0<<9;		
		Delay_wxc(1000000);
		GPIOB_ORD=0x1<<9;		
		Delay_wxc(1000000);
		
		GPIOC_ORD=0x0<<15;		
		Delay_wxc(1000000);
		GPIOC_ORD=0x1<<15;		
		Delay_wxc(1000000);
	}
}
