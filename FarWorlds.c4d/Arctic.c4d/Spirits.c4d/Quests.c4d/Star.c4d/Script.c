#strict

protected func Initialize() {
  SetAction("Exist");
}

public func Sparkle() {
  Sound("Sparkle");
  for(var i=0;i<8;++i)
    CreateParticle("PxSpark",RandomX(-5,5),RandomX(-5,2),RandomX(-20,20),RandomX(-20,0),RandomX(15,25),RGB(255,196,RandomX(0,196)));
}

protected func Hit()   { Sparkle(); }

// Blitzeinschlag
public func LightningStrike() {
  Sparkle();
  return(1);
}