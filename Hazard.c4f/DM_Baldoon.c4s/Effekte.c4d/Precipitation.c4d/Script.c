/*-- Precipitation --*/

#strict

func Niederschlag() {
  var count = 1;
  while(count) {Precipitation("Rain"); count--;}
  }

func Precipitation(mat) {
  CastPXS(mat,1,1,Random(LandscapeWidth()),0); 
  return(1);
}
