#ifndef _KEYBOARDBOSS_H_
#define _KEYBOARDBOSS_H_

#include "KeyboardInput.h"

class KeyboardBoss 
{		
	public:
			/* Pointer for all the KeyboardManagers */
			KeyboardManager * Managers_List;

			Char_Loader *LoaderArray[256];
			string Dir_Path;

			/* GlobalKeyboardManager Constructor */
			KeyboardBoss();

			/* A function to allocate memory to a loader */
			void Initialize_Loader(int position , char c, string s);

			/* Linking all Local Keyboard Managers to one Main Global KeyboardManagerList */
			void Attach_Manager(KeyboardManager *Manager);

			void Update_Managers (void);

			/* Rendering each KeyboardManager */
			void Render_Managers(void);

			~KeyboardBoss();
};

#endif