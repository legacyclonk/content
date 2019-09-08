/*-- Abschlieﬂbarer Turm --*/

#strict
#appendto CPT2
#appendto CPT3
#appendto CPT4
#appendto CPTR
#appendto CPTL

 protected func ControlThrowDouble(){  
   if(FindObject(CPTE,-5,-46,10,10)){ 
     Sound("Error");  
     Message("$MsgOpenAlready$",this()); 
   }else{ 
     CreateObject(CPTE, 0, -31, GetOwner()); 
     Message("$MsgOpen$",this()); 
   }
   return(1);
 }
 
 protected func ControlDigDouble(){ 
   if(Var()=FindObject(CPTE,-5,-46,10,10)){
     RemoveObject(Var(),1);
     Message("$MsgClosed$",this()); 
   }
   return(1); 
 }