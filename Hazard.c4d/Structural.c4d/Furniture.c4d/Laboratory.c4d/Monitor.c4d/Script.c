/*-- Monitor --*/

#strict

local pLight;

protected func Initialize() {
  SetAction("Off");
  pLight = AddLightAmbience(15);
  pLight->TurnOff();
  return(1);
}

public func On(int iAction) {
	if(pLight)
	  pLight->TurnOn();
  if(!iAction)
    iAction = Random(GetActions()+1);
  return(SetAction(Format("On%d", iAction)));
}

public func Off() {
	if(pLight)
	  pLight->TurnOff();

	SetGraphics(0,0,0,1);

  return(SetAction("Off"));
}

// Anzahl verfügbarer Animationen
private func GetActions() { return(3); }

/* Kaputtgehen */

public func IsBulletTarget(id def) {
  if(GetAction() ne "Broken") return(1);
  if(def->~NoDecoDamage()) return();
}

protected func Damage() {
  // schon kaputt
  if(GetAction() eq "Broken") return();
  // Licht aus
  if(pLight) RemoveObject(pLight);
  // RUMMS!
  CastObjects(SPRK, 7, 50);
  Sparks(15, RGB(210, 210, 0));
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());

  Sound("CrystalHit*");
  Sound("Blast1");
  // Fliegen!
  SetSpeed(RandomX(-25, 25), RandomX(-45, -35));
  SetRDir(GetXDir()*2);
  // Kaputt
  SetAction("Broken");
  SetPhase(Random(3));
}

private func ChooseImage() {
  var id, defs;
  while(id = GetDefinition(defs++))
    if(!Random(18))
      break;
  if(!id)
    return(SetGraphics(0,0,0,1));

  SetGraphics(0,0,id,1,1);
  SetObjDrawTransform(7000/GetDefWidth(id),0,0,0,5000/GetDefHeight(id),-1000,0,1  );
}

/* EMP */

public func IsMachine()
{
  if(GetAction() ne "Off" && GetAction() ne "Broken")
    return(1);
}

public func EMPShock() { Off(); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetAction() eq "On1")
		extra[GetLength(extra)] = "SetAction(\"On1\")";
	if (GetAction() eq "On2")
		extra[GetLength(extra)] = "SetAction(\"On2\")";
	if (GetAction() eq "On3")
		extra[GetLength(extra)] = "SetAction(\"On3\")";
}
