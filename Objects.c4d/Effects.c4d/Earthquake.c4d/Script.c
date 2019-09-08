/* Erdbeben */

#strict

local iLevel;
local iLifeTime;

func ShakeEarth() {
  if(Random(3)) return(1);

  ShakeObjects(GetX(),GetY(),iLevel);

  var x = GetX()+Random(2*iLevel+1)-iLevel;
  var y = GetY()+Random(2*iLevel+1)-iLevel;
  var step_x = Random(7)-3;
  var step_y = Random(5)-2;
  var i, amount = 35+Random(6);
  while (++i<amount) ShakeFree(x+=step_x,y+=step_y,Random(15));
  
	if(GetActTime()>iLifeTime) return(RemoveObject());	
}

// LaunchEarthquake(GetX(),GetY())
global func LaunchEarthquake(int iX, int iY, int iLevel) {
	var quake=CreateObject(FXQ1,iX-GetX(),iY-GetY(),-1);
	quake->Activate(iLevel);
	return(quake);
}
	
public func Activate(int _iLevel) {
	iLevel=_iLevel;
	if(!iLevel) iLevel=100+Random(101);
	iLifeTime=iLevel/2;
  SetAction("Quake");
  var eff=AddEffect("QuakeEffect",this(),200,1,this());
  EffectVar(0,this(),eff)=iLevel;
	// Schütteleffekt am Anfang und Ende schwächer (Berechnung der Konstanten der Funktion)
  // iStrength(iTime) = a*x - b*x^2 (verlauf einer parabel wimni)
  // Maximalwert: iMaxStrength = iLevel/10
  // Errechnung: a = 4 * iMaxStrength / iTime
  //                       b = a / iTime
	EffectVar(1,this(),eff) = (4*10*iLevel) / (10*iLifeTime);
	EffectVar(2,this(),eff) = (100*EffectVar(1,this(),eff)) / iLifeTime;
	//Log("a: %d b: %d time: %d",EffectVar(1,this(),eff),EffectVar(1,this(),eff), iLifeTime);
  return(1);
	}

func FxQuakeEffectTimer(object pTarget, int iEffectNumber, int iTime) {
	var iPlr, iLevel;
	var a=EffectVar(1,pTarget,iEffectNumber), b=EffectVar(2,pTarget,iEffectNumber);
  for(var i=0; i<GetPlayerCount(); i++) {
    iPlr=GetPlayerByIndex(i);
		// Schütteleffekt ist am Anfang und Ende des Erdbebens schwächer
		iLevel=(a*iTime-(b*iTime**2)/100)/10;
    // Schütteleffekt verringert sich je nach Entfernung
    iLevel=(300*iLevel)/Max(300,Distance(GetX(GetCursor(iPlr)),GetY(GetCursor(iPlr)),GetX(),GetY()));
    SetViewOffset(iPlr,Sin(iTime*100,iLevel),Cos(iTime*100,iLevel));
  }
  if((a*iTime-(b*iTime**2)/100)/10<0) return(-1);
}