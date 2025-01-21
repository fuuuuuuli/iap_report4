#include <stdio.h>
#include "wavelib.h"

int culculate_nagasa(int kaisu, int span_mode, double span, double span_decrease, int decrease_mode, double decrease, struct fmt fmt1, struct data data1);

int main()
{
    int kaisu, span_mode = 0, decrease_mode;
    double span, span_decrease, decrease = 0;
    printf("回数を入力してください\n");
    scanf("%d", &kaisu);
    while (span_mode != 1 && span_mode != 2)
    {
        printf("間隔の方式を半角数字で選択してください:[1]等間隔[2]次第に短く\n");
        scanf("%d", &span_mode);
    }
    if (span_mode == 1)
    {
        printf("等間隔を選択しました\n");
        printf("間隔を入力してください(sec)\n");
        scanf("%lf", &span);
    }
    else if (span_mode == 2)
    {
        printf("次第に短くを選択しました\n");
        printf("初期の間隔を入力してください(sec)\n");
        scanf("%lf", &span);
        while (span_decrease <= 0 || span_decrease >= 1)
        {
            printf("間隔の減少率を入力してください(0<r<1)\n");
            scanf("%lf", &span_decrease);
        }
    }
    while (decrease_mode != 1 && decrease_mode != 2)
    {
        printf("減衰モードを入力してください:[1]等差減少[2]等比減少\n");
        scanf("%d", &decrease_mode);
    }
    if (decrease_mode == 1)
    {
        printf("等差減少を選択しました\n");
        while (decrease <= 0 || decrease >= 1)
        {
            printf("公差を入力してください(0<d<1)\n");
            scanf("%lf", &decrease);
        }
    }
    else if (decrease_mode == 2)
    {
        printf("等比減少を選択しました\n");
        while (decrease <= 0 || decrease >= 1)
        {
            printf("公比を入力してください(0<r<1)\n");
            scanf("%lf", &decrease);
        }
    }

    struct RIFF RIFF1;  /* RIFFチャンク用構造体変数       */
    struct fmt fmt1;    /* fmt チャンク用構造体変数       */
    struct data data1;  /* dataチャンク用構造体変数       */
    int count = 0;      /* 制御変数                       */
    int nagasa = 0;     /* 連結後の音声の長さ             */
    double volume = 1;  /* 音量の倍率                     */
    unsigned char *oto; /* 連結後の音声を指すポインタ     */

    nagasa = culculate_nagasa(kaisu, span_mode, span, span_decrease, decrease_mode, decrease, fmt1, data1);

    load_wave_data(&RIFF1, &fmt1, &data1, "org1.wav");

    oto = (unsigned char *)malloc(nagasa);
    if (oto == NULL)
    {
        printf("メモリが確保できません．プログラムを終了します．\n");
        exit(1);
    }
    else
    {
        for (int i = 0; i < data1.size_of_sounds; i++)
        {
            *(oto + count + i) = *(data1.sounds + i) * volume;
        }
        count += fmt1.BytesPerSec * span;
        if (span_mode == 2)
        {
            span *= span_decrease;
        }
        if (decrease_mode == 1)
        {
            volume -= decrease;
        }
        else if (decrease_mode == 2)
        {
            volume *= decrease;
        }
    }

    /* 音声１にコピー */
    data1.size_of_sounds = nagasa;      /* 音声データサイズを修正   */
    data1.sounds = oto;                 /* 音声のコピー             */
    RIFF1.SIZE += data2.size_of_sounds; /* RIFFチャンクサイズの修正 */

    /* wave ファイルの保存 */
    printf("連結した音声を保存します．\n");
    save_wave_data(&RIFF1, &fmt1, &data1, "out1.wav");

    return 0;
}

int culculate_nagasa(int kaisu, int span_mode, double span, double span_decrease, int decrease_mode, double decrease, struct fmt fmt1, struct data data1)
{
    int nagasa = 0;
    for (int i = 0; i < kaisu; i++)
    {
        nagasa += fmt1.BytesPerSec * span;
        if (span_mode == 2)
        {
            span *= span_decrease;
        }
    }
    nagasa += data1.size_of_sounds;
    return nagasa;
}