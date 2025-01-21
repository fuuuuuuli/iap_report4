#include <stdio.h>
#include "wavelib.h"

// 関数プロトタイプ宣言
int culculate_nagasa(int kaisu, int span_mode, double span, double span_decrease, int decrease_mode, double decrease, struct fmt fmt1, struct data data1);

int main()
{
    // ユーザー入力用変数の宣言と初期化
    int kaisu = 0;           // 繰り返し回数
    int span_mode = 0;       // 間隔の方式
    double span = 0.0;       // 間隔
    double span_decrease = 0.0; // 間隔の減少率
    int decrease_mode = 0;   // 減衰モード
    double decrease = 0.0;   // 減衰量

    // ユーザーからの入力を受け取る
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
            printf("間隔を減少させる際の公比を入力してください(0<r<1)\n");
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

    // 構造体変数の宣言
    struct RIFF RIFF1;  // RIFFチャンク用構造体変数
    struct fmt fmt1;    // fmt チャンク用構造体変数
    struct data data1;  // dataチャンク用構造体変数
    int count = 0;      // 制御変数
    int nagasa = 0;     // 連結後の音声の長さ
    int sound_data = 0; // 音声データ制御変数
    double volume = 1;  // 音量の倍率
    unsigned char *oto; // 連結後の音声を指すポインタ

    // waveファイルの読み込み
    load_wave_data(&RIFF1, &fmt1, &data1, "org1.wav");

    // 音声の長さを計算
    nagasa = culculate_nagasa(kaisu, span_mode, span, span_decrease, decrease_mode, decrease, fmt1, data1);
    printf("Calculated nagasa: %d\n", nagasa); // 追加

    // メモリの確保
    oto = (unsigned char *)calloc(nagasa, sizeof(unsigned char));
    if (oto == NULL || nagasa > 300000)
    {
        printf("メモリが確保できません．プログラムを終了します．\n");
        exit(1);
    }
    else
    {
        // 音声データの連結処理を回数分繰り返す
        for (int j = 0; j < kaisu; j++)
        {
            // 各音声データのサンプルを処理
            for (int i = 0; i < data1.size_of_sounds; i++)
            {
                // printf("j: %d, i: %d, count: %d, sound_data: %d, volume: %f\n", j, i, count, sound_data, volume);
                // 最初のループの場合、音声データをそのままコピー
                if (j == 0)
                {
                    *(oto + count + i) = *(data1.sounds + i) * volume;
                }
                else
                {
                    // 音声データの値が0未満になる場合
                    if (*(oto + count + i) + (*(data1.sounds + i) - 128) * volume < 0)
                    {
                        // printf("oto[%5d] = %3d <= (org[%5d]- 128) * %2lf  = %3d\n", count + i, *(oto + count + i), count + i, volume, (*(data1.sounds + i) - 128) * volume);
                        *(oto + count + i) = 0;
                    }
                    // 音声データの値が255を超える場合
                    else if (*(oto + count + i) + (*(data1.sounds + i) - 128) * volume > 255)
                    {
                        // printf("oto[%5d] = %3d <= (org[%5d]- 128) * %2lf  = %3d\n", count + i, *(oto + count + i), count + i, volume, (*(data1.sounds + i) - 128) * volume);
                        *(oto + count + i) = 255;
                    }
                    // 音声データの値が0以上255以下の場合
                    else
                    {
                        *(oto + count + i) += (*(data1.sounds + i) - 128) * volume;
                    }
                }
            }
            // 次の音声データの開始位置を計算
            count += fmt1.BytesPerSec * span;
            // 間隔の方式が次第に短くなる場合
            if (span_mode == 2)
            {
                span *= span_decrease;
            }
            // 減衰モードが等差減少の場合
            if (decrease_mode == 1)
            {
                volume -= decrease;
            }
            // 減衰モードが等比減少の場合
            else if (decrease_mode == 2)
            {
                volume *= decrease;
            }
        }
    }

    // RIFFチャンクサイズの修正
    RIFF1.SIZE += nagasa - data1.size_of_sounds;
    // 音声データサイズを修正
    data1.size_of_sounds = nagasa;
    // 音声のコピー
    data1.sounds = oto;

    // waveファイルの保存
    save_wave_data(&RIFF1, &fmt1, &data1, "out1.wav");
    printf("ファイルout1.wavを出力しました。");
    return 0;
}

// 音声の長さを計算する関数
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