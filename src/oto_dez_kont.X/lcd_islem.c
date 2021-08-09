#include "tanimlamalar.h"
#define MAX_MENU_DEGER  6
unsigned int16 duty = 0;


void motor_kontrol(int duty) {

}
 
void donanim_kontrol(void) {
   
    signed int btn = 0;
    while(1){
        
        switch (btn) {
            case 0:
                printf(lcd_putc, "\fROLE:");
                printf(lcd_putc, "\n<<<    <>    >>>");
                delay_ms(1000);
                bip_basla();
                do {
                    if (!input(SAG_BTN_PIN)){
                        delay_ms(250);
                       output_high((ROLE_PIN));
                    }
                    if (!input(SOL_BTN_PIN)) {
                        delay_ms(250);
                        output_low((ROLE_PIN));
                    }

                }while (input(ORTA_BTN_PIN));
                bip_basla();
                delay_ms(1000);

                btn = 1;
                break;
            case 1:
                printf(lcd_putc, "\fSEVIYE:");
                printf(lcd_putc, "\n<<<    <>    >>>");
                delay_ms(1000);
                bip_basla();
                do {
                     sens_adc_oku();
                      //if(input(SEVIYE_SENS1_PIN))
                    if(an_sv1>800)      //4 V gibi
                    {
                        depo_durum=DEPO_BOS;
                        output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
                    }
                    //if(!input(SEVIYE_SENS1_PIN))
                    if(an_sv1<700) //3.5 V
                    {
                        depo_durum=DEPO_MIN;
                        output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
                    }
                    //if(!input(SEVIYE_SENS1_PIN) && !input(SEVIYE_SENS2_PIN) )
                    if((an_sv1<700)&& (an_sv2<700))
                    {
                        depo_durum=DEPO_ORTA;
                        output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
                    }
                    //if(!input(SEVIYE_SENS1_PIN) && !input(SEVIYE_SENS2_PIN) && !input(SEVIYE_SENS3_PIN))
                    if((an_sv1<700)&& (an_sv2<700) && (an_sv3<700))
                    {
                        depo_durum=DEPO_MAX;
                        output_low(ROLE_PIN);       //Dalgýç pompanýn gerilimini kes
                    }
                     
                    lcd_msg_baslik=dilTxtGetir(dil_sec,1);
                    printf(lcd_putc,"\f%s",lcd_msg_baslik);

                    if(depo_durum==DEPO_BOS){
                       lcd_msg_depo=dilTxtGetir(dil_sec,2);
                       lcd_msg_depo_drm=dilTxtGetir(dil_sec,9);
                       printf(lcd_putc,"\n%s:%s",lcd_msg_depo,lcd_msg_depo_drm); 
                       delay_ms(1000);
                    }
                    if(depo_durum==DEPO_MIN){
                       lcd_msg_depo=dilTxtGetir(dil_sec,2); 
                       printf(lcd_putc,"\n%s:MIN-25",lcd_msg_depo); 
                    }
                    if(depo_durum==DEPO_ORTA){
                       lcd_msg_depo=dilTxtGetir(dil_sec,2); 
                       lcd_msg_depo_drm=dilTxtGetir(dil_sec,4);
                       printf(lcd_putc,"\n%s:%s-50",lcd_msg_depo,lcd_msg_depo_drm); 

                    }
                    if(depo_durum==DEPO_MAX){
                       lcd_msg_depo=dilTxtGetir(dil_sec,2); 
                       printf(lcd_putc,"\n%s:MAX-100",lcd_msg_depo); 

                    }
                    delay_ms(250);
                    
              } while (input(ORTA_BTN_PIN));
                bip_basla();
                delay_ms(1000);
                btn = 2;
                break;
            case 2:
                printf(lcd_putc, "\fDUR LED:");
                printf(lcd_putc, "\n<<<    <>    >>>");
                delay_ms(1000);
                bip_basla();
                do {
                    output_high((DUR_LED_PIN));
                    delay_ms(100);
                    output_low((DUR_LED_PIN));
                    delay_ms(100);
                } while (input(ORTA_BTN_PIN));
                bip_basla();
                delay_ms(1000);
                btn = 3;
                break;
            case 3:
                printf(lcd_putc, "\fGEC LED:");
                printf(lcd_putc, "\n<<<    <>    >>>");
                delay_ms(1000);
                bip_basla();
                do {
                    output_high((GEC_LED_PIN));
                    delay_ms(100);
                    output_low((GEC_LED_PIN));
                    delay_ms(100);
                } while (input(ORTA_BTN_PIN));
                bip_basla();
                delay_ms(1000);
                btn = 4;
                break;
            case 4:
                printf(lcd_putc, "\fGECIS SENS.:");
                printf(lcd_putc, "\n<<<    <>    >>>");
                delay_ms(1000);
                bip_basla();
                do {
                    if (!input(GIR_SENS1_PIN)){
                        printf(lcd_putc, "\fGECIS SENS.:");
                        printf(lcd_putc, "\nGEC 1");
                        delay_ms(250);
                        bip_hata();
                    }
                    if (!input(GIR_SENS2_PIN)){
                        printf(lcd_putc, "\fGECIS SENS.:");
                        printf(lcd_putc, "\nGEC 2");
                        delay_ms(250);
                        bip_hata();
                    }
                } while (input(ORTA_BTN_PIN));
                bip_basla();
                delay_ms(1000);
                btn = 5;
                break;
            case 5:
                printf(lcd_putc, "\fMOTOR:");
                printf(lcd_putc, "\n<<<    <>    >>>");
                delay_ms(1000);
                bip_basla();
                set_pwm1_duty(0);
                duty=0;
                do {
                   if (!input(SAG_BTN_PIN)) {
                        delay_ms(50);
                        duty += 1;
                        if (duty > 1000) duty = 1000;
                         printf(lcd_putc, "\f\nduty:%Lu   ", duty);
                    }

                    if (!input(SOL_BTN_PIN)) {
                        delay_ms(50);
                        duty -= 1;
                        if (duty < 10) duty = 10;
                        printf(lcd_putc, "\f\nduty:%Lu   ", duty);
                    }

                    set_pwm1_duty(duty);

                } while (input(ORTA_BTN_PIN));
                duty=0;
                bip_basla();
                delay_ms(1000);
                set_pwm1_duty(0);
                btn = 6;
                
                break;
        }
        
        if(btn==6)break;
    }
    

}
void sureAyar(void){
    char *sure_str;
    int sn=read_eeprom(EE_ADR_SURE);
    if(sn>100) sn=1;
    sure_str=dilTxtGetir(dil_sec,3);
    printf(lcd_putc,"\f%s=%u S\n",sure_str,sn);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\f%s=%u S\n",sure_str,sn);
        printf(lcd_putc,"\n<<<    <>    >>>");
        if(!input(SOL_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            sn--;
            if(sn<1) sn=1;
        }
        if(!input(SAG_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            sn++;
            if(sn>300) sn=60;
        }
        delay_ms(100);
    } while(input(ORTA_BTN_PIN));
     bip_basla();
     
     write_eeprom(EE_ADR_SURE,sn);
     dezen_sure=sn;
     sure_str=dilTxtGetir(dil_sec,5);
     printf(lcd_putc,"\f%s",sure_str);
     
     delay_ms(1000);
}

void beklemeSureAyar(void){
    char *sure_str;
    int sn=read_eeprom(EE_ADR_BEKLEME_SURE);
    if(sn>100) sn=1;
    sure_str=dilTxtGetir(dil_sec,3);
    printf(lcd_putc,"\f%s=%u S\n",sure_str,sn);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\f%s=%u S\n",sure_str,sn);
        printf(lcd_putc,"\n<<<    <>    >>>");
        if(!input(SOL_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            sn--;
            if(sn<1) sn=1;
        }
        if(!input(SAG_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            sn++;
            if(sn>300) sn=60;
        }
        delay_ms(100);
    } while(input(ORTA_BTN_PIN));
     bip_basla();
     
     write_eeprom( EE_ADR_BEKLEME_SURE,sn);
     bekle_sure=sn;
     sure_str=dilTxtGetir(dil_sec,5);
     printf(lcd_putc,"\f%s",sure_str);
     
     delay_ms(1000);
}

void valfSureAyar(void){
    char *sure_str;
    int sure=read_eeprom(EE_ADR_VALF_SURE);
    if(sure>250) sure=1;     //10*100=1 saniye
    sure_str=dilTxtGetir(dil_sec,3);
    printf(lcd_putc,"\f%s=%u S\n",sure_str,sure);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\f%s=%Lu mS\n",sure_str,(unsigned long)(sure*100L));
        printf(lcd_putc,"\n<<<    <>    >>>");
        if(!input(SOL_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            sure--;
            if(sure<1) sure=1;
        }
        if(!input(SAG_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            sure++;
            if(sure>100) sure=100;
        }
        delay_ms(100);
    } while(input(ORTA_BTN_PIN));
     bip_basla();
     
     write_eeprom( EE_ADR_VALF_SURE,sure);
     valf_sure=sure;
     sure_str=dilTxtGetir(dil_sec,5);
     printf(lcd_putc,"\f%s",sure_str);
     
     delay_ms(1000);
}

void dil_secim_islemi(void){
    
    int8 dil=read_eeprom(EE_ADR_DIL_SEC);
    char dil_deg=20;
    char *dil_str;
    if(dil>100) dil=DIL_TR;
    
    if(dil==DIL_TR) dil_str=splitStr(lcd_diller, ",", 1);
    if(dil==DIL_FR) dil_str=splitStr(lcd_diller, ",", 2);
    if(dil==DIL_ALM) dil_str=splitStr(lcd_diller, ",", 3);
    if(dil==DIL_ING) dil_str=splitStr(lcd_diller, ",", 4);
    
    printf(lcd_putc,"\f%s",dil_str);
    printf(lcd_putc,"\n<<<    <>    >>>");
    delay_ms(1000);
    
    do{
        lcd_gotoxy(1,1);
        printf(lcd_putc,"\f%s",dil_str);
        printf(lcd_putc,"\n<<<    <>    >>>");
        
        if(!input(SOL_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            dil_deg--;
            if(dil_deg<20) dil_deg=20;
            
        }
        if(!input(SAG_BTN_PIN)){
            delay_ms(100);
            bip_basla();
            dil_deg++;
            if(dil_deg>23) dil_deg=23;
           
        }
        
        if(dil_deg==DIL_TR) dil_str=splitStr(lcd_diller, ",", 1);
        if(dil_deg==DIL_FR) dil_str=splitStr(lcd_diller, ",", 2);
        if(dil_deg==DIL_ALM) dil_str=splitStr(lcd_diller, ",", 3);
        if(dil_deg==DIL_ING) dil_str=splitStr(lcd_diller, ",", 4);
        
        delay_ms(100);
    } while(input(ORTA_BTN_PIN));
     bip_basla();
     
     write_eeprom(EE_ADR_DIL_SEC,dil_deg);
     dil_str=dilTxtGetir(dil_deg,5);
     printf(lcd_putc,"\f!!%s!!",dil_str);
     dil_sec=dil_deg;
     delay_ms(1000);
}

void lcdMenuIslem(void) {
    
    signed int btn = 0;
    char *menu_str;
    printf(lcd_putc, "\f      MENU");        //Butun dillerde ayný
    printf(lcd_putc, "\n<<<    <>    >>>");
    delay_ms(1000);
    duty = 0;
    
    while (1) {

        if (!input(SAG_BTN_PIN)) {
            delay_ms(150);
            btn++;
            if (btn > MAX_MENU_DEGER)btn = 1;
            bip_basla();
        }

        if (!input(SOL_BTN_PIN)) {
            delay_ms(150);
            btn--;
            if (btn <= 0)btn = MAX_MENU_DEGER;
            bip_basla();
        }
        
        if(btn==1)      //Sure Ayarý
        {
             menu_str=dilTxtGetir(dil_sec,6);

             printf(lcd_putc,"\f %s",menu_str);
             printf(lcd_putc,"\n<<<    <>    >>>");
             
             if(!input(ORTA_BTN_PIN)){
                delay_ms(100);
                bip_basla();
                sureAyar();
                btn=2;
             }
        }
        
        if(btn==2)      //Bekleme Sure Ayarý
        {
             menu_str=dilTxtGetir(dil_sec,12);

             printf(lcd_putc,"\f %s",menu_str);
             printf(lcd_putc,"\n<<<    <>    >>>");
             
             if(!input(ORTA_BTN_PIN)){
                delay_ms(100);
                bip_basla();
                beklemeSureAyar();
                btn=3;
             }
        }
        
        if(btn==3)  //DONANIM KONT.
        {
            menu_str=dilTxtGetir(dil_sec,7);
            printf(lcd_putc,"\f %s",menu_str);
            printf(lcd_putc,"\n<<<    <>    >>>");
             
             if(!input(ORTA_BTN_PIN)){
                delay_ms(100);
                bip_basla();
                donanim_kontrol();
                btn=4;
             }
        }
        if(btn==4)//DIL SEC
        {
            menu_str=dilTxtGetir(dil_sec,8);
            printf(lcd_putc,"\f %s",menu_str);
            printf(lcd_putc,"\n<<<    <>    >>>");
             
            if(!input(ORTA_BTN_PIN)){
                delay_ms(100);
                bip_basla();
                dil_secim_islemi();
                btn=5;
            }
        }
        
        if(btn==5)  //VALf Sure
        {
            menu_str="VALF SURE";   //dilTxtGetir(dil_sec,12);

             printf(lcd_putc,"\f %s","VALF SURE");
             printf(lcd_putc,"\n<<<    <>    >>>");
             
             if(!input(ORTA_BTN_PIN)){
                delay_ms(100);
                bip_basla();
                valfSureAyar();
                btn=6;
             }
        }
        if(btn==6)  //CIKIS
        {
            menu_str=dilTxtGetir(dil_sec,9);
            printf(lcd_putc,"\f %s",menu_str);
            printf(lcd_putc,"\n<<<    <>    >>>");
            if(!input(ORTA_BTN_PIN)){
                delay_ms(250);
                btn=0;
                break;
            }
        }
        
        delay_ms(250);
    }//while
    
    lcd_msg_baslik=dilTxtGetir(dil_sec,1);
    //printf(lcd_putc,"\fOTO DEZENFEKTE");
    //printf(lcd_putc,"\nDepo : ORTA");
}
