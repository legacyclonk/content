/*-- Druckwelle --*/

#strict

local fHit,iTime;     // ist irgendwo aufgekommen

private func MaxTime() { return(70); }

public func Start(int iAngle)
{
  iTime=MaxTime();
  DoCon(-90);
  SetAction("Fly");
  SetXDir(+Sin(iAngle,50));
  SetYDir(-Cos(iAngle,50));
  SetR(iAngle);
}

protected func Check()
{

  // größer und durchsichtiger werden
  DoCon(+2);
  iTime-=1+fHit;
  fHit=false;
  SetClrModulation(RGBa(255,255,255,255-iTime*255/MaxTime()));
  
  // Hit-Check & nach Lebewesen suchen
  var iYob = 12*GetCon()/10, iX,iY;
  for(var i=-iYob; i<iYob; i+=10)
  {  
    iX = i;
    iY = -Cos(i*90/iYob,GetCon()/2)+GetCon()/5;
    IncludeAngle(iX,iY);

    if(!iX && !iY) iX=1;

    // nach Lebewesen zum wegschleudern suche'
    var pNext;
    while(pNext = FindObject(0,iX,iY,0,0,OCF_Living(),0,0,NoContainer(),pNext))
      if(!GetEffect("Shockwave",pNext,0,1))
      {
        DoEnergy(-10+GetCon()/10,pNext);
        Drift((100-GetCon()/2)/10,Angle(0,0,GetXDir(),GetYDir()),pNext);
      }
  
    // an was festes geraten -> schneller auflösen
    if(GBackSolid(iX,iY))
      fHit = true;
  }
  
  if(iTime<=0) return(RemoveObject());
}

global func Drift(int iStr, int iAngle, object pTarget)
{
  // global darf er nicht sein (0 != this)
  if(!pTarget) return();
  // Schaden machen
  //DoEnergy(-10,pTarget);
  // wegschleudern
  return(AddEffect("Shockwave",pTarget,100,1,0,SKWV/*GetID()*/,iStr,iAngle));
}

func FxShockwaveStart(object pT, int iNum, int iTemp, int iStr, int iA)
{
  // temporäre Aufrufe ignorieren
  if(iTemp) return();
  
  // Stärke der Schockwelle speichern
  EffectVar(0,pT,iNum)=iStr;
  EffectVar(1,pT,iNum)=iA;
}

func FxShockwaveTimer(object pT, int iNum, int iTime)
{
  // Stärke verringern
  if(!(iTime%12)) EffectVar(0,pT,iNum)--;
  if(!EffectVar(0,pT,iNum)) return(-1);

  // abdriften lassen
  var iStr = EffectVar(0,pT,iNum), iA = EffectVar(1,pT,iNum);
  var iX = GetX(pT), iY = GetY(pT), i;
  while(!Stuck(pT) && (++i)<=iStr)
    SetPosition(iX+Sin(iA,i),iY-Cos(iA,i),pT);
  i-=2;
  SetPosition(iX+Sin(iA,i),iY-Cos(iA,i),pT);
  
  // Richtung anpassen
  if(Inside(iA, 0, 180) )
    SetDir(DIR_Right(), pT);
  else
    SetDir(DIR_Left(), pT);
  //SetXDir(+Sin(iA,iStr),pT);
  //SetYDir(-Cos(iA,iStr),pT);
}

func FxShockwaveEffect(string szNewEffect)
{
  // irgendein anderer Effekt...
  if(szNewEffect ne "Shockwave") return();
  // ansonsten mit aufnehmen
  return(-2);
}

func FxShockwaveAdd(object pT, int iNum, foo, bar, int iStr2, int iA2)
{
  // neue Stärke+Winkel bestimmen:
  var iStr = EffectVar(0,pT,iNum), iA = EffectVar(1,pT,iNum);
  // Als Vektoren addieren
  var iXDir = Sin(iA,iStr)+Sin(iA2,iStr2), iYDir = -Cos(iA,iStr)-Cos(iA2,iStr2);
  
  // neue Stärke ist Betrag des Vektors
  EffectVar(0,pT,iNum) = Sqrt(iXDir*iXDir + iYDir*iYDir);
  // Winkel des Vektors
  EffectVar(1,pT,iNum) = Angle(0,0,iXDir,iYDir);
} 

// x,y: 0 bei Lokalem aufruf
global func IncludeAngle(&x,&y,domul) {
  // Vorgangsweise:
  // x,y => alpha, rad
  // alpha+=GetR();
  // alpha, rad => x,y
  var alpha, rad;
    // Transformation
  alpha=Angle(x,y,0,0);
  rad=(100*domul+100*y)/Cos(alpha,100);
  
    alpha+=GetR();
  
    // Rücktransformation
  y=(Cos(alpha,100)*rad)/100;
  x=-(Sin(alpha,100)*rad)/100;
}
