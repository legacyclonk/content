#strict

local fRemove, iSpeed, pShot, w, l, r, x, y;

public func Set(int iWidth, int iLength, object pSht, string actionName)
{
  pShot = pSht; 
	if(!actionName) actionName = "Travel";

  w = 1000*iWidth/20;
  l = 1000*iLength/100;

  var iXDir = GetXDir(pShot,100);
  var iYDir = GetYDir(pShot,100);

  iSpeed = Sqrt(iXDir*iXDir/100+iYDir*iYDir/100);

  SetAction(actionName);
  SetXDir(iXDir,0,100);
  SetYDir(iYDir,0,100);

  r = Angle(0,0,iXDir,iYDir);
  x = GetX();
  y = GetY();

  // richtig positionieren
  SetPosition(GetX(pShot),GetY(pShot));
  Traveling();
}

/* Timer */

private func Traveling()
{
  // The shot is gone -> remove
  if(!fRemove)
    if(!pShot)
      Remove();

  // Display
  DrawTransform();
  if(pShot) 
    if(pShot->~TrailColor())
      SetClrModulation(pShot->~TrailColor(GetActTime()));

  if(l == 0) return(RemoveObject());
}

public func Remove() {
  SetXDir();
  SetYDir();
  l = Min(l,10*Distance(x,y,GetX(),GetY()));
  fRemove = true;
}

public func DrawTransform() {

  // skip because nothing has to be transformed
  if(!fRemove && l < 10*Distance(x,y,GetX(),GetY())) return(0);

  // stretch >-<
  if(fRemove) l = Max(0,l-iSpeed); 

  // stretch <->
  var h = Min(l,10*Distance(x,y,GetX(),GetY()));

  var fsin = -Sin(r, 1000), fcos = Cos(r, 1000);

  var xoff = -(GetActMapVal("Facet",GetAction(),0,2)*w/1000)/2-1;
  var yoff = 0;

  var width = +fcos*w/1000, height = +fcos*h/1000;
  var xskew = +fsin*h/1000, yskew = -fsin*w/1000;

  var xadjust = +fcos*xoff + fsin*yoff;
  var yadjust = -fsin*xoff + fcos*yoff;

  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust
  );
}

func NoWarp() { return(true); }
