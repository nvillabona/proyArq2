#include <htc.h>
__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & LVP_OFF); 

#define  _XTAL_FREQ  4000000

#define IrqIntRB RBIF
#define IrqTMR1  TMR1IF
//#define IrqIntExt INTF

#define Pul1 RB7
#define Pul2 RB6
#define Pul3 RB5
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
//#define Leds PORTD
#define Display PORTC
#define Op 1

#define DisLed_A RC7
#define DisLed_B RC6
#define DisLed_C RC5
#define DisLed_D RC4
#define DisLed_E RC3
#define DisLed_F RC2
#define DisLed_G RC1

#define PINM RB3 //pind12
#define PINC RB2 //pind11
#define PIND RB1 //pind10
#define PINU RB0 //pind9

unsigned char Reloj[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
unsigned char ContadorSegundos = 0;


void interrupt FunInt(void); //Prototipos
//void ConfigIntExt(void);
void ConfigIntRB(void);
void ConfigTMR1(void);
void reloj(void);
void reset(void);

	unsigned char unidades = 0;
	unsigned char decenas = 0;
	unsigned char centenas = 0;
	unsigned char miles = 0;
	unsigned char BanderaResortico = 1;
	unsigned char BanderaReloj = 1;
	unsigned char ContDisplay = 0;
	
	
void main (void){
	
TRISC = 0X00;
TRISD = 0x00;
TMR1ON = 1;
ConfigTMR1();
ConfigIntRB();

unsigned int ContResortico = 0;
unsigned char validar = 1;
unsigned char Resortico[9]={0x03,0x07,0x0F,0x1E,0x3C,0x78,0xF0,0xE0,0xC0};
unsigned char PosResortico = 0;
unsigned char Direccion = 1;
while(1){
	__delay_ms(5);
	if(BanderaResortico == 1){
		ContResortico++;
		if(ContResortico == 260){
			if(Direccion == 1){
				PORTD = Resortico[PosResortico];
				
				if(PosResortico == 8){
					PosResortico--;
					Direccion = 0;
				}else{
					PosResortico++;
				}
			}else{
				PORTD = Resortico[PosResortico];
				
				if(PosResortico == 0){
					PosResortico++;
					Direccion = 1;
				}else{
					PosResortico--;
				}
			}
			ContResortico = 0;
			}
				}
			
			}
		}	
		
void reloj(void){

    if(BanderaReloj == 1){

		ContadorSegundos++;
        if (ContadorSegundos == 200){
            unidades++;
            } 
            if(unidades == 9){
                unidades = 0;
                decenas++;
            }
             if( decenas == 5){
                decenas = 0;
                centenas++;
            }
             if(centenas == 9){
                centenas= 0;
                miles++;
            }
             if(miles == 9){
                miles = 0;
            }
	}
}

void resetReloj (void){
	unidades = 0;
	decenas = 0;
	centenas = 0;
	miles= 0;
	ContadorSegundos= 0;
}


void interrupt FunInt(void){
	
	if(IrqIntRB == 1){
		__delay_ms(5); 
		
		if(Pul1 == Op) 
			BanderaResortico = ~BanderaResortico; 
		if(Pul2 == Op){ 
			BanderaReloj = ~BanderaReloj; 
		}
        if (Pul3 == Op){
            resetReloj();
        }
		
		IrqIntRB = 0; // Reposicion
	}
	
	if(IrqTMR1 == 1){ 
			
			TMR1 = -5000;
			ContDisplay++;
			
			if (ContDisplay == 1){
				PINM = 1;
				Display = ~Reloj[unidades];
				PINU = 0;
			
			}else if (ContDisplay == 2){
				PINU = 1;
				Display = ~Reloj[decenas];
				PIND = 0;
			}else if (ContDisplay == 3){
				PIND = 1;
				Display = ~Reloj[centenas];
				PINC = 0;
			}else if (ContDisplay == 4){
				PINC = 1;
				Display = ~Reloj[miles];
				PINM = 0;
				
			}
			if (ContDisplay == 4){
				ContDisplay = 0;
			}
			reloj();
			
			//TMR1ON = 0; //Para el TMR1
	
			IrqTMR1 = 0; // Reposicion
	}

}  

void ConfigIntRB(void){
//Configura y Habilita la interrupción del Puerto B

//Configuración: Una opción
TRISB |= 0B11110000; //Puertos RB7-RB4 de entrada

//Habilitación:
GIE=1; RBIE=1; //Bits: INTCON<7,3>
} 


void ConfigTMR1(void){
	
		TMR1 = -5000; 
		
		#define FM 0x00  //FM = 1  
		//#define FM 0x10  //FM = 2
		//#define FM 0x20  //FM = 4
		//#define FM 0x30  //FM = 8
		T1CON = 0b00000001 | FM;
		//Habilitación: NO cambiar
		GIE=1; PEIE=1; TMR1IE=1;
}