//#include"Standard Header"
//#include"Graphics.h"
//#include"Clipping.h"
//
//void Character::Draw(void)
//{
//	Point_Float V[3];
//	for(int i=0;i<Primitives_Num;i++)
//	{
//		/*Filling Temporary Point Float array*/
//		V[0].x=Primitives[i].V0.x;V[1].x=Primitives[i].V1.x;V[2].x=Primitives[i].V2.x;
//		V[0].y=Primitives[i].V0.y;V[1].y=Primitives[i].V1.y;V[2].y=Primitives[i].V2.y;
//
//		/*Draw Each Primitive Depending on its type*/
//		switch(Primitives[i].Type)
//		{
//			case Type_Line:
//							if(LB_Clipp(&V[0],&V[1]))
//								LINE(V[0].x,V[0].y,V[1].x,V[1].y,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);
//							break;
//			case Type_Circle:
//							CIRCLE(V[0].x,V[0].y,V[1].x,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);
//							break;
//			case Type_Filled_Circle:
//							FILL_CIRCLE(V[0].x,V[0].y,V[1].x,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);
//							break;
//			case Type_Triangle:
//							TRIANGLE(V,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);
//							break;
//			case Type_Filled_Triangle:
//							FILL_TRIANGLE(V,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);
//							break;
//		}
//	}
//}