#strict

local idKnow;

public func Selection()
{
  // wer bin ich? der da!
  if(idKnow) PlayerMessage(GetOwner(Contained()),"{{%i}} %s",Contained(),idKnow,GetName(0,idKnow));
}

protected func Entrance(object pContainer) { if(GetOCF(pContainer) & OCF_CrewMember()) Selection(); }

protected func Activate(object pByClonk)
{
  if(!idKnow) MagicMenu(pByClonk);
  else DoPlan(pByClonk);
  return(1);
}

private func MagicMenu(object pByClonk) {
  // Nicht bereit
  if(GetAction(pByClonk) ne "Walk") return(0);
  // Clonk soll anhalten
  SetComDir(COMD_Stop(), pByClonk);
  // Menü öffnen
  CreateMenu(MCMS,pByClonk, 0, 0, "$MnuNoPlans$");
  var idPlan, i = 0;
  while (idPlan = GetPlrKnowledge(GetOwner(pByClonk), 0, i++, C4D_Structure())) 
    AddMenuItem("$MnuPlan$: %s", "SetPlan", idPlan, pByClonk,0,pByClonk);
  i = 0;
  while (idPlan = GetPlrKnowledge(GetOwner(pByClonk), 0, i++, C4D_Vehicle())) 
    AddMenuItem("$MnuPlan$: %s", "SetPlan", idPlan, pByClonk,0,pByClonk);
  i = 0;
  while (idPlan = GetPlrKnowledge(GetOwner(pByClonk), 0, i++, C4D_Object())) 
    AddMenuItem("$MnuPlan$: %s", "SetPlan", idPlan, pByClonk,0,pByClonk);
}

private func SetPlan(id idPlan, object pByClonk) {

  SetKnowledge(idPlan);

  // Effekte!
  CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pByClonk)));

  // Nachricht!
  PlayerMessage(GetOwner(pByClonk),"$MsgPlanSet$", pByClonk, GetName(0,idPlan));
}

public func SetKnowledge(id idPlan) {
  // Rolle ist nun beschrieben
  SetGraphics("Full");

  // Grafik setzen
  SetGraphics(0,0,idPlan,1,GFXOV_MODE_Picture);
  SetObjDrawTransform(350,-300,3000,150,500,-2000, this(), 1);

  // Aktion für die Schriftrolle setzen
  SetAction("Be");

  idKnow = idPlan;
  
  // Name setzen
  UpdateName();
}

private func DoPlan(object pByClonk) {
  // Zauber
  if(GetPlrKnowledge(GetOwner(pByClonk), idKnow)) return(1, Message("$HasAlreadyPlan$", pByClonk, GetName(0,idKnow)));
  SetPlrKnowledge(GetOwner(pByClonk), idKnow);
  // Effekte!
  CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pByClonk)));
  // Nachricht!
  PlayerMessage(GetOwner(pByClonk),"$MsgGetPlan$", pByClonk, GetName(0,idKnow));
  var i;
  // Nützlich für Szen's: mehrfachbelegung
  while(Local(i))
    SetPlrKnowledge(GetOwner(pByClonk), Local(i++));
  RemoveObject();
}

protected func UpdateTransferZone()
  {
  // Für Objects.txt-Schriftrollen
  UpdateName();
  }
  
protected func UpdateName()
  {
  // Namen nach Zauber setzen
  if (idKnow)
    SetName(Format("%s: %s", GetName(0, GetID()), GetName(0,idKnow)));
  else
    SetName();
  }
  
