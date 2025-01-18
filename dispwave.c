/* このプログラムの名前：dispwave.c   */
/* WAVEファイルの情報を示すプログラム */
#include<stdio.h>
#include"wavelib.h"

int main(void)
{
    struct RIFF RIFF1;   /* RIFFチャンク用構造体変数 */
    struct fmt  fmt1;    /* fmt チャンク用構造体変数 */
    struct data data1;   /* dataチャンク用構造体変数 */
    
    printf("===== wave ファイルの内容を表示するプログラム ====\n");
    
    /* wave ファイルの読み込み */
    load_wave_data( &RIFF1, &fmt1, &data1, "" );
    
    /* wave ファイルの内容の表示 */
    printf("\n***** WAVEデータの情報 *****\n");
    printf("===== RIFF チャンク =====\n");
    printf("  RIFFチャンクID     = %s\n", RIFF1.ID );
    printf("  RIFFチャンクサイズ = %d [byte]\n", RIFF1.SIZE );
    printf("  RIFFチャンクタイプ = %s\n", RIFF1.TYPE );
    printf("===== fmt  チャンク =====\n");
    printf("  fmtチャンクID      = %s\n", fmt1.ID );
    printf("  fmtチャンクサイズ  = %d [byte]\n", fmt1.SIZE );
    printf("  fmtチャンクタイプ  = %d", fmt1.TYPE );
        if ( fmt1.TYPE == 1 ) printf(" (= monoral)\n");
            else printf(" (= stereo)\n");
    printf("  チャンネル数       = %d\n", fmt1.Channel );
    printf("  サンプリングレート = %d [Hz]\n", fmt1.SamplesPerSec );
    printf("  データ速度         = %d [bytes/sec]\n", fmt1.BytesPerSec );
    printf("  ブロックサイズ     = %d [byte/sample x channel]\n", 
        fmt1.BlockSize );
    printf("  サンプルあたりのビット数 = %d [bit/sample]\n", fmt1.BitsPerSample );
    printf("===== data チャンク =====\n");
    printf("  dataチャンクID     = %s\n", data1.ID );
    printf("  dataチャンクサイズ = %d[byte]\n", data1.size_of_sounds );
    printf("  以下は波形データ\n");
    
    return 0;
}
