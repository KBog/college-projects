#include "Standard Header.h"

/*Defining All Characters Primitives and Collisions*/

/*"TicTacMap" Primitives and Collision Data*/
Primitive TicTacMap_Primitives[5]=
{
	{Type_Line , {(float)-319,(float)80} , {(float)318,(float)80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-319,(float)-80} , {(float)318,(float)-80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-110,(float)239} , {(float)-110,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)110,(float)239} , {(float)110,(float)-238} , {0,0} , {100,100,100}},
};

Collision_Data TicTacMap_Collisions[1];

/*"Cic" Primitives and Collision Data*/
Primitive White_Primitives[2]=
{
	{Type_Filled_Circle , {(float)0,(float)-0} , {(float)20,(float)-20} , {0,0} , {255,255,255}},
};

Collision_Data White_Collisions[1];

/*"Cic" Primitives and Collision Data*/
Primitive Black_Primitives[2]=
{
	{Type_Filled_Circle , {(float)0,(float)-0} , {(float)20,(float)-20} , {0,0} , {255,0,0}},
};

Collision_Data Black_Collisions[1];

/*"TicTacMap" Primitives and Collision Data*/
Primitive Cro_Primitives[3]=
{
	{Type_Line , {(float)-10,(float)10} , {(float)10,(float)-10} , {0,0} , {255,0,0}},
	{Type_Line , {(float)10,(float)10} , {(float)-10,(float)-10} , {0,0} , {255,0,0}},
};

Collision_Data Cro_Collisions[1];

/*"CheckersBoard" Primitives and Collision Data*/
Primitive CheckersBoard_Primitives[19]=
{
	{Type_Line , {(float)-240,(float)239} , {(float)-240,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)-239} , {(float)240,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)240,(float)-239} , {(float)240,(float)239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)240,(float)239} , {(float)-241,(float)239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-180,(float)238} , {(float)-180,(float)-238} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-120,(float)238} , {(float)-120,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-60,(float)238} , {(float)-60,(float)-238} , {0,0} , {100,100,100}},
	{Type_Line , {(float)0,(float)239} , {(float)0,(float)-238} , {0,0} , {100,100,100}},
	{Type_Line , {(float)60,(float)238} , {(float)60,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)120,(float)238} , {(float)120,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)180,(float)239} , {(float)180,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)180} , {(float)240,(float)180} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)120} , {(float)240,(float)120} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)60} , {(float)240,(float)60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)0} , {(float)240,(float)0} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)-60} , {(float)240,(float)-60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)-120} , {(float)240,(float)-120} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-240,(float)-179} , {(float)240,(float)-179} , {0,0} , {100,100,100}},
};

Collision_Data CheckersBoard_Collisions[1];

#include "Standard Header.h"

/*Defining All Characters Primitives and Collisions*/

/*"Menu" Primitives and Collision Data*/
Primitive Menu_Primitives[129]=
{
	/*{Type_Line , {(float)-100,(float)-21} , {(float)-96,(float)2} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-96,(float)2} , {(float)-88,(float)-23} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-88,(float)-23} , {(float)-80,(float)7} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-72,(float)-7} , {(float)-61,(float)-5} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-61,(float)-5} , {(float)-72,(float)2} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-72,(float)2} , {(float)-75,(float)-15} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-75,(float)-15} , {(float)-63,(float)-19} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-55,(float)-1} , {(float)-49,(float)-21} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-49,(float)-21} , {(float)-42,(float)-8} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-42,(float)-8} , {(float)-38,(float)-20} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-38,(float)-20} , {(float)-30,(float)2} , {0,0} , {255,255,255}},
	{Type_Line , {(float)22,(float)-4} , {(float)3,(float)9} , {0,0} , {255,0,0}},
	{Type_Line , {(float)3,(float)9} , {(float)-11,(float)-10} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-11,(float)-10} , {(float)6,(float)-25} , {0,0} , {255,0,0}},
	{Type_Line , {(float)6,(float)-25} , {(float)19,(float)-12} , {0,0} , {255,0,0}},
	{Type_Line , {(float)19,(float)-12} , {(float)3,(float)-8} , {0,0} , {255,0,0}},
	{Type_Line , {(float)43,(float)-13} , {(float)31,(float)-4} , {0,0} , {255,0,0}},
	{Type_Line , {(float)31,(float)-4} , {(float)22,(float)-12} , {0,0} , {255,0,0}},
	{Type_Line , {(float)22,(float)-12} , {(float)33,(float)-22} , {0,0} , {255,0,0}},
	{Type_Line , {(float)33,(float)-22} , {(float)40,(float)-13} , {0,0} , {255,0,0}},
	{Type_Line , {(float)40,(float)-13} , {(float)46,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)51,(float)-18} , {(float)48,(float)-4} , {0,0} , {255,0,0}},
	{Type_Line , {(float)49,(float)-10} , {(float)58,(float)-3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)58,(float)-3} , {(float)61,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)59,(float)-8} , {(float)70,(float)-3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)70,(float)-3} , {(float)72,(float)-16} , {0,0} , {255,0,0}},
	{Type_Line , {(float)85,(float)-9} , {(float)101,(float)-8} , {0,0} , {255,0,0}},
	{Type_Line , {(float)101,(float)-8} , {(float)88,(float)-1} , {0,0} , {255,0,0}},
	{Type_Line , {(float)88,(float)-1} , {(float)82,(float)-14} , {0,0} , {255,0,0}},
	{Type_Line , {(float)82,(float)-14} , {(float)95,(float)-19} , {0,0} , {255,0,0}},*/
	{Type_Line , {(float)-80,(float)53} , {(float)-81,(float)89} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-81,(float)89} , {(float)-59,(float)87} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-59,(float)87} , {(float)-79,(float)68} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-57,(float)78} , {(float)-59,(float)52} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-41,(float)60} , {(float)-51,(float)67} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-51,(float)67} , {(float)-53,(float)56} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-53,(float)56} , {(float)-44,(float)55} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-44,(float)55} , {(float)-41,(float)61} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-41,(float)61} , {(float)-35,(float)53} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-28,(float)65} , {(float)-22,(float)53} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-14,(float)64} , {(float)-33,(float)33} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-3,(float)57} , {(float)4,(float)62} , {0,0} , {255,255,255}},
	{Type_Line , {(float)4,(float)62} , {(float)-4,(float)69} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-4,(float)69} , {(float)-9,(float)57} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-9,(float)57} , {(float)3,(float)51} , {0,0} , {255,255,255}},
	{Type_Line , {(float)11,(float)66} , {(float)14,(float)51} , {0,0} , {255,255,255}},
	{Type_Line , {(float)12,(float)60} , {(float)23,(float)68} , {0,0} , {255,255,255}},
	{Type_Line , {(float)39,(float)64} , {(float)43,(float)51} , {0,0} , {255,255,255}},
	{Type_Line , {(float)43,(float)51} , {(float)49,(float)65} , {0,0} , {255,255,255}},
	{Type_Line , {(float)63,(float)62} , {(float)56,(float)65} , {0,0} , {255,0,0}},
	{Type_Line , {(float)56,(float)65} , {(float)53,(float)58} , {0,0} , {255,0,0}},
	{Type_Line , {(float)53,(float)58} , {(float)62,(float)57} , {0,0} , {255,0,0}},
	{Type_Line , {(float)62,(float)57} , {(float)60,(float)50} , {0,0} , {255,0,0}},
	{Type_Line , {(float)60,(float)50} , {(float)52,(float)54} , {0,0} , {255,0,0}},
	{Type_Line , {(float)75,(float)51} , {(float)84,(float)92} , {0,0} , {255,0,0}},
	{Type_Line , {(float)84,(float)92} , {(float)93,(float)91} , {0,0} , {255,0,0}},
	{Type_Line , {(float)93,(float)91} , {(float)101,(float)49} , {0,0} , {255,0,0}},
	{Type_Line , {(float)97,(float)70} , {(float)79,(float)69} , {0,0} , {255,0,0}},
	{Type_Line , {(float)120,(float)92} , {(float)116,(float)50} , {0,0} , {255,0,0}},
	{Type_Line , {(float)107,(float)50} , {(float)135,(float)51} , {0,0} , {255,0,0}},
	{Type_Line , {(float)105,(float)91} , {(float)133,(float)88} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-114,(float)-22} , {(float)-118,(float)19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-118,(float)19} , {(float)-90,(float)14} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-90,(float)14} , {(float)-115,(float)-2} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-85,(float)6} , {(float)-89,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-67,(float)-10} , {(float)-77,(float)-3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-77,(float)-3} , {(float)-81,(float)-14} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-81,(float)-14} , {(float)-73,(float)-17} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-73,(float)-17} , {(float)-68,(float)-9} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-68,(float)-9} , {(float)-62,(float)-16} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-57,(float)-3} , {(float)-52,(float)-14} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-44,(float)-2} , {(float)-64,(float)-34} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-37,(float)-10} , {(float)-27,(float)-4} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-27,(float)-4} , {(float)-37,(float)-1} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-37,(float)-1} , {(float)-35,(float)-16} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-35,(float)-16} , {(float)-24,(float)-13} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-18,(float)-3} , {(float)-20,(float)-18} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-19,(float)-9} , {(float)-10,(float)-3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)9,(float)-2} , {(float)15,(float)-15} , {0,0} , {255,0,0}},
	{Type_Line , {(float)15,(float)-15} , {(float)21,(float)-1} , {0,0} , {255,0,0}},
	{Type_Line , {(float)36,(float)-4} , {(float)29,(float)0} , {0,0} , {255,255,255}},
	{Type_Line , {(float)29,(float)0} , {(float)25,(float)-7} , {0,0} , {255,255,255}},
	{Type_Line , {(float)25,(float)-7} , {(float)36,(float)-10} , {0,0} , {255,255,255}},
	{Type_Line , {(float)36,(float)-10} , {(float)33,(float)-17} , {0,0} , {255,255,255}},
	{Type_Line , {(float)33,(float)-17} , {(float)24,(float)-12} , {0,0} , {255,255,255}},
	{Type_Line , {(float)55,(float)-23} , {(float)48,(float)23} , {0,0} , {255,255,255}},
	{Type_Line , {(float)48,(float)23} , {(float)74,(float)16} , {0,0} , {255,255,255}},
	{Type_Line , {(float)74,(float)16} , {(float)52,(float)1} , {0,0} , {255,255,255}},
	{Type_Line , {(float)81,(float)8} , {(float)75,(float)-20} , {0,0} , {255,255,255}},
	{Type_Line , {(float)96,(float)-9} , {(float)89,(float)-2} , {0,0} , {255,255,255}},
	{Type_Line , {(float)89,(float)-2} , {(float)84,(float)-11} , {0,0} , {255,255,255}},
	{Type_Line , {(float)84,(float)-11} , {(float)90,(float)-17} , {0,0} , {255,255,255}},
	{Type_Line , {(float)90,(float)-17} , {(float)96,(float)-8} , {0,0} , {255,255,255}},
	{Type_Line , {(float)96,(float)-8} , {(float)99,(float)-17} , {0,0} , {255,255,255}},
	{Type_Line , {(float)105,(float)-2} , {(float)111,(float)-14} , {0,0} , {255,255,255}},
	{Type_Line , {(float)117,(float)-2} , {(float)101,(float)-35} , {0,0} , {255,255,255}},
	{Type_Line , {(float)127,(float)-9} , {(float)135,(float)-4} , {0,0} , {255,255,255}},
	{Type_Line , {(float)135,(float)-4} , {(float)124,(float)-1} , {0,0} , {255,255,255}},
	{Type_Line , {(float)124,(float)-1} , {(float)125,(float)-18} , {0,0} , {255,255,255}},
	{Type_Line , {(float)125,(float)-18} , {(float)134,(float)-14} , {0,0} , {255,255,255}},
	{Type_Line , {(float)141,(float)-2} , {(float)141,(float)-17} , {0,0} , {255,255,255}},
	{Type_Line , {(float)142,(float)-11} , {(float)149,(float)0} , {0,0} , {255,255,255}},

	{Type_Line , {(float)-40,(float)-53} , {(float)-63,(float)-44} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-63,(float)-44} , {(float)-66,(float)-75} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-66,(float)-75} , {(float)-45,(float)-70} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-35,(float)-60} , {(float)-34,(float)-74} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-34,(float)-67} , {(float)-25,(float)-59} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-16,(float)-66} , {(float)-5,(float)-60} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-5,(float)-60} , {(float)-14,(float)-55} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-14,(float)-55} , {(float)-19,(float)-73} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-19,(float)-73} , {(float)-8,(float)-76} , {0,0} , {255,255,255}},
	{Type_Line , {(float)14,(float)-64} , {(float)2,(float)-57} , {0,0} , {255,255,255}},
	{Type_Line , {(float)2,(float)-57} , {(float)1,(float)-75} , {0,0} , {255,255,255}},
	{Type_Line , {(float)1,(float)-75} , {(float)13,(float)-75} , {0,0} , {255,255,255}},
	{Type_Line , {(float)13,(float)-75} , {(float)17,(float)-40} , {0,0} , {255,255,255}},
	{Type_Line , {(float)14,(float)-73} , {(float)21,(float)-77} , {0,0} , {255,255,255}},
	{Type_Line , {(float)25,(float)-59} , {(float)28,(float)-78} , {0,0} , {255,0,0}},
	{Type_Line , {(float)25,(float)-51} , {(float)28,(float)-51} , {0,0} , {255,0,0}},
	{Type_Line , {(float)39,(float)-41} , {(float)38,(float)-76} , {0,0} , {255,0,0}},
	{Type_Line , {(float)38,(float)-76} , {(float)49,(float)-80} , {0,0} , {255,0,0}},
	{Type_Line , {(float)49,(float)-80} , {(float)54,(float)-73} , {0,0} , {255,0,0}},
	{Type_Line , {(float)37,(float)-56} , {(float)51,(float)-56} , {0,0} , {255,0,0}},
	{Type_Line , {(float)76,(float)-62} , {(float)64,(float)-59} , {0,0} , {255,0,0}},
	{Type_Line , {(float)64,(float)-59} , {(float)60,(float)-66} , {0,0} , {255,0,0}},
	{Type_Line , {(float)60,(float)-66} , {(float)75,(float)-69} , {0,0} , {255,0,0}},
	{Type_Line , {(float)75,(float)-69} , {(float)74,(float)-79} , {0,0} , {255,0,0}},
	{Type_Line , {(float)74,(float)-79} , {(float)61,(float)-75} , {0,0} , {255,0,0}},

	{Type_Line , {(float)-24,(float)-105} , {(float)-40,(float)-103} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-40,(float)-103} , {(float)-41,(float)-127} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-41,(float)-127} , {(float)-22,(float)-128} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-39,(float)-116} , {(float)-25,(float)-115} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-7,(float)-113} , {(float)4,(float)-129} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-6,(float)-127} , {(float)6,(float)-110} , {0,0} , {255,0,0}},
	{Type_Line , {(float)20,(float)-112} , {(float)18,(float)-129} , {0,0} , {255,255,255}},
	{Type_Line , {(float)18,(float)-129} , {(float)23,(float)-129} , {0,0} , {255,255,255}},
	{Type_Line , {(float)19,(float)-106} , {(float)22,(float)-106} , {0,0} , {255,255,255}},
	{Type_Line , {(float)41,(float)-92} , {(float)36,(float)-126} , {0,0} , {255,255,255}},
	{Type_Line , {(float)36,(float)-126} , {(float)46,(float)-129} , {0,0} , {255,255,255}},
	{Type_Line , {(float)46,(float)-129} , {(float)54,(float)-121} , {0,0} , {255,255,255}},
	{Type_Line , {(float)40,(float)-105} , {(float)53,(float)-104} , {0,0} , {255,255,255}},

	/* TITLE */
	/*{Type_Line , {(float)-164,(float)207} , {(float)-183,(float)113} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-222,(float)198} , {(float)-101,(float)211} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-101,(float)211} , {(float)-99,(float)111} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-99,(float)111} , {(float)-216,(float)113} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-72,(float)109} , {(float)-18,(float)217} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-18,(float)217} , {(float)38,(float)86} , {0,0} , {255,0,0}},
	{Type_Line , {(float)15,(float)140} , {(float)-48,(float)154} , {0,0} , {255,0,0}},
	{Type_Line , {(float)161,(float)79} , {(float)203,(float)221} , {0,0} , {255,0,0}},
	{Type_Line , {(float)203,(float)221} , {(float)260,(float)118} , {0,0} , {255,0,0}},
	{Type_Line , {(float)240,(float)155} , {(float)179,(float)139} , {0,0} , {255,0,0}},
	{Type_Line , {(float)156,(float)79} , {(float)112,(float)219} , {0,0} , {255,255,255}},
	{Type_Line , {(float)43,(float)85} , {(float)90,(float)220} , {0,0} , {255,255,255}},
	{Type_Line , {(float)90,(float)220} , {(float)100,(float)180} , {0,0} , {255,255,255}},
	{Type_Line , {(float)100,(float)180} , {(float)112,(float)220} , {0,0} , {255,255,255}},*/
};

Collision_Data Menu_Collisions[1];

/*"Character 1" Primitives and Collision Data*/
Primitive Title_Primitives[15]=
{
	{Type_Line , {(float)-164,(float)207} , {(float)-183,(float)113} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-222,(float)198} , {(float)-101,(float)211} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-101,(float)211} , {(float)-99,(float)111} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-99,(float)111} , {(float)-216,(float)113} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-72,(float)109} , {(float)-18,(float)217} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-18,(float)217} , {(float)38,(float)86} , {0,0} , {255,0,0}},
	{Type_Line , {(float)15,(float)140} , {(float)-48,(float)154} , {0,0} , {255,0,0}},
	{Type_Line , {(float)161,(float)79} , {(float)203,(float)221} , {0,0} , {255,0,0}},
	{Type_Line , {(float)203,(float)221} , {(float)260,(float)118} , {0,0} , {255,0,0}},
	{Type_Line , {(float)240,(float)155} , {(float)179,(float)139} , {0,0} , {255,0,0}},
	{Type_Line , {(float)156,(float)79} , {(float)112,(float)219} , {0,0} , {255,255,255}},
	{Type_Line , {(float)43,(float)85} , {(float)90,(float)220} , {0,0} , {255,255,255}},
	{Type_Line , {(float)90,(float)220} , {(float)100,(float)180} , {0,0} , {255,255,255}},
	{Type_Line , {(float)100,(float)180} , {(float)112,(float)220} , {0,0} , {255,255,255}},
};

Collision_Data Title_Collisions[1];

/*"Credits_Karl" Primitives and Collision Data*/
Primitive Credits_Karl_Primitives[52]=
{
	{Type_Line , {(float)-96,(float)182} , {(float)-98,(float)155} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-97,(float)166} , {(float)-84,(float)181} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-96,(float)168} , {(float)-83,(float)156} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-80,(float)157} , {(float)-76,(float)172} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-76,(float)172} , {(float)-68,(float)156} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-73,(float)164} , {(float)-79,(float)163} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-64,(float)157} , {(float)-61,(float)173} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-61,(float)173} , {(float)-51,(float)166} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-51,(float)166} , {(float)-62,(float)165} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-62,(float)165} , {(float)-52,(float)157} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-46,(float)173} , {(float)-47,(float)159} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-47,(float)159} , {(float)-35,(float)158} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-20,(float)157} , {(float)-16,(float)184} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-16,(float)184} , {(float)1,(float)179} , {0,0} , {255,255,255}},
	{Type_Line , {(float)1,(float)179} , {(float)-14,(float)170} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-14,(float)170} , {(float)1,(float)161} , {0,0} , {255,255,255}},
	{Type_Line , {(float)1,(float)161} , {(float)-20,(float)158} , {0,0} , {255,255,255}},
	{Type_Line , {(float)19,(float)170} , {(float)9,(float)174} , {0,0} , {255,255,255}},
	{Type_Line , {(float)9,(float)174} , {(float)6,(float)159} , {0,0} , {255,255,255}},
	{Type_Line , {(float)6,(float)159} , {(float)18,(float)160} , {0,0} , {255,255,255}},
	{Type_Line , {(float)18,(float)160} , {(float)19,(float)170} , {0,0} , {255,255,255}},
	{Type_Line , {(float)36,(float)175} , {(float)24,(float)174} , {0,0} , {255,255,255}},
	{Type_Line , {(float)24,(float)174} , {(float)25,(float)161} , {0,0} , {255,255,255}},
	{Type_Line , {(float)25,(float)161} , {(float)37,(float)163} , {0,0} , {255,255,255}},
	{Type_Line , {(float)37,(float)163} , {(float)37,(float)170} , {0,0} , {255,255,255}},
	{Type_Line , {(float)37,(float)170} , {(float)30,(float)169} , {0,0} , {255,255,255}},
	{Type_Line , {(float)43,(float)175} , {(float)42,(float)162} , {0,0} , {255,255,255}},
	{Type_Line , {(float)53,(float)174} , {(float)50,(float)163} , {0,0} , {255,255,255}},
	{Type_Line , {(float)52,(float)168} , {(float)40,(float)168} , {0,0} , {255,255,255}},
	{Type_Line , {(float)68,(float)174} , {(float)57,(float)175} , {0,0} , {255,255,255}},
	{Type_Line , {(float)57,(float)175} , {(float)60,(float)161} , {0,0} , {255,255,255}},
	{Type_Line , {(float)60,(float)161} , {(float)69,(float)168} , {0,0} , {255,255,255}},
	{Type_Line , {(float)69,(float)168} , {(float)67,(float)174} , {0,0} , {255,255,255}},
	{Type_Line , {(float)86,(float)172} , {(float)78,(float)175} , {0,0} , {255,255,255}},
	{Type_Line , {(float)78,(float)175} , {(float)74,(float)167} , {0,0} , {255,255,255}},
	{Type_Line , {(float)74,(float)167} , {(float)85,(float)167} , {0,0} , {255,255,255}},
	{Type_Line , {(float)85,(float)167} , {(float)84,(float)161} , {0,0} , {255,255,255}},
	{Type_Line , {(float)84,(float)161} , {(float)74,(float)162} , {0,0} , {255,255,255}},
	{Type_Line , {(float)101,(float)173} , {(float)91,(float)176} , {0,0} , {255,255,255}},
	{Type_Line , {(float)91,(float)176} , {(float)90,(float)168} , {0,0} , {255,255,255}},
	{Type_Line , {(float)90,(float)168} , {(float)101,(float)168} , {0,0} , {255,255,255}},
	{Type_Line , {(float)101,(float)168} , {(float)100,(float)161} , {0,0} , {255,255,255}},
	{Type_Line , {(float)100,(float)161} , {(float)91,(float)162} , {0,0} , {255,255,255}},
	{Type_Line , {(float)107,(float)174} , {(float)106,(float)160} , {0,0} , {255,255,255}},
	{Type_Line , {(float)107,(float)181} , {(float)108,(float)181} , {0,0} , {255,255,255}},
	{Type_Line , {(float)111,(float)161} , {(float)115,(float)176} , {0,0} , {255,255,255}},
	{Type_Line , {(float)115,(float)176} , {(float)122,(float)161} , {0,0} , {255,255,255}},
	{Type_Line , {(float)116,(float)167} , {(float)121,(float)167} , {0,0} , {255,255,255}},
	{Type_Line , {(float)127,(float)160} , {(float)128,(float)171} , {0,0} , {255,255,255}},
	{Type_Line , {(float)128,(float)171} , {(float)134,(float)161} , {0,0} , {255,255,255}},
	{Type_Line , {(float)134,(float)161} , {(float)137,(float)173} , {0,0} , {255,255,255}},
};

Collision_Data Credits_Karl_Collisions[1];

/*"Credits_Elie" Primitives and Collision Data*/
Primitive Credits_Elie_Primitives[49]=
{
	{Type_Line , {(float)-138,(float)21} , {(float)-160,(float)19} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-160,(float)19} , {(float)-162,(float)-23} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-162,(float)-23} , {(float)-139,(float)-17} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-161,(float)0} , {(float)-147,(float)2} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-132,(float)-3} , {(float)-133,(float)-20} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-133,(float)-20} , {(float)-119,(float)-20} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-108,(float)-5} , {(float)-108,(float)-20} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-113,(float)-5} , {(float)-103,(float)-4} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-112,(float)-19} , {(float)-102,(float)-19} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-85,(float)-3} , {(float)-99,(float)-2} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-99,(float)-2} , {(float)-96,(float)-20} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-96,(float)-20} , {(float)-86,(float)-14} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-97,(float)-8} , {(float)-91,(float)-9} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-66,(float)-22} , {(float)-52,(float)27} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-52,(float)27} , {(float)-40,(float)-21} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-62,(float)-4} , {(float)-44,(float)-3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-31,(float)-3} , {(float)-32,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-32,(float)-19} , {(float)-20,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-20,(float)-19} , {(float)-20,(float)-13} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-20,(float)-13} , {(float)-32,(float)-9} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-32,(float)-9} , {(float)-18,(float)-3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-18,(float)-3} , {(float)-23,(float)3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-23,(float)3} , {(float)-31,(float)-2} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-5,(float)0} , {(float)-6,(float)-18} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-14,(float)-16} , {(float)3,(float)-18} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-13,(float)3} , {(float)3,(float)-1} , {0,0} , {255,0,0}},
	{Type_Line , {(float)49,(float)22} , {(float)24,(float)24} , {0,0} , {255,0,0}},
	{Type_Line , {(float)24,(float)24} , {(float)16,(float)-23} , {0,0} , {255,0,0}},
	{Type_Line , {(float)16,(float)-23} , {(float)43,(float)-21} , {0,0} , {255,0,0}},
	{Type_Line , {(float)52,(float)0} , {(float)53,(float)-22} , {0,0} , {255,0,0}},
	{Type_Line , {(float)65,(float)2} , {(float)67,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)52,(float)-9} , {(float)67,(float)-10} , {0,0} , {255,0,0}},
	{Type_Line , {(float)74,(float)-21} , {(float)81,(float)2} , {0,0} , {255,0,0}},
	{Type_Line , {(float)81,(float)2} , {(float)89,(float)-22} , {0,0} , {255,0,0}},
	{Type_Line , {(float)77,(float)-11} , {(float)86,(float)-10} , {0,0} , {255,0,0}},
	{Type_Line , {(float)100,(float)1} , {(float)98,(float)-20} , {0,0} , {255,0,0}},
	{Type_Line , {(float)112,(float)2} , {(float)111,(float)-20} , {0,0} , {255,0,0}},
	{Type_Line , {(float)98,(float)-9} , {(float)112,(float)-9} , {0,0} , {255,0,0}},
	{Type_Line , {(float)128,(float)2} , {(float)125,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)120,(float)0} , {(float)134,(float)2} , {0,0} , {255,0,0}},
	{Type_Line , {(float)116,(float)-17} , {(float)133,(float)-18} , {0,0} , {255,0,0}},
	{Type_Line , {(float)140,(float)-20} , {(float)144,(float)1} , {0,0} , {255,0,0}},
	{Type_Line , {(float)144,(float)1} , {(float)151,(float)-20} , {0,0} , {255,0,0}},
	{Type_Line , {(float)151,(float)-20} , {(float)154,(float)3} , {0,0} , {255,0,0}},
	{Type_Line , {(float)175,(float)2} , {(float)160,(float)1} , {0,0} , {255,0,0}},
	{Type_Line , {(float)160,(float)1} , {(float)158,(float)-16} , {0,0} , {255,0,0}},
	{Type_Line , {(float)158,(float)-16} , {(float)172,(float)-17} , {0,0} , {255,0,0}},
	{Type_Line , {(float)160,(float)-9} , {(float)168,(float)-8} , {0,0} , {255,0,0}},
};

Collision_Data Credits_Elie_Collisions[1];

/*"Credits_Ziad" Primitives and Collision Data*/
Primitive Credits_Ziad_Primitives[29]=
{
	{Type_Line , {(float)-120,(float)7} , {(float)-95,(float)5} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-95,(float)5} , {(float)-119,(float)-18} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-119,(float)-18} , {(float)-93,(float)-20} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-117,(float)-4} , {(float)-95,(float)-7} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-83,(float)-7} , {(float)-82,(float)-22} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-86,(float)-20} , {(float)-74,(float)-18} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-89,(float)-6} , {(float)-77,(float)-7} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-70,(float)-22} , {(float)-65,(float)-6} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-65,(float)-6} , {(float)-57,(float)-22} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-62,(float)-15} , {(float)-69,(float)-15} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-52,(float)-8} , {(float)-50,(float)-19} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-56,(float)-15} , {(float)-41,(float)-21} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-41,(float)-21} , {(float)-43,(float)-9} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-43,(float)-9} , {(float)-55,(float)-6} , {0,0} , {255,255,255}},
	{Type_Line , {(float)-19,(float)5} , {(float)-24,(float)-23} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-6,(float)5} , {(float)-7,(float)-23} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-21,(float)-11} , {(float)-7,(float)-10} , {0,0} , {255,0,0}},
	{Type_Line , {(float)-1,(float)-19} , {(float)3,(float)-5} , {0,0} , {255,0,0}},
	{Type_Line , {(float)3,(float)-5} , {(float)8,(float)-20} , {0,0} , {255,0,0}},
	{Type_Line , {(float)0,(float)-13} , {(float)8,(float)-13} , {0,0} , {255,0,0}},
	{Type_Line , {(float)14,(float)-6} , {(float)25,(float)-7} , {0,0} , {255,0,0}},
	{Type_Line , {(float)22,(float)-7} , {(float)24,(float)-18} , {0,0} , {255,0,0}},
	{Type_Line , {(float)24,(float)-18} , {(float)13,(float)-16} , {0,0} , {255,0,0}},
	{Type_Line , {(float)13,(float)-16} , {(float)15,(float)-12} , {0,0} , {255,0,0}},
	{Type_Line , {(float)28,(float)-6} , {(float)39,(float)-7} , {0,0} , {255,0,0}},
	{Type_Line , {(float)34,(float)-5} , {(float)38,(float)-19} , {0,0} , {255,0,0}},
	{Type_Line , {(float)38,(float)-19} , {(float)30,(float)-18} , {0,0} , {255,0,0}},
	{Type_Line , {(float)30,(float)-18} , {(float)29,(float)-15} , {0,0} , {255,0,0}},
};

Collision_Data Credits_Ziad_Collisions[1];