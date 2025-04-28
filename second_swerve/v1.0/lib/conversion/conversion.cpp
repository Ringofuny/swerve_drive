#include "conversion.h"
#include <math.h>

void Conversion::Send_Data::update(int16_t Strength) {
    /* -- データを送信できる形にする関数（半分に分ける）
        ex) [ 1001 0010 1111 1100 ] の時 [ 1001 0010 ](上位バイト)と[ 1111 1100 ](下位バイト)に分けて
        それぞれ上位バイトは[ High_Byte ] に 下位バイトは[ Low_Byte ] に代入する */
    High_Byte = (Strength >> 8);
    Low_Byte = (Strength & (0x00FF));
}

int16_t Conversion::Available_Data::Become(double Current) {
    /* -- データをCANで送って制御できる形にして返す（int16_t）
        -10(A) ~ 10(A) を -16384 ~ 16384 の間のデータに変換 */

    double scaled = (Current / 10.0) * 16384.0;
    int32_t calc = static_cast<int32_t>(std::round(scaled));

    // クリッピング
    if (calc > 16384) calc = 16384;
    if (calc < -16384) calc = -16384;

    Result = static_cast<int16_t>(calc);
    return Result;
}

