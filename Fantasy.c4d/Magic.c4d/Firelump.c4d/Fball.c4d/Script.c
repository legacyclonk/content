/*-- Feuerklumpen --*/

#strict

//#2do: sounds finden

local iBounces,iCAngle, fExplodeOnHit, iTotalBounces, iLastBouncePos;


  /* Object-Calls */

protected func Initialize() // Initialisierung
  {
  Departure();
  SetMaxBounces(RandomX(8,12));
  iCAngle=Random(360);
  }
  
public func SetExplodeOnHit()
  {
  fExplodeOnHit = true;
  }

protected func Departure()  // Austritt
  {
  Incinerate();
  SetAction("Bounce");
  SetRDir(RandomX(-50,50));
  }

protected func Entrance()   // Eintritt
  {
  SetAction("Exist");
  }

protected func Hit()        // Auftreffen
  {
  //Sound
  Sound("LumpHit*");
  // Explosion? Nur nicht an SolidMask
  if (fExplodeOnHit)
    {
    var iBouncePos = GetX() + (GetY()+10)*LandscapeWidth();
    if (iBouncePos != iLastBouncePos)
      Explode(10, CreateObject(COAL,0,0,GetOwner()));
    iLastBouncePos = iBouncePos;
    }
  //irgendwann ist er alle
  if(--iBounces < 0 || ++iTotalBounces >= 80) return(ChangeDef(COAL));

   //einmal auftanken bitte! (in lava)
  if(GetMaterialVal("Incindiary","Material",GetMaterial(0,0)))
     iBounces+=5;

  //Geschwindigkeit neu bestimmen (völlig unkontrolliert herumspringen)
  SetBounceSpeed(RandomX(10,40),RandomX(25,45));
  //drehen
  SetRDir(RandomX(-50,50));
  }


  /* Timer */

protected func Bouncing()       // StartCall in Aktion "Bounce"
  {
  //In Wasser löschen
  if(!OnFire()) ChangeDef(COAL);
  }

protected func Existing()
  {
  var iR,iG;
  iCAngle+=10;
  iR = Sin(iCAngle,24)+24;
  iG = Cos(iCAngle,32)+32;
  SetClrModulation(RGB(iR,iG,64));
  }


  /* Script-Aufrufe */

private func SetBounceSpeed(iXDir,iYDir)    // Abprall
  {
  // normalerweise nach oben springen, doch an der Decke prallt er nach unten ab
  if(GBackSolid(0,-7))      SetYDir(2);
  else                      SetYDir(-iYDir);

  // falls er an eine Wand kommt, abprallen. Sonst chaotisch rumspringen
  if(GBackSolid(+7,0))      SetXDir(-iXDir);
  else if(GBackSolid(-7,0)) SetXDir(+iXDir);
  else                      SetXDir((Random(2)*2-1)*iXDir);
  }

public func SetMaxBounces(iNewBounces)      // Lebenszeit setzen
  {
  iBounces=iNewBounces;
  }
