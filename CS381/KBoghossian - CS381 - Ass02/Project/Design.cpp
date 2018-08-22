#include "Standard Header.h"

/*Defining All Characters Primitives and Collisions*/

/*"MainCharacter" Primitives and Collision Data*/
Primitive MainCharacter_Primitives[40]=
{
	{Type_Line , {(float)-20,(float)0} , {(float)0,(float)20} , {0,0} , {100,100,100}},
	{Type_Line , {(float)0,(float)20} , {(float)20,(float)0} , {0,0} , {100,100,100}},
	{Type_Line , {(float)20,(float)0} , {(float)0,(float)-60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)0,(float)-60} , {(float)-20,(float)0} , {0,0} , {100,100,100}},
	{Type_Line , {(float)20,(float)60} , {(float)40,(float)80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)40,(float)80} , {(float)119,(float)120} , {0,0} , {100,100,100}},
	{Type_Line , {(float)119,(float)120} , {(float)99,(float)60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)99,(float)60} , {(float)40,(float)40} , {0,0} , {100,100,100}},
	{Type_Line , {(float)40,(float)40} , {(float)20,(float)59} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-20,(float)60} , {(float)-40,(float)81} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-40,(float)81} , {(float)-120,(float)119} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-120,(float)119} , {(float)-100,(float)60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-100,(float)60} , {(float)-40,(float)40} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-40,(float)40} , {(float)-21,(float)60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-10,(float)-160} , {(float)10,(float)-160} , {0,0} , {100,100,100}},
	{Type_Line , {(float)10,(float)-160} , {(float)20,(float)-80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)20,(float)-80} , {(float)-20,(float)-80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-20,(float)-80} , {(float)-11,(float)-159} , {0,0} , {100,100,100}},
	{Type_Line , {(float)60,(float)-60} , {(float)60,(float)-139} , {0,0} , {100,100,100}},
	{Type_Line , {(float)60,(float)-139} , {(float)24,(float)-157} , {0,0} , {100,100,100}},
	{Type_Line , {(float)24,(float)-157} , {(float)34,(float)-71} , {0,0} , {100,100,100}},
	{Type_Line , {(float)34,(float)-71} , {(float)60,(float)-60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-25,(float)-155} , {(float)-60,(float)-140} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-60,(float)-140} , {(float)-60,(float)-60} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-60,(float)-60} , {(float)-33,(float)-72} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-33,(float)-72} , {(float)-25,(float)-154} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-100,(float)-160} , {(float)-41,(float)-200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-41,(float)-200} , {(float)-21,(float)-180} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-21,(float)-180} , {(float)21,(float)-180} , {0,0} , {100,100,100}},
	{Type_Line , {(float)21,(float)-180} , {(float)40,(float)-199} , {0,0} , {100,100,100}},
	{Type_Line , {(float)40,(float)-199} , {(float)100,(float)-160} , {0,0} , {100,100,100}},
	{Type_Line , {(float)100,(float)-160} , {(float)80,(float)-20} , {0,0} , {100,100,100}},
	{Type_Line , {(float)80,(float)-20} , {(float)160,(float)80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)160,(float)80} , {(float)140,(float)160} , {0,0} , {100,100,100}},
	{Type_Line , {(float)140,(float)160} , {(float)0,(float)140} , {0,0} , {100,100,100}},
	{Type_Line , {(float)0,(float)140} , {(float)-140,(float)160} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-140,(float)160} , {(float)-160,(float)79} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-160,(float)79} , {(float)-80,(float)-20} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-80,(float)-20} , {(float)-100,(float)-159} , {0,0} , {100,100,100}},
};

Collision_Data MainCharacter_Collisions[1];


/*"Node_Shape" Primitives and Collision Data*/
Primitive Node_Shape_Primitives[5]=
{
	{Type_Line , {(float)-20,(float)0} , {(float)20,(float)0} , {0,0} , {200,200,0}},
	{Type_Line , {(float)20,(float)0} , {(float)20,(float)-40} , {0,0} , {200,200,0}},
	{Type_Line , {(float)20,(float)-40} , {(float)-20,(float)-40} , {0,0} , {200,200,0}},
	{Type_Line , {(float)-20,(float)-40} , {(float)-20,(float)0} , {0,0} , {200,200,0}},
	//{Type_Line , {(float)-20,(float)-20} , {(float)-80,(float)-20} , {0,0} , {200,0,0}},
	//{Type_Line , {(float)-20,(float)-20} , {(float)-40,(float)-10} , {0,0} , {200,0,0}},
	//{Type_Line , {(float)-40,(float)-10} , {(float)-40,(float)-29} , {0,0} , {200,0,0}},
	//{Type_Line , {(float)-40,(float)-29} , {(float)-20,(float)-20} , {0,0} , {200,0,0}},
	/*{Type_Line , {(float)20,(float)-20} , {(float)40,(float)-20} , {0,0} , {0,0,200}},
	{Type_Line , {(float)40,(float)-20} , {(float)20,(float)-30} , {0,0} , {0,0,200}},
	{Type_Line , {(float)20,(float)-30} , {(float)20,(float)-10} , {0,0} , {0,0,200}},
	{Type_Line , {(float)20,(float)-10} , {(float)40,(float)-20} , {0,0} , {0,0,200}},*/
};

Collision_Data Node_Shape_Collisions[1];

/********************************** MAP ************************************/
/*"Final0" Primitives and Collision Data*/
Primitive Final0_Primitives[1];
Collision_Data Final0_Collisions[22]=
{
	{Type_Collision_Line , {(float)-296,(float)71} , {(float)-250,(float)92} , {(float)0.415292,(float)0.909688}},
	{Type_Collision_Line , {(float)-250,(float)92} , {(float)-204,(float)182} , {(float)0.890435,(float)0.455111}},
	{Type_Collision_Line , {(float)-204,(float)182} , {(float)-100,(float)105} , {(float)-0.595043,(float)0.803694}},
	{Type_Collision_Line , {(float)-100,(float)105} , {(float)-78,(float)15} , {(float)-0.971399,(float)0.237453}},
	{Type_Collision_Line , {(float)-78,(float)15} , {(float)-110,(float)-165} , {(float)-0.984563,(float)-0.175033}},
	{Type_Collision_Line , {(float)-110,(float)-165} , {(float)-246,(float)-76} , {(float)0.547581,(float)-0.836753}},
	{Type_Collision_Line , {(float)-246,(float)-76} , {(float)-296,(float)71} , {(float)0.946733,(float)-0.322018}},
	{Type_Collision_Line , {(float)3,(float)161} , {(float)98,(float)197} , {(float)0.354357,(float)0.93511}},
	{Type_Collision_Line , {(float)98,(float)197} , {(float)139,(float)15} , {(float)-0.975552,(float)0.219767}},
	{Type_Collision_Line , {(float)139,(float)15} , {(float)66,(float)-106} , {(float)-0.856242,(float)-0.516576}},
	{Type_Collision_Line , {(float)66,(float)-106} , {(float)9,(float)-106} , {(float)0,(float)-1}},
	{Type_Collision_Line , {(float)9,(float)-106} , {(float)-37,(float)-65} , {(float)0.665371,(float)-0.746513}},
	{Type_Collision_Line , {(float)-37,(float)-65} , {(float)3,(float)161} , {(float)0.984696,(float)0.174282}},
	{Type_Collision_Line , {(float)295,(float)-188} , {(float)185,(float)-206} , {(float)-0.161489,(float)-0.986875}},
	{Type_Collision_Line , {(float)185,(float)-206} , {(float)141,(float)-56} , {(float)0.994706,(float)-0.102759}},
	{Type_Collision_Line , {(float)141,(float)-56} , {(float)199,(float)47} , {(float)0.27146,(float)0.96245}},
	{Type_Collision_Line , {(float)199,(float)47} , {(float)211,(float)197} , {(float)0.996815,(float)0.0797452}},
	{Type_Collision_Line , {(float)211,(float)197} , {(float)261,(float)87} , {(float)-0.910366,(float)0.413803}},
	{Type_Collision_Line , {(float)261,(float)87} , {(float)305,(float)100} , {(float)0.283346,(float)0.959018}},
	{Type_Collision_Line , {(float)305,(float)100} , {(float)261,(float)-115} , {(float)-0.979695,(float)-0.200496}},
	{Type_Collision_Line , {(float)261,(float)-115} , {(float)295,(float)-188} , {(float)-0.9065,(float)0.422206}},
};

/*"PatTest" Primitives and Collision Data*/
Primitive Map_Primitives[1];
Collision_Data Map_Collisions[17]=
{
	{Type_Collision_Line , {(float)140,(float)140} , {(float)180,(float)60} , {(float)-0.894427,(float)0.447214}},
	{Type_Collision_Line , {(float)180,(float)60} , {(float)240,(float)100} , {(float)0.5547,(float)0.83205}},
	{Type_Collision_Line , {(float)240,(float)100} , {(float)280,(float)39} , {(float)-0.836244,(float)0.548357}},
	{Type_Collision_Line , {(float)280,(float)39} , {(float)139,(float)20} , {(float)-0.133545,(float)-0.991043}},
	{Type_Collision_Line , {(float)139,(float)20} , {(float)200,(float)-60} , {(float)-0.795204,(float)0.606343}},
	{Type_Collision_Line , {(float)200,(float)-60} , {(float)80,(float)-100} , {(float)-0.316228,(float)-0.948683}},
	{Type_Collision_Line , {(float)80,(float)-100} , {(float)-10,(float)-21} , {(float)0.659686,(float)-0.751541}},
	{Type_Collision_Line , {(float)-10,(float)-21} , {(float)140,(float)140} , {(float)0.731659,(float)0.68167}},

	{Type_Collision_Line , {(float)-214,(float)208} , {(float)-120,(float)188} , {(float)-0.208108,(float)0.978106}},
	{Type_Collision_Line , {(float)-120,(float)188} , {(float)-98,(float)133} , {(float)-0.928477,(float)0.371391}},
	{Type_Collision_Line , {(float)-98,(float)133} , {(float)-195,(float)9} , {(float)0.0617377,(float)-0.998092}},
	{Type_Collision_Line , {(float)-195,(float)9} , {(float)-214,(float)208} , {(float)0.964116,(float)-0.265481}},

	{Type_Collision_Line , {(float)-271,(float)-113} , {(float)-38,(float)-70} , {(float)0.180735,(float)0.983532}},
	{Type_Collision_Line , {(float)-38,(float)-70} , {(float)-58,(float)-175} , {(float)-0.980581,(float)-0.196116}},
	{Type_Collision_Line , {(float)-58,(float)-175} , {(float)-230,(float)-185} , {(float)-0.0580415,(float)-0.998314}},
	{Type_Collision_Line , {(float)-230,(float)-185} , {(float)-271,(float)-113} , {(float)0.868985,(float)-0.494839}},

	
};

/*"Solid_Small_2" Primitives and Collision Data*/
Primitive Solid_Small_2_Primitives[1];
Collision_Data Solid_Small_2_Collisions[6]=
{
	{Type_Collision_Line , {(float)-30,(float)178} , {(float)196,(float)109} , {(float)-0.292003,(float)0.956417}},
	{Type_Collision_Line , {(float)196,(float)109} , {(float)239,(float)-36} , {(float)-0.958731,(float)0.284313}},
	{Type_Collision_Line , {(float)239,(float)-36} , {(float)-9,(float)-103} , {(float)-0.260811,(float)-0.96539}},
	{Type_Collision_Line , {(float)-9,(float)-103} , {(float)-201,(float)3} , {(float)0.483318,(float)-0.875445}},
	{Type_Collision_Line , {(float)-201,(float)3} , {(float)-30,(float)178} , {(float)0.715234,(float)0.698885}},
};