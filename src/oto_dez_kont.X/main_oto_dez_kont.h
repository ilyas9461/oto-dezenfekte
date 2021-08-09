/* 
 * File:   main_oto_dez_kont.h
 * Author: ilyas
 *
 * Created on 22 Nisan 2020 Çarþamba, 12:10
 */

#ifndef MAIN_OTO_DEZ_KONT_H
#define	MAIN_OTO_DEZ_KONT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include <C:\Program Files (x86)\PICC\Devices\18F45K22.h> //tam yolu yazmak dosyayý açamama hatasýný giderdi.
    #device adc=10
    #device HIGH_INTS=TRUE 
    #device PASS_STRINGS=IN_RAM  

    #FUSES NOWDT                    //No Watch Dog Timer
    #FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
    #FUSES INTRC_IO                 //Internal RC Osc, no CLKOUT
    #FUSES PLLEN                  //4X HW PLL disabled, 4X PLL enabled in software
    #FUSES NOPUT                      //Power Up Timer
    #FUSES NOBROWNOUT               //No brownout reset
    #FUSES WDT_SW                   //No Watch Dog Timer, enabled in Software
    #FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
    #FUSES MCLR                   //Master Clear pin used for I/O
    #FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
    #FUSES PROTECT                  //Code protected from reads
    //#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
    #FUSES NOFCMEN
    #FUSES NOHFOFST     

    #use delay(internal=64MHz) // çalýþtý 4MHZ çaýþtý

     //#use rs232(baud=9600,parity=N,UART1,bits=8,stream=RFID_serial)  //uart1
   // #use rs232(baud=9600,parity=N,UART2,bits=8,stream=PORT2) 
    
      extern unsigned int32 timerIntSay;
  
      const char lcd_diller[]={"TURKCE,FRANCAIS,Deutsch,English,"};
                                                                                                               //11  12
      const char lcd_tr[]={"OTO DEZENFEKTE,Depo,Sure,Orta,Kaydedildi,Sure Ayar,DONANIM KONT.,DIL SEC,CIKIS,BOS,Sayac,Bekleme Suresi,"};
      //#define lcd_tr "OTO DEZENFEKTE,Depo,Sure,Orta,Kaydedildi,Sure Ayar,DONANIM KONT.,DIL SEC,CIKIS,BOS,"
      const char lcd_fr[]={"Desinfection...,Reservoir,Temps,Milieu,Enregistre,Reglages temps,Cont. materiel,choisissez lang.,Sortie,VIDES,Compter,Temps d'attente,"};
      const char lcd_alm[]={"Desinfektion...,Speicher,Zeit,Mitte,Gespeichert,Zeiteinstellung,Hardwaresteuerung,Wahle die Sprache,Ausfahrt,LEER,Anzahl,Wartezeit,"};
      const char lcd_ing[]={"Disinfection...,Store,Time,Middle,Saved,Time Setting,Hardware Control,Choose Language,Exit,EMPTY,Count,Standby Time,"};

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_OTO_DEZ_KONT_H */

