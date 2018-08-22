#include "KeyboardBoss.h"


/* GlobalKeyboardManager Constructor */
KeyboardBoss::KeyboardBoss()
{
	Dir_Path=app.GetDirectory() + "Resources\\";
	Managers_List= NULL;

	/* Store the special characters */
	for(char k=0; k<33; k++)
		LoaderArray[k] = new Char_Loader(k);

	/* Here we must allocate memory for all the 255 characters but now we only have the G file */
	unsigned int i=33;
	char j=i;
	for(; i<256; i++, j=i)
	{
		//string s = Dir_Path + "Char_" + j + ".3DS";
		string s = Dir_Path + "Char_" + 'E' + ".3DS";
		Initialize_Loader(i,j,s);
	}
}

/* A function to allocate memory to a loader */
void KeyboardBoss::Initialize_Loader(int position , char c, string s)
{
	LoaderArray[position] = new Char_Loader(c,new _3DSLoader);
		
	/* Load the model */
	_3DSModel model;
	strcpy(model.fileName, s.c_str());
	LoaderArray[position]->CharLoader->Load3DSFiles(&model,1);
}

/* Linking all Local Keyboard Managers to one Main Global KeyboardManagerList */
void KeyboardBoss::Attach_Manager(KeyboardManager *Manager)
{
	Object *Head = (Object *)Managers_List;
	Manager->LoaderArray=LoaderArray;
	Add_ToList(&Head, (Object *)Manager);
	Managers_List=(KeyboardManager *)Head;
}

void KeyboardBoss::Update_Managers (void)
{
	for(Object *tmp = Managers_List; tmp; tmp=tmp->next)
		tmp->Update_Object();
}

/* Rendering each KeyboardManager */
void KeyboardBoss::Render_Managers(void)
{
	for(Object *tmp = Managers_List; tmp; tmp=tmp->next)
		((KeyboardManager *)tmp)->Render_Manager();
}

KeyboardBoss::~KeyboardBoss()
{
	/* Delete the memory allocated in the array of loaders */
	for(unsigned int i=0; i<256; i++)
		delete LoaderArray[i];
}