/* 
 * File:   tanimlamalar.h
 * Author: ilyas
 *
 * Created on 22 Nisan 2020 Çarþamba, 12:49
 */

#ifndef TANIMLAMALAR_H
#define	TANIMLAMALAR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #define MAX_DUTY    800

    #define LED_PIN   PIN_A4

    #define pcb_led_yak() output_low(LED_PIN);
    #define pcb_led_son() output_high(LED_PIN);
    

    #define SOL_BTN_PIN     PIN_A5
    #define ORTA_BTN_PIN    PIN_A3
    #define SAG_BTN_PIN     PIN_A2

    #define GEC_LED_PIN     PIN_A6
    #define DUR_LED_PIN     PIN_A7

    #define SEVIYE_SENS1_PIN   PIN_E0
    #define SEVIYE_SENS2_PIN   PIN_E1
    #define SEVIYE_SENS3_PIN   PIN_E2

    #define SEVIYE_LED1_PIN     PIN_B2
    #define SEVIYE_LED2_PIN     PIN_B3
//    #define SEVIYE_LED3_PIN     PIN_B4
//    #define SEVIYE_LED4_PIN     PIN_B5
    #define GEC2_LED_PIN     PIN_B4
    #define DUR2_LED_PIN     PIN_B5

    #define GIR_SENS1_PIN   PIN_B0
    #define GIR_SENS2_PIN   PIN_B1
    
    #define ROLE_PIN        PIN_C0
    #define VALF_ROLE_PIN   PIN_C1  

    #define BUZZER_PIN  PIN_C5
    
    #define MOTOR_PIN   PIN_C2

    #define EE_ADR_SURE         0x00
    #define EE_ADR_KUL_SAY_BL   0x01
    #define EE_ADR_KUL_SAY_B2   0x02
    #define EE_ADR_KUL_SAY_B3   0x03
    #define EE_ADR_KUL_SAY_BH   0x04
    #define EE_ADR_BEKLEME_SURE 0x05
    #define EE_ADR_VALF_SURE    0x06
    #define EE_ADR_DIL_SEC      0x10

    

    #define GIR1_GIR2    1
    #define GIR2_GIR1    2
    #define GIRIS_YOK    0

    #define DEPO_BOS    10
    #define DEPO_MIN    11
    #define DEPO_ORTA   12
    #define DEPO_MAX    13

    #define DIL_TR      20
    #define DIL_FR      21
    #define DIL_ALM     22
    #define DIL_ING     23

    #define KUL_SAY_EE_KAYDET 'K'
    #define KUL_SAY_EE_OKU    'O'

    #define bip_bekle() bipp(10,10,BUZZER_PIN) //sure,sayi,pin
    #define bip_basla() bipp(50,2,BUZZER_PIN);  //sure,sayi,pin   
    #define bip_giris() bipp(100,5,BUZZER_PIN);  //sure,sayi,pin
    #define bip_hata() bipp(250,5,BUZZER_PIN);

    #define valfOn()  output_high(VALF_ROLE_PIN);   // basýncý boþaltýr Normalde kapalý valf 
    #define valfOff()  output_low(VALF_ROLE_PIN);   // basýncý tutar  Normalde kapalý valf 


#ifdef	__cplusplus
}
#endif

#endif	/* TANIMLAMALAR_H */

