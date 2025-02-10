#include <Trade\SymbolInfo.mqh>
#include <ChartObjects\ChartObjectsLines.mqh>

input int numBars = 100;       // Number of bars to analyze
input int pivotLength = 5;     // Pivot points lookback period

//+------------------------------------------------------------------+
//| Script program start function                                    |
//+------------------------------------------------------------------+
void OnStart()
{
   // Get price data
   MqlRates rates[];
   ArraySetAsSeries(rates, true);
   int copied = CopyRates(_Symbol, _Period, 0, numBars, rates);
   
   if(copied <= 0) {
      Print("Error getting historical data");
      return;
   }

   // Identify trendlines
   IdentifyTrendLines(rates);
   
   // Find support/resistance levels
   FindSupportResistance(rates);
}
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| Identify trendlines function                                     |
//+------------------------------------------------------------------+
void IdentifyTrendLines(const MqlRates &rates[])
{
   // Arrays to store swing points
   int upTrendPoints[];
   int downTrendPoints[];
   
   // Find swing lows (potential uptrend points)
   for(int i = pivotLength; i < ArraySize(rates)-pivotLength; i++) {
      if(IsSwingLow(rates, i, pivotLength)) {
         ArrayResize(upTrendPoints, ArraySize(upTrendPoints)+1);
         upTrendPoints[ArraySize(upTrendPoints)-1] = i;
      }
   }
   
   // Find swing highs (potential downtrend points)
   for(int i = pivotLength; i < ArraySize(rates)-pivotLength; i++) {
      if(IsSwingHigh(rates, i, pivotLength)) {
         ArrayResize(downTrendPoints, ArraySize(downTrendPoints)+1);
         downTrendPoints[ArraySize(downTrendPoints)-1] = i;
      }
   }
   
   // Draw uptrend lines (connect higher lows)
   for(int i = 0; i < ArraySize(upTrendPoints)-2; i++) {
      if(rates[upTrendPoints[i]].low < rates[upTrendPoints[i+1]].low &&
         rates[upTrendPoints[i+1]].low < rates[upTrendPoints[i+2]].low)
      {
         CreateTrendLine("UpTrend"+IntegerToString(i), 
                        rates[upTrendPoints[i]].time, rates[upTrendPoints[i]].low,
                        rates[upTrendPoints[i+2]].time, rates[upTrendPoints[i+2]].low,
                        clrGreen);
      }
   }
   
   // Draw downtrend lines (connect lower highs)
   for(int i = 0; i < ArraySize(downTrendPoints)-2; i++) {
      if(rates[downTrendPoints[i]].high > rates[downTrendPoints[i+1]].high &&
         rates[downTrendPoints[i+1]].high > rates[downTrendPoints[i+2]].high)
      {
         CreateTrendLine("DownTrend"+IntegerToString(i), 
                        rates[downTrendPoints[i]].time, rates[downTrendPoints[i]].high,
                        rates[downTrendPoints[i+2]].time, rates[downTrendPoints[i+2]].high,
                        clrRed);
      }
   }
}
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| Find support and resistance levels                               |
//+------------------------------------------------------------------+
void FindSupportResistance(const MqlRates &rates[])
{
   double supportLevels[];
   double resistanceLevels[];
   
   // Identify potential support/resistance zones
   for(int i = pivotLength; i < ArraySize(rates)-pivotLength; i++) {
      if(IsSwingLow(rates, i, pivotLength)) {
         AddUniqueLevel(supportLevels, rates[i].low, _Point*10);
      }
      if(IsSwingHigh(rates, i, pivotLength)) {
         AddUniqueLevel(resistanceLevels, rates[i].high, _Point*10);
      }
   }
   
   // Sort and select top 3 levels
   ArraySort(supportLevels);
   ArraySort(resistanceLevels);
   
   // Draw support levels
   for(int i = 0; i < MathMin(3, ArraySize(supportLevels)); i++) {
      CreateHorizontalLine("Support"+IntegerToString(i), supportLevels[i], clrBlue);
   }
   
   // Draw resistance levels
   for(int i = ArraySize(resistanceLevels)-1; i >= MathMax(ArraySize(resistanceLevels)-3, 0); i--) {
      CreateHorizontalLine("Resistance"+IntegerToString(i), resistanceLevels[i], clrOrange);
   }
}
//+------------------------------------------------------------------+

// Helper functions
bool IsSwingLow(const MqlRates &rates[], int index, int range)
{
   for(int i = 1; i <= range; i++) {
      if(rates[index].low > rates[index-i].low || rates[index].low > rates[index+i].low)
         return false;
   }
   return true;
}

bool IsSwingHigh(const MqlRates &rates[], int index, int range)
{
   for(int i = 1; i <= range; i++) {
      if(rates[index].high < rates[index-i].high || rates[index].high < rates[index+i].high)
         return false;
   }
   return true;
}

void AddUniqueLevel(double &levels[], double value, double threshold)
{
   for(int i = 0; i < ArraySize(levels); i++) {
      if(MathAbs(levels[i] - value) < threshold)
         return;
   }
   ArrayResize(levels, ArraySize(levels)+1);
   levels[ArraySize(levels)-1] = value;
}

void CreateTrendLine(string name, datetime t1, double p1, datetime t2, double p2, color clr)
{
   ObjectDelete(0, name);
   ObjectCreate(0, name, OBJ_TREND, 0, t1, p1, t2, p2);
   ObjectSetInteger(0, name, OBJPROP_COLOR, clr);
   ObjectSetInteger(0, name, OBJPROP_WIDTH, 2);
}

void CreateHorizontalLine(string name, double price, color clr)
{
   ObjectDelete(0, name);
   ObjectCreate(0, name, OBJ_HLINE, 0, 0, price);
   ObjectSetInteger(0, name, OBJPROP_COLOR, clr);
   ObjectSetInteger(0, name, OBJPROP_WIDTH, 1);
}