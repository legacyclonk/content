
#strict

local iDone, oKey, fOutro;

static const KEY_MAXKEYS = 7;

func Initialize()
{
  SetAction("Warp");
  SetRDir(2);
	// Liste der Schlüssel
	Local(0) = _KE1;
	Local(1) = _KE2;
	Local(2) = _KE3;
	Local(3) = _KE4;
	Local(4) = _KE5;
	Local(5) = _KE6;
	Local(6) = _KE7;
	
// Anfängliche Zielnachricht
FindObject(SCRG)->SetMessages("$DescWon$", "$DescGoToPortal$");
}

protected func Timer()
{
	// Definitionswechsel prüfen (toter Fisch)
	if(oKey) if(GetID(oKey) != Local(iDone)) oKey = 0;
	
	if(oKey)
	{
		var d = ObjectDistance(oKey);
		// Nah genug?
		if(d < 20)
			Absorb(oKey);
		else if(d < 100)
		{
			// Anziehen
			var a = Angle(GetX(), GetY(), GetX(oKey), GetY(oKey));
			oKey->SetXDir(oKey->GetXDir() - Sin(a, 10));
			oKey->SetYDir(oKey->GetYDir() + Cos(a, 10));
		}
	}
	
	// Jemand da, den die aktuelle Mission interessiert?
	if(FindObject(0, -100, -100, 200, 200, OCF_CrewMember))
	{
		if(!oKey) CreateKey();
		if(oKey) oKey->Announce(this());
	}
}

private func CreateKey()
{
	var idKey = Local(iDone);
	// Fertig?
	if(!idKey) { Done(); return(); }
	// Schlüssel erzeugen
	oKey = CreateObject(Local(iDone));
	if(!oKey->Init(this()))
		RemoveObject(oKey);
	else
		FindObject(SCRG)->SetMessages("$DescWon$", Format("$DescFindKey$|{{%i}} %s", idKey, idKey->GetKeyDesc()));
}

private func Absorb(object oKey)
{
	// Schlüssel abgeliefert
	if(oKey) RemoveObject(oKey);
	iDone++;
	// Effekte...
	for(var j = 0; j < 2000; j++) {
		var iAngle = Random(360), iDist;
		for(var i = 0; i < 10; i++) iDist += Random(200); iDist = Abs(iDist - 1000) + 10;
		CreateParticle("PSpark", Cos(iAngle, iDist), Sin(iAngle, iDist), Cos(iAngle+30, Min(iDist/10, 50)),Sin(iAngle+30, Min(50, iDist/10)), 5*5+Random(5*5), RGBa(255,255,255,0));     
	}
	// Symbol erzeugen
	CreateKeySymbol(iDone-1);
	// In allen Spielern speichern
	for(var i = 0, cnt = 0; cnt < GetPlayerCount(); i++)
		if(GetPlayerName(i))
		{
			SetPlrExtraData(i, "SevenKeys_KeysDone", Max(iDone, GetPlrExtraData(i, "SevenKeys_KeysDone")));
			cnt++;
		}
		
	// Zielbeschreibung: Wieder zum Portal gehen
	FindObject(SCRG)->SetMessages("$DescWon$", "$DescGoToPortal$");
}

private func CreateKeySymbol(int iKey)
{
	var iAngle = (iKey-1) * 360 / KEY_MAXKEYS;
	var x = Sin(iAngle, 50), y = -Cos(iAngle, 50);
	var oKeyFound = CreateObject(_FKE, x, y, -1);
	oKeyFound->SetPosition(x+GetX(),y+GetY());
	oKeyFound->InitAsKey(iKey);
	DrawParticleLine("NoGravSpark", 0,0,x,y, 10, 50, 0, 16777215);
	return(oKeyFound);
}

protected func SkipKey(int iKey)
{
	// Nicht wenn gerade einer aktiv ist
	if(oKey) return();
	// Ansonsten: Setzen
	while (iDone < iKey) CreateKeySymbol(iDone++);
}

/* Outro */

local iEndAngle0, iEndProgress, oEndFB;

private func Done()
{
	if (fOutro) return();
	fOutro=true;
	iEndProgress=0;
	ScheduleCall(this(), "End1", 2);
	GainMissionAccess("StormPortal");
}

protected func End1()
  {
  // Schlüssel anziehen
  var o,d,n;
  ++iEndProgress;
  while (o = FindObject(_FKE, 0,0,0,0, 0, 0,0, 0, o))
    if ((d=ObjectDistance(o)) > 10)
      {
      var a = Angle(GetX(), GetY(), GetX(o), GetY(o))+Max(d-50);
      o->SetCategory(C4D_Vehicle);
      o->SetXDir(-Sin(a, 120-d));
      o->SetYDir(Cos(a, 120-d));
      ++n;
      }
    else
      {
      o->SetXDir();
      o->SetYDir();
      }
  if (n && iEndProgress<80) return(ScheduleCall(this(), "End1", 2));
  // Alle Schlüssel da: Feuerball strten
  SetPosition(GetX(), GetY(), o = CreateObject(WWNG,0,0,-1));
  Explode(20, o); RemoveAll(_FKE);
  FindObject(TIME)->SetTime(40);
  SetAction("WarpS");
  SetCategory(C4D_Structure);
  SetRDir(4);
  (oEndFB=CreateObject(_FKE, 0,10, -1))->InitAsFireball();
  oEndFB->SetCategory(C4D_Vehicle);
  ScheduleCall(this(), "End2", 2);
  }

protected func End2()
  {
  // Noch auf dem Weg nach draußen?
  var o=oEndFB,d,a = Angle(GetX(), GetY(), GetX(o), GetY(o));
  if ((d=ObjectDistance(o)) < 48)
    {  
    SetXDir(Sin(a+d, 2000),o,1000);
    SetYDir(-Cos(a+d, 2000),o,1000);
    return(ScheduleCall(this(), "End2", 2));
    }
  // Kreis ziehen
  iEndAngle0=a;
  iEndProgress=0;  
  o->SetPermanentFire();
  End3();
  }  

protected func End3()
  {
  // Kreis ziehen
  var o=oEndFB,a = Angle(GetX(), GetY(), GetX(o), GetY(o));
  SetXDir(Cos(a, 2000),o,1000);
  SetYDir(Sin(a, 2000),o,1000);
  if (++iEndProgress > 30)
    if ((a+360-iEndAngle0) % 360 < 10)
      return(End4(o));
  return(ScheduleCall(this(), "End3", 2));
  }
  
protected func End4(o)
  {
  RemoveObject(o);
  Sound("Thunder1", true);
  SetAction("Portal");
  var x=GetX(), y=GetY();
  DoCon(150);
  SetPosition(x,y);
  ScheduleCall(this(), "End5", 20);
  }
  
protected func End5()
  {
  SetClrModulation(1);
  SetWind();
  var o =CreateObject(BLMP,0,0,-1);
  o->SetClrModulation(RGBa(255,255,255,254));
  o->FadeIn(1,2);
  ScheduleCall(this(), "End6", 100);
  }
  
protected func End6()
  {
  Message("@$TxtGoPortal$", this());
  return(SetAction("EndCheck"));
  }
  
protected func EndChecking()
  {
  var oBlmp = FindObject(BLMP,0,0,-1,-1);
  if (ObjectDistance(oBlmp) > 30) return();
  var oC;
  if (!(oC = FindObject(0, 0,0,0,0, OCF_CrewMember, "Push", oBlmp))) return();
  oC->FadeOut(1,2);
  oBlmp->FadeOut(1,2);
  ScheduleCall(this(), "End7", 100);
  Sound("Applause", true);
  Message("@$TxtWin$", this());
  return(SetAction("Portal"));
  }
  
protected func End7()
  {
  if (ObjectCount(SCRG)) FindObject(SCRG)->Fulfill();
  // Spielziel: Missionspasswort (falls es spaeter noch mehr Missionen gibt?)
  GainMissionAccess("PortalOpen");
  return(GameOver(/*Format("C:")*/));
  }
  
  
/* Fade: Objekte nach sichtbar/unsichtbar faden */

global func FadeIn(int iTimer, int iStep, object pObj)
  {
  // Lokaler Aufruf/Sicherheit
  if (!pObj) if (!(pObj=this())) return();
  if (!iTimer) iTimer=1;
  if (!iStep) iStep=1;
  // Faden
  return(AddEffect("IntFade", pObj, 2, iTimer, 0, 0, -iStep));
  }

global func FadeOut(int iTimer, int iStep, object pObj)
  {
  // Lokaler Aufruf/Sicherheit
  if (!pObj) if (!(pObj=this())) return();
  if (!iTimer) iTimer=1;
  if (!iStep) iStep=1;
  // Faden
  return(AddEffect("IntFade", pObj, 2, iTimer, pObj, 0, iStep));
  }

global func FxIntFadeStart(object pTarget, int iEff, int iTemp, int iFadeDir)
  {
  if (iTemp) return();
  EffectVar(0, pTarget, iEff) = iFadeDir;
  }

global func FxIntFadeTimer(object pTarget, int iEff)
  {
  // Alpha aufzählen
  var dwMod = pTarget->GetClrModulation();
  var iAlpha = BoundBy((dwMod>>24&255) + EffectVar(0, pTarget, iEff), 0, 255);
  if (iAlpha && !dwMod) dwMod=16777215;
  pTarget->SetClrModulation(dwMod & 16777215 | (iAlpha<<24));
  // Ende?
  if (!iAlpha) return(-1);
  if (iAlpha==255) return(-1, RemoveObject(pTarget));
  }

global func FxIntFadeEffect(string szNewEffect, object pTarget, int iEff)
  {
  if (szNewEffect eq "IntFade") return(-2);
  }

global func FxIntFadeAdd(object pTarget, int iEff, string szName, int iNewTimer, int iFadeDir)
  {
  // Fading ändern
  EffectVar(0, pTarget, iEff) = iFadeDir;
  ChangeEffect(0, pTarget, iEff, "IntFade", iNewTimer);
  }
  