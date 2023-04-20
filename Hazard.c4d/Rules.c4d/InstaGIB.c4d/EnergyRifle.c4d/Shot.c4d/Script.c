#strict
#include SHT1


/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {

  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail) {
    pTrail->Set(iSize+2,iTrail,this(),"Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Treffer */

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
    LaserLight(60, Color(GetActTime()),0,0,10);
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj) {
    DoDmg(10000, DMG_Energy, pObj);
    if(GetAlive(pObj)) Kill(pObj);
  }
  return(1);
}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,127-iPrg,127,iPrg*2));
}

private func GlowColor() {
  return(RGB(255,90,110));
}

public func TrailColor(int iATime) { return(Color(iATime)); }
