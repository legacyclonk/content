/*-- Katapult --*/

#strict

local iPhase, iFiringPlayer;

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
  SetDir(Random(2));
  iFiringPlayer = NO_OWNER;
}

/* Prozesse */

private func HaltPushers()
{
  var pClonk;
  // Alle Clonks die das Katapult anfassen, sollen stoppen
  while(pClonk=FindObject(0,0,0,0,0,0,"Push",this(),0,pClonk))
    SetComDir(COMD_Stop(),pClonk);
}

private func FireSound() 
{ 
  Sound("Catapult"); 
}

private func Charging()
{
  if (GetPhase()<iPhase) return(0);
  // genug geladen
  SetAction("Ready");
  SetPhase(iPhase);
}

public func Fire(bool fAuto)
{ 
  iFiringPlayer = GetController();
  SetAction("Fire");
  SetPhase(7-iPhase);
  // Sicht verfolgen (wenn kein automatischer Schuss)
  if (!fAuto)
    if (GetPlrViewMode(GetController())!=2)
      SetActionTargets(0,GetCursor(GetController()));
  return(1);
}

private func Projectile()
{
  // what if contents got destroyed between animation start and this call?
  // actually, this is funny. let's keep it as an easter egg ;)
  var pProjectile=Contents();
  var iX = (GetDir()*2-1)*12;
  var iY = -10;
  var iR = Random(360);
  var iXDir =  +8 * (GetDir() * 2 - 1) * iPhase / 6;
  var iYDir = -12 * iPhase / 6;
  var iRDir = 30;
  if (pProjectile)
  {
    if (GetOCF(pProjectile) & OCF_Alive) pProjectile->SetKiller(iFiringPlayer);
    pProjectile->SetController(iFiringPlayer);
    Exit(pProjectile,iX,iY,iR,iXDir,iYDir,iRDir);
  }
  iFiringPlayer = NO_OWNER;
  // Zufällige Streuung
  // Bei minimaler Spannung: -30 bis +30
  // Bei voller Spannung:    -80 bis +80
  var iDeviation = RandomX(-iPhase * 10 - 20, iPhase * 10 + 20);
  SetXDir(iXDir * 100 + iDeviation, pProjectile, 100);
  SetYDir(iYDir * 100 + iDeviation, pProjectile, 100);
  // Sicht verfolgen wenn kein automatischer Schuss
  if (GetActionTarget(1))
    SetPlrView(GetOwner(GetActionTarget(1)), pProjectile);
}

private func FireAt(int iTX)
{
  var iX,iY,iXDir,iYDir,iDist=-1,iFavPhase;
  // Nicht bereit
  if(GetAction() ne "Ready" || !GetPhase()) return(0);
  // Richtung anpassen
  if(iTX>GetX()) SetDir(DIR_Right());
  if(iTX<GetX()) SetDir(DIR_Left());
  // Phase aus einer Annäherungsformel ableiten: X = 22*Phase^2
  iPhase = BoundBy( Sqrt(Abs(GetX()-iTX)/22) ,1,6);
  SetPhase(iPhase);
  return(Fire());
}

/* Laden */

protected func Collection() { Sound("Connect"); }

/* Steuerung */

public func ControlCommand(string szCommand, object pTarget, int iX, int iY)
{
  // Feuern mit der Maus
  if (szCommand eq "MoveTo")
    if (Distance(GetX(), GetY(), iX, iY) > 50)
    {
      // Nicht geladen: nur Kommando abfangen
      if (!ContentsCount())
      {
        Sound("Click");
        return(1);
      }
      // Geladen: feuern
      else
      {
        HaltPushers();
        return(FireAt(iX,iY));
      }
    }
}

public func ControlThrow() // Feuern / Inhalt
{
  [$TxtFire$|Image=CAT1:1]
  // Nicht bereit: Clonk kann nichts machen
  if (GetAction() ne "Ready") return(1);
  // Nicht gespannt: Clonk kann nur laden/entladen
  if (!GetPhase()) return(0);
  // Gespannt, aber kein Inhalt: kann nicht feuern (Clonk darf reingucken)
  if (!ContentsCount()) return(0);
  // Gespannt und geladen: feuern (und Throw-Befehl des Clonks abfangen)
  return(Fire());
}

public func ControlConf(int conf)
{
  if(AimStdConf(conf))
  {
    iPhase = GetPhase();
    Sound("CatapultSet");
  }
}

public func ControlDig(object clonk) // Katapult mehr spannen (weiter Schießen)
{
  [$TxtAim1$|Method=Classic|Image=CAT1:0]
  AimDown(clonk, 8, "ControlConf");
}

public func ControlDown(object clonk)
{
  [$TxtAim1$|Method=JumpAndRun|Image=CAT1:0]
}

public func ControlUp(object clonk) // Katapult weniger spannen (kürzer Schießen)
{
  [$TxtAim2$|Image=CAT1:2]
  AimUp(clonk, 8, "ControlConf");
}

public func ControlUpdate(object clonk, int comdir)
{
  AimUpdate(clonk, comdir, 8, "ControlConf");
}

public func ControlDownSingle() // Sicht zurücksetzen
{
  [$TxtResetview$|Method=None]
}
