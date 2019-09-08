/*-- Neueres Script --*/

#strict

//Löscht ein Item aus einem Array
global func DeleteArrayItem(int iNumber,&aArray)
{
 var temp=[];
 for(var cnt;cnt<GetLength(aArray);cnt++)
 {
  if(cnt==iNumber)continue;
  var dif=0;
  if(cnt>iNumber)dif=-1;
  temp[cnt+dif]=aArray[cnt];
 } 
 
 aArray=temp;
 return(aArray);
}

//Sucht ein Item im array
global func GetArrayItemPosition(value,aArray)
{
 var number=-1;
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 {
  if(aArray[cnt] == value)
  {
   number=cnt;
   break;
  }
  else continue;
 }
 
 return(number);
}
