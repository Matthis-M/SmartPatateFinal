//****************************************************************************************
// PWM Générateur de signal carré
// VLO 2013-2014
//****************************************************************************************

int GetCas(); //Prototypes des différentes fonctions
void pushValue(int value);
int* CopyThenBubbleSort(int* toCopy);
void setFrequency(float frequency);


const int outPin = 9; // Code de la sortie
const float maxFreq = 16000000; //Fréquence maximale supportée par l'arduino (16MHz)

const int LED1 = 7;
const int LED2 = 6;
const int LED3 = 5;

const int width = 100; //nombre de valeur analysées

int* values;

void setup()
{

  Serial.begin(115200); //Pour envoyer les valeurs du pin A0
  
  pinMode(outPin,OUTPUT);        //Configure les pins en sorties
  pinMode(LED1,OUTPUT); 
  pinMode(LED2,OUTPUT); 
  pinMode(LED3,OUTPUT); 

  values = new int[width];
 
  
  setFrequency(350000); //génère un signal carré
  
  //consignes de test
  //Serial.println();
  //Serial.println(ICR1);
  //Serial.println(TCCR1B);

  digitalWrite(LED1,HIGH);
  delay (200);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,HIGH);
  delay (200);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,HIGH);
  delay (200);
  digitalWrite(LED3,LOW);
  delay (100);
  digitalWrite(LED3,HIGH);
  delay (200);
  digitalWrite(LED3,LOW);
  digitalWrite(LED2,HIGH);
  delay (200);
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,HIGH);
  delay (200);
  digitalWrite(LED1,LOW);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  delay (300);
}

void loop()
{


  int val = analogRead(0); 
  
  pushValue(val);  

  int cas = GetCas();

  switch(cas)
  {
    case 1: // 1 doigt
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    break;

    case 2: // 2 doigts
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,LOW);
    break;


    case 3: // main
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,HIGH);
    break;


    default:
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    break;
  }
  

                                 
  Serial.write( 0xff );                                                         
  Serial.write( (val >> 8) & 0xff );                                            
  Serial.write( val & 0xff );
 
}

//Règle la fréquence (Hz)
//Fréquence minimale: 0.24 Hz
//Fréquence maximale : 8MHz
void setFrequency(float frequency)
{
 
  if(frequency >=0.24 || frequency <=8000000)
  {
    
    TCCR1A=0b10000010;        // Règle la fréquence du générateur au mode 14 (PWM rapide)
    //WGM11 = 1, COM1A1 = 1
    
  
    unsigned int v=0;
    int indscale=0;
    float prescale[] = {1.0,8.0,64.0,256.0,1024.0};
    float period=1/frequency;
    
    while(v==0)
    {
      float curfreq=maxFreq/prescale[indscale];
      float tickperiod=1/curfreq;
      float nbtick=period/tickperiod;
     
      if(nbtick>65535)
      {
        indscale=indscale+1;
      }
      else
      {
        v=nbtick;
      }

    }
    
    int c=prescale[indscale];
    
     switch (c) {
      
         case 1: TCCR1B=0b00011001; break;
         case 8: TCCR1B=0b00011010; break;
         case 64: TCCR1B=0b00011011; break;
         case 256: TCCR1B=0b00011100; break;
         case 1024: TCCR1B=0b00011101; break;
         default: TCCR1B=0b00011000;
     }
    
    //WGM12 = 1, WGM13 = 1
    
    //trois dernier bit de TCCR1B:              CS12   CS11   CS10
    // 0: pas d'horloge (le décompte est stoppé)  0      0      0
    // horloge/1: aucun préréglages               0      0      1 
    // horloge/8                                  0      1      0
    // horloge/64                                 0      1      1
    // horloge/256                                1      0      0
    // horloge/1024                               1      0      1

    
    ICR1=v; //durée de la pulsation = ICR1 valeur x temps par tick du compteur.
    
    //Pour 16Mhz (Fréquence de la puce)
    //Préréglages     Temps par tick du compteur
    //1	                       0.0625 uS
    //8	                         0.5 uS
    //64	                        4 uS
    //256                       	16 uS
    //1024	                      64uS
    
    OCR1A=v/2; //Registre de comparaison en sortie //état faible

  }
  
  
}


int GetCas()
{
  int* trier = CopyThenBubbleSort(values); //tri du tableau
  int crete = trier[width-1] - trier[0];
  delete trier;

 if(crete < ((1.70 * 1023.0) / 5))
    if(crete < ((0.70 * 1023.0) / 5))
        if(crete < ((0.61 * 1023.0) / 5))
          return 3;
        else 
        return 2;
    else 
    return 1;
  else 
  return 0;

 
}


void pushValue(int value)
{
  for (int i=0; i<width-1; i++)
    values[i] = values[i+1];
  values[width-1] = value;
}


int* CopyThenBubbleSort(int* toCopy)
{
  int *tab = new int[width];
    for(int i = 0; i<width ; i++)
    {
       tab[i] = toCopy[i];
    }

  int temp;
  for(int i = width-1; i>0; i--)
  {
      for(int j = 0; j<=i-1; j++)
      {
         if(tab[j+1] < tab[j])
         {
          temp = tab[j];
          tab[j] = tab[j+1];
          tab[j+1] = temp;
         
          }
        
        }
  
  }

  return tab;
}






