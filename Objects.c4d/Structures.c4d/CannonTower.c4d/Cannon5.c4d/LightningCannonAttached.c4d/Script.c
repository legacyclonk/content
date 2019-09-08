/*--- Blitzgeschütz (angeschraubt) ---*/

#strict

local energy;

#include CTW2

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(ACT3); }

private func EnergyPerShot() { return(350); } // Energiebedarf für einen Schuss

/* Turm weg */

public func ComEnter(object pObj)
{
  // Batterien
  if(GetID(pObj) == ADM3) return(ConvertBattery(pObj) );
  // Kristalle
  if(GetID(pObj) == CRYS)
  {
    Sound("Electric");
    Enter(this(), pObj);
  }
}

private func EnergyTransfer()
{
  if(GetEnergy(GetActionTarget()) > 9) DrainEnergy(GetActionTarget(), 10);
}

private func DrainEnergy(object pTower, int iLevel)
{
  // Energie vom Turm abziehen
  DoEnergy(-iLevel, pTower);
  energy += iLevel;
  if(energy > EnergyPerShot() )
    ConvertEnergy();
}

private func ConvertEnergy()
{
  Sound("LightningLoad");
  CreateContents(CRYS);
  energy -= EnergyPerShot();
}
   
private func ConvertBattery(object pBattery)
{
  // Sounds
  Sound("Connect");
  Sound("LightningLoad");
  for(var i = 0; i < 5; ++ i)
    CreateContents(CRYS);
  RemoveObject(pBattery);
}

/* Blitzumwandlung */

public func LightningStrike()
{
  // Nur wenn fertig gebaut
  if(GetCon() < 100) return(0);
  // Energiegewinnung
  CreateContents(CRYS);
  // Sound
  Sound("Electric");
}

public func ComFire(object pClonk)
{
  // SetRDir(0)
  SetRDir(0);
  SetAction("Attaching",GetActionTarget());
  
  // Pruefen, ob genug Kristalle zur Verfuegung stehen
  var power;
  var i = 0, obj;
  
  while(obj = Contents(i ++) )
    if(GetID(obj) == CRYS)
    {
      power += (GetCon(obj) - 50);
      if(power >= 50)
        break;
    }
  if(power < 50) return(Sound("Error") );
  
  // Kristalle verbrauchen. Beim Pruefen konnten die Kristalle
  // nicht gleichzeitig verbraucht werden, da, wenn die Energie
  // nicht ausreicht, eventuell bereits Kristalle geloescht worden waeren.
  power = 0;
  // Von oben nach unten zaehlen, da Objekte geloescht werden.
  i = ContentsCount();
  while(obj = Contents(-- i) )
    if(GetID(obj) == CRYS)
    {
      // Kraft des Kristalls zur Gesamtkraft hinzufeugen
      power += (GetCon(obj) - 50);
      // Mehr als noetig?
      if(power > 50)
      {
        // Nur das was noetig ist dem Kristall abziehen
        var con = (GetCon(obj) ) - power;
        DoCon(-con, obj);
      }
      else
        // Reicht noch nicht? Kristall ganz verbrauchen.
        RemoveObject(obj);
        
      // Genug power? Ok.
      if(power >= 50)
        break;
    }

  Sound("LightningCannon");
  
  var lgt1=CreateObject(ADM4, 0, 0, -1);
  lgt1->SetController(pClonk->GetController());
  lgt1->ADM4::Activate(GetX(), GetY(), Sin(GetR(), 18), 4, -Cos(GetR(), 18), 4);
  var lgt2=CreateObject(ADM4, 0, 0, -1);
  lgt2->SetController(pClonk->GetController());
  lgt2->ADM4::Activate(GetX() + Sin(GetR() + 90, 5), GetY() + Cos(GetR() + 90, 5), Sin(GetR(), 18), 4, -Cos(GetR(), 18), 4);
  
  CastParticles("Spark", Random(8) + 4, Random(35), Sin(GetR(), 18), -Cos(GetR(), 18) );
}

