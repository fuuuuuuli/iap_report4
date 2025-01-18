/* このプログラムの名前：concatwave.c    */
/* ２つの音声データを連結するプログラム */
#include<stdio.h>
#include"wavelib.h"

int main(void)
{
    struct RIFF RIFF1, RIFF2;  /* RIFFチャンク用構造体変数       */
    struct fmt  fmt1, fmt2;    /* fmt チャンク用構造体変数       */
    struct data data1, data2;  /* dataチャンク用構造体変数       */
    int i,j;                   /* 制御変数                       */
    int nagasa;                /* 連結後の音声の長さ             */
    unsigned char *oto;        /* 連結後の音声を指すポインタ     */
    
    printf("===== ２つの音声データを連結する ====\n");
    
    /* wave ファイルの読み込み */
    printf("１つ目の音声データを読み込みます．\n");
    load_wave_data( &RIFF1, &fmt1, &data1, "" );
    printf("２つ目の音声データを読み込みます．\n");
    load_wave_data( &RIFF2, &fmt2, &data2, "" );
    
    /* 音声を連結する */
    printf("\n１つ目の音声の後に２つ目の音声を連結します．\n\n");
    /* 連結した音声データをメモリ上に作成する */
    nagasa = data1.size_of_sounds + data2.size_of_sounds;
    oto = (unsigned char *)malloc( nagasa );
    if ( oto == NULL ){
        printf("メモリが確保できません．プログラムを終了します．\n");
        exit(1);
    } else {
        /* １つ目の音声のコピー */
        for(i=0;i<data1.size_of_sounds;i++){
            *(oto + i) = *( data1.sounds + i );
        }
        /* ２つ目の音声のコピー */
        for(i=0;i<data2.size_of_sounds;i++){
            *(oto + data1.size_of_sounds + i) = *( data2.sounds + i );
        }
    }
    /* 音声１にコピー */
    data1.size_of_sounds = nagasa;       /* 音声データサイズを修正   */
    data1.sounds = oto;                  /* 音声のコピー             */
    RIFF1.SIZE += data2.size_of_sounds;  /* RIFFチャンクサイズの修正 */
    
    /* wave ファイルの保存 */
    printf("連結した音声を保存します．\n");
    save_wave_data( &RIFF1, &fmt1, &data1, "" );
    
    return 0;
}
