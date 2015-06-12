const short int dirs0  = 6;
const short int steps0 = 7;
const short int sleep0 = 8;

const short int dirs1  = 9;
const short int steps1 = 10;
const short int sleep1 = 11;

const short int dirs2  = 3;
const short int steps2 = 4;
const short int sleep2 = 5;


const unsigned short int max_notes = 35;
const short int FETCH_NOTE = 1;
const short int PLAY_NOTE = 2;
const short int PLAY_SILENCE = 3;
const short int MICRO_DELAY = 40;
//const float ADJUSTMENT = 0.75;//ajuste para 3 motores, sin memoria externa.
const float ADJUSTMENT = 0.81;//ajuste para 2 motores, sin memoria externa.
char dummy;
float freq [90];
unsigned short int frecuencia;
unsigned short int duracion;

unsigned short int i = 0;
unsigned short int j = 0;
unsigned short int k = 0;
unsigned short int m = 0;

struct nota
{
   unsigned short int pitch;
   unsigned short int duration;
};

nota notas0[max_notes];
nota notas1[max_notes];
nota notas2[max_notes];
//nota notas2[5];

void tocar_cancion();

void setup()
{
   Serial.begin(9600);
   freq[50] = 293.66;
   freq[51] = 311.13;
   freq[52] = 329.63 ;
   freq[53] = 349.23;
   freq[54] = 369.99;
   freq[55] = 392.00;
   freq[56] = 415.30;
   freq[57] = 440;
   freq[58] = 466.16;
   freq[59] = 493.88;
   freq[60] = 523.25;
   freq[61] = 554.37;
   freq[62] = 587.33;
   freq[63] = 622.25;
   freq[64] = 659.26;
   freq[65] = 698.46;
   freq[66] = 739.99;
   freq[67] = 783.99;
   freq[68] = 830.61;
   freq[69] = 880.00;
   freq[70] = 932.33;
   freq[71] = 987.77;
   freq[72] = 1046.50;
   freq[73] = 1108.73;
   freq[74] = 1174.66;
   freq[75] = 1244.51;
   freq[76] = 1318.51;
   freq[77] = 1396.91;
   freq[78] = 1479.98;
   freq[79] = 1567.98;
   freq[80] = 1661.22;
   freq[81] = 1760.00;
   freq[82] = 1864.66;
   freq[83] = 1975.53;
   freq[84] = 2093.00;
   freq[85] = 2217.46;
   freq[86] = 2349.32;
   
   pinMode(steps0,OUTPUT);
   pinMode(dirs0,OUTPUT);
   pinMode(sleep0,OUTPUT);
   digitalWrite(sleep0,LOW);//set to sleep
   digitalWrite(dirs0,LOW);

   pinMode(steps1,OUTPUT);
   pinMode(dirs1,OUTPUT);
   pinMode(sleep1,OUTPUT);
   digitalWrite(sleep1,LOW);//set to sleep
   digitalWrite(dirs1,LOW);

   pinMode(steps2,OUTPUT);
   pinMode(dirs2,OUTPUT);
   pinMode(sleep2,OUTPUT);
   digitalWrite(sleep2,LOW);//set to sleep
   digitalWrite(dirs2,LOW);
}

void loop()
{
   unsigned short int index_of_d;
   String msg = NULL;

   while(true)//loop infinito
   {
      if (Serial.available()>0)
      {
         msg = NULL;
         while(Serial.available()>0)
         {
            dummy = Serial.read();               
            msg += dummy;
            delay(3);
         }  
         delay(1);
         m = msg.substring(0,1).toInt();  
   // Serial.println(msg.charAt(1));  
         switch(msg.charAt(1))
         {
            
            case 'N':  
               index_of_d = msg.indexOf('D');
               frecuencia = (unsigned short int) msg.substring(2,index_of_d).toInt();         
               duracion = (unsigned short int) msg.substring(index_of_d+1).toInt();
               if (0 == m and i < max_notes)
               {
                  notas0[i].pitch = frecuencia;
                  notas0[i].duration = duracion;
                  i++;
               }  
               if (1 == m && j < max_notes)
               {
                  notas1[j].pitch = frecuencia;
                  notas1[j].duration = duracion;
                  j++;
               }
               if (2 == m && k < max_notes)
               {
                  notas2[k].pitch = frecuencia;
                  notas2[k].duration = duracion;
                  k++;
               }
    
               Serial.println(msg);         
               break;
            
            case 'S':
               duracion = msg.substring(2).toInt();
         
               if (0==m && i < max_notes)
               {
                  notas0[i].pitch = 0;
                  notas0[i].duration = duracion;
                  i++;                 
               }   
               if(1 == m && j < max_notes)
               {
                  notas1[j].pitch = 0;
                  notas1[j].duration = duracion;
                  j++;
               }  
               if(2 == m && k < max_notes)
               {
                  notas2[k].pitch = 0;
                  notas2[k].duration = duracion;
                  k++;
               }  
               Serial.println(msg);         
               break;
         
            case 'X':
//        Serial.print(msg);
               tocar_cancion();
               break;
            default:
               Serial.print("(");
               Serial.print(msg);
               Serial.println(") is not valid ");                 
         } 
//    Serial.println(msg);
      }
   }    
}

void tocar_cancion()
{
   int pulses0,pulses1,pulses2;
   int current_pulse0,current_pulse1,current_pulse2;
   long int pause0,pause1,pause2;
   long int current_pause0 = 0,current_pause1 = 0,current_pause2 = 0;
   int nota0 = 0, nota1 = 0, nota2 = 0;
   int status0 = FETCH_NOTE, status1 = FETCH_NOTE, status2 = FETCH_NOTE;
    
    
    
   //despertar los motores
   digitalWrite(sleep0,HIGH);//awaken
   digitalWrite(sleep1,HIGH);
   digitalWrite(sleep2,HIGH);
    
   //setear las direcciones
   digitalWrite(dirs0,LOW);
   digitalWrite(dirs1,LOW);
   digitalWrite(dirs2,HIGH);
    
   //poner los steps en LOW
   digitalWrite(steps0,LOW);
   digitalWrite(steps1,LOW);
   digitalWrite(steps2,LOW);
   delayMicroseconds(2);    

   //marron
   //k = 0;
     
   //hacerlo para un solo motor primero
   while(nota0 < i || nota1 < j || nota2 < k)
   {         
      if(nota0 < i)
      {
         if(status0 == FETCH_NOTE)
         {
            if(0 != notas0[nota0].pitch)
            {
               pulses0 = freq[notas0[nota0].pitch] * (notas0[nota0].duration  - 15) / 1000;
               pause0 = 1000000 / freq[notas0[nota0].pitch] * ADJUSTMENT;
               current_pulse0 = 0;                  
               current_pause0 += pause0;
               status0 = PLAY_NOTE;               
            }
            
            else
            {
               current_pause0 = (long int)notas0[nota0].duration * 1000;
               status0 = PLAY_SILENCE;
            } 
             
         }
         
         if(status0 == PLAY_NOTE)
         {
            if(current_pulse0 >= pulses0)//already played last pulse
            {
               //now wait 15 ms
               current_pause0+= 15000;
               status0 = PLAY_SILENCE;
            }
            else
            {
               if(current_pause0 <= 0)//done waiting for next pulse
               {
                  //play a pulse
                  digitalWrite(steps0,HIGH);
                  delayMicroseconds(2);
                  digitalWrite(steps0,LOW);
                  //reset the pause, and set next pulse
                  current_pause0 += pause0;
                  current_pulse0++;
                  //adjust the others
                  current_pause1-=2;
                  current_pause2-=2;
               }
               current_pause0-=MICRO_DELAY;//we wait for the delay
            }  
         }
         if(PLAY_SILENCE == status0)
         {
            if(current_pause0 <= 0)//done waiting
            {
               status0 = FETCH_NOTE;
               nota0++;
            }
            current_pause0 -= MICRO_DELAY;
         }
      }

      if(nota1 < j)
      {
         if(status1 == FETCH_NOTE)
         {
            if( 0 != notas1[nota1].pitch)
            {
               pulses1 = freq[notas1[nota1].pitch] * (notas1[nota1].duration  - 15) / 1000;
               pause1 = 1000000 / freq[notas1[nota1].pitch] * ADJUSTMENT;
               current_pulse1 = 0;                  
               current_pause1 += pause1;
               status1 = PLAY_NOTE;
            }
            else
            {
               current_pause1 = (long int)notas1[nota1].duration * 1000;
               status1 = PLAY_SILENCE;
            }   
         }
         if(status1 == PLAY_NOTE)
         {
            if(current_pulse1 >= pulses1)//already played last pulse
            {
               //now wait 15 ms
               current_pause1+= 15000;
               status1 = PLAY_SILENCE;
            }
            else
            {
               if(current_pause1 <= 0)//done waiting for next pulse
               {
                  //play a pulse                  
                  digitalWrite(steps1,HIGH);
                  delayMicroseconds(2);
                  digitalWrite(steps1,LOW);
                  //reset the pause, and set next pulse
                  current_pause1+=pause1;
                  current_pulse1++;
                  current_pause0-=2;
                  current_pause2-=2;
               }
               current_pause1-=MICRO_DELAY;//we wait 
            }  
         }
         if(PLAY_SILENCE == status1)
         {
            if(current_pause1 <= 0)//done waiting
            {
               status1 = FETCH_NOTE;
               nota1++;
            }
            else
            {
               current_pause1 -= MICRO_DELAY;
            }
         }
      }

      if(nota2 < k)
      {
         if(status2 == FETCH_NOTE)
         {
            if( 0 != notas2[nota2].pitch)
            {
               pulses2 = freq[notas2[nota2].pitch] * (notas2[nota2].duration  - 15) / 1000;
               pause2 = 1000000 / freq[notas2[nota2].pitch] * ADJUSTMENT;
               current_pulse2 = 0;                  
               current_pause2 += pause2;
               status2 = PLAY_NOTE;               
            }
            
            else
            {
               current_pause2 = (long int)notas2[nota2].duration * 1000;
               status2 = PLAY_SILENCE;
            } 
         }
         
         if(status2 == PLAY_NOTE)
         {
            if(current_pulse2 >= pulses2)//already played last pulse
            {
               //now wait 15 ms
               current_pause2+= 15000;
               status2 = PLAY_SILENCE;
            }
            else
            {
               if(current_pause2 <= 0)//done waiting for next pulse
               {
                  //play a pulse
                  digitalWrite(steps2,HIGH);
                  delayMicroseconds(2);
                  digitalWrite(steps2,LOW);
                  //reset the pause, and set next pulse
                  current_pause2 += pause2;
                  current_pulse2++;
                  //adjust the others
                  current_pause0-=2;
                  current_pause1-=2;
               }
               current_pause2-=MICRO_DELAY;//we wait for the delay
            }  
         }
         if(PLAY_SILENCE == status2)
         {
            if(current_pause2 <= 0)//done waiting
            {
               status2 = FETCH_NOTE;
               nota2++;
            }
            current_pause2 -= MICRO_DELAY;
         }
      }
      delayMicroseconds(MICRO_DELAY); 
   }
   //apagar los motores
   digitalWrite(sleep0,LOW);//sleep
   digitalWrite(sleep1,LOW);
   digitalWrite(sleep2,LOW);
   i = 0;
   j = 0;
   k = 0;
}
