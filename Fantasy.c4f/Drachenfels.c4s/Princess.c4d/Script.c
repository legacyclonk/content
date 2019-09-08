#strict

#include CLNK

protected func Timer() // Tick100
  {
  if (!Random(5))  if (FindObject(_CAG, 0,1)) if (FindObject(_CAG, 0,1)->GetAction() ne "Open") Message("Hilfe!", this());
  }
