#include <stdio.h>
#include "wavelib.h"

int main()
{

    struct RIFF RIFF1; /* RIFF チャンク用構造体変数  */
    struct fmt fmt1;   /* fmt  チャンク用構造体変数  */
    struct data data1; /* data チャンク用構造体変数  */

    load_wave_data(&RIFF1, &fmt1, &data1, "org1.wav");
    return 0;
}