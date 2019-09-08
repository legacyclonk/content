/* Kleines Kraftfeld (Objekt) */

#strict 2
#include FRCA

local is_dead;

public func GetForceFieldTime() { return 1000; }
private func CheckStuck() {}
public func GetMaxDamage() { return 40; }

static const FRCS_Radius = 45,
              FRCS_RadiusMargin = 6, // Halbe Dicke des regulaeren Effektbereiches
              FRCS_TopSolidWidth = 30, // Halbe Breite der oberen, soliden Kante
              FRCS_Strength = 10; // Rausschleuder-Geschwindigkeit

public func GetLightningDamage() { return GetMaxDamage(); }

protected func Damage(int iChange)
{
  // Effekt
  Sound("Electric");
  
  // Lebenszeit verringern
  EffectVar(0, this(), GetEffect("ForceFieldPSpell", this())) += GetForceFieldTime() / GetMaxDamage() * iChange;
}

public func LightningStrike(object lightning)
{
  // Rekursion verhindern
  if (is_dead) return;
  is_dead = true;
  // Blitzeffekt
  var lgt = CreateObject(FXL1, -GetX(), -GetY(), GetOwner(lightning));
  if (lgt)
  {
    var start_angle = 0;
    if (lightning)
    {
      lgt->SetController(GetController(lightning));
      var vtx_num = Max(lightning->GetVertexNum()-3), vtx_x = lightning->GetVertex(vtx_num, VTX_X), vtx_y = lightning->GetVertex(vtx_num, VTX_Y);
      start_angle = Angle(GetX(), GetY(), vtx_x, vtx_y);
    }
    for (var angle = 0; angle <= 360; angle += 20)
    {
      var a = angle + start_angle;
      lgt->AddVertex(GetX()+Sin(a, FRCS_Radius)+Random(FRCS_RadiusMargin*2+1)-FRCS_RadiusMargin,GetY()-Cos(a, FRCS_Radius)+Random(FRCS_RadiusMargin*2+1)-FRCS_RadiusMargin);
    }
    lgt->SetAction("Connect");
  }
  // Kraftfeld zerstoeren!
  return _inherited(lightning, ...);
}

protected func Timer()
{
  var protege = GetActionTarget();
  var inner_radius = FRCS_Radius-FRCS_RadiusMargin;
  // Alles im Kraftfeld nach aussen wegschleudern
  for (var obj in FindObjects(Find_Distance(FRCS_Radius+FRCS_RadiusMargin), Find_Category(28), Find_Exclude(this), Find_NoContainer(), Find_Layer(GetObjectLayer()), Find_Exclude(protege)))
    if (!Stuck(obj))
    {
      if (obj == GetActionTarget(0, protege))
      {
        // Reitziel (Drachen) ignorieren
        if (GetProcedure(protege) == "ATTACH") continue;
      }
      // Keine attachten Objekte
      if (GetProcedure(obj) == "ATTACH")
      {
        // Kraftfelder zerstoeren sich gegenseitig
        if (GetID(obj) == GetID())
        {
          DoDamage(+5, obj, FX_Call_DmgScript, GetController()+1);
        }
        continue;
      }
      
      // Keine Clonks am Boden festklemmen - das Kraftfeld ist durch Laufen passierbar
      if(GetOCF(obj) & OCF_Alive)
      	if(GetContact(obj, -1) & CNAT_Bottom)
      		continue;
      
      // Keine nur-Fahrzeuge (Burgtore etwa)
      if(GetCategory(obj) == C4D_Vehicle) // Absichtlich kein &
      	continue;
      	
      // Nicht im oberen, durch SolidMask geschuetzten Kegel
      var dx = GetX(obj)-GetX(), dy = GetY(obj)-GetY();
      if (dy<0 && Abs(dx)<FRCS_TopSolidWidth*dy/FRCS_Radius) continue;
      // Objekte tief innerhalb des Kraftfeldes nur, wenn sie sich schnell auf den Clonk zu bewegen (-> Haubitzenschuesse, etc.)
      var pvx = GetXDir(obj), pvy = GetYDir(obj);
      if (dx*dx+dy*dy < inner_radius*inner_radius)
        if (pvx*dx + pvy*dy >= 0 || Abs(pvx)+Abs(pvy) < 12) continue;
      // Ansonsten beschleunigen
      var dvx = dx*FRCS_Strength/FRCS_Radius, dvy = dy*FRCS_Strength/FRCS_Radius;
      // Gegenrichtung neutralisieren
      if (dvx * pvx < 0) dvx -= pvx;
      if (dvy * pvy < 0) dvy -= pvy;
      // Nicht gegen die Wand
      var contact_mask = GetContact(obj, -1, 15);
      if (dvx<0 && (contact_mask & CNAT_Left)) dvx=0;
      if (dvx>0 && (contact_mask & CNAT_Right)) dvx=0;
      if (dvy<0 && (contact_mask & CNAT_Top)) dvy=0;
      if (dvy>0 && (contact_mask & CNAT_Bottom)) dvy=0;
      if (!(dvx|dvy)) continue;
      // Und Speed drauf
      if (GetAlive(obj))
        {
        Fling(obj, dvx, dvy, 10, true);
        }
      else
        {
        SetController(GetController(), obj);
        SetXDir(dvx + pvx, obj);
        SetYDir(dvy + pvy, obj);
        }
      Sound("MgWind*", false, obj, 50, 0, false, true, 300); // Fantasy only
      CreateParticle("NoGravSpark", dx, dy, dvx,dvy, 30, 0xa0c0ff);
    }
}
