/* Wiederbelebung */

#strict

/* Locals */

local clonk, obj_cnt, iDestPlr;
// Locals: Wiederbelebte Objekte


/* Aktivierung */

public func Activate(caster, real_caster)
  {
  // Zaubernden Clonk ermitteln
  clonk = caster;
  if (real_caster) clonk = real_caster;
  if (!clonk) return(0, RemoveObject());

  // Clonks ermitteln, die wiederbelebt werden sollen
  if (!AssignReviveClonks()) return (0, Sound("Error"), Message("$MsgNoCorpses$", clonk), RemoveObject());

  // Menü mit allen Spielern erzeugen (keine Spieler, die gerade relaunchen..., oder noch nicht in der Runde sind)
  CreateMenu(GetID(), clonk, this(), 0, "$MnuPlrSel$", 0, 1);
  for (var i,j=GetPlayerCount(), sName; j; ++i)
    if (sName=GetPlayerName(i))
      {
      --j;
      if (!Hostile(GetOwner(clonk), i))
        if (!FindObjectOwner(_RLN, i))
          if (!FindObjectOwner(_KSL, i))
            AddMenuItem(sName, "MnuSelectDestPlr", GetID(GetHiRank(i)), clonk, GetRank(GetHiRank(i)), i, Format("$MnuPlrInfo$", sName), 1);
      }

  // Noch ist kein Spieler gewählt
  iDestPlr = -1;
  
  // Menü erzeugt?
  if (GetMenu(clonk) != GetID()) return(0, RemoveObject());

  // OK, Effekt starten
  dwMod=16777215;
  Sound("Magic*");
  return(SetAction("BeginEffect"));
  }

protected func MnuSelectDestPlr(idType, iPlr)
  {
  // Spieler setzen
  if (!GetPlayerName(iPlr)) return(RemoveObject());
  iDestPlr = iPlr;
  }

private func AssignReviveClonks()
  {
  var obj;
  while (obj = FindObject(0, -100, -100, 200, 200, OCF_Living(), 0,0, 0, obj))  
    if (ObjectDistance(this(), obj) <= 100)
      if (!GetAlive(obj) && obj->~IsClonk())
        Local(obj_cnt++) = obj;
  return(obj_cnt);
  }

protected func MenuQueryCancel()
  {
  return(RemoveObject());
  }


/* Wiederbelebung */

local dwMod;

protected func Effect1()
  {
  // Effekt1: Wiederzubelebende Clonks verdunkeln
  var i=obj_cnt, obj;
  while (i--)  if (obj=Local(i))
    {
    SetClrModulation(dwMod=DarkenClr(dwMod), obj);
    SetColorDw(DarkenClr(GetColorDw(obj)), obj);
    }
  }

private func DarkenClr(dwClr)
  {
  var dwClr2 = Max((dwClr>>16 & 255)-16)<<16 | Max((dwClr>>8 & 255)-16)<<8 | Max((dwClr&255)-16);
  if (!dwClr2) ++dwClr2;
  return (dwClr2);
  }

protected func Effect2()
  {
  // Dauereffekt
  var i=10, a;
  while (i--) CreateParticle("MSpark", Sin(a=Random(360), 100), Cos(a, 100), 0,-10, 20, RGBa(255,127,63,100));
  return(1);
  }

protected func CheckChoice()
  {
  // Warten, bis die Auswahl getroffen wurde
  if (iDestPlr<0)
    if (GetMenu(clonk) != GetID()) return(Abort()); else return();
  // Der Zielspieler könnte inzwischen ungültig sein?
  if (FindObjectOwner(_RLN, iDestPlr) || FindObjectOwner(_KSL, iDestPlr) || !GetPlayerName(iDestPlr)) return(Abort());
  // OK, Wiederbelebung ausführen
  var i=obj_cnt, obj;
  while (i--)  if (obj=Local(i))
    {  
    if (Local(i) = Resurrect(obj))
      CastParticles("MSpark", 20, 5, GetX(obj)-GetX(),GetY(obj)-GetY(), 10, 50, GetColorDw(clonk), RGBa(255,255,255,127));
    }
  // Effekt
  SetAction("Resurrect");
  }

private func Resurrect(pDeadClonk)
  {
  // Toter Clonk ist wirklich tot? (Könnte vom anderen Magier schon wiederbelebt worden sein)
  if (GetAlive(pDeadClonk)) return();
  // Neuen Clonk dort erzeugen
  var pNewClonk = CreateObject(GetID(pDeadClonk), 0,0, iDestPlr);
  if (!pNewClonk) return();
  SetPosition(GetX(pDeadClonk), GetY(pDeadClonk), pNewClonk);
  // Wenn der Clonk eine Rüstung hatte: Übertragen
  if (pDeadClonk->~HasArmor()) pNewClonk->SetArmored(1);
  // Objekt nach der Leiche ausrichten
  SetDir(GetDir(pDeadClonk), pNewClonk);
  // Auferstehen
  ObjectSetAction(pNewClonk, "FlatUp");
  // In die Crew einfügen
  MakeCrewMember(pNewClonk, iDestPlr); 	
  // Volle Energie gibts erstmal nicht
  // Leiche entfernen
  RemoveObject(pDeadClonk);
  // Kleiner Effekt ;)
  Sound("Scream", 0, pNewClonk);
  // Fertig
  return(pNewClonk);
  }

private func Abort()
  {
  var i=obj_cnt, obj;
  while (i--)  if (obj=Local(i))
    {
    SetClrModulation(0, obj);
    SetColorDw(GetColorDw(GetHiRank(GetOwner(obj))), obj);
    }
  return(RemoveObject());
  }

protected func Effect3()
  {
  // Auferstandene Clonks mit einem Effekt beglücken
  var i=obj_cnt,obj;
  while (i--)  if (obj=Local(i))
    CastParticles("MSpark", 20, 5+GetActTime()*2, GetX(obj)-GetX(),GetY(obj)-GetY()-10-GetActTime()*2, 10, 50, GetColorDw(clonk), RGBa(255,255,255,127));
  return(1);
  }

protected func Remove() { return(RemoveObject()); }
