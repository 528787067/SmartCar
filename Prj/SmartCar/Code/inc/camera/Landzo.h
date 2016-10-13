#ifndef _Landzo_H_
#define _Landzo_H_

// …„œÒÕ∑ºƒ¥Ê∆˜≈‰÷√π‹Ω≈
//#define BFEND      PTE11_OUT
#define BFSDA      PTA25_OUT
#define BFCLK      PTA26_OUT
#define BFSDAI     PTA25_IN
//#define BFDDRA     DDRE12 


#define IICEorr    (0)
#define IICOK      (1)

void BFdelay_1us(uint8_t us) ;
void BFDly_ms(uint8_t ms) ;
void iic_start(void) ;
void iic_stop(void) ;
void slave_ACK(void) ;
void slave_NOACK(void) ;
uint8_t check_ACK(void);
void IICSendByte(uint8_t ch) ;
uint8_t IICreceiveByte(void) ;
uint8_t writeNbyte(uint8_t slave_add, uint8_t *slave_data,uint16_t n) ;
uint8_t receiveNbyte(uint8_t slave_add,uint8_t *rece_data, uint16_t n) ;
uint8_t LandzoIICEEROM_INIT(void) ;



#endif /* _Landzo_H_*/