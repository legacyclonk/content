/*-- Eisball --*/

// IceStrikeFlight:
//  EffectVar(0): Größe
//  EffectVar(1): Zielgröße
//  EffectVar(2): Richtungswinkel
//  EffectVar(3): Abschuss frei? Wenn ja: Abschussframe+1
//  EffectVar(4): Caster (Für Positionierung)
//  EffectVar(5): Drehgeschwindigkeit

static const ICEB_Speed          = 40,
             ICEB_AimSearchRange = 200,
             ICEB_AimDirAdjust   = 20;

#strict

protected func Initialize()
  {
  // Feuerballeffekt zur Steuerung
  var iResult = AddEffect("IceStrikeFlight", this(), 20, 1, this(), ICEB, 1, 100, 0);
  // Effekt abgelehnt?
  if (iResult <= 0) return(RemoveObject());
  // Flugaktivität
  SetAction("Fly");
  return(1);
  }

protected func Hit()
  {
  var iSize=EffectVar(0,this(),GetEffect("IceStrikeFlight", this()));
  var pCaster=EffectVar(4,this(),GetEffect("IceStrikeFlight", this()));
  if(!iSize) return();
  for( var i=20; i>0; --i)
    {
    CreateParticle("Snow", RandomX(-2,2), RandomX(-2,2), Sin(Random(360), RandomX(50,65))+RandomX(-2,2), Cos(Random(360), RandomX(50,65))+RandomX(-2,2), iSize+Random(55), RGBa(0,0,Random(255),90));
    CreateParticle("NoGravSpark",RandomX(-2,2), RandomX(-2,2), Sin(Random(360), RandomX(50,65))+RandomX(-2,2), Cos(Random(360), RandomX(50,65))+RandomX(-2,2), iSize*60/100+Random(15), RGB(10,120,210));   
    }
  AddEffect("FrostwaveNSpell",0,130,1,0,GetID(),GetX(),GetY(),pCaster);
  RemoveObject();
  }

public func FxIntFadeTimer(pTarget, iNummer)
{
  EffectVar(0, pTarget, iNummer)++;
  SetClrModulation(RGBa(255,255,255,EffectVar(0, pTarget, iNummer)*2)); 
  if(EffectVar(0, pTarget, iNummer)>100) RemoveObject(pTarget);
}

public func ControlLeft()
{
  // Bei Klassisch für jedes Mal drücken etwas schneller drehen
  if(!GetPlrJumpAndRunControl(GetController()))
  {
    if(EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))>0)
      EffectVar(5, this(), GetEffect("IceStrikeFlight", this())) = 0;
    if(EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))>-5)
      EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))--;
  }
  // JnR gleich volle Drehungsgeschwindigkeit
  else EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))-=5;
}

public func ControlRight()
{
  // Bei Klassisch für jedes Mal drücken etwas schneller drehen
  if(!GetPlrJumpAndRunControl(GetController()))
  {
    if(EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))<0)
      EffectVar(5, this(), GetEffect("IceStrikeFlight", this())) = 0;
    if(EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))<5)
      EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))++;
  }
  // JnR gleich volle Drehungsgeschwindigkeit
  else EffectVar(5, this(), GetEffect("IceStrikeFlight", this()))+=5;
}

// Die restlichen Tasten mit Stop belegen, damit Klassisch auch gerade fliegen kann
public func ControlDig()
{
  EffectVar(5, this(), GetEffect("IceStrikeFlight", this())) = 0;
}

public func ControlUp()
{
  EffectVar(5, this(), GetEffect("IceStrikeFlight", this())) = 0;
}

public func ControlDown()
{
  EffectVar(5, this(), GetEffect("IceStrikeFlight", this())) = 0;
}

// Bei JnR wenn die Taste losgelassen wird auch keine Drehung mehr
public func ControlLeftReleased()
{
  EffectVar(5, this(), GetEffect("IceStrikeFlight", this())) = 0;
}

public func ControlRightReleased()
{
  EffectVar(5, this(), GetEffect("IceStrikeFlight", this())) = 0;
}

// Werfen löst den Eisschlag aus
public func ControlThrow() { Hit(); }

public func SetCaster(object pToCaster)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("IceStrikeFlight", this()), "SetCaster", pToCaster));
  }
  
public func SetAngle(int iToAngle)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("IceStrikeFlight", this()), "SetAngle", iToAngle));
  }

public func AimAngle(int iToAngle)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("IceStrikeFlight", this()), "AimAngle", iToAngle));
  }

public func Abort()
  {
  // Abbruch: Objekt entfernen (Entfernt den Effekt gleich mit)
  CastParticles("Fire", 7, 25, 0, 0, 10, RGBa(0,0,0,50), RGBa(255,0,0,90));
  RemoveObject();
  }

public func FxIceStrikeFlightSetAngle(object pTarget, int iEffectNumber, int iAngle)
  {
  // Winkel setzen; Abschuss freigeben
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  EffectVar(3, pTarget, iEffectNumber)=GetEffect(0, pTarget, iEffectNumber, 6)+1;
  return(1);
  }

public func FxIceStrikeFlightAimAngle(object pTarget, int iEffectNumber, int iAngle)
  {
  // Winkel aktualisieren
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  return(1);
  }

public func FxIceStrikeFlightSetCaster(object pTarget, int iEffectNumber, object pCaster)
  {
  // Caster setzen
  EffectVar(4, pTarget, iEffectNumber)=pCaster;
  return(1);
  }
  
func FxIceStrikeFlightStart(object pTarget, int iEffectNumber, int iTemp, int var1, int var2, int iAngle, bool fLaunchFree)
{
  if(iTemp) return();
  EffectVar(0, pTarget, iEffectNumber)=var1;
  if(var2)
    EffectVar(1, pTarget, iEffectNumber)=var2;
  else
    EffectVar(1, pTarget, iEffectNumber)=var1;
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  EffectVar(3, pTarget, iEffectNumber)=fLaunchFree;
}

func FxIceStrikeFlightTimer(pTarget, iEffectNumber, iEffectTime)
  {
  var iSize=EffectVar(0, pTarget, iEffectNumber);
  var iMaxSize=EffectVar(1, pTarget, iEffectNumber);
  var iAngle=EffectVar(2, pTarget, iEffectNumber)+=EffectVar(5, pTarget, iEffectNumber);
  var iLaunchFree=EffectVar(3, pTarget, iEffectNumber);
  var pCaster = EffectVar(4, pTarget, iEffectNumber);
  if(iSize<=0) return(RemoveObject());
  // Effekt
  for( var i=1; i>0; --i)
    {
    CreateParticle("NoGravSpark",RandomX(-iSize/50,iSize/50), RandomX(-iSize/50,iSize/50), +RandomX(-2,2), +RandomX(-2,2), iSize*60/100+Random(15), RGB(10,RandomX(40,140),RandomX(250, 100)));   
    CreateParticle("Freeze",RandomX(-iSize/50,iSize/50), RandomX(-iSize/50,iSize/50), +RandomX(-2,2), +RandomX(-2,2), iSize*60/100+Random(15), RGB(10,RandomX(250,100),RandomX(40,140)));   
    }
  // Ausrichtung nach Caster
  if (!iLaunchFree && pCaster)
    {
    var x=GetX(pCaster)+Sin(iAngle, 15), y=GetY(pCaster)-Cos(iAngle, 15);
    if (GetX()!=x || GetY()!=y)
      {
      DrawParticleLine("Snow", x-GetX(), y-GetY(), 0, 0, 5, 40, 16777215, 16777215, 0);
      SetPosition(x, y);
      }
    }
  // Noch nicht voll aufgeladen?
  if (iSize < iMaxSize)
    {
    for( var i=5; i>0; --i)
      {
      var iR, iD;
      var iX=Sin(iR=Random(360), iD=RandomX(iSize/10,iSize/5));
      var iY=Cos(iR, iD);
      CreateParticle("Snow", iX, iY, -iX/3, -iY/3, Random(25), RGBa(Random(255),0,0,90));
      CreateParticle("Snow", iX, iY, -iX/3, -iY/3, Random(35));
      }
    // Vergrößern
    EffectVar(0, pTarget, iEffectNumber) = iSize+=10;
    // Nichts weiter während des Wachsens
    return(1);
    }
  // Abschuss noch nicht freigegeben?
  if (!iLaunchFree) return(1);
  // Maximale Laufzeit
  if(iEffectTime-iLaunchFree>550) return(RemoveObject());
  // Geschwindigkeit anpassen
  SetXDir(Sin(iAngle, ICEB_Speed));
  SetYDir(-Cos(iAngle, ICEB_Speed));
  // Suche nach Zielen
  var pObj;
  for(pObj in FindObjects(Find_AtPoint(0,1),Find_OCF(OCF_CrewMember),Find_NoContainer(),Find_Layer(GetObjectLayer())))
    {
    if(GetOCF(pObj) & OCF_CrewMember) if (GetAlive(pObj)) return(Hit());
    }
  // OK; weiter existieren
  return(1);
  }

func FxIceStrikeFlightStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
  {
  if (!fTemp) RemoveObject(pTarget);
  return(1);
  }


func FxFrostwaveNSpellStart(object pTarget, int iNumber, int iTemp, int iX, int iY, object pCaller) {
  if(iTemp) return();

  // Richtung speichern
  EffectVar(1,pTarget,iNumber) = 5;

  // Position
  EffectVar(2,pTarget,iNumber) = iX;
  EffectVar(3,pTarget,iNumber) = iY;
  
  // Zauberer
  EffectVar(4,pTarget,iNumber) = pCaller;
}

func FxFrostwaveNSpellTimer(object pTarget, int iNumber, int iTime) {

  var iRaduis = EffectVar(1,pTarget,iNumber);
  // löschen falls schon zu weit alles weggeblasen
  if(iRaduis >= EffectCall(pTarget,iNumber,"MaxRange")) return(-1);


  var iX = EffectVar(2,pTarget,iNumber);
  var iY = EffectVar(3,pTarget,iNumber);
  
  CreateParticle("FrostWave",iX,iY, RandomX(-1,1),RandomX(-1,1), 400+10*iRaduis,RGB(255,255,255));
  var obj;

  var angle;
  while(obj = FindObject(0, -iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10, OCF_Alive, 0,0, NoContainer(), obj))
  {
    if(obj == Contained()) continue;
    if(Inside(Distance(GetX(obj), GetY(obj), iX, iY), iRaduis-5, iRaduis+5))
    {   
        angle=Angle(iX, iY, GetX(obj), GetY(obj) );     
        Fling(obj, Sin(angle, 80)/9, -Cos(angle, 80)/9);
        if(!Frozen(obj)) Freeze(obj, EffectVar(4,pTarget,iNumber));
    }
  } 
    
  EffectVar(1,pTarget,iNumber)+=8;
}

func FxFrostwaveNSpellMaxRange() { return(70); }
