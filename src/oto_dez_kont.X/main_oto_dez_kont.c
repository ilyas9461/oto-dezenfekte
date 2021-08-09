/* 
 * File:   main_oto_dez_kont.c
 * Author: ilyas
 *
 * Created on 22 Nisan 2020 Çarþamba, 12:10
 */

//#define BARIYER_SENS 
#define PIR_SENS_TEKLI

#include "main_oto_dez_kont.h"
//#include <stdio.h>
#include <C:\Program Files (x86)\PICC\Drivers\stdlib.h> //rand vb ,için  tam yolu yazmak dosyayý açamama hatasýný giderdi
#include "timer.c"
#include "timer.h"
#include "flex_lcd.c"
#include "tanimlamalar.h"

int menu_say = 0;
unsigned long long time_dezen_sure = 0;
unsigned long long time_valf_sure = 0;
unsigned long long time_bekle_sure = 0;
unsigned long long time_lcd = 0;

unsigned int32 kul_say = 0;
unsigned int8 dezen_sure = 0;
unsigned int8 valf_sure = 1; //15*100=1500mS
unsigned int8 bekle_sure = 0;
int8 giris_yon = 0;
int8 depo_durum = DEPO_BOS;

unsigned int16 an_sv1 = 0, an_sv2 = 0, an_sv3 = 0;

unsigned int8 dil_sec = 0;

char *lcd_msg_baslik;
char *lcd_msg_depo;
char *lcd_msg_depo_drm;
char *lcd_msg_sure;
char *lcd_msg_sayac;

void bipp(int sure, int sayi, long pin);
void sens_adc_oku(void);
char *splitStr(char *input_string, char *separator, int segment_number);
char *dilTxtGetir(char dil, char txt_sayi);
unsigned long kul_say_ee_islem(char islem, unsigned int32 sayi);

#include "lcd_islem.c"
#include "donanim_kont.c"

void main() {
    setup_oscillator(OSC_64MHZ);

    setup_dac(DAC_OFF);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);
    //setup_adc(ADC_OFF);

    setup_ccp2(CCP_OFF);
    setup_ccp3(CCP_OFF);
    setup_ccp4(CCP_OFF);
    setup_ccp5(CCP_OFF);

    setup_timer_1(T1_DISABLED);
    setup_timer_2(T2_DIV_BY_16, 199, 1); //5Khz max PWM deger=200 peryot kadar osilaskop sonucu 22.01.20 bu uygulamada olmadý
    // 17.01.2020 : Ugulamada  700 deðeri hesaplamaya göre görüldü 22.01.20 evet....
    // excell hesabýnda %50 duty deðeri 400 çýkýyor. 5KHz PWM 
    // **uygulamada ölçüldü max deger 800 22/04/20  

    //setup_ccp1(CCP_PWM);
    setup_ccp1(CCP_OFF);

    setup_timer_4(T4_DISABLED, 0, 1);
    setup_timer_5(T5_DISABLED);
    setup_timer_6(T6_DISABLED, 0, 1);


    setup_adc(ADC_CLOCK_INTERNAL); //  9 bit (max 511) okuyan adc bu þekilde 10 bit oldu.....
    setup_adc_ports(sAN5 | sAN6 | sAN7, VSS_VDD);
    //setup_adc_ports(AN0_TO_AN1);   //18f4520
    // setup_adc(ADC_CLOCK_DIV_8);

    //    set_tris_a(0b00101111);
    //    set_tris_b(0b00000011);
    //    set_tris_c(0b10000000);
    //    set_tris_d(0b10000000);
    //    set_tris_e(0b1111);

    timer_kur();

    set_pwm1_duty(0);

    lcd_init(); // Always call this first.
    printf(lcd_putc, "\f  ..LYS-2020..");
    printf(lcd_putc, "\n  __YOYUNCAK__");

    output_a(0x00);

    pcb_led_yak();
    delay_ms(1000);
    pcb_led_son();
    delay_ms(1000);

    dezen_sure = read_eeprom(EE_ADR_SURE); //mS
    //printf(lcd_putc,"\fdez:%u",dezen_sure);
    //delay_ms(1000);
    if (dezen_sure == 0xFF) dezen_sure = 5;

    bekle_sure = read_eeprom(EE_ADR_BEKLEME_SURE);
    if (bekle_sure == 0xFF) bekle_sure = 5;
    
    valf_sure=read_eeprom(EE_ADR_VALF_SURE);
    if(valf_sure==0xFF) valf_sure=1;   //10*100=1000 mS
    bip_basla();

    dil_sec = read_eeprom(EE_ADR_DIL_SEC);

    // printf(lcd_putc,"\fdil:%u",dil_sec);

    if (dil_sec == 0xFF || dil_sec == 0)
        dil_sec = DIL_TR;

    lcd_msg_baslik = dilTxtGetir(dil_sec, 1);
    lcd_msg_depo = dilTxtGetir(dil_sec, 2);
    lcd_msg_sure = dilTxtGetir(dil_sec, 3);

    kul_say = kul_say_ee_islem(KUL_SAY_EE_OKU, kul_say); //1999999LL;
    lcd_msg_sayac = dilTxtGetir(dil_sec, 11);

    printf(lcd_putc, "\f%s", lcd_msg_baslik);
    printf(lcd_putc, "\n%s:%Lu", lcd_msg_sayac, kul_say);
    delay_ms(1500);

    printf(lcd_putc, "\f%s", lcd_msg_baslik);
    printf(lcd_putc, "\n%s: ???", lcd_msg_depo);

    //delay_ms(1500);

    time_lcd = millis();

#ifdef PIR_SENS_TEKLI
    while ((millis() - time_lcd) < 30000LL) {
        printf(lcd_putc, "\f%s", lcd_msg_baslik);
        printf(lcd_putc, "\nSENS:%lu", millis());
        delay_ms(250);
        if (input(GIR_SENS1_PIN)) break;
    }
#endif
    
    output_high(VALF_ROLE_PIN);
    time_lcd = millis();
    
    /*      ANA DÖngü   */
    
    while (1) {

#ifdef BARIYER_SENS

        if (!input(GIR_SENS1_PIN) && giris_yon == GIRIS_YOK && depo_durum != DEPO_BOS) {
            delay_ms(1000);
            bip_giris();

            output_high(MOTOR_PIN);
            giris_yon = GIR1_GIR2;

            output_high(DUR_LED_PIN);
            output_high(DUR2_LED_PIN);
            output_low(GEC_LED_PIN);
            output_low(GEC2_LED_PIN);

            time_dezen_sure = millis();
        }

        if (!input(GIR_SENS2_PIN) && giris_yon == GIRIS_YOK && depo_durum != DEPO_BOS) {
            delay_ms(1000);
            bip_giris();
            output_high(MOTOR_PIN);
            giris_yon = GIR2_GIR1;

            output_high(DUR_LED_PIN);
            output_high(DUR2_LED_PIN);
            output_low(GEC_LED_PIN);
            output_low(GEC2_LED_PIN);

            time_dezen_sure = millis();
        }

        if (giris_yon == GIR1_GIR2) {

            if (!input(GIR_SENS2_PIN)) {
                delay_ms(500);
                output_low(MOTOR_PIN);
                giris_yon = GIRIS_YOK;

                output_high((GEC_LED_PIN));
                output_high((GEC2_LED_PIN));
                output_low(DUR_LED_PIN);
                output_low(DUR2_LED_PIN);

                time_dezen_sure = millis();
            }
        }

        if (giris_yon == GIR2_GIR1) {

            if (!input(GIR_SENS1_PIN)) {
                delay_ms(500);
                output_low(MOTOR_PIN);
                giris_yon = GIRIS_YOK;

                output_high((GEC_LED_PIN));
                output_high((GEC2_LED_PIN));
                output_low(DUR_LED_PIN);
                output_low(DUR2_LED_PIN);
                time_dezen_sure = millis();
            }
        }

        if (giris_yon != GIRIS_YOK) {
            printf(lcd_putc, "\fOTO DEZENFEKTE");
            printf(lcd_putc, "\nSURE:%Lu", (millis() - time_dezen_sure));

            if ((millis() - time_dezen_sure)>((unsigned long) (dezen_sure * 1000))) {
                output_low(MOTOR_PIN);
                giris_yon = GIRIS_YOK;

                output_high((GEC_LED_PIN));
                output_high((GEC2_LED_PIN));
                output_low(DUR_LED_PIN);
                output_low(DUR2_LED_PIN);

                setMillis(0);
                time_dezen_sure = millis();
            }

        } else {

            if (millis() - time_lcd > 1000) {
                printf(lcd_putc, "\fOTO DEZENFEKTE");

                if (depo_durum == DEPO_BOS) {
                    printf(lcd_putc, "\nDepo : BOS");
                    output_low(MOTOR_PIN);
                    giris_yon = GIRIS_YOK;
                    bip_hata();
                    delay_ms(1000);
                }
                if (depo_durum == DEPO_MIN) {
                    printf(lcd_putc, "\nDepo:MIN-25");
                }
                if (depo_durum == DEPO_ORTA) {
                    printf(lcd_putc, "\nDepo:ORTA-50");

                }
                if (depo_durum == DEPO_MAX) {
                    printf(lcd_putc, "\nDepo:MAX-100");

                }

                time_lcd = millis();
            }

        }

        if (depo_durum == DEPO_BOS) {
            output_low(MOTOR_PIN);
            giris_yon = GIRIS_YOK;
        }

#endif

#ifdef PIR_SENS_TEKLI

        if ((input(GIR_SENS1_PIN) || !input(GIR_SENS2_PIN)) && giris_yon == GIRIS_YOK && depo_durum != DEPO_BOS) {
            delay_ms(100);
            bip_giris();
            output_high(MOTOR_PIN);
            valfOff()
            giris_yon = GIR1_GIR2;
            time_dezen_sure = millis();
            
        }
        
        if(millis()-time_valf_sure>(unsigned long long)(valf_sure*100LL)){
            valfOff(); // basýnç boþaltma süesi bitti basýncý tut.
        }

        if (giris_yon != GIRIS_YOK) {
            lcd_msg_baslik = dilTxtGetir(dil_sec, 1);
            lcd_msg_sure = dilTxtGetir(dil_sec, 3);
            printf(lcd_putc, "\f%s", lcd_msg_baslik);
            printf(lcd_putc, "\n%s:%Lu", lcd_msg_sure, (millis() - time_dezen_sure));

            if (((millis() - time_dezen_sure)>((unsigned long long) (dezen_sure * 1000))) && giris_yon == GIR1_GIR2) {
                output_low(MOTOR_PIN);
                valfOn();   //dezenfekte bitti basýncý boþalt
                kul_say++;
                kul_say_ee_islem(KUL_SAY_EE_KAYDET, kul_say);

                bip_bekle();
                time_bekle_sure = millis();
                time_dezen_sure = millis();
                giris_yon = GIR2_GIR1;
                time_valf_sure = millis();
            }

            if ((millis() - time_bekle_sure > ((unsigned long long) (bekle_sure * 1000LL))) && giris_yon == GIR2_GIR1) {
                setMillis(0);
                giris_yon = GIRIS_YOK;
                time_dezen_sure = millis();
            }

        } else {

            if (millis() - time_lcd > 1000) {

                lcd_msg_baslik = dilTxtGetir(dil_sec, 1);
                printf(lcd_putc, "\f%s", lcd_msg_baslik);

                if (depo_durum == DEPO_BOS) {
                    lcd_msg_depo = dilTxtGetir(dil_sec, 2);
                    lcd_msg_depo_drm = dilTxtGetir(dil_sec, 10);
                    printf(lcd_putc, "\n%s:%s", lcd_msg_depo, lcd_msg_depo_drm);
                    output_low(MOTOR_PIN);
                    output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
                    giris_yon = GIRIS_YOK;
                    
                    bip_hata();
                    delay_ms(1000);
                }
                if (depo_durum == DEPO_MIN) {
                    lcd_msg_depo = dilTxtGetir(dil_sec, 2);
                    printf(lcd_putc, "\n%s:MIN-25", lcd_msg_depo);
                    output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
                }
                if (depo_durum == DEPO_ORTA) {
                    lcd_msg_depo = dilTxtGetir(dil_sec, 2);
                    lcd_msg_depo_drm = dilTxtGetir(dil_sec, 4);
                    printf(lcd_putc, "\n%s:%s-50", lcd_msg_depo, lcd_msg_depo_drm);
                    output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.

                }
                if (depo_durum == DEPO_MAX) {
                    lcd_msg_depo = dilTxtGetir(dil_sec, 2);
                    printf(lcd_putc, "\n%s:MAX-100", lcd_msg_depo);
                    output_low(ROLE_PIN); // dalgýç pompaya gerilim ver.

                }else{
                     output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
                }
            }

            if (millis() - time_lcd > 2000) {

                lcd_msg_sayac = dilTxtGetir(dil_sec, 11);
                printf(lcd_putc, "\f%s", lcd_msg_baslik);
                printf(lcd_putc, "\n%s:%Lu", lcd_msg_sayac, kul_say);

                time_lcd = millis();
            }

        }
#endif

        sens_adc_oku();

        //if(input(SEVIYE_SENS1_PIN))
        if (an_sv1 > 800) //4 V gibi
        {
            depo_durum = DEPO_BOS;
            output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
        }
        //if(!input(SEVIYE_SENS1_PIN))
        if (an_sv1 < 700) //3.5 V
        {
            depo_durum = DEPO_MIN;
            output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
        }
        //if(!input(SEVIYE_SENS1_PIN) && !input(SEVIYE_SENS2_PIN) )
        if ((an_sv1 < 700)&& (an_sv2 < 700)) {
            depo_durum = DEPO_ORTA;
            output_high(ROLE_PIN); // dalgýç pompaya gerilim ver.
        }
        //if(!input(SEVIYE_SENS1_PIN) && !input(SEVIYE_SENS2_PIN) && !input(SEVIYE_SENS3_PIN))
        if ((an_sv1 < 700)&& (an_sv2 < 700) && (an_sv3 < 700)) {
            depo_durum = DEPO_MAX;
            output_low(ROLE_PIN); //Dalgýç pompanýn gerilimini kes
        }

        if (!input(ORTA_BTN_PIN)) {
            delay_ms(250);
            menu_say++;
            if (menu_say > 6) {
                menu_say = 0;
                bip_basla();
                lcdMenuIslem();
            }
        }
        delay_ms(100);
    }//while(1))
}//main

void bipp(int sure, int sayi, long pin) {
    int a = 0;
    for (a = 0; a < sayi; a++) {
        output_high(pin);
        delay_ms(sure / 2);
        output_low(pin);
        delay_ms(sure / 2);
    }
}//

void sens_adc_oku(void) {
    unsigned int16 _adc = 0;
    int i = 0;

    for (i = 5; i < 8; i++) {
        set_adc_channel(i);
        delay_us(20);
        _adc = read_adc();

        if (i == 5) an_sv1 = _adc;
        if (i == 6) an_sv2 = _adc;
        if (i == 7) an_sv3 = _adc;

    }

    //printf(lcd_putc,"\f%Lu,%Lu,%Lu",an_sv1,an_sv2,an_sv3);
}

char *dilTxtGetir(char dil, int txt_sayi) {
    char *txt;

    if (dil == DIL_TR) txt = splitStr(lcd_tr, ",", txt_sayi);
    if (dil == DIL_FR) txt = splitStr(lcd_fr, ",", txt_sayi);
    if (dil == DIL_ALM) txt = splitStr(lcd_alm, ",", txt_sayi);
    if (dil == DIL_ING) txt = splitStr(lcd_ing, ",", txt_sayi);

    return txt;
}


#define MAX_STRING_LEN 250   //splitStr için gerekli
//char *s = splitStr(buf_rf24_rcv, ",", 7);
//sprintf(text, "%Lu,%u,%u,%c,%4Lu,%u,%s", this_id, master_node, msg_id_say++, 'm',
//                                    panel_rastgele_renk, panel_rastgele_renk,s);// s yazýnca oldu

//    char *kimden = splitStr(msg, ",", 1);

char *splitStr(char *input_string, char *separator, int segment_number) //index 1 den ba?l?yor (segment_number)
{
    //String ifadenin sonu her zaman '\0' karakteri olmal?d?r. Serial veri al?m?nda bu karakter string sonuna eklenmez.
    //Bir stringe ?evirme i?leminde de eklenmeyebilir. String sonuna eklenmelidir.
    //Split i?leminde string bir daha b?l?necekse sonuna muhakkak '\0' eklenmelidir.

    char *act, *sub, *ptr;
    static char copy[MAX_STRING_LEN];
    int i;

    strcpy(copy, input_string);

    for (i = 1, act = copy; i <= segment_number; i++, act = NULL) {

        sub = strtok(act, separator);
        if (sub == NULL)
            break;
    }
    return sub;
}

unsigned long kul_say_ee_islem(char islem, unsigned int32 sayi) {
    unsigned int8 b1, b2, b3, b4;


    if (islem == KUL_SAY_EE_KAYDET) {
        write_eeprom(EE_ADR_KUL_SAY_BL, make8(sayi, 0));
        write_eeprom(EE_ADR_KUL_SAY_BL + 1, make8(sayi, 1));
        write_eeprom(EE_ADR_KUL_SAY_BL + 2, make8(sayi, 2));
        write_eeprom(EE_ADR_KUL_SAY_BL + 3, make8(sayi, 3));
    }

    if (islem == KUL_SAY_EE_OKU) {
        b1 = read_eeprom(EE_ADR_KUL_SAY_BL);
        b2 = read_eeprom(EE_ADR_KUL_SAY_BL + 1);
        b3 = read_eeprom(EE_ADR_KUL_SAY_BL + 1);
        b4 = read_eeprom(EE_ADR_KUL_SAY_BL + 1);

        if (b1 == 0xFF && b2 == 0xFF && b3 == 0xFF && b4 == 0xFF) return 0x00LL;
        else return make32(b4, b3, b2, b1);
    }

}

