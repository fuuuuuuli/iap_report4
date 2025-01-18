/* このプログラムの名前：copywave.c   */
/* WAVEファイルをコピーするプログラム */
#include<stdio.h>
#include"wavelib.h"

int main(void)
{
    struct RIFF RIFF1;   /* RIFFチャンク用構造体変数 */
    struct fmt  fmt1;    /* fmt チャンク用構造体変数 */
    struct data data1;   /* dataチャンク用構造体変数 */
    
    printf("===== wave ファイルをコピーするプログラム ====\n");
    
    /* wave ファイルの読み込み */
    load_wave_data( &RIFF1, &fmt1, &data1, "" );
    
    /* wave ファイルの書き込み */
    save_wave_data( &RIFF1, &fmt1, &data1, "" );
    
    return 0;
}
