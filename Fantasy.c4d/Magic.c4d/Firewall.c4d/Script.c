/*-- Feuerwand --*/

#strict


func Activate(caster)
  {
  // Effekt prüfen
  var iResult;
  if(iResult = CheckEffect("FireWallNSpell", 0, 150)) return(iResult != -1 && RemoveObject());

  var obj,clonk;
  SetPosition(-40+80*GetDir(caster)+GetX(),GetY());
  // sonst gibts Siedlungspunkte
  SetOwner(-1);
  
  // Das Feuer auf dem Boden der Tatsachen erstellen
  var i=60;
  while(!Stuck())
    {
        SetPosition(GetX(),GetY()+1);
        if(!--i) return(0, RemoveObject());
    }
  i = 60;
  while(Stuck())
    {
        SetPosition(GetX(),GetY()-1);
        if(!--i) return(0, RemoveObject());
    }
    // Im Wasser auch nicht
  if(GetMaterialVal("Extinguisher", 0, GetMaterial(0, -3))) return(0, RemoveObject());
  // unsichtbar machen
  SetClrModulation(RGBa(0,0,0,255));
  
  // Controller vor dem Anzünden setzen
  SetController(GetController(caster));
  
  // anzünden
  Incinerate();
  
  return(1);
  }


func Timer() { //6Frames
  var pSulphur,pFireball,pObj;
    
  // wenn gelöscht, löschen
  if(!OnFire()) return(RemoveObject());

  // anzünden
  while(pObj = FindObject(0,-GetDefWidth()/2,-GetDefHeight()*GetCon()/100,GetDefWidth(),GetDefHeight()*GetCon()/100,OCF_Inflammable,0,0,NoContainer(),pObj))
    {
      Fling(pObj,-GetXDir(pObj)/9,-3);
      Incinerate(pObj);
    }

  if(pSulphur=FindObject(0,-GetDefWidth()/2,-GetDefHeight()*GetCon()/100,GetDefWidth(),GetDefHeight()*GetCon()/100,OCF_Collectible,0,0,NoContainer()))
    if(GetID(pSulphur) != FRBL && !(GetOCF(pSulphur) & OCF_Alive) && (GetOCF(pSulphur) & OCF_Inflammable || GetDefBlastIncinerate(GetID(pSulphur))) )
        {
      pFireball=CreateObject(FRBL,AbsX(GetX(pSulphur)),AbsY(GetY(pSulphur)),GetOwner(pSulphur));
      if(GetYDir(pSulphur))
                SetSpeed(GetXDir(pSulphur),GetYDir(pSulphur),pFireball);
      else
                pFireball->~Hit();
      pFireball->~SetMaxBounces(RandomX(4,5));
      RemoveObject(pSulphur);
    }
    SetCon(GetCon()-1);
}


/* Zaubercombo */

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("544"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
