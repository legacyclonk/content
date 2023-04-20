/*-- Fading --*/

#strict

global func SetObjAlpha(byAlpha, pObj,iLayer)
{
  byAlpha = BoundBy(byAlpha,0,255);
  var dwClrMod=GetClrModulation(pObj,iLayer);
  if(!dwClrMod)
    dwClrMod=RGB(255,255,255);

  dwClrMod=SetRGBaValue(dwClrMod,byAlpha);
  if(iLayer)
   return SetClrModulation(dwClrMod, pObj,iLayer);
  else
   return SetClrModulation(dwClrMod, pObj);
}

global func FxFadeOutStart(target, no, temp, int level, int speed)
{
  if(!level)
    EffectVar(1, target, no) = 255;
  else
    EffectVar(1, target, no) = level;
	
	if(!speed) speed = 1;
	EffectVar(2, target, no) = speed;
}

global func FxFadeOutTimer(target, no)
{
  EffectVar(0, target, no) += EffectVar(2, target, no);
  
  var alpha = EffectVar(0, target, no);
  var maxAlpha = EffectVar(1, target, no);
  SetObjAlpha(alpha, target);
  if(alpha >= maxAlpha)
  {
    if(maxAlpha >= 255) RemoveObject(target);
    return -1;
  }
}

global func FadeOut(object pObject, int level, int speed)
{
  if(!pObject)
    pObject = this();
    
  if(GetEffect("*FadeOut*", pObject)) return();
  return(AddEffect("FadeOut", pObject, 101, 2,0,0,level,speed));
}

global func FxFadeInStart(target, no, temp, int level, int speed)
{
	if(!speed) speed = 1;
	
  EffectVar(0, target, no) = 255;
  EffectVar(1, target, no) = level;
	EffectVar(2, target, no) = speed;
}

global func FxFadeInTimer(target, no)
{
  EffectVar(0, target, no) -= EffectVar(2, target, no);

  var alpha = EffectVar(0, target, no);
  var maxAlpha = EffectVar(1, target, no);

  SetObjAlpha(alpha, target);
	
  if(alpha <= maxAlpha) return -1;
}

global func FadeIn(object pObject, int level, int speed)
{
  if(!pObject)
    pObject = this();
  
  if(GetEffect("*FadeIn*", pObject)) return();
  SetObjAlpha(255, pObject);
  return(AddEffect("FadeIn", pObject, 101, 2,0,0,level,speed));
}

global func StopFading(int color, object pObject) {
  if(!pObject)
    pObject = this();
  
  var eff = GetEffectCount("*Fade*",pObject);
  for(eff; eff; eff--)
    RemoveEffect("*Fade*",pObject);
  
  SetClrModulation(color,pObject);
}
