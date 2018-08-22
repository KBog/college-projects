#include "Viewing.h"
#include "Standard Header.h"
#include "Support_Functions.h"
#include <vector>
#include <list>

extern ViewPort *ViewPort_List;
extern Character *Map;

class Target
{
	public:
		Character *Shape;
		VERTEX InitialPosition;
		float Damage;
		
		Target (Character *shape)
		{
			Damage=10;
			Shape=shape;
			InitialPosition=Shape->GetSettings().Position;
		}

		void Reset (void)
		{
			Shape->GetSettings().Position=InitialPosition;
			Shape->GetSettings().Vector_Direction.x=0;
			Shape->GetSettings().Vector_Direction.y=0;
		}

		void Update (void)
		{
			if(Keyboard_IsPressed(VK_RIGHT))
			{
				Shape->GetSettings().Vector_Direction.x=1;
				Shape->GetSettings().Vector_Direction.y=0;
			}
			else if(Keyboard_IsPressed(VK_LEFT))
			{
				Shape->GetSettings().Vector_Direction.x=-1;
				Shape->GetSettings().Vector_Direction.y=0;
			}
			if(Keyboard_IsPressed(VK_UP))
			{
				Shape->GetSettings().Vector_Direction.x=0;
				Shape->GetSettings().Vector_Direction.y=1;
			}
			else if(Keyboard_IsPressed(VK_DOWN))
			{
				Shape->GetSettings().Vector_Direction.x=0;
				Shape->GetSettings().Vector_Direction.y=-1;
			}

			if(Shape->GetSettings().Position.x>290)
				Shape->GetSettings().Vector_Direction.x=-1;
			if(Shape->GetSettings().Position.x<-290)
				Shape->GetSettings().Vector_Direction.x=1;
			if(Shape->GetSettings().Position.y>220)
				Shape->GetSettings().Vector_Direction.y=-1;
			if(Shape->GetSettings().Position.y<-220)
				Shape->GetSettings().Vector_Direction.y=1;

			Shape->GetSettings().Position.x += Shape->GetSettings().Vector_Direction.x*Shape->GetSettings().Speed;
			Shape->GetSettings().Position.y += Shape->GetSettings().Vector_Direction.y*Shape->GetSettings().Speed;

			Damage=-1;
			float D=Shape->GetSettings().Position.GetDistance(Map->GetPrimitives()[4].V0);
			if(D<Map->GetPrimitives()[4].V1.x)
			{
				Damage=45;
				return;
			}
			D=Shape->GetSettings().Position.GetDistance(Map->GetPrimitives()[5].V0);
			if(D<Map->GetPrimitives()[5].V1.x)
			{
				Damage=90;
				return;
			}
			D=Shape->GetSettings().Position.GetDistance(Map->GetPrimitives()[6].V0);
			if(D<Map->GetPrimitives()[6].V1.x)
			{
				Damage=75;
				return;
			}
			D=Shape->GetSettings().Position.GetDistance(Map->GetPrimitives()[7].V0);
			if(D<Map->GetPrimitives()[7].V1.x)
			{
				Damage=60;
				return;
			}
			Damage=10;
		}

		void Draw (void)
		{
			Matrix<3> Object_Matrix, Mapping_Matrix, Final;
			/* Building the viewing matrix */
			Viewing_Transformation(ViewPort_List, Mapping_Matrix);
			
			/* Transforming the objects from their model coodinates to the world coord */
			Shape->BuildCharacterMatrix(Object_Matrix.M);
			/* Concatinating the final matrix */
			Final=Mapping_Matrix*Object_Matrix;
			/* Drawing the object */
			Shape->Draw(Final.M);

			//Point_Float V;
			//V.x=Shape->GetSettings().Position.x;
			//V.y=Shape->GetSettings().Position.y;				
			///*Get Transformed Vertices*/				
			//V=Final.M*V;	

			//Graphics_WritePixel((int)V.x,(int)V.y,200,0,0);
		}
};