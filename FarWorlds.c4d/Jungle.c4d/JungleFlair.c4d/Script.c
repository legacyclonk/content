/*-- Urwaldlaute --*/

#strict

protected func Initialize() { SetAction("Initialize"); }

protected func Initialized()
{
  // alle Büsche dunkler färben
  var pBush;
  while(pBush = FindObject(BUSH,0,0,0,0,0,0,0,0,pBush))
    SetClrModulation(RGB(192,192,127),pBush);
  
  // einige Bäume in den Vordergrund verschieben
  var pPalm;
  while(pPalm = FindObject(0,0,0,0,0,OCF_Chop(),0,0,0,pPalm))
    if(!Random(2))
      SetCategory(C4D_Foreground()|GetCategory(pPalm),pPalm);

  SetAction("Sound");
  SetPhase(Random(20));
}

private func DoSound() { if(!Random(8)) Sound("Jungle*",1); }