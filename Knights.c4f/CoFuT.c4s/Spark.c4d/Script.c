/* Funken */

#strict

protected func CreateSpark()
{
 // Immer je links und rechts einen Spark erschaffen, die sich an der Mittelachse spiegeln
 // Über den Burgen seltener
 var x = Random(Random(LandscapeWidth() / 2 - 25));
 CreateObject(GetID(), LandscapeWidth()/2+x-GetX(),0, -1);
 CreateObject(GetID(), LandscapeWidth()/2-x-GetX(),0, -1);
 return(1);
}

protected func Initialize() { return(SetAction("Fall")); }
protected func Hit() { return(SetAction("Sparkle")); }
protected func Removal() { return(RemoveObject()); }

protected func CastObject()
{
 var fIsInMiddle = (Abs(GetX() - LandscapeWidth()/2) < LandscapeWidth()/6);
 CreateObject(RndObjectID(fIsInMiddle), 0,0, -1);
 Sound("Boing");
 return(1);
}

protected func RndObjectID(bool fMiddle)
{
 // Seltene Objekte
 var r = Random(85);
 if (!--r) return(MAGW);
 if (!--r) return(TENT);
 if (!--r) return(TRE1);
 if (!--r) return(WMIL);
 if (!--r) return(SNKE);
 if (!--r && fMiddle) return(CPT2);
 if (!--r && fMiddle) return(PALS);
 if (!--r && fMiddle) return(PAL2);
 if (!--r && fMiddle) return(WGTW);

 if (!--r) return(BALN);
 if (!--r) return(BLMP);

 if (!--r) return(BRED);
 if (!--r) return(SPER);
 
 // Nur in der Mitte
 if (fMiddle)
   {
   r = Random(9);
   if (!r)   return(LOAM);
   if (!--r) return(METL);
   if (!--r) return(SPHR);
   
   if (!Random(24)) return(CATA);
   }

 // Normale Objekte
 r = Random(13);
 if (!r)   return(FLNT);
 if (!--r) return(SFLN);
 if (!--r) return(TFLN);
 if (!--r) return(STFN);
 if (!--r) return(FBMP);
 if (!--r) return(BOW1);
 if (!--r) return(FARP);
 if (!--r) return(SWOR);
 if (!--r) return(AXE1);
 if (!--r) return(SHIE);
 if (!--r) return(TENP);
 if (!--r && fMiddle) return(XARP);
 return(COKI);
}

protected func Sparkle()
{
 CreateParticle("PSpark",0,0,0,0,200,RGB(50,50,255),this(),1);
}
