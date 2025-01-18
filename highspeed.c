/* このプログラムの名前：highspeed.c    */
/* 音声の再生速度を上げるプログラム */
#include<stdio.h>
#include"wavelib.h"

int main(void)
{
    struct RIFF RIFF1, RIFF2;  /* RIFFチャンク用構造体変数 */
    struct fmt  fmt1, fmt2;    /* fmt チャンク用構造体変数 */
    struct data data1, data2;  /* dataチャンク用構造体変数 */
    double alpha;              /* 時間軸方向の短縮係数     */
    int newsize, i, n;         /* 作業用変数               */
    
    printf("===== 音声の再生速度を上げる ====\n");
    
    /* wave ファイルの読み込み */
    printf("元の音声データを読み込みます．\n");
    load_wave_data( &RIFF1, &fmt1, &data1, "" );

    /* 音声２へのコピー */
    RIFF2 = RIFF1;
    fmt2  = fmt1;
    data2 = data1;
    
    /* 音声２のデータを詰める */
    do{
        printf("\n音声を時間軸方向に縮めるときの係数（ [0, 1] の実数）：");
        scanf("%lf",&alpha);
    }while( alpha <= 0.0 || alpha >= 1.0 );
    
    /* 音声２を作る */
    /* サイズの変更 */
    newsize = (int)( data2.size_of_sounds * alpha );
    /* RIFFチャンク全体の大きさも変わるので修正する */
    RIFF2.SIZE = RIFF2.SIZE - data2.size_of_sounds + newsize;
    data2.size_of_sounds = newsize;
    /* 元音声のデータを間引いてコピーする */
    for(i=0;i<data2.size_of_sounds;i++){
        /* 元の何番目のデータにするかを決める */
        n = (int)( data1.size_of_sounds * ( i / (double)newsize ) );
        /* データのコピー */
        *( data2.sounds + i ) = *( data1.sounds + n );
    }

    /* 音声２を wave ファイルとして保存 */
    printf("\n生成した音声を保存します．\n");
    save_wave_data( &RIFF2, &fmt2, &data2, "" );
    
    return 0;
}
