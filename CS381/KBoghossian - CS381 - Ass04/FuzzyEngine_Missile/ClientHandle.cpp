#include "Standard Header.h"
#include "Viewing.h"
#include "Keyboard.h"
#include <math.h>

extern Camera *Camera_List;
extern ViewPort *ViewPort_List;


void Client_Handle (Character *Object_List)
{
	for(Character *O_tmp=Object_List; O_tmp; O_tmp=O_tmp->next)
		O_tmp->pf(O_tmp);
}

void Client_Draw (ViewPort *ViewPort_List, Character *Object_List)
{
	Matrix<3> Object_Matrix, Mapping_Matrix, Final;
	/* Looping through all the viewports */
	for(ViewPort *VP_Temp=ViewPort_List; VP_Temp; VP_Temp=VP_Temp->next)
	{
		/* Building the viewing matrix */
		Viewing_Transformation(VP_Temp, Mapping_Matrix);
		/* Looping through all the objects */
		for(Character *tmp=Object_List; tmp; tmp=tmp->next)
		{
			/* Transforming the objects from their model coodinates to the world coord */
			tmp->BuildCharacterMatrix(Object_Matrix.M);
			/* Concatinating the final matrix */
			Final=Mapping_Matrix*Object_Matrix;
			/* Drawing the object */
			tmp->Draw(Final.M);
		}
	}
}