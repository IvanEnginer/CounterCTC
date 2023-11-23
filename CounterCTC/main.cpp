#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#define F_CPU 8000000

unsigned int Step = 0; // Счетчик состояния 
bool NewState = false;
bool State = false;
bool IRQ_Chek_button = false;

/*void Test()
{
    if (State)
    {
        PORTB = 0xFF;
    }
    else
    {
        PORTB = 0x00;
    }
}*/

// настройка таймера
void Init_TIMER1()
{
    TCCR1A = (1<<WGM12); // Определяем режим работы таймер СТС
 //   OCR1AH = 0xFF; // Регистр сравнения Hight    
 //   OCR1AL = 0xFF; // Регистр сравнения Low   
 //   TIMSK = (1<<OCIE1A); // разрешаем прерывание по совпадению
//    TCCR1B = (0<<CS10)|(1<<CS11)|(0<<CS12); // Устанавливаем предделитель и разрешаем работу счетчика              
}

//обработчик прерывания по совпадению А
ISR(TIMER1_COMPA_vect)
{    
    switch (Step)
    {
        case 0:
            TCCR1B = 0;
            TIMSK = (0<<OCIE1A);
            OCR1AH = 0x00;
            OCR1AL = 0x7F;                      
            Step++; 
            PORTB = (1<<PB0)|(0<<PB1); 
            TCNT1H = 0;
            TCNT1L = 0;
            TCCR1B =(1<<CS11)|(1<<CS10);
            TIMSK = (1<<OCIE1A);                                    
            break;
        case 1:
            TCCR1B = 0;        
            TIMSK = (0<<OCIE1A);        
            OCR1AH = 0x00;
            OCR1AL = 0x0F;  
            TCCR1B = (1<<CS10); 
            Step++;                             
            PORTB = (0<<PB0)|(0<<PB1);            
            TCNT1H = 0;
            TCNT1L = 0; 
            TIMSK = (1<<OCIE1A);                           
            break;  
        case 2:
            TCCR1B = 0;        
            TIMSK = (0<<OCIE1A);        
            PORTB = (0<<PB0)|(1<<PB1);
            TCCR1B =(1<<CS10)|(1<<CS11);            
            OCR1AH = 0x01;
            OCR1AL = 0xFF;
            Step++;
            TCNT1H = 0;
            TCNT1L = 0; 
            TIMSK = (1<<OCIE1A);                       
            break;            
        case 3:
            TCCR1B = 0;        
            TIMSK = (0<<OCIE1A);        
            OCR1AH = 0x00;
            OCR1AL = 0x0F;  
            TCCR1B = (1<<CS10); 
            Step = 0;                             
            PORTB = (0<<PB0)|(0<<PB1);            
            TCNT1H = 0;
            TCNT1L = 0; 
            TIMSK = (1<<OCIE1A);                      
           break;
    }               
}

void Chek_Button()
{
    if (bit_is_set(PIND, PIND7))
        NewState = true;
    if (bit_is_clear(PIND, PIND7))
        NewState = false;
    if (NewState != State)
    {
//        IRQ_Chek_button = true;
            if (State)
            {
                Step = 0;
//                OCR1AH = 0x00;
//                OCR1AL = 0x0F;                
                TIMSK = (1<<OCIE1A);
                TCCR1B = (1<<CS10)|(0<<CS11)|(0<<CS12); // Устанавливаем предделитель и разрешаем работу счетчика 
                TCNT1H = 0;
                TCNT1L = 0;                                            
            }
            else
            {   
                Step = 0;
//                TIMSK = (0<<OCIE1A);
//                OCR1AH = 0xFF;
//                OCR1AL = 0xFF;
                
                TCCR1B = 0;   // Запрещаем работу счетчика
                PORTB = 0; // Порт в 0
                TCNT1H = 0;
                TCNT1L = 0;                 
            }
    }
    State = NewState;
/*    if (!(PIND&(1<<PIND7)))
    {
        switch(n)
        case 0:
        
        break;
        TCCR1B = (1<<CS10)|(1<<CS11)|(0<<CS12); // Устанавливаем предделитель и разрешаем работу счетчика                   
    } 
    else
    {
        TCCR1B = 0;   // Запрещаем работу счетчика 
        PORTB = 0;   
    }   */     
}

/*void Service_IRQ_Chel_button ()
{
    if (IRQ_Chek_button)
    {
        if (State)
        {
            TCCR1B = (1<<CS10)|(1<<CS11)|(0<<CS12); // Устанавливаем предделитель и разрешаем работу счетчика           
        }
        else
        {
            TCCR1B = 0;   // Запрещаем работу счетчика
            PORTB = 0; // Порт в 0
        }    
            IRQ_Chek_button = false;        
    } 
    else
    {
    }

};*/

int main()
{
    DDRB=0xFF;//настраиваем OC1A на выход
    PORTB=0x00;// Начальное значение
    Init_TIMER1(); 
    sei();//разрешаем прерывания глобально
    while(1)
    Chek_Button();
//    Service_IRQ_Chel_button();
}    

