/* Schwarmmine */

#strict

protected func Activate(object pByClonk) {
    [$Activate$]
    ControlDigDouble(pByClonk);
}

public func ControlDigDouble(object pByClonk) { // Clonk aktiviert Mine

    var szAct = GetAction(pByClonk);
    SetOwner(pByClonk->GetOwner());
    // in welche Richtung soll die Mine angeheftet werden?

    if(szAct eq "Walk" || szAct eq "Fight") {
      SetActive(0);
    }
    else if(szAct eq "Hangle") {
      SetActive(180);
    }
    else if(szAct eq "Scale" || szAct eq "ScaleDown") {
      if(GetDir(pByClonk) == DIR_Left)
        SetActive(90);
      else
        SetActive(270);
    }
    
    return(1);
}

public func SetActive(int iAngle, int iMaxDist) {   // Mine wird aktiviert

    if(!iMaxDist) iMaxDist = 15;
    var x=GetX(),y=GetY();
    var mx=-Sin(iAngle,iMaxDist), my=+Cos(iAngle,iMaxDist);

    // nicht platzierbar: abbrechen
    if(PathFree(x,y,x+mx,y+my))
      return(false);

    // ansonsten platzieren
    var i=0;
    Exit();
    SetR(iAngle);
    while(!Stuck() && iMaxDist>++i) {
      SetPosition(x+mx*i/iMaxDist,y+my*i/iMaxDist);
    }
    if(iAngle == 0)
      SetPosition(GetX(),GetY()-1);
    else {
      SetVertex(1,1,2,0,2);
      SetVertex(2,1,2,0,2);
    }
    SetAction("Activate");

    return(true);
}

protected func Warning() {

    var a = 64;
    if(GetAction() eq "Active")
      a = 160-120*GetDamage()/MaxDamage();
    else
      Sound("Bip");
    CreateParticle("PSpark",Sin(GetR(),3),-Cos(GetR(),3),0,0,
                   60,RGBa(0,64,255,a),this());
}

protected func OnActive() {             // Mine ist nun aktiv
    SetVertex(1,1,0,0,2);
    SetVertex(2,1,0,0,2);
    var xdir, ydir,
    xdir = Sin(GetR(), 30);
    ydir = -Cos(GetR(), 30);

    SetAction("Fly");           // Hüpfen
    SetSpeed(xdir, ydir);
    SetRDir(RandomX(-50,50));
    ScheduleCall(this(), "Swarm", 10);
}

protected func Swarm() {            // Ordnungsgemäß aktiviert, gesprungen; jetzt schwärmen
    for(var i = 0,swarmy ; i < 10 ; i++)
      {
      swarmy = CreateObject(SWRM, 0, 0, GetOwner());
      swarmy -> Activate();
      }
    Sparks(10, RGB(255, 125, 0));           // Sound!!!
    RemoveObject();
}

protected func Damage(int iChange) {            // kriegt Schaden
    if(GetAction() ne "Idle")
      SetClrModulation(RGBa(255,255,255,200-200*GetDamage()/MaxDamage()));
    if(GetDamage() >= MaxDamage())
      Trigger();
}

public func Trigger() {                 // Auslösen!
    ScheduleCall(0,"Triggered",ExploDelay());
}

protected func Triggered() {                // ausgelöst (nach delay)
    Explode(10);
}

protected func RejectEntrance() {
    if(GetAction() ne "Idle") return(true);
}

private func ExploDelay() { return(10); }
private func MaxDamage() { return(30); }

public func IsBulletTarget() { return(true); }
// Status
public func IsEquipment() { return(1); }
public func IsMine() { return(1); }
