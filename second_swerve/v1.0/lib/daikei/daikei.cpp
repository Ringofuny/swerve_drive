#include "daikei.h"

MILLIS t;

//台形制御の初期化、呼び出し不要
Daikei::Daikei() {
  current = 0.0;
  firstRun = true;
  lastTimestamp = 0;
  tolerance = 1.0;
  target = 0.0;
  targetMin = 0.0;
  targetMax = 255.0;
  deltaPerSec = 10.0;
}

/*
   目標値設定
   引数：目標値（double型）
   戻り値：なし
*/
void Daikei::setTarget(double trgt) {
  target = trgt;
  if(trgt > targetMax)
    target = targetMax;
  if(trgt < targetMin)
    target = targetMin;
  /*printTARGET SET AT ");
    print(target);*/

}

/*
   一秒間における変化量の設定
   引数：変化量（double型）
   戻り値：なし
*/
void Daikei::setDeltaPerSecond(double delta) {
  deltaPerSec = delta;
  /*printDELTA SET AT ");
    print(deltaPerSec);*/
}

/*
   台形制御で目標値からずれても良い範囲の設定
   引数：許容範囲（double型）
   戻り値：なし
*/
void Daikei::setTolerance(double tol) {
  tolerance = tol;
}

/*
   最大値や最小値の設定
   引数１：最小値（double型）
   引数２：最大値（double型）
   戻り値：なし
*/
void Daikei::setMinMax(double min, double max) {
  targetMin = min;
  targetMax = max;
}

/*
   台形制御の更新関数---この関数・メソッドを呼び出すと台形制御の結果を得られる
   引数：なし
   戻り値：計算結果（double型）
*/
double Daikei::update() {
  newTimestamp = t.millis();
  if (firstRun) {
    firstRun = false;
    lastTimestamp = newTimestamp;
    // printf("first run\n");
    return current;
  } else {
    deltaTimestamp = abs(newTimestamp - lastTimestamp);
    double deltaStepDouble = deltaPerSec * deltaTimestamp / 1000.0;
    if (abs(target - current) >= tolerance) {
      if ((target - current) >= 0) {
        //差が正
        current += deltaStepDouble;
      } else {
        //差が負
        current -= deltaStepDouble;
      }
      if (current < targetMin)
        current = targetMin;
      else if (current > targetMax)
        current = targetMax;
    }
  }
  lastTimestamp = newTimestamp;
  return current;
}