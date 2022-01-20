/*-- Düsternis --*/

#strict

//Ob es wirklich dunkel ist.
local darkness;

// Kann mittels des Spielzielauswählers ausgewählt werden
//public func IsChooseable() { return(true); }
// Das war einmal. Der Regelwähler behandelt Dunkelheit jetzt gesondert.

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

public func Initialize() {
  ScheduleCall(this(),"PostInitialize",1);
}

private func PostInitialize()	{
  //nur eine Dunkelheit!
  var count = 1;
  for(var d in FindObjects(Find_ID(GetID()),Find_Exclude(this()))) {
    count++;
    RemoveObject(d);
  }

  if(FindObject(CHOS)) count = 0;

  //sehr wichtig
  SetDarkness(count);
}

public func UpdateLights() {
  var obj;
  
  for(obj in FindObjects(Find_Func("IsLight"))) {
    obj->~Update();
  }
  
  var d = GetDarkness(100);
  for(obj in FindObjects(Find_Func("IsHUD")))
    obj->~UpdateColor(d);
}

global func CalcLight(&alphamod, &sizemod) {
  sizemod = 100+GetDarkness(1000)*3/2/10; // bis zu 250% so groß
  alphamod = (1000-GetDarkness(1000))/50; // 0-20 alpha werden aufaddiert
  
  // keine Dunkelheit: beinahe unsichtbar
  // Genauso bei Dunkelheit = 0
  if(!IsDark() || !GetDarkness(1000)) alphamod = 30;
}

public func InitializePlayer(int iPlr) {
  SetFoW(true,iPlr);
}

public func OnClonkRecruitment(object pClonk) {
  if(FindObject(CHOS)) return();
  if(ObjectCount(GetID()) > 1)
    return(ScheduleCall(this(),"OnClonkRecruitment",1,0,pClonk));
  SetPlrViewRange(700-3*GetDarkness(100),pClonk);
  var tmp = AddLightAmbience(80,pClonk);
  tmp->ChangeColor(RGBa(255,255,255,110));
  SetVisibility(VIS_Owner,tmp);
}

public func ChooserFinished() {
  for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Not(Find_Owner(NO_OWNER))))
  {
    SetPlrViewRange(700-3*GetDarkness(100),pClonk);
    var tmp = AddLightAmbience(80,pClonk);
    tmp->ChangeColor(RGBa(255,255,255,110));
    tmp->SetOwner(pClonk->GetOwner());
    SetVisibility(VIS_Owner,tmp);
  }
}

global func IsDark() {
  var obj;
  if(GetID(this()) != DARK)
    obj = FindObject(DARK);
  else
    obj = this();
  //kein Dunkelheit-Objekt -> Keine Dunkelheit
  if(!obj)
    return(false);

  return(obj);
}

global func GetDarkness(int precision) {
  var dark;
  if(!(dark = IsDark()))
    return(0);

  if(!precision) precision = 10;  

  var val;
  
  var darkness = dark->LocalN("darkness");
  
  val = darkness/(1000/precision);
    
  return(val);
}

global func SetDarkness(int iGrade) {
  var obj;
  if(GetID(this()) != DARK)
    obj = FindObject(DARK);
  else
    obj = this();
  //kein Dunkelheit-Objekt -> Keine Dunkelheit
  if(!obj)
    return(false);

  iGrade = BoundBy(iGrade,0,10);

  obj->LocalN("darkness") = iGrade*100;
  var g = BoundBy(128-iGrade*10,0,128);
  SetGamma(RGB(0,0,0),RGB(g,g,g),RGB(255,255,255),2);
  SetSkyAdjust(RGB(127+g,127+g,127+g),RGB(100+g,100+g,100+g));
  
  obj->UpdateLights();
  
  return(true);
}

// iStep: wie viel Änderung der Dunkelheit pro 10 Frames
global func FadeDarkness(int iGrade, int iStep) {
  var obj;
  if(GetID(this()) != DARK)
    obj = FindObject(DARK);
  else
    obj = this();
  //kein Dunkelheit-Objekt -> Keine Dunkelheit
  if(!obj)
    return(false);

  if(!iStep) iStep = 100;

  iGrade = BoundBy(iGrade,0,10);
  
  var darkness = obj->LocalN("darkness");
  
  AddEffect("Fading",obj,20,1,obj,DARK,iGrade*100,darkness,iStep);
  
  return(true);
}

public func Destruction() {
  SetDarkness(0);
  UpdateLights();
  for(var light in FindObjects(Find_Func("IsLight"),Find_Func("LocalN(\"bAmbience\")")))
    RemoveObject(light);
}

func FxFadingStart(object pTarget, int iEffectNumber, int iTemp, int endgrade, int startgrade, int frames) {
  if(iTemp)
    return();
  EffectVar(0, pTarget, iEffectNumber) = endgrade;
  EffectVar(1, pTarget, iEffectNumber) = startgrade;
  EffectVar(2, pTarget, iEffectNumber) = frames;  
  EffectVar(3, pTarget, iEffectNumber) = 0;  
}

func FxFadingTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var grade = EffectVar(1,pTarget,iEffectNumber);
  var end = EffectVar(0,pTarget,iEffectNumber);
  var fade = EffectVar(2,pTarget,iEffectNumber);
  var go = EffectVar(3,pTarget,iEffectNumber);
  if(grade == end)
    return(-1);
  
  go += fade;

  if(go >= 10) {
    if(grade > end) {
      grade = Max(grade-go/10, end);
    }
    else {
      grade = Min(grade+go/10, end);
    }
    go = 0;
    
    EffectVar(1,pTarget,iEffectNumber) = grade;

    //Log("%d -> %d -> %d",grade,g,end);
  
    var g = BoundBy(128-grade*100/1000,0,128);
  
    SetGamma(RGB(0,0,0),RGB(g,g,g),RGB(255,255,255),2);
    SetSkyAdjust(RGB(127+g,127+g,127+g),RGB(100+g,100+g,100+g));

    pTarget->LocalN("darkness") = grade;
    
    UpdateLights();
  }

  EffectVar(3,pTarget,iEffectNumber) = go;

}

func FxFadingEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, int endgrade, int stargrade, int frames) {
  if(szNewEffectName S= "Fading")
    return(-3);
}

func FxFadingAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int endgrade, int stargrade, int frames) {
  EffectVar(0,pTarget,iEffectNumber) = endgrade;
  EffectVar(2,pTarget,iEffectNumber) = frames;
}
