#include <stdlib.h>
#include "millis.h"

extern MILLIS t;

class Daikei {
    public: 
      //台形制御の初期化、呼び出し不要
      Daikei();
  
      /*
        目標値設定
        引数：目標値（double型）
        戻り値：なし
      */
      void setTarget(double trgt);
  
      /*
        １秒間における変化量の設定
        引数：変化量（double型）
        戻り値：なし
      */
      void setDeltaPerSecond(double delta);
  
      /*
        台形制御で目標値からずれても良い範囲の設定
        引数：許容範囲（double型）
        戻り値：なし
      */
      void setTolerance(double tol);
  
      /*
         最大値や最小値の設定
         引数１：最小値（double型）
         引数２：最大値（double型）
         戻り値：なし
      */
      void setMinMax(double min, double max);
  
      /*
         台形制御の更新関数---この関数・メソッドを呼び出すと台形制御の結果を得られる
         引数：なし
         戻り値：計算結果（double型）
      */
      double update();
      double current;//現在地
  
    private:
      bool firstRun;//初期実行フラグ
      double tolerance;//許容範囲
      double target, targetMax, targetMin;//目標値、最大値、最小値
      double deltaPerSec;//１秒間における変化量
      long int deltaTimestamp, newTimestamp, lastTimestamp;//実行時間差
  };
   