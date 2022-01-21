/*-- Wurm --*/

#strict 2

static const WORM_Part = 17; //Wie groß ein Teil ist
static const WORM_Count = 7; //Wieviele Teile
static const WORM_Cutoff = 3; // so Offsetzeugs

static const WORM_MaxPreyDist = 500;
static const WORM_MaxMealSize = 25;

static const WORM_PartGraphics = 7;

local head;
local prey;
local posx, posy, posr;
local parts;

public func IsClosed() { return head->GetAction() == "Be"; }

protected func Initialize()
{
  SetCategory(C4D_Vehicle);
  SetAction("Exist");
  
  //wir brauchen Teile!
  head = CreateObject(ALNH,0,0,GetOwner());
  head->Init(this());
  SetParts(WORM_Count);
  
  // Animationseffekt
  AddEffect("WormDraw",this(),10,1,this());
}

protected func InitParts(bool norand) {

  for(var x=1; x <= WORM_Count; x++)
  {
    if(x <= parts) {
		if(!norand) {
		    var actionName = Format("Part%d",Random(WORM_PartGraphics)+1);
		    SetGraphics(0,this(),GetID(),x,GFXOV_MODE_Action,actionName);//,GFX_BLIT_Parent);
			SetClrModulation(RGBa(255,255,255,Max(0,120-x*60)),this(),x);
		}
	}
	else {
		SetGraphics(0,this(),0,x);
	}
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Interface */
/* ++++++++++++++++++++++++++++++++++++++++++ */

public func MoveHead(int angle, int distance, int speed)
{
  distance = BoundBy(distance,0,1000);

  if(!speed) speed = 1;
  var blub = speed;
  if(blub > 4) blub = 4;
  
  var hmax = (WORM_Part-WORM_Cutoff)*parts;
  var x,y;

  var h = hmax*distance/1000;

  x = Sin(angle,h);
  y = -Cos(angle,h);

  angle = Normalize(angle,-180);
  
  head->Move(GetX()+x,GetY()+y, true, speed);
	
  // je weiter in der Mitte desto verdrehter.
  var dir = angle/Abs(angle)*((Abs(angle)<90)*2-1);
  var anglemod = blub * 180 * dir * (1000*1000-distance*distance)/1000/1000 / speed;
	  
  head->Rotate(angle + anglemod, true, speed);

}

public func LookToTarget(object target) {
  if(!target) return;
  var angle = Angle(head->GetX(),head->GetY(),target->GetX(),target->GetY());
  head->Rotate(angle, true, 5);
}

public func MoveToTarget(object target, int speed) {
  if(!target) return;
  if(!speed) speed = 15;
  var x = GetX();
  var y = GetY();
  var tx = target->GetX()+target->GetXDir();
  var ty = target->GetY()+target->GetYDir();
  var angle = Angle(x,y,tx,ty);
  var h = Distance(x,y,tx,ty);
  var hmax = (WORM_Part-WORM_Cutoff)*parts;
  var distance = 1000*h/hmax;
  
  MoveHead(angle,distance,speed,true);
}

public func MoveSomewhere() {
    var r = RandomX(-90,90);
	var dist = RandomX(700,900);
    MoveHead(r,dist,0,true);
}

public func OpenMouth() {
	head->Open();
}

public func SetPrey(object p) {
	prey = p;
}

public func SetParts(int p) {
	var norand = false;
	if(p < parts) norand = true;

	parts = BoundBy(p,0,WORM_Count);
	
	InitParts(norand);
	AdjustParts();
}

public func GetParts() {
	return parts;
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Aktivität */
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func Activity()
{
  if(GetAction() == "Dead")
    return;

  // neue Beute suchen
  if(Chance(40)) {
	if(!prey) prey = SearchForPrey();
	if(prey) {
		Tactic(prey);
		return;
	}
  }

  if(IsSnapping()) return;
  
  // ansonsten doof hin und herschwanken
  if(Chance(10))
  {
	MoveSomewhere();
  }
}

private func SearchForPrey() { // search for prey

  var targets = FindTargets(head, WORM_MaxPreyDist);
  
  // irgendein Objekt anivisieren
  if(GetLength(targets))
    return targets[0];

  return false;
}

// ist das Opfer noch lecker?
private func PreyIsGood(int maxdist) {
  if(!prey) return false;
  if(!maxdist) maxdist = WORM_MaxPreyDist;

  if(Contained(prey) == this()) return true;
  
  return CheckTarget(prey, head, maxdist);
}

// Schaut wie angegriffen werden muss und so
private func Tactic(object target)
{
  // Länge!
  var th = (WORM_Part-WORM_Cutoff)*parts;

  var dist;
  dist = Distance(head->GetX(),head->GetY(), target->GetX(),target->GetY());

  // Prey ist garnicht mehr relevant?
  if(!PreyIsGood())
  {
    prey = 0;
    return;
  }

  if(IsSnapping()) return;

  // Ausrichten
  LookToTarget(target);

  // so, fangen wir an!

  // Mampfen
  if(dist < th+20) {
    if(Chance(30))
	  if(!GetEffect("NoSnap",this()))
	    if(GetObjWidth(target) < WORM_MaxMealSize && GetObjHeight(target) < WORM_MaxMealSize)
	      Snap(target);
  }

  // Explosionsartige Freigabe von halbverdautem zeug
  if(Chance(10))
    if(!GetEffect("NoSchlorz",this()))
      Schlorz();
}


/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Angriffstechniken */
/* ++++++++++++++++++++++++++++++++++++++++++ */

public func IsSnapping() { return GetEffect("Snap*",this()); }
public func IsSchlorzing() { return GetEffect("Schlorz*",this()); }

public func IsAttacking() { return(IsSnapping() || IsSchlorzing()); }

/* Auffressen */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Effekte:
// Snapping - gerade am zuschnappen. Wird von FinishedMoving beendet.
// SnapBack - Clonk aufgefressen -> Schnelle Bewegung nach oben. Wird von FinishedMoving beendet.
// NoSnap - 3sek Delay bis zum nächstne Snap

private func Snap(object target)
{
  DebugMessage("Snap","Worm");
  if(IsAttacking())
    return;

  Sound("JumperAttack",head);
  MoveToTarget(target);
  head->Open();
  AddEffect("Snapping",this(),1,0,this(),GetID());
}

private func EndSnapping(int Effect) {

   if(!PreyIsGood(20)) prey = 0;

   if(prey) Enter(this(),prey);

    RemoveEffect("Snapping",this());
    // Zumachen!
    head->Close();
	
    // Fressen erfolgreich?
    if(Contained(prey) != this()) {
		prey = 0;
		MoveSomewhere();
		return;
	}

    // Animation, danach ausspucken
	//MoveSomewhere();
    AddEffect("SnapBack",this(),1,1,this(),0,prey);
}

protected func FxSnapBackStart(object target, int num, int temp, object prey) {
	if(temp) return;
		
	EffectVar(0,target,num) = prey;
}

protected func FxSnapBackTimer(object target, int num, int time) {
	// Durchgängig geringen Schaden machen
	if(time%8 == 0) if(Chance(60))
		target->DoDmg(3, DMG_Melee, EffectVar(0,target,num));
	
	// grafischer Effekt
	if(time == 1)	{ AddEffect("WormDraw",target,10,1,this(),0, 8, 600, 1, 2); }
	// erstmal schön weitergehen
	if(time == 20)	{ target->MoveSomewhere(); }
	// Verdaut... moment mal, das kommt wieder hoch...urääägh
	if(time == 110)	{ AddEffect("WormDraw",target,10,1,target,0, 8, 600, 0, 2); }
	// Verdauungsschwierigkeiten bringen Zuckungen
	if(time > 110 && time < 250) {
		var dringend = 100*(250-time)/140; // Werte von 0-100
		var haufigkeit = 7 + dringend/5; // Werte von 7 bis 27
		var schnelligkeit = 2+(100-dringend)/12; // Werte von 2 - 10
		if(time%haufigkeit == 0)
			if(Chance(80))
				{ target->MoveHead(RandomX(-70,70),RandomX(700,1000),schnelligkeit); }
	}
	// Ende (Ausspucken)
	if(time == 250)	{ target->EndSnapBack(EffectVar(0,target,num)); }
}

private func EndSnapBack(object myprey)
{

    RemoveEffect("SnapBack",this());
	AddEffect("WormDraw",this(),10,1,this());
	
    if(myprey)
    {
      var a = head->GetR();
      Exit(myprey,head->GetX()-GetX(),head->GetY()-GetY(),0,Sin(a,parts),-Cos(a,parts));
      myprey->SetAction("Tumble");
      head->SetAction("Open");
	  DoDmg(20,DMG_Bio,myprey);
      //Punch(prey,0);
      AddEffect("NoSnap",this(),1,150);
    }
}


/* Schlorz-Attacke */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Kotzen!
private func Schlorz()
{
  if(IsAttacking())
    return;

  // wir sind sowieso ausgerichtet, also zusammenziehen!
  AddEffect("Schlorzing",this(),1,70,this(),GetID());
}

protected func FxSchlorzingStart(object target, int num, temp) {
	if(temp) return;

	// Uhrr, da kommt was hoch....
	AddEffect("WormDraw",target,10,1,target,0, 16, 600, 0, 2);
	target->MoveHead(RandomX(-70,70),600,3);
}

protected func FxSchlorzingStop(object target, int number, int reason, temp) {
	
	target->EndSchlorzAttack();
}

private func EndSchlorzAttack() {

	if(!prey) return;

    var a = Normalize(head->GetR(),-180);
    var x = head->GetX()-GetX() + Sin(a,10),
        y = head->GetY()-GetY() - Cos(a,10);
    var d = 50;

   	var target = prey;

	MoveHead(a,1000,10);
	
    var chunk = CreateObject(WACC,x,y+14,GetOwner());
    var xdir,ydir;

    var dist, correction;
    dist = ObjectDistance(head,target)/2;
    correction = dist/20;
    if(a < 0)
    	a += correction;
    else
    	a -= correction;

    a += RandomX(-1,1);

    xdir = Sin(a, dist);
    ydir =-Cos(a, dist);

    chunk->Launch(xdir,ydir, this());

    /*
    var t = 15; // t für TOLERANZ!

    if(prey)
      d = Distance(head->GetX(),head->GetY(), prey->GetX(),prey->GetY());

    for(var i = 3+Random(2); i; i--)
      CreateObject(SLST,x,y,GetController())->Launch(+Sin(a+RandomX(-t, t),130+Random(50)), -Cos(a+RandomX(-t,t),130+Random(50)),true,15);

    for(var i = 8+Random(8); i; i--)
      CreateObject(GLBL,x,y,GetController())->Launch(a+RandomX(-t, t), 130+Random(50),600,6, 60, 10,0,1);

    for(var i = 15+Random(10); i; i--)
      CreateObject(FFLM,x,y,GetController())->Launch(+Sin(a+RandomX(-t, t),130+Random(50)), -Cos(a+RandomX(-t,t),130+Random(50)),true,15);
    */

    head->Close();

    AddEffect("NoSchlorz",this(),1,200);
	AddEffect("WormDraw",this(),10,1,this());
}


/* Trigger für viele Aktionen: Kopf ist an Ziel angekommen */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Wird vom Kopf aufgerufen
public func FinishedMoving()
{
	var Effect;
  // Wir haben gerade zugeschnappt
  if(Effect = GetEffect("Snapping",this()))				EndSnapping(Effect);

}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Zeichnen des Wurms! */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Plaziert die Teilelemente des Wurms.
// An dieser Funktion bin ich Tage gesessen
// und hab sie 4 mal neu geschrieben. Würdigt das!!!! %(
public func AdjustParts()
{
  posx = CreateArray(parts);
  posy = CreateArray(parts);
  posr = CreateArray(parts); // ja, das pos ist nur damit es passt \o/

  var bx, by, //base-pos
      hx,hy;  //head-pos

  bx = GetX();
  by = GetY();
  hx = head->GetX();
  hy = head->GetY();

  var hr = Normalize(head->GetR(),-180);
  if(Abs(hr) > 90) hr = Normalize(180-hr,-180);
  
  var xh; //x-head für sin()
  var xs; //x-step

  var amp;

  var dif;
  var kfh; // Kugeln für Höhe
  var hd;  // Höhenunterschied zwischen den Kugeln
  var wd;  // Breitenunterschied zwischen den Kugeln

  var ph = WORM_Part-WORM_Cutoff;
  var th = parts*ph; //max. höhe des Wurms

  var dist = Distance(bx,by,hx,hy);
  var angle = Angle(bx,by,hx,hy);
  
  // Zuerst alle Positionen kalkulieren
  // Allgemeines
  dif = Abs(hy-by)+WORM_Part*4/3;

  var maximum = Min( Abs(hr*1200/180), 1000);
  var sinus = Sin(Max(55,Abs(hr)),1000);
  
  amp = Abs(hy-by)*1000*1000/Max(maximum,sinus);

  var ydir = -Abs(hy-by)/(hy-by);
  
  //xh = 180 - xh*2;
  xh = Abs(Normalize(angle-head->GetR(),-180));//; Min(170,Max(55,Abs(hr)));
  
  xh = BoundBy(xh,55,130);
  
  xs = xh*100 / parts;

  //Log("%d, %d",xh,xs);

  wd = ((hx-bx)*1000)/parts;

  // Dann für jeden Teil
  for(var i=0; i < parts; i++)
  {
    posx[i] = wd*i;
    posy[i] = -Sin(xs*i+700,amp*ydir,100);
  }

  // Dann Rotieren
  posr[0] = Angle(posx[0],posy[0],posx[1],posy[1]);
  posx[parts] = (hx-bx)*1000;
  posy[parts] = (hy-by)*1000;
  //posr[WORM_Count-1] = Angle(posx[WORM_Count-1],posy[WORM_Count-1],(GetX()-head->GetX())*1000,(GetY()-head->GetY())*1000);
  for(var i=1; i < parts; i++)
  {
    posr[i] = Angle(posx[i],posy[i],posx[i+1],posy[i+1]);
  }

  return 1;
}

private func Pulse(int pos) { // values from -1000 to 1000
	var pulse = Sin(3*pos/5,1000);
	var fire = 1000-Abs(Sin(pos/7+90,1000));
	return (pulse * fire ) / 1000;
}

public func RedrawParts(int time, int speed, int blow, int dir, int mode) {

  if(time == -1) time = GetActTime();
  if(!speed) speed = 15;
  if(!blow) blow = 100;
  if(!dir) dir = -1;

  for(var i=0; i <= parts; i++)
  {
    var r = dir*i*7+time*1;
	var pulse = blow*Pulse(r*speed)/1000;
	//if(mode == 0) pulse = pulse;
	if(mode == 1) pulse = Abs(pulse);
	else if(mode == 2) pulse = Max(0,pulse);

	var thorn = 500-i*100;
    var w = thorn+1000+pulse;
	var l = thorn/2+1000+pulse;

	// auch der Kopf
	if(i == parts) {
		SetObjDrawTransform(w,0,0,0,l,0,head);
		break;
	}
	
    var fsin=-Sin(posr[i], 1000), fcos=Cos(posr[i], 1000);

    var width = +fcos*w/1000, height = +fcos*l/1000;
    var xskew = +fsin*l/1000, yskew = -fsin*w/1000;

    SetObjDrawTransform(width,xskew,posx[i],yskew,height,posy[i],this(),i+1);
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Blubb0r Effekt0rz */
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func FxWormDrawStart(object target, int num, int temp, int speed, int size, int reverse, int mode) {
	if(temp) return;
	
	EffectVar(1,target,num) = speed;
	EffectVar(2,target,num) = size;
	EffectVar(3,target,num) = reverse;
	EffectVar(4,target,num) = mode;
}

protected func FxWormDrawTimer(object target, int num, int time) {

	RedrawParts(time-EffectVar(0,target,num),EffectVar(1,target,num),EffectVar(2,target,num),EffectVar(3,target,num),EffectVar(4,target,num));
}

protected func FxWormDrawEffect(string name, object target, int num, int newnum, int speed, int size, int reverse, int mode) {

	if(name == "WormDraw") {
		// übernehmen und ablehnen, hurrhurr
		EffectVar(0,target,num) = GetEffect(0,target,num,6);
		EffectVar(1,target,num) = speed;
		EffectVar(2,target,num) = size;
		EffectVar(3,target,num) = reverse;
		EffectVar(4,target,num) = mode;
		
		return -1;
	}
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Sterbeeffekt  */
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func FxWormDieTimer(object target, int num, int time) {
	
	target->MoveHead(RandomX(-70,70),RandomX(500,700),15);
	
	if(!(time%8)) {
		var count = target->GetParts();
		var x = LocalN("posx",target)[count-1]/1000;
		var y = LocalN("posy",target)[count-1]/1000;
		AlienBlood(70, AbsX(target->GetX()+x), AbsY(target->GetY())+y);
		target->SetParts(count-1);
	}
	
	if(!(target->GetParts())) return -1;
}

protected func FxWormDieStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
	if(fTemp) return 0;
	
	RemoveEffect("WormDraw",pTarget);
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Kein Ablegen*/
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func ContainedThrow() { return(1); }
protected func ContainedLeft() { return(1); }
protected func ContainedRight() { return(1); }

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Sterbeanimation */
/* ++++++++++++++++++++++++++++++++++++++++++ */

public func OnHit(int dmg, int type, object byObj)
{
  // ein Feind?? Wo!
  SetPrey(byObj);
  // Aua!!! (falls nicht angreifen)
  //if(!IsSnapping()) MoveHead(RandomX(-70,70),RandomX(700,1000),BoundBy(dmg,1,15));
}

public func Die()
{
  while(Contents()) Contents()->Exit();
  AddEffect("WormDraw",this(),10,1,this(),0, 18, 600, 1, 1);
  AddEffect("WormDie",this(),1,20,this());
  SetAction("Dead");
  SetAlive(false);
  FadeOut();
}


// Globale Einstellung: Kann von Paketen oder Szenarien überladen werden
// Ja, wir überladen es. \o/
global func ANIM_IsCollectible(object pByContainer)
{
  // Der Wurm darf!
  if(GetID(pByContainer) == ALN1)
    return true;

  return _inherited(pByContainer);
}

/* ... */

protected func Destruction() {
	if(head) RemoveObject(head);
	while(Contents()) Contents()->Exit();
}

public func NoWarp() { return(1); }
public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }
public func IsAlien() { return(GetAction()!="Dead"); }
