/* Große Eisstatue */

#strict

protected func CheckTemp() { if(GetTemperature()>10) Melt(); }

private func Melt() {
  DoCon(-2);
  CastPXS("Water",8,0);
}