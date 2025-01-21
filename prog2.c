#include <stdio.h>
#include "wavelib.h"

int main()
{
    double distance_noise = 0.0;    // ノイズ源の距離
    double distance_speaker = 0.0;  // スピーカーとノイズ源の距離

    // ノイズ源の距離を入力
    while (distance_noise <= 0)
    {
        printf("ノイズ源の距離を入力してください[m]\n");
        scanf("%lf", &distance_noise);
    }

    // スピーカーとノイズ源の距離を入力
    while (distance_speaker <= 0)
    {
        printf("スピーカーとノイズ源の距離を入力してください[m]\n");
        scanf("%lf", &distance_speaker);
    }

    // 構造体変数の宣言
    struct RIFF RIFF1;  // RIFFチャンク用構造体変数
    struct fmt fmt1;    // fmt チャンク用構造体変数
    struct data data1;  // dataチャンク用構造体変数
    int delay_sec = 0;  // 遅延時間（秒）
    int delay_byte = 0; // 遅延時間（バイト）
    int nagasa = 0;     // 音声データの長さ
    double volume = 1.0; // 音量の倍率
    unsigned char *oto; // 連結後の音声を指すポインタ

    // waveファイルの読み込み
    load_wave_data(&RIFF1, &fmt1, &data1, "org2.wav");

    // 音声データの長さを取得
    nagasa = data1.size_of_sounds;

    // 遅延時間を計算（秒）
    delay_sec = (int)(distance_speaker / 343.0);

    // 遅延時間を計算（バイト）
    delay_byte = delay_sec * fmt1.BytesPerSec;

    // メモリの確保
    oto = (unsigned char *)calloc(nagasa + delay_byte, sizeof(unsigned char));
    if (oto == NULL)
    {
        printf("メモリが確保できません．プログラムを終了します．\n");
        exit(1);
    }

    // 元の音声データをコピー
    for (int i = 0; i < nagasa; i++)
    {
        *(oto + i) = *(data1.sounds + i);
    }

    // 遅延音声データを追加
    for (int i = 0; i < nagasa; i++)
    {
        if (i + delay_byte < nagasa)
        {
            // 音声データの値が255を超える場合
            if (-1 * ((*(data1.sounds + i) - 128) * volume) >= 255)
            {
                *(oto + i + delay_byte) = 255;
            }
            // 音声データの値が0未満になる場合
            else if (-1 * ((*(data1.sounds + i) - 128) * volume) <= 0)
            {
                *(oto + i + delay_byte) = 0;
            }
            // 音声データの値が0以上255以下の場合
            else
            {
                *(oto + i + delay_byte) = -1 * ((*(data1.sounds + i) - 128) * volume);
            }
        }
    }

    // データサイズの更新
    data1.size_of_sounds += delay_byte;
    data1.sounds = oto;
    RIFF1.SIZE += delay_byte;

    // waveファイルの保存
    save_wave_data(&RIFF1, &fmt1, &data1, "out2.wav");

    return 0;
}