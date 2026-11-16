#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int main() {
    // ---------------------------------------------------------
    // 1. HAZIRLIK VE RASTGELE SAYI ÜRETECİ BAŞLATMA
    // ---------------------------------------------------------
    srand(time(NULL));

    // Değişken Tanımlamaları (0-100 aralığı sınırları korunacak)
    int saglik = 100;
    int enerji = 100;
    int oksijen = 100;
    int su = 0;
    int yedekParca = 0;
    int modulHasari = 0;
    int gun = 1;
    int tur = 0;
    int kilitMuhurlu = 0;
    char zorluk;
    char komut;
    int oyunBitti = 0;

    // ---------------------------------------------------------
    // 2. ZORLUK SEVİYESİ SEÇİMİ (SWITCH-CASE KONTROLÜ)
    // ---------------------------------------------------------
    do {
        printf("==================================================\n");
        printf("     MARS KOLONISI YASAM DESTEK SIMULATORU      \n");
        printf("==================================================\n");
        printf("Lutfen Zorluk Seviyesi Seciniz:\n");
        printf(" [K]olay (Bol kaynak, dusuk hasar)\n");
        printf(" [N]ormal (Standart kosullar)\n");
        printf(" [Z]or   (Sinirli kaynak, yuksek hasar)\n");
        printf("Seciminiz: ");
        scanf(" %c", &zorluk);
        zorluk = toupper(zorluk);

        switch (zorluk) {
            case 'K':
                su = 40;
                yedekParca = 5;
                modulHasari = 10;
                printf("\n-> Zorluk: Kolay secildi. Baslangic kaynaklari bol tutuldu.\n\n");
                break;
            case 'N':
                su = 25;
                yedekParca = 3;
                modulHasari = 25;
                printf("\n-> Zorluk: Normal secildi. Dengeli oyun modu basliyor.\n\n");
                break;
            case 'Z':
                su = 15;
                yedekParca = 1;
                modulHasari = 40;
                printf("\n-> Zorluk: Zor secildi. Kaynaklar kritik seviyede!\n\n");
                break;
            default:
                printf("\n[HATA] Gecersiz secim yaptiniz! Lutfen sadece K, N veya Z giriniz.\n\n");
                zorluk = '\0'; // Döngünün tekrarlanması için sıfırlanır
                break;
        }
    } while (zorluk == '\0');

    // ---------------------------------------------------------
    // 3. ANA OYUN DÖNGÜSÜ (DO-WHILE DÖNGÜSÜ)
    // ---------------------------------------------------------
    do {
        // Değerlerin Sınır Kontrolü (Hiçbir zaman 0'ın altına düşmez, 100'ü aşmaz)
        if (saglik < 0) saglik = 0; 
        if (saglik > 100) saglik = 100;
        if (enerji < 0) enerji = 0; 
        if (enerji > 100) enerji = 100;
        if (oksijen < 0) oksijen = 0; 
        if (oksijen > 100) oksijen = 100;
        if (modulHasari < 0) modulHasari = 0; 
        if (modulHasari > 100) modulHasari = 100;

        // Oyun Bitiş / Kazanma / Kaybetme Koşulları Kontrolü
        if (saglik <= 0 || oksijen <= 0) {
            printf("\n========================================\n");
            printf("[KRITIK] Saglik veya oksijen degeri sifirlandi!\n");
            printf("MARS KOLONISINDE HAYATINI KAYBETTIN.\n");
            printf("========================================\n");
            oyunBitti = 1;
            break;
        }
        if (gun > 15) {
            printf("\n========================================\n");
            printf("[TEBRIKLER] 15 gun basariyla tamamlandi!\n");
            printf("Kurtarma ekibi istasyona ulasti. OYUNU KAZANDIN!\n");
            printf("========================================\n");
            oyunBitti = 1;
            break;
        }

        // Anlık Durum Paneli
        printf("--------------------------------------------------\n");
        printf("GUN: %d | TUR: %d | Hava Kilidi Muhurlu: %d\n", gun, tur, kilitMuhurlu);
        printf("Saglik: %d | Enerji: %d | Oksijen: %d | Su: %d | Parca: %d | Hasar: %d\n", 
               saglik, enerji, oksijen, su, yedekParca, modulHasari);
        printf("--------------------------------------------------\n");
        printf("KOMUTLAR: [G]esif  [O]ksijen Uret  [T]amir  [D]urum Raporu\n");
        printf("          [U]yu    [K]um Firtinasi [H]ava Kilidi [Q]Cikis\n");
        printf("Komut giriniz: ");
        scanf(" %c", &komut);
        komut = toupper(komut);

        int turGecerli = 1;

        // ---------------------------------------------------------
        // 4. KOMUT SİSTEMİ (SWITCH-CASE VE FALL-THROUGH KULLANIMI)
        // ---------------------------------------------------------
        switch (komut) {
            case 'G': // Keşif Görevi (Büyük harf)
            case 'g': // Keşif Görevi (Küçük harf - Fall Through)
                if (enerji < 15) {
                    printf("[HATA] Enerjiniz yetersiz! Kesif icin en az 15 enerji gereklidir.\n");
                    turGecerli = 0;
                    break;
                }
                enerji -= 15;
                oksijen -= 10;

                {
                    int zar = rand() % 100;
                    int esik = (enerji > 40 && oksijen > 30) ? 50 : 25;
                    printf("Kesif Zari Sonucu -> Zar: %d / Esik: %d -> ", zar, esik);

                    // Zar ve modül hasarına bağlı yaralanma kontrolü
                    if (zar < 10 || modulHasari > 70) {
                        printf("YARALANDINIZ! Saglik dustu. ");
                        saglik -= 15;
                    }

                    if (zar < esik) {
                        su += 10;
                        printf("BASARILI! Kesifte 10 birim su bulundu.\n");
                    } else if (zar < esik + 25) {
                        yedekParca += 2;
                        printf("BASARILI! Kesifte 2 yedek parca bulundu.\n");
                    } else {
                        printf("SONUÇSUZ: Kaynak bulunamadi, eli bos donuldu.\n");
                    }
                }
                break;

            case 'O': // Oksijen Üretimi
            case 'o':
                {
                    int suMiktari;
                    printf("Uretim icin harcanacak su miktarini giriniz (Mevcut Su: %d): ", su);
                    scanf("%d", &suMiktari);

                    if (suMiktari > su || suMiktari <= 0) {
                        printf("[HATA] Gecersiz veya mevcut sudan fazla miktar girdiniz!\n");
                        turGecerli = 0;
                    } else {
                        su -= suMiktari;
                        // Mod operatörü ile artık su hesabı (Çiftse hepsi, tekse 1 birim depoda kalır)
                        int artikSu = suMiktari % 2;
                        int uretilenOksijen = (suMiktari / 2) * 3;
                        su += artikSu; 

                        oksijen += uretilenOksijen;
                        if (oksijen > 100) oksijen = 100;
                        printf("Basarili! Oksijen uretildi. Artan su depoda birikti: %d birim.\n", artikSu);
                    }
                }
                break;

            case 'T': // Tamir İşlemi
            case 't':
                if (yedekParca <= 0) {
                    printf("[HATA] Yedek parcaniz yok! Tamir islemi yapilamaz.\n");
                    turGecerli = 0;
                    break;
                }
                yedekParca--;

                // IF-ELSE IF-ELSE kademeleri ile hasar onarımı
                if (modulHasari <= 30) {
                    modulHasari -= 15;
                    printf("Hafif hasar giderildi (-15 Hasar duşuruldu).\n");
                } else if (modulHasari <= 70) {
                    modulHasari -= 10;
                    printf("Orta seviye hasar giderildi (-10 Hasar duşuruldu).\n");
                } else {
                    modulHasari -= 5;
                    printf("Agir hasar! Zor şartlar altinda onarildi (-5 Hasar duşuruldu).\n");
                }
                break;

            case 'D': // Durum Raporu
            case 'd':
                turGecerli = 0; // Durum raporu tur/zaman harcamaz
                printf("\n========================================\n");
                printf("          DETAYLI DURUM RAPORU          \n");
                printf("========================================\n");
                
                // Kritik seviye uyarıları
                if (oksijen < 25) printf("[KRITIK UYARI] Oksijen seviyesi kritik (<25)!\n");
                if (saglik < 30) printf("[KRITIK UYARI] Saglik seviyesi kritik (<30)!\n");
                if (modulHasari > 70) printf("[KRITIK UYARI] Modul hasari cok yuksek (>70)!\n");

                // && ile birleştirilmiş tek koşullu sistem normal bildirimi
                if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70) {
                    printf("TUM SISTEMLER NORMAL - Durumlar stabil.\n");
                }
                printf("========================================\n");
                break;

            case 'U': // Dinlenme / Uyku
            case 'u':
                enerji += 20;
                // İç içe (nested) IF yapısı ile oksijen kontrolü
                if (oksijen < 20) {
                    saglik -= 10;
                    printf("[TEHLIKE] Oksijen cok dusuk oldugu icin dinlenirken saglik kaybettiniz!\n");
                } else {
                    saglik += 10;
                    printf("Dinlenildi. Enerji ve saglik seviyeniz artti.\n");
                }
                break;

            case 'K': // Kum Fırtınası (FOR DÖNGÜSÜ İLE SİMÜLASYON)
            case 'k':
                {
                    int firtinaTuru = (zorluk == 'K') ? 3 : (zorluk == 'N') ? 5 : 7;
                    printf("Kum firtinasi basladi! Firtina suresi: %d tur surucek.\n", firtinaTuru);
                    int toplamCanKaybi = 0;
                    int toplamOksijenKaybi = 0;

                    for (int i = 1; i <= firtinaTuru; i++) {
                        // Hava kilidi mühürlüyse continue ile o turki sızıntı es geçilir
                        if (kilitMuhurlu == 1) {
                            printf("Tur %d: Hava kilidi onceden muhurlendigi icin oksijen sizintisi engellendi!\n", i);
                            continue;
                        }

                        int zar = rand() % 100;
                        int katSayi = (i % 2 == 0) ? 2 : 1; // Mod operatörü ile çift tur kontrolü (2 kat hasar)
                        int canKaybi = 5 * katSayi;
                        int oksijenSizintisi = 4 * katSayi;

                        saglik -= canKaybi;
                        oksijen -= oksijenSizintisi;
                        toplamCanKaybi += canKaybi;
                        toplamOksijenKaybi += oksijenSizintisi;

                        printf("Tur %d (Siddet Carpan x%d) - Zar: %d -> Firtina hasari! Can: -%d, Oksijen: -%d\n", 
                               i, katSayi, zar, canKaybi, oksijenSizintisi);

                        if (saglik <= 0 || oksijen <= 0) {
                            printf("Firtina sirasinda kritik degerler tukendi! Dongu break ile sonlandiriliyor.\n");
                            break; 
                        }
                    }
                    printf("Firtina Ozeti -> Toplam Can Kaybi: %d, Toplam Oksijen Kaybi: %d\n", 
                           toplamCanKaybi, toplamOksijenKaybi);
                }
                break;

            case 'H': // Hava Kilidi Şifresi (DO-WHILE DÖNGÜSÜ)
            case 'h':
                {
                    char hedefSifre = 'A' + (gun % 26);
                    int denemeHakki = 3;
                    char girilenHarf;
                    int sifreBasarili = 0;

                    printf("Hava Kilidi Guvenlik Protokolu Devrede:\n");
                    printf("Ipucu: Sifre, alfabede 'A' harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);

                    do {
                        printf("Kalan deneme hakki (%d). Bir harf giriniz (A-Z): ", denemeHakki);
                        scanf(" %c", &girilenHarf);
                        girilenHarf = toupper(girilenHarf);

                        if (girilenHarf == hedefSifre) {
                            printf("Tebrikler! Dogru sifre. Hava kilidi basariyla muhurlendi.\n");
                            kilitMuhurlu = 1;
                            sifreBasarili = 1;
                            break;
                        } else {
                            denemeHakki--;
                            enerji -= 5;
                            if (girilenHarf < hedefSifre) {
                                printf("Yanlis! Ileri git (alfabede daha ileride bir harf deneyin).\n");
                            } else {
                                printf("Yanlis! Geri git (alfabede daha geride bir harf deneyin).\n");
                            }
                        }
                    } while (denemeHakki > 0 && girilenHarf != hedefSifre); // Tek mantıksal ifadeyle kontrol

                    if (!sifreBasarili) {
                        printf("Tum deneme haklari bitti! Sifre hatasi nedeniyle modul hasari artti.\n");
                        modulHasari += 20;
                    }
                }
                break;

            case 'Q': // Çıkış Komutu
            case 'q':
                printf("Simulasyondan cikis yapiliyor...\n");
                oyunBitti = 1;
                break;

            default: // Tanımsız Komut (Fall-Through / Hata Yönetimi)
                printf("[UYARI] Tanimsiz komut girdiniz! Gecersiz komut tur harcamadi.\n");
                turGecerli = 0;
                break;
        }

        // ---------------------------------------------------------
        // 5. ZAMAN AKIŞI VE GÜN KONTROLÜ
        // ---------------------------------------------------------
        if (turGecerli) {
            tur++;
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8; // Her gün başında oksijen azalması
                printf("\n========================================\n");
                printf("       YENI GUN BASLADI: GUN %d          \n", gun);
                printf("========================================\n");
                if (modulHasari > 70) {
                    saglik -= 5;
                    printf("Kritik Durum: Modul hasari 70'in uzerinde oldugu icin gun basinda ekstra 5 saglik kaybettiniz!\n");
                }
            }
        }

    } while (!oyunBitti);

    // ---------------------------------------------------------
    // 6. OYUN SONU RAPORU VE PUAN HESAPLAMA
    // ---------------------------------------------------------
    int puan = (gun * 100) + saglik + oksijen - modulHasari;
    printf("\n========================================\n");
    printf("            OYUN SONU RAPORU            \n");
    printf("========================================\n");
    printf("Hayatta Kalinan Gun : %d\n", gun);
    printf("Toplam Tur Sayisi   : %d\n", tur);
    printf("Son Saglik Durumu   : %d\n", saglik);
    printf("Son Oksijen Durumu  : %d\n", oksijen);
    printf("Son Modul Hasari    : %d\n", modulHasari);
    printf("HESAPLANAN PUAN     : %d\n", puan);
    printf("========================================\n");

    return 0;
}