
#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include <math.h>
#include <TcpClientRev2.h>
#include "Messages.h"

#include "Encoders.h"
#include "Motor.h"

#include "Utils.h"

//float interruptPeriod = 0.003125;
float interruptPeriod = 0.005f; // seconds

Encoders ShaftEncoders;
Motor    StepperMotor;

//***************************************************************************

// socket

#include <Secret.h>

TcpClientRev2 socket;
//EncoderCountsMessage message;

//***************************************************************************

void setup() 
{
    Serial.begin (9600);

    Serial.println (F("Motors and Encoders"));

    char obuf [100];
    char nbuf [20];

    ShaftEncoders.Setup ();
    StepperMotor.Setup ();
    SetupInterrupt ();

    /****/
     
     
  //******************************************************************
    int retryCounter = 5;
    bool success = false;

    Serial.print (F("Connecting to network"));
    
    do 
    {
        success = TcpClientRev2::ConnectToNetwork (SSID, PASS);
        if (success == false) Serial.print (F("."));
    
    } while ((success == false) && (retryCounter-- > 0));

    if (success)
        Serial.println (F(" succeeded"));
    else
    {
        Serial.println (F(" failed, stopping"));

        while (1)
        ;
    }
    
  //******************************************************************

    retryCounter = 5;
    
    Serial.print (F("Connecting to server"));
    
    do
    {
        success = socket.ConnectToServer (server, 11000);
        if (success == false) Serial.print (F("."));
    
    } while ((success == false) && (retryCounter-- > 0));

    if (success)
        Serial.println (F(" succeeded"));
    else
    {
        Serial.println (F(" failed, stopping"));

        while (1)
        ;
    }
}

//*************************************************************************************
//*************************************************************************************
//*************************************************************************************

static float currentTime = 0;
static bool tic = false;
static unsigned int ticCount = 0;

// buffer for received messages
#define MaxMessageBytes 64
char messageBytes [MaxMessageBytes];

unsigned int ExpectedSequenceNumber = 1;

bool first = true;

void loop() 
{
    if (first)
    {
        first = false;
        Serial.println ("loop");  
    }
    
    if (tic)
    {
        tic = false;
        StepperMotor.Loop (currentTime);
        
        if ((ticCount & 0xf) == 0)
          ShaftEncoders.Loop (socket);      
    }

  //  return;


    if (StepperMotor.Busy () == false)
    //if ((ticCount & 0xf) == 7)
    if (socket.IsConnected ())
    {
        if (socket.checkForMessages (messageBytes, sizeof (messageBytes)))
        {
          
         //   Serial.println (currentTime);
            
            MessageHeader *header = (MessageHeader *) messageBytes;

          //
          // check sequence number
          //
            if (ExpectedSequenceNumber != header->SequenceNumber)
            {
                char obuf [100];
                sprintf (obuf, "Sequence Error, expected %d, received %d", ExpectedSequenceNumber, header->SequenceNumber);
                Serial.println (obuf);
            }

            ExpectedSequenceNumber = header->SequenceNumber + 1;
            
            
            switch (header->MsgId)
            {
                case StartMotorMsgId: 
                {
                  //  char obuf [40];
                  //  sprintf (obuf, "StartMotor at %f", currentTime);
                  //  Serial.println (obuf);
                    
                    StepperMotor.Start (currentTime); 
                    break;
                }
                
           /**
                case StopMotorMsgId:  StepperMotor.Stop (); break;
                case SendStatusMsgId:    SendStatus (); break;                                   
                case StartSamplingMsgId: StartSampling ((StartSamplingMsg *) messageBytes); break;
                case StopSamplingMsgId:  StopSampling  ((StopSamplingMsg *)  messageBytes); break;    

                case ClearHistoryMsgId:  ClearHistory (); break;
                case SendHistoryMsgId:   SendHistory (); break;
                
                case DisconnectMsgId:    Disconnect (); break;
           **/
             
                default: //Serial.println ("Unsupported message received"); 
                     break;                    
            }   
        }
    }
}

//*******************************************************************************************

void SetupInterrupt ()
{
 // NOTE: i2c speed changed in this file
 // C:\Users\Randy\Documents\ArduinoV2\arduino-nightly\hardware\arduino\avr\libraries\Wire\utility\twi.h
 // per web post http://forum.arduino.cc/index.php?topic=16793.0
  
  
  
    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Use "Periodic Interrupt" mode

    // input clock rate (from TCA0) is 250kHz
    TCB0.CCMP = (int) (250000 * interruptPeriod);
    TCB0.INTCTRL = TCB_CAPT_bm; // Enable the interrupt
    TCB0.CTRLA = TCB_CLKSEL_CLKTCA_gc | TCB_ENABLE_bm; // Use Timer A as clock, enable timer
}

//*******************************************************************************************

ISR (TCB0_INT_vect)
{
    currentTime += interruptPeriod; // seconds  
    tic = true;
    ticCount++;
    
   // Clear interrupt flag
   TCB0.INTFLAGS = TCB_CAPT_bm;
}
