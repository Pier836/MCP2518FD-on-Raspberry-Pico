/*******************************************************************************
  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
  developped by Pierpaolo Scorrano of SG Electronic Systems srls

  www.sg-electronic-systems.com

  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/SoftwareLibraries/Firmware/MCP251xFDcanfdspiAPIforSAMV71_v1_1.zip
 *******************************************************************************/

/*******************************************************************************
  Application:  Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    Implementation of application.

  Description:
    .
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and 
any derivatives exclusively with Microchip products. It is your responsibility 
to comply with third party license terms applicable to your use of third party 
software (including open source software) that may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, 
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES 
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER 
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF 
THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED 
BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
//DOM-IGNORE-END


// Include files
#include "app.h"
#include "drv_canfdspi_api.h"

#define APP_N_LED LED_COUNT

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

APP_DATA appData;

#ifdef ENABLE_CAN1
CAN_CONFIG config_can1;
CAN_OPERATION_MODE opMode_can1;

// Transmit objects
CAN_TX_FIFO_CONFIG txConfig_can1;
CAN_TX_FIFO_EVENT txFlags_can1;
CAN_TX_MSGOBJ txObj_can1;
uint8_t txd_can1[MAX_DATA_BYTES];

// Receive objects
CAN_RX_FIFO_CONFIG rxConfig_can1;
REG_CiFLTOBJ fObj_can1;
REG_CiMASK mObj_can1;
CAN_RX_FIFO_EVENT rxFlags_can1;
CAN_RX_MSGOBJ rxObj_can1;
uint8_t rxd_can1[MAX_DATA_BYTES];
CAN_BUS_DIAGNOSTIC busDiagnostics_can1;
uint8_t tec_can1;
uint8_t rec_can1;
CAN_ERROR_STATE errorFlags_can1;
#endif //ENABLE_CAN1

#ifdef ENABLE_CAN2
CAN_CONFIG config_can2;
CAN_OPERATION_MODE opMode_can2;

// Transmit objects
CAN_TX_FIFO_CONFIG txConfig_can2;
CAN_TX_FIFO_EVENT txFlags_can2;
CAN_TX_MSGOBJ txObj_can2;
uint8_t txd_can2[MAX_DATA_BYTES];

// Receive objects
CAN_RX_FIFO_CONFIG rxConfig_can2;
REG_CiFLTOBJ fObj_can2;
REG_CiMASK mObj_can2;
CAN_RX_FIFO_EVENT rxFlags_can2;
CAN_RX_MSGOBJ rxObj_can2;
uint8_t rxd_can2[MAX_DATA_BYTES];
CAN_BUS_DIAGNOSTIC busDiagnostics_can2;
uint8_t tec_can2;
uint8_t rec_can2;
CAN_ERROR_STATE errorFlags_can2;
#endif //ENABLE_CAN2

uint32_t delayCount = APP_LED_TIME;

REG_t reg;

APP_SwitchState lastSwitchState;

APP_Payload payload;

uint8_t ledCount = 0, ledState = 0;

uint8_t i;




// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void APP_LED_Clear(uint8_t led)
{
    switch (led) {
        case 0: led1_off();
            break;
        case 1: led2_off();
            break;
        default: break;
    }
}

void APP_LED_Set(uint8_t led)
{
    switch (led) {
        case 0: led1_on();
            break;
        case 1: led2_on();
            break;
        default: break;
    }
}

void APP_LED_Write(uint8_t led)
{
    uint8_t mask, pin;

    mask = 1;
    pin = 0;
    Nop();

    for (i = 0; i < APP_N_LED; i++, pin++) {
        if (led & mask) {
            // Set LED
            APP_LED_Set(pin);
        } else {
            // Clear LED
            APP_LED_Clear(pin);
        }

        mask = mask << 1;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void APP_Initialize(void)
{
    set_spi_pins();
    DRV_SPI_Initialize();

    //led_init();

    // Switch state
    lastSwitchState.S1 = APP_SWITCH_RELEASED;
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    //irq_add_shared_handler(IO_IRQ_BANK0, DRV_CANFDSPI_ReceiveChannelEventGet, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);

}

void APP_Tasks(void)
{

    /* Check the application's current state. */
    switch (appData.state) {
            /* Application's initial state. */
        case APP_STATE_INIT:
        {
            Nop();
            APP_LED_Set(APP_INIT_LED);

            APP_CANFDSPI_Init();

            APP_LED_Clear(APP_INIT_LED);

            appData.state = APP_STATE_INIT_TXOBJ;
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_INIT\n");
#endif            
            break;
        }

            /* Initialize TX Object */
        case APP_STATE_INIT_TXOBJ:
        {
#ifdef ENABLE_CAN1				
            // Configure transmit message
            txObj_can1.word[0] = 0;
            txObj_can1.word[1] = 0;

            txObj_can1.bF.id.SID = TX_RESPONSE_ID;
            txObj_can1.bF.id.EID = 0;

            txObj_can1.bF.ctrl.BRS = 1;
            txObj_can1.bF.ctrl.DLC = CAN_DLC_64;
            txObj_can1.bF.ctrl.FDF = 1;
            txObj_can1.bF.ctrl.IDE = 0;

            // Configure message data
            for (i = 0; i < MAX_DATA_BYTES; i++) txd_can1[i] = txObj_can1.bF.id.SID + i;
#endif //ENABLE_CAN1

#ifdef ENABLE_CAN2				
            // Configure transmit message
            txObj_can2.word[0] = 0;
            txObj_can2.word[1] = 0;

            txObj_can2.bF.id.SID = TX_RESPONSE_ID;
            txObj_can2.bF.id.EID = 0;

            txObj_can2.bF.ctrl.BRS = 1;
            txObj_can2.bF.ctrl.DLC = CAN_DLC_64;
            txObj_can2.bF.ctrl.FDF = 1;
            txObj_can2.bF.ctrl.IDE = 0;

            // Configure message data
            for (i = 0; i < MAX_DATA_BYTES; i++) txd_can2[i] = txObj_can2.bF.id.SID + i;
#endif // ENABLE_CAN2
            appData.state = APP_STATE_FLASH_LEDS;
            
            
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_INIT_TXOBJ\n");
#endif 

            break;
        }

            /* Flash all LEDs */
        case APP_STATE_FLASH_LEDS:
        {
            // Delay loop
            Nop();
            Nop();

            // Delay expired, update LEDs and reset delayCount
            if (delayCount == 0) {
                if (ledCount < APP_N_LED) {
                    ledState |= 1 << ledCount;
                } else {
                    ledState = 0;
                }
                Nop();
                APP_LED_Write(ledState);

                ledCount++;
                delayCount = APP_LED_TIME;
            } else {
                delayCount--;
            }

            if (ledCount > (APP_N_LED + 1)) {
                appData.state = APP_STATE_RECEIVE;
            } else {
                appData.state = APP_STATE_FLASH_LEDS;
            }
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_FLASH_LEDS\n");
#endif             

            break;
        }

            /* Receive a message */
        case APP_STATE_RECEIVE:
        {
            Nop();
            Nop();
            appData.state = APP_ReceiveMessage_Tasks();

            appData.state = APP_STATE_TRANSMIT;
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_RECEIVE\n");
#endif               
            break;
        }

/*        case APP_STATE_PAYLOAD:
        {
            Nop();
            Nop();
            APP_PayLoad_Tasks();

            appData.state = APP_STATE_SWITCH_CHANGED;
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_PAYLOAD\n");
#endif             
            break;
        }

        case APP_STATE_SWITCH_CHANGED:
        {
            APP_TransmitSwitchState();
            appData.state = APP_STATE_RECEIVE;
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_SWITCH_CHANGED\n");
#endif  
            break;
        }*/
        
        case APP_STATE_TRANSMIT:
        {
            APP_TransmitMessageQueue();
            appData.state = APP_STATE_RECEIVE;
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_TRANSMIT\n");
#endif  
            break;
        }

            /* RAM access test */
        case APP_STATE_TEST_RAM_ACCESS:
        {
            bool passed = APP_TestRamAccess();

            if (!passed) {
                Nop();
            }

            appData.state = APP_STATE_TEST_REGISTER_ACCESS;
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_TEST_RAM_ACCESS\n");
#endif
            break;
        }

            /* Register access test */
        case APP_STATE_TEST_REGISTER_ACCESS:
        {
            bool passed = APP_TestRamAccess();

            if (!passed) {
                Nop();
            }

            appData.state = APP_STATE_TEST_RAM_ACCESS;
#ifdef STATE_MACHINE_DEBUG            
            printf("APP_STATE_TEST_REGISTER_ACCESS\n");
#endif
            break;
        }


            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            Nop();
            Nop();
            appData.state = APP_STATE_INIT;
#ifdef STATE_MACHINE_DEBUG            
            printf("IDLE\n");
#endif
            break;
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void APP_CANFDSPI_Init(void)
{
#ifdef ENABLE_CAN1	
    // Reset device
    DRV_CANFDSPI_Reset(CAN1);

    // Enable ECC and initialize RAM
    DRV_CANFDSPI_EccEnable(CAN1);

    DRV_CANFDSPI_RamInit(CAN1, 0xff);

    // Configure device
    DRV_CANFDSPI_ConfigureObjectReset(&config_can1);
    config_can1.IsoCrcEnable = 1;
    config_can1.StoreInTEF = 0;

    DRV_CANFDSPI_Configure(CAN1, &config_can1);

    // Setup TX FIFO
    DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig_can1);
    txConfig_can1.FifoSize = 7;
    txConfig_can1.PayLoadSize = CAN_PLSIZE_64;
    txConfig_can1.TxPriority = 1;

    DRV_CANFDSPI_TransmitChannelConfigure(CAN1, APP_TX_FIFO, &txConfig_can1);

    // Setup RX FIFO
    DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig_can1);
    rxConfig_can1.FifoSize = 15;
    rxConfig_can1.PayLoadSize = CAN_PLSIZE_64;

    DRV_CANFDSPI_ReceiveChannelConfigure(CAN1, APP_RX_FIFO, &rxConfig_can1);

    // Setup RX Filter
    fObj_can1.word = 0;
    fObj_can1.bF.SID = 0xda;
    fObj_can1.bF.EXIDE = 0;
    fObj_can1.bF.EID = 0x00;

    DRV_CANFDSPI_FilterObjectConfigure(CAN1, CAN_FILTER0, &fObj_can1.bF);

    // Setup RX Mask
    mObj_can1.word = 0;
    mObj_can1.bF.MSID = 0x0;
    mObj_can1.bF.MIDE = 1; // Only allow standard IDs
    mObj_can1.bF.MEID = 0x0;
    DRV_CANFDSPI_FilterMaskConfigure(CAN1, CAN_FILTER0, &mObj_can1.bF);

    // Link FIFO and Filter
    DRV_CANFDSPI_FilterToFifoLink(CAN1, CAN_FILTER0, APP_RX_FIFO, true);

    // Setup Bit Time
    DRV_CANFDSPI_BitTimeConfigure(CAN1, CAN_500K_5M, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);

    // Setup Transmit and Receive Interrupts
    DRV_CANFDSPI_GpioModeConfigure(CAN1, GPIO_MODE_INT, GPIO_MODE_INT);
	#ifdef APP_USE_TX_INT
    DRV_CANFDSPI_TransmitChannelEventEnable(CAN1, APP_TX_FIFO, CAN_TX_FIFO_NOT_FULL_EVENT);
	#endif
    DRV_CANFDSPI_ReceiveChannelEventEnable(CAN1, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
    DRV_CANFDSPI_ModuleEventEnable(CAN1, CAN_TX_EVENT | CAN_RX_EVENT);

    // Select Normal Mode
    DRV_CANFDSPI_OperationModeSelect(CAN1, CAN_NORMAL_MODE);
#endif // ENABLE_CAN1	    

#ifdef ENABLE_CAN2	
    // Reset device
    DRV_CANFDSPI_Reset(CAN2);

    // Enable ECC and initialize RAM
    DRV_CANFDSPI_EccEnable(CAN2);

    DRV_CANFDSPI_RamInit(CAN2, 0xff);

    // Configure device
    DRV_CANFDSPI_ConfigureObjectReset(&config_can2);
    config_can2.IsoCrcEnable = 1;
    config_can2.StoreInTEF = 0;

    DRV_CANFDSPI_Configure(CAN2, &config_can2);

    // Setup TX FIFO
    DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig_can2);
    txConfig_can2.FifoSize = 7;
    txConfig_can2.PayLoadSize = CAN_PLSIZE_64;
    txConfig_can2.TxPriority = 1;

    DRV_CANFDSPI_TransmitChannelConfigure(CAN2, APP_TX_FIFO, &txConfig_can2);

    // Setup RX FIFO
    DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig_can2);
    rxConfig_can2.FifoSize = 15;
    rxConfig_can2.PayLoadSize = CAN_PLSIZE_64;

    DRV_CANFDSPI_ReceiveChannelConfigure(CAN2, APP_RX_FIFO, &rxConfig_can2);

    // Setup RX Filter
    fObj_can2.word = 0;
    fObj_can2.bF.SID = 0xda;
    fObj_can2.bF.EXIDE = 0;
    fObj_can2.bF.EID = 0x00;

    DRV_CANFDSPI_FilterObjectConfigure(CAN2, CAN_FILTER0, &fObj_can2.bF);

    // Setup RX Mask
    mObj_can2.word = 0;
    mObj_can2.bF.MSID = 0x0;
    mObj_can2.bF.MIDE = 1; // Only allow standard IDs
    mObj_can2.bF.MEID = 0x0;
    DRV_CANFDSPI_FilterMaskConfigure(CAN2, CAN_FILTER0, &mObj_can2.bF);

    // Link FIFO and Filter
    DRV_CANFDSPI_FilterToFifoLink(CAN2, CAN_FILTER0, APP_RX_FIFO, true);

    // Setup Bit Time
    DRV_CANFDSPI_BitTimeConfigure(CAN2, CAN_500K_5M, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);

    // Setup Transmit and Receive Interrupts
    DRV_CANFDSPI_GpioModeConfigure(CAN2, GPIO_MODE_INT, GPIO_MODE_INT);
	#ifdef APP_USE_TX_INT
    DRV_CANFDSPI_TransmitChannelEventEnable(CAN2, APP_TX_FIFO, CAN_TX_FIFO_NOT_FULL_EVENT);
	#endif
    DRV_CANFDSPI_ReceiveChannelEventEnable(CAN2, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
    DRV_CANFDSPI_ModuleEventEnable(CAN2, CAN_TX_EVENT | CAN_RX_EVENT);

    // Select Normal Mode
    DRV_CANFDSPI_OperationModeSelect(CAN2, CAN_NORMAL_MODE);
#endif // ENABLE_CAN2	
    
}

APP_STATES APP_ReceiveMessage_Tasks(void)
{
    APP_STATES nextState;

    // Normally we go to APP_STATE_TRANSMIT
    Nop();
    Nop();
    nextState = APP_STATE_TRANSMIT;
#ifdef ENABLE_CAN1
    // Check if FIFO is not empty
#ifdef APP_USE_RX_INT
    if (APP_RX_INT()) {

#else
    DRV_CANFDSPI_ReceiveChannelEventGet(CAN1, APP_RX_FIFO, &rxFlags_can1);

	if (rxFlags_can1 & CAN_RX_FIFO_NOT_EMPTY_EVENT) 
	{
#endif
		APP_LED_Set(APP_RX_LED);

		// Get message
		DRV_CANFDSPI_ReceiveMessageGet(CAN1, APP_RX_FIFO, &rxObj_can1, rxd_can1, MAX_DATA_BYTES);

		printf("CAN1 ID %02x ", rxObj_can1.bF.id.SID);
		printf("DLC %02x Data ", rxObj_can1.bF.ctrl.DLC);
		 for (i = 0; i < 8; i++) 
		 {
			printf("%02x ", rxd_can1[i]);
		 }
		 printf("\n");
	}

        APP_LED_Clear(APP_RX_LED);
#endif // ENABLE_CAN1	

#ifdef ENABLE_CAN2				
    // Check if FIFO is not empty
#ifdef APP_USE_RX_INT
    if (APP_RX_INT()) {

#else
    DRV_CANFDSPI_ReceiveChannelEventGet(CAN2, APP_RX_FIFO, &rxFlags_can2);

	if (rxFlags_can2 & CAN_RX_FIFO_NOT_EMPTY_EVENT) 
	{
#endif
		APP_LED_Set(APP_RX_LED);

		// Get message
		DRV_CANFDSPI_ReceiveMessageGet(CAN2, APP_RX_FIFO, &rxObj_can2, rxd_can2, MAX_DATA_BYTES);

		printf("CAN2 ID %02x ", rxObj_can2.bF.id.SID);
		printf("DLC %02x Data ", rxObj_can2.bF.ctrl.DLC);
		 for (i = 0; i < 8; i++) 
		 {
			printf("%02x ", rxd_can2[i]);
		 }
		 printf("\n");
	}

        APP_LED_Clear(APP_RX_LED);
#endif // ENABLE_CAN2	

    return nextState;
}

void APP_TransmitMessageQueue(void)
{
    APP_LED_Set(APP_TX_LED);

#ifdef ENABLE_CAN1
    uint8_t attempts_can1 = MAX_TXQUEUE_ATTEMPTS;

    // Check if FIFO is not full
    do {
        DRV_CANFDSPI_TransmitChannelEventGet(CAN1, APP_TX_FIFO, &txFlags_can1);
        if (attempts_can1 == 0) {
            Nop();
            Nop();
            DRV_CANFDSPI_ErrorCountStateGet(CAN1, &tec_can1, &rec_can1, &errorFlags_can1);
            return;
        }
        attempts_can1--;
    }
    while (!(txFlags_can1 & CAN_TX_FIFO_NOT_FULL_EVENT));

    // Load message and transmit
    uint8_t n_can1 = DRV_CANFDSPI_DlcToDataBytes(txObj_can1.bF.ctrl.DLC);

    DRV_CANFDSPI_TransmitChannelLoad(CAN1, APP_TX_FIFO, &txObj_can1, txd_can1, n_can1, true);
#endif //ENABLE_CAN1

#ifdef ENABLE_CAN2
    uint8_t attempts_can2 = MAX_TXQUEUE_ATTEMPTS;

    // Check if FIFO is not full
    do {
        DRV_CANFDSPI_TransmitChannelEventGet(CAN2, APP_TX_FIFO, &txFlags_can2);
        if (attempts_can2 == 0) {
            Nop();
            Nop();
            DRV_CANFDSPI_ErrorCountStateGet(CAN2, &tec_can2, &rec_can2, &errorFlags_can2);
            return;
        }
        attempts_can2--;
    }
    while (!(txFlags_can2 & CAN_TX_FIFO_NOT_FULL_EVENT));

    // Load message and transmit
    uint8_t n_can2 = DRV_CANFDSPI_DlcToDataBytes(txObj_can2.bF.ctrl.DLC);

    DRV_CANFDSPI_TransmitChannelLoad(CAN2, APP_TX_FIFO, &txObj_can2, txd_can2, n_can2, true);
#endif //ENABLE_CAN2
    APP_LED_Clear(APP_TX_LED);
}

/*void APP_TransmitSwitchState(void)
{
    APP_SwitchState newSwitchState;

    // Check if switch has changed
    Nop();
    //Pier dovrebbe essere la gestione dell'interrupt
   // newSwitchState.S1 = APP_S1_READ();

    bool switchChanged = newSwitchState.S1 != lastSwitchState.S1;

    if (switchChanged) {
        // Transmit new state
        txObj.bF.id.SID = BUTTON_STATUS_ID;

        txObj.bF.ctrl.DLC = CAN_DLC_1;
        txObj.bF.ctrl.IDE = 0;
        txObj.bF.ctrl.BRS = 1;
        txObj.bF.ctrl.FDF = 1;

        txd[0] = 0;
        if (newSwitchState.S1 == APP_SWITCH_PRESSED) txd[0] += 0x1;

        APP_TransmitMessageQueue();
    }

    lastSwitchState.S1 = newSwitchState.S1;
}

void APP_PayLoad_Tasks(void)
{
    uint8_t n;

    // Send payload?
    if (payload.On) {
        // Delay transmission
        if (delayCount == 0) {
            delayCount = payload.Delay;

            // Prepare data
            Nop();
            Nop();
            txObj.bF.id.SID = TX_RESPONSE_ID;

            txObj.bF.ctrl.DLC = payload.Dlc;
            txObj.bF.ctrl.IDE = 0;
            txObj.bF.ctrl.BRS = payload.BRS;
            txObj.bF.ctrl.FDF = 1;

            n = DRV_CANFDSPI_DlcToDataBytes((CAN_DLC) payload.Dlc);

            if (payload.Mode) {
                // Random data
                for (i = 0; i < n; i++) txd[i] = rand() & 0xff;
            } else {
                // Counter
                for (i = 0; i < n; i++) txd[i] = payload.Counter;
                payload.Counter++;
            }

            APP_TransmitMessageQueue();
        } else {
            delayCount--;
        }
    } else {
        delayCount = 0;
    }
}*/

uint8_t APP_TestRegisterAccess(void)
{
    // Variables
    uint8_t length_can1;
    uint8_t length_can2;
    uint8_t status = 0;
    bool good = false;

    Nop();

    // Verify read/write with different access length
    // Note: registers can be accessed in multiples of bytes
#ifdef ENABLE_CAN1    
    for (length_can1 = 1; length_can1 <= MAX_DATA_BYTES; length_can1++) {
        for (i = 0; i < length_can1; i++) {
            txd_can1[i] = rand() & 0x7f; // Bit 31 of Filter objects is not implemented
            rxd_can1[i] = 0xff;
        }

        Nop();

        // Write data to registers
        DRV_CANFDSPI_WriteByteArray(CAN1, cREGADDR_CiFLTOBJ, txd_can1, length_can1);

        // Read data back from registers
        DRV_CANFDSPI_ReadByteArray(CAN1, cREGADDR_CiFLTOBJ, rxd_can1, length_can1);

        // Verify ???
        good = false;
        for (i = 0; i < length_can1; i++) {
            good = txd_can1[i] == rxd_can1[i];

            if (!good) {
                Nop();
                Nop();

                // Data mismatch
                status = 1;
            }
        }
    }
#endif //ENABLE_CAN1    

#ifdef ENABLE_CAN2    
    for (length_can2 = 2; length_can2 <= MAX_DATA_BYTES; length_can2++) {
        for (i = 0; i < length_can2; i++) {
            txd_can2[i] = rand() & 0x7f; // Bit 32 of Filter objects is not implemented
            rxd_can2[i] = 0xff;
        }

        Nop();

        // Write data to registers
        DRV_CANFDSPI_WriteByteArray(CAN2, cREGADDR_CiFLTOBJ, txd_can2, length_can2);

        // Read data back from registers
        DRV_CANFDSPI_ReadByteArray(CAN2, cREGADDR_CiFLTOBJ, rxd_can2, length_can2);

        // Verify
        good = false;
        for (i = 0; i < length_can2; i++) {
            good = txd_can2[i] == rxd_can2[i];

            if (!good) {
                Nop();
                Nop();

                // Data mismatch
                status += 2;
            }
        }
    }
#endif //ENABLE_CAN2  

    Nop();
    Nop();

    return status;
}

//! Test RAM access

uint8_t APP_TestRamAccess(void)
{
    // Variables
    uint8_t length_can1;
    uint8_t length_can2;
    uint8_t status = 0;
    bool good = false;

    Nop();

    // Verify read/write with different access length
    // Note: RAM can only be accessed in multiples of 4 bytes
#ifdef ENABLE_CAN1    
    for (length_can1 = 4; length_can1 <= MAX_DATA_BYTES; length_can1 +=4) {
        for (i = 0; i < length_can1; i++) {
            txd_can1[i] = rand() & 0xff; 
            rxd_can1[i] = 0xff;
        }    


        Nop();

        // Write data to RAM
        DRV_CANFDSPI_WriteByteArray(CAN1, cRAMADDR_START, txd_can1, length_can1);

        // Read data back from RAM
        DRV_CANFDSPI_ReadByteArray(CAN1, cRAMADDR_START, rxd_can1, length_can1);

        // Verify
        good = false;
        for (i = 0; i < length_can1; i++) {
            good = txd_can1[i] == rxd_can1[i];

            if (!good) {
                Nop();
                Nop();

               // Data mismatch
                status = 1;
            }
        }
    }
#endif //ENABLE_CAN1    


#ifdef ENABLE_CAN2    
    for (length_can2 = 4; length_can2 <= MAX_DATA_BYTES; length_can2 +=4) {
        for (i = 0; i < length_can2; i++) {
            txd_can2[i] = rand() & 0xff; 
            rxd_can2[i] = 0xff;
        }    


        Nop();

        // Write data to RAM
        DRV_CANFDSPI_WriteByteArray(CAN2, cRAMADDR_START, txd_can2, length_can2);

        // Read data back from RAM
        DRV_CANFDSPI_ReadByteArray(CAN2, cRAMADDR_START, rxd_can2, length_can2);

        // Verify
        good = false;
        for (i = 0; i < length_can2; i++) {
            good = txd_can2[i] == rxd_can2[i];

            if (!good) {
                Nop();
                Nop();

               // Data mismatch
                status += 2;
            }
        }
    }
#endif //ENABLE_CAN2  

    return status;
}
