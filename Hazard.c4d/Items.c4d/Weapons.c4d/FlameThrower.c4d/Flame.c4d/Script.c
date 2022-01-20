/*-- Anzünderobjekt --*/
 
#strict 2

local slimy, iDamage;
local shooter; // Dingens/Clonk das den Schuss abgefeuert hat.
local hits;
local maxlifetime;

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return;
  // Waffe?
  shooter = GetShooter(byObj);
  
  maxlifetime = 50;
  hits = 0;
}


public func MaxLifeTime() { return maxlifetime; }

public func SetMaxLifetime(int max) {
	maxlifetime = max;
}

public func Launch(int xdir, int ydir, bool slime, int iDamagepar)
{
  slimy = slime;
  iDamage = iDamagepar;
  if(!iDamage)
    iDamage = 1;

  SetSpeed(xdir,ydir);
  SetAction("Fly");
  AddEffect("HitCheck", this(), 1,1, 0,GetID(this),shooter);

}


protected func Flying()
{

  if(InLiquid()) {
    // Blubberblubber
    for(var i=0; i<5; ++i) Bubble(RandomX(-5,+5),RandomX(-5,5));
    RemoveObject();
	return;
  }

  if(GetActTime() > MaxLifeTime()-Random(5)) {
    RemoveObject();
    return;
  }
  
  if(!Random(4) && GetActTime() > MaxLifeTime()/4)
    SetYDir(GetYDir()+RandomX(-4,2));

  if(slimy) {
    CastSlimeEffects(GetActTime());
  } else {
    CastFireEffects(GetActTime());
  }
}

public func HitObject(object pObj)
{
  var color = FIRE_Green;
  // 100..70
  var schwaecher = Max(100 - hits*20,0);
  //Message("@%d%",this,schwaecher);
  if(!slimy) {
    color = FIRE_Red;
    DoDmg(iDamage*schwaecher/100, DMG_Fire, pObj,1);
  }
  else {
    DoDmg(iDamage*schwaecher/100, DMG_Bio, pObj, 1);
  }

  hits++;
  
  //anzündbares anzünden, aber nicht lebewesen.
  if(pObj) //existiert es überhaupt noch?
    if(pObj->GetOCF() & OCF_Inflammable) {
      if(!(pObj->GetOCF() & OCF_Living))
        Incinerate(pObj);
	  else
	    AddFireEffect(pObj,50,color,1);
	}
}

protected func FireSize()
{
	return Min(180,20 + GetActTime()*6);
}

protected func CastFireEffects(int time)
{
  var maxtime = MaxLifeTime();
  var dir = (GetXDir()>0)*2-1;
  var angle = Angle(0,0,GetXDir(),GetYDir());
  var speed = Sqrt(GetXDir()*GetXDir()+GetYDir()*GetYDir());
  
  var ntime = 40*time/maxtime;

  for (var i = 0; i < 3; i++) 
  {
    var xoff = RandomX(-2,5)+ntime/2;
    var yoff = RandomX(-ntime , ntime);
  
    var xdir = +Cos(angle,yoff)-Sin(angle,xoff);
    var ydir = +Sin(angle,yoff)+Cos(angle,xoff);

    var size = FireSize();

    var x = Sin(angle,(+10-2*i));
    var y = RandomX(-size,size)/20;

    var alpha = BoundBy(180 - speed/4 - time/2,0,255);
    var color = RGBa(255, 255, 255, alpha);

    CreateParticle("Fire2", x, y, xdir, ydir, size, color);
  }  
  
}

protected func CastSlimeEffects(int time)
{
  var maxtime = MaxLifeTime();
  var dir = (GetXDir()>0)*2-1;
  var angle = Angle(0,0,GetXDir(),GetYDir());
  
  var ntime = 40*time/maxtime;
  
  for (var i = 0; i < 3; i++) 
  {

    var xoff = RandomX(-2,5);
	var yoff = RandomX(-ntime , ntime)/2;
  
    var xdir = +Cos(angle,yoff)-Sin(angle,xoff);
    var ydir = +Sin(angle,yoff)+Cos(angle,xoff);
	
    var size = 20 + ntime*8;

	var x = (+10-2*i)*dir;
    var y = RandomX(-size,size)/20;

	var alpha = BoundBy(ntime ,0,255);
    var color = RGBa(100, 255, 0, alpha);

    CreateParticle("AlienPlasma", x, y, xdir, ydir, size, color);
  }  
}

/* Effekt für Trefferüberprüfung */

// weitgehend kopiert aus Schuss :(

// EffectVars:
// 2 - Schütze (Objekt, das die Waffe abgefeuert hat, üblicherweise ein Clonk)
// 3 - ID des Schützen
// 4 - Scharf? Wenn true wird der Schütze vom Projektil getroffen 

private func FxHitCheckStart(object target, int effect, int temp, object byObj)
{
  if(temp) return;
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = false;
}

private func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var x = GetX(target);
  var y = GetY(target);
  var angle = Angle(0,0,GetXDir(target),GetYDir(target));
  var wdt = target->~FireSize()/7;
  var hgt = target->~FireSize()/7;
  
  target->SetObjDrawTransform(wdt*1000/4,0,0,0,hgt*1000/4);
  
  // Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  if(EffectVar(4, target, effect)) exclude = target;
  
  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (nähere zuerst)

  for(obj in FindObjects(Find_Or(Find_AtRect(x-wdt/2,y-hgt/2,wdt,hgt)),Find_NoContainer()))
  {
		// Excludes
		if(obj == target) continue;
		if(obj == exclude) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect)) || GetOCF(obj) & OCF_Alive) {
			return target->~HitObject(obj);
	  }
  }
  
  // OKOK, Ich hab sogar das Restzeug kommentiert. :P
  if(!EffectVar(5,target,effect)) {
	  if(!EffectVar(4, target, effect)) {
	    // ready gibt an, ob wir schon "scharf" sind. True = Scharf
	    var ready = true;
	    // Wir suchen alle Objekte mit der ID unseres Schützens an unserer momentanen Stelle
	    for(var foo in FindObjects(Find_AtPoint(x,y),Find_ID(EffectVar(3, target, effect))))
	    	// und schauen, ob es der Schütze ist.
	      if(foo == EffectVar(2, target, effect))
	        // wir haben den Schützen gefunden -> Er ist noch an unserer Position
	        ready = false;
	    // wir haben den Schützen nicht gefunden
	    if(ready)
	      // -> Wir treffen ihn ab jetzt.
	      EffectVar(4, target, effect) = true;
	  }
  }
}

protected func Hit()
{
	RemoveObject();
}

func NoWarp() { return true; }
