#include "windows.h"		//Include the Windows library
#include "direct.h"
#include "Graphics.h"
#include "State Machine.h"
#include "Code Generation.h"
#include "View.h"
#include "Character.h"
#include "Interface.h"
#include "resource.h"
#include <commctrl.h>

HINSTANCE GlobalInstance;
HWND GlobalHwnd=NULL,hwndDlg=NULL;
enum Algorithm Algorithm_Selection=None;
BOOL WinMove=0;					//Declaring a Boolean to check if window moved
BOOL DlgMove=0;					//Declaring a Boolean to check if dialog moved
INT_PTR nResult;
char Dir[250];					//Current Working Directory
Color_RGB Primitive_Color={0,0,0};
extern int num;
extern POINT ppt[MAXCLICKS];
char Window_Styles[7]={0,0,0,0,0,1,0};
int Character_Count=0;
int Draw_Preview=1;
int Pick_Color=0;
enum Mode Character_Mode=Mode_Draw;
Selection Current_Selection={0};
RECT SourceRect={0, 0, 639, 480}, DestinationRect={0, 0, 639, 480};		//Source and Destination rectangles
bool ShowRects=1;
int Character_Toolbar_Button=0;	//Used to disable/enable character toolbar button

struct _TBBUTTON ToolArray[4];	//Used for the toolbar

/*Current Character User is working with*/
Character* Current_Character=NULL;

/* Current Character Settings */
Character_Settings Current_Character_Settings={0,0,1};

/*Configurations Settings*/
Game_Settings GameSettings={800,600,"WS_POPUP|WS_BORDER",60};
Camera_Settings Main_Camera={0.0f,0.0f,0.0f,2.0f,200.0f,150.0f,{0,0,0,0}},Radar_Camera={0.0f,0.0f,0.0f,0.5f,200.0f,150.0f,{0,0,0,0}};
ViewPort_Settings Main_ViewPort={480.0f,300.0f,400.0f,300.0f}, Radar_ViewPort={50.0f,40.0f,100.0f,75.0f};

void Disable_Enable_All (HWND Character_hwnd, bool check)
{
	EnableWindow(GetDlgItem(Character_hwnd, IDC_RADIO_DRAW_LINE), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_RADIO_DRAW_CIRCLE), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_RADIO_DRAW_TRIANGLE), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_CHECK_DRAW_PRIMITIVE_FILLED), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_EDIT_CHAR_RED), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_EDIT_CHAR_GREEN), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_EDIT_CHAR_BLUE), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_BUTTON_CHAR_PICKCOLOR), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_BUTTON_CHAR_COLLISION), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_BUTTON_CHAR_PROPERTIES), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_BUTTON_CHAR_ADD), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_BUTTON_CHAR_REMOVECHAR), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_BUTTON_CHAR_MODE), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_BUTTON_CHAR_REMOVE), check);
	EnableWindow(GetDlgItem(Character_hwnd, IDC_EDIT_CHAR_NAME), check);
	Algorithm_Selection=(Algorithm)check;
}

void Reset_Character_Dialog(HWND Character_hwndDlg)
{
	/* Hiding the TwoWay, Flip normal checkbox */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_TWOWAYCHECK), SW_HIDE);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_FLIPNORMAL), SW_HIDE);
	/*Hiding the draw button */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_DRAW), SW_HIDE);

	/* Setting the DrawLine radio to check and disabling all the others */
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_LINE), BM_SETCHECK, 1, 0);
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_CIRCLE), BM_SETCHECK, 0, 0);
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_TRIANGLE), BM_SETCHECK, 0, 0);
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), BM_SETCHECK, 0, 0);

	/* Filling the color edit boxes with the primitive color */
	SetDlgItemInt(Character_hwndDlg, IDC_EDIT_CHAR_RED, Primitive_Color.r, 0);
	SetDlgItemInt(Character_hwndDlg, IDC_EDIT_CHAR_GREEN, Primitive_Color.g, 0);
	SetDlgItemInt(Character_hwndDlg, IDC_EDIT_CHAR_BLUE, Primitive_Color.b, 0);

	/* Showing the PickColor button */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_PICKCOLOR), SW_SHOW);
	/* Showing the drawprimitivefilled checkbox */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), SW_SHOW);
	/* Showing the charactercollision button */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_COLLISION), SW_SHOW);
	/* Showing the triangle radio button */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_TRIANGLE), SW_SHOW);
	/* Showing the R, G, B static texts */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_RED), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_GREEN), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_BLUE), SW_SHOW);
	/* Showing the R, G, B edit boxes */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_RED), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_GREEN), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_BLUE), SW_SHOW);
	/* Showing the colorselection and the colorpreview static texts */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_COLORSELECTION), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_COLORPREVIEW), SW_SHOW);

	/* Showing the static texts */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_1), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_2), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_3), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_4), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_5), SW_SHOW);
	/* Hiding the collision radio buttons */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_LINE), SW_HIDE);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_CIRCLE), SW_HIDE);
	/* Showing the draw line and circle radio buttons */
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_LINE), SW_SHOW);
	ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_CIRCLE), SW_SHOW);
	/* Setting the algorithm selection to the line algo */
	Algorithm_Selection = Algorithm_Line;
	/* Setting the draw preview to 1 */
	Draw_Preview=1;
	/* Setting the collision line radio button to true and all the others to false */
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_LINE), BM_SETCHECK, 1, 0);
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_CIRCLE), BM_SETCHECK, 0, 0);
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_TWOWAYCHECK), BM_SETCHECK, 0, 0);
	SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_FLIPNORMAL), BM_SETCHECK, 0, 0);
}

void AlignDialogToWindow(HWND hwnd,HWND hwndDlg)
{
	/*Align Dialog Box to the right side of the Window*/
	RECT r,rd;
	GetWindowRect(hwnd,&r);
	GetWindowRect(hwndDlg,&rd);
	MoveWindow(hwndDlg,r.right,r.top,rd.right-rd.left,r.bottom-r.top,TRUE);
}

void AlignWindowToDialog(HWND hwnd,HWND hwndDlg)
{
	/*Align Window to  the left side of the Dialog Box*/
	RECT r,rd;
	GetWindowRect(hwnd,&r);
	GetWindowRect(hwndDlg,&rd);
	MoveWindow(hwnd,rd.left-(r.right-r.left),rd.top,r.right-r.left,r.bottom-r.top,TRUE);
}

void CenterInWindow(HWND hwnd1,HWND hwnd2)
{
	RECT r1,r2;
	/*Get both windows rectangles*/
	GetWindowRect(hwnd1,&r1);
	GetWindowRect(hwnd2,&r2);
	/*Center second window in first window*/
	MoveWindow(hwnd2,r1.left+((r1.right-r1.left)-(r2.right-r2.left))/2,r1.top+((r1.bottom-r1.top)-(r2.bottom-r2.top))/2,r2.right-r2.left,r2.bottom-r2.top,TRUE);
}

void AdjustWindowBack(HWND hwndDlg)
{
	RECT WinRect, DlgRect;							
	GetWindowRect(GlobalHwnd,&WinRect);
	GetWindowRect(hwndDlg,&DlgRect);
	RECT NewRect;
	NewRect.left = WinRect.left + ((DlgRect.right-DlgRect.left)>>1);
	NewRect.right = WinRect.right + ((DlgRect.right-DlgRect.left)>>1);
	NewRect.top = WinRect.top;
	NewRect.bottom = WinRect.bottom;
	SmoothTransition(GlobalHwnd, WinRect, NewRect, 3);
	//MoveWindow(GlobalHwnd,WinRect.left + ((DlgRect.right-DlgRect.left)>>1),WinRect.top, WinRect.right-WinRect.left,WinRect.bottom-WinRect.top,1);
}

LRESULT CALLBACK MovementDlgProc(HWND Camera_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
							CenterInWindow(GlobalHwnd,Camera_hwndDlg);
							return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDOK:
							case IDCANCEL:
										EndDialog(Camera_hwndDlg,nResult);
										break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK Radar_ViewPort_DlgProc(HWND ViewPort_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	bool Check_View_Dimensions=0;
	switch(message)
	{
		case WM_INITDIALOG:
							SetWindowText(ViewPort_hwndDlg, "Radar ViewPort Settings");
							CenterInWindow(GlobalHwnd, ViewPort_hwndDlg);
							/*Initializing Controls*/
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_X,(int)Radar_ViewPort.Pos_x,1);
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_Y,(int)Radar_ViewPort.Pos_y,1);
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_WIDTH,(int)Radar_ViewPort.Width,1);
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_HEIGHT,(int)Radar_ViewPort.Height,1);
							return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDOK:
										{
											/*Remember Radar Camera Settings*/
											char F[50];
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_X,F,50);
											Radar_ViewPort.Pos_x=(float)atof(F);
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_Y,F,50);
											Radar_ViewPort.Pos_y=(float)atof(F);
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_WIDTH,F,50);
											Radar_ViewPort.Width=(float)atof(F);
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_HEIGHT,F,50);
											Radar_ViewPort.Height=(float)atof(F);
													
											/* Checking if the center and the viewport dimensions are compatible with
											   each other relative to the window dimensions */
											if(Radar_ViewPort.Pos_x>0 && Radar_ViewPort.Pos_x<GameSettings.Width)
											{
												if((Radar_ViewPort.Pos_x + Radar_ViewPort.Width/2)>GameSettings.Width)
												{
													Radar_ViewPort.Width = 2*(GameSettings.Width - Radar_ViewPort.Pos_x);
													Check_View_Dimensions=1;
												}
												else if((Radar_ViewPort.Pos_x - Radar_ViewPort.Width/2)<0)
												{
													Radar_ViewPort.Width = 2*Radar_ViewPort.Pos_x;
													Check_View_Dimensions=1;
												}
											}
											else
											{
												if(Radar_ViewPort.Pos_x>GameSettings.Width)
												{
													Radar_ViewPort.Pos_x = (float)(GameSettings.Width-Radar_ViewPort.Width/2);
													if(Radar_ViewPort.Pos_x>GameSettings.Width)
													{
														Radar_ViewPort.Pos_x = (float)(GameSettings.Width-50);
														Radar_ViewPort.Width = 100;
													}
													else
													{
														Radar_ViewPort.Pos_x = 50;
														Radar_ViewPort.Width = 100;
													}
												}
												else
												{
													Radar_ViewPort.Pos_x = (float)(Radar_ViewPort.Width/2);
													if(Radar_ViewPort.Pos_x<0)
													{
														Radar_ViewPort.Pos_x = 50;
														Radar_ViewPort.Width = 100;
													}
													else if(Radar_ViewPort.Pos_x>GameSettings.Width)
													{
														Radar_ViewPort.Pos_x = (float)(GameSettings.Width-50);
														Radar_ViewPort.Width = 100;
													}
												}
												Check_View_Dimensions=1;
											}

											if(Radar_ViewPort.Pos_y>0 && Radar_ViewPort.Pos_y<GameSettings.Height)
											{
												if((Radar_ViewPort.Pos_y + Radar_ViewPort.Height/2)>GameSettings.Height)
												{
													Radar_ViewPort.Height = 2*(GameSettings.Height - Radar_ViewPort.Pos_y);
													Check_View_Dimensions=1;
												}
												else if((Radar_ViewPort.Pos_y - Radar_ViewPort.Height/2)<0)
												{
													Radar_ViewPort.Height = 2*Radar_ViewPort.Pos_y;
													Check_View_Dimensions=1;
												}
											}
											else
											{
												if(Radar_ViewPort.Pos_y>GameSettings.Height)
												{
													Radar_ViewPort.Pos_y = (float)(GameSettings.Height-Radar_ViewPort.Height/2);
													if(Radar_ViewPort.Pos_y>GameSettings.Height)
													{
														Radar_ViewPort.Pos_y = (float)(GameSettings.Height-36);
														Radar_ViewPort.Height = 75;
													}
													else if(Radar_ViewPort.Pos_y<0)
													{
														Radar_ViewPort.Pos_y = 36;
														Radar_ViewPort.Height = 75;
													}
												}
												else
												{
													Radar_ViewPort.Pos_y = (float)(Radar_ViewPort.Height/2);
													if(Radar_ViewPort.Pos_y<0)
													{
														Radar_ViewPort.Pos_y = 36;
														Radar_ViewPort.Height = 75;
													}
													else if(Radar_ViewPort.Pos_y>GameSettings.Height)
													{
														Radar_ViewPort.Pos_y = (float)(GameSettings.Height-36);
														Radar_ViewPort.Height = 75;
													}
												}
												Check_View_Dimensions=1;
											}

											if(Check_View_Dimensions)
												MessageBox(ViewPort_hwndDlg,"ViewPort dimensions re-adjusted to fit Window!","Warning",MB_OK|MB_TOPMOST|MB_ICONEXCLAMATION);
										}
							case IDCANCEL:
										EndDialog(ViewPort_hwndDlg, nResult);
										break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK Radar_Camera_DlgProc(HWND Camera_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	static Checked=0;
	switch(message)
	{
		case WM_INITDIALOG:
							SetWindowText(Camera_hwndDlg, "Radar Camera Settings");
							CenterInWindow(GlobalHwnd,Camera_hwndDlg);
							/*Initializing Controls*/
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_X,(int)Radar_Camera.Pos_x,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_Y,(int)Radar_Camera.Pos_y,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_WIDTH,(int)Radar_Camera.Width,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_HEIGHT,(int)Radar_Camera.Height,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_ANGLE,(int)Radar_Camera.Angle,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_SCALE,(int)Radar_Camera.Scale,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_LEFT,(int)Radar_Camera.Scrolling_Box.left,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_TOP,(int)Radar_Camera.Scrolling_Box.top,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT,(int)Radar_Camera.Scrolling_Box.right,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM,(int)Radar_Camera.Scrolling_Box.bottom,1);
							SendDlgItemMessage(Camera_hwndDlg,IDC_CHECK_SBB,BM_SETCHECK,Checked,0);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_LEFT),Checked);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_TOP),Checked);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT),Checked);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM),Checked);
							return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDC_CHECK_SBB:
											Checked=!Checked;
											/*Enabling or Disabling SBB controls*/
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_LEFT),Checked);
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_TOP),Checked);
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT),Checked);
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM),Checked);
											break;
							case IDOK:
										{
											/*Remember Radar Camera Settings*/
											char F[50];
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_X,F,50);
											Radar_Camera.Pos_x=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_Y,F,50);
											Radar_Camera.Pos_y=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_WIDTH,F,50);
											Radar_Camera.Width=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_HEIGHT,F,50);
											Radar_Camera.Height=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_ANGLE,F,50);
											Radar_Camera.Angle=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_SCALE,F,50);
											Radar_Camera.Scale=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_LEFT,F,50);
											Radar_Camera.Scrolling_Box.left=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_TOP,F,50);
											Radar_Camera.Scrolling_Box.top=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT,F,50);
											Radar_Camera.Scrolling_Box.right=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM,F,50);
											Radar_Camera.Scrolling_Box.bottom=(float)atof(F);
										}
							case IDCANCEL:
										EndDialog(Camera_hwndDlg,nResult);
										break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK Main_ViewPort_DlgProc(HWND ViewPort_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	bool Check_View_Dimensions=0;
	switch(message)
	{
		case WM_INITDIALOG:
							SetWindowText(ViewPort_hwndDlg, "Main ViewPort Settings");
							CenterInWindow(GlobalHwnd, ViewPort_hwndDlg);
							/*Initializing Controls*/
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_X,(int)Main_ViewPort.Pos_x,1);
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_Y,(int)Main_ViewPort.Pos_y,1);
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_WIDTH,(int)Main_ViewPort.Width,1);
							SetDlgItemInt(ViewPort_hwndDlg,IDC_EDIT_VIEW_HEIGHT,(int)Main_ViewPort.Height,1);
							return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDOK:
										{
											/*Remember Main Camera Settings*/
											char F[50];
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_X,F,50);
											Main_ViewPort.Pos_x=(float)atof(F);
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_CENTER_Y,F,50);
											Main_ViewPort.Pos_y=(float)atof(F);
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_WIDTH,F,50);
											Main_ViewPort.Width=(float)atof(F);
											GetDlgItemText(ViewPort_hwndDlg,IDC_EDIT_VIEW_HEIGHT,F,50);
											Main_ViewPort.Height=(float)atof(F);
													
											/* Checking if the center and the viewport dimensions are compatible with
											   each other relative to the window dimensions */
											if(Main_ViewPort.Pos_x>0 && Main_ViewPort.Pos_x<GameSettings.Width)
											{
												if((Main_ViewPort.Pos_x + Main_ViewPort.Width/2)>GameSettings.Width)
												{
													Main_ViewPort.Width = 2*(GameSettings.Width - Main_ViewPort.Pos_x);
													Check_View_Dimensions=1;
												}
												else if((Main_ViewPort.Pos_x - Main_ViewPort.Width/2)<0)
												{
													Main_ViewPort.Width = 2*Main_ViewPort.Pos_x;
													Check_View_Dimensions=1;
												}
											}
											else
											{
												if(Main_ViewPort.Pos_x>GameSettings.Width)
												{
													Main_ViewPort.Pos_x = (float)(GameSettings.Width-Main_ViewPort.Width/2);
													if(Main_ViewPort.Pos_x>GameSettings.Width)
													{
														Main_ViewPort.Pos_x = (float)(GameSettings.Width-200);
														Main_ViewPort.Width = 400;
													}
													else
													{
														Main_ViewPort.Pos_x = 200;
														Main_ViewPort.Width = 400;
													}
												}
												else
												{
													Main_ViewPort.Pos_x = (float)(Main_ViewPort.Width/2);
													if(Main_ViewPort.Pos_x<0)
													{
														Main_ViewPort.Pos_x = 200;
														Main_ViewPort.Width = 400;
													}
													else if(Main_ViewPort.Pos_x>GameSettings.Width)
													{
														Main_ViewPort.Pos_x = (float)(GameSettings.Width-200);
														Main_ViewPort.Width = 400;
													}
												}
												Check_View_Dimensions=1;
											}

											if(Main_ViewPort.Pos_y>0 && Main_ViewPort.Pos_y<GameSettings.Height)
											{
												if((Main_ViewPort.Pos_y + Main_ViewPort.Height/2)>GameSettings.Height)
												{
													Main_ViewPort.Height = 2*(GameSettings.Height - Main_ViewPort.Pos_y);
													Check_View_Dimensions=1;
												}
												else if((Main_ViewPort.Pos_y - Main_ViewPort.Height/2)<0)
												{
													Main_ViewPort.Height = 2*Main_ViewPort.Pos_y;
													Check_View_Dimensions=1;
												}
											}
											else
											{
												if(Main_ViewPort.Pos_y>GameSettings.Height)
												{
													Main_ViewPort.Pos_y = (float)(GameSettings.Height-Main_ViewPort.Height/2);
													if(Main_ViewPort.Pos_y>GameSettings.Height)
													{
														Main_ViewPort.Pos_y = (float)(GameSettings.Height-150);
														Main_ViewPort.Height = 300;
													}
													else if(Main_ViewPort.Pos_y<0)
													{
														Main_ViewPort.Pos_y = 150;
														Main_ViewPort.Height = 300;
													}
												}
												else
												{
													Main_ViewPort.Pos_y = (float)(Main_ViewPort.Height/2);
													if(Main_ViewPort.Pos_y<0)
													{
														Main_ViewPort.Pos_y = 150;
														Main_ViewPort.Height = 300;
													}
													else if(Main_ViewPort.Pos_y>GameSettings.Height)
													{
														Main_ViewPort.Pos_y = (float)(GameSettings.Height-150);
														Main_ViewPort.Height = 300;
													}
												}
												Check_View_Dimensions=1;
											}

											if(Check_View_Dimensions)
												MessageBox(ViewPort_hwndDlg,"ViewPort dimensions re-adjusted to fit Window!","Warning",MB_OK|MB_TOPMOST|MB_ICONEXCLAMATION);
										}
							case IDCANCEL:
										EndDialog(ViewPort_hwndDlg, nResult);
										break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK Main_Camera_DlgProc(HWND Camera_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	static Checked=0;
	switch(message)
	{
		case WM_INITDIALOG:
							SetWindowText(Camera_hwndDlg, "Main Camera Settings");
							CenterInWindow(GlobalHwnd,Camera_hwndDlg);
							/*Initializing Controls*/
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_X,(int)Main_Camera.Pos_x,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_Y,(int)Main_Camera.Pos_y,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_WIDTH,(int)Main_Camera.Width,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_HEIGHT,(int)Main_Camera.Height,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_ANGLE,(int)Main_Camera.Angle,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_CAM_SCALE,(int)Main_Camera.Scale,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_LEFT,(int)Main_Camera.Scrolling_Box.left,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_TOP,(int)Main_Camera.Scrolling_Box.top,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT,(int)Main_Camera.Scrolling_Box.right,1);
							SetDlgItemInt(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM,(int)Main_Camera.Scrolling_Box.bottom,1);
							SendDlgItemMessage(Camera_hwndDlg,IDC_CHECK_SBB,BM_SETCHECK,Checked,0);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_LEFT),Checked);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_TOP),Checked);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT),Checked);
							EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM),Checked);
							return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDC_CHECK_SBB:
											Checked=!Checked;
											/*Enabling or Disabling SBB controls*/
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_LEFT),Checked);
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_TOP),Checked);
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT),Checked);
											EnableWindow(GetDlgItem(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM),Checked);
											break;
							case IDOK:
										{
											/*Remember Main Camera Settings*/
											char F[50];
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_X,F,50);
											Main_Camera.Pos_x=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_CENTER_Y,F,50);
											Main_Camera.Pos_y=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_WIDTH,F,50);
											Main_Camera.Width=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_HEIGHT,F,50);
											Main_Camera.Height=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_ANGLE,F,50);
											Main_Camera.Angle=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_CAM_SCALE,F,50);
											Main_Camera.Scale=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_LEFT,F,50);
											Main_Camera.Scrolling_Box.left=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_TOP,F,50);
											Main_Camera.Scrolling_Box.top=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_RIGHT,F,50);
											Main_Camera.Scrolling_Box.right=(float)atof(F);
											GetDlgItemText(Camera_hwndDlg,IDC_EDIT_SBB_BOTTOM,F,50);
											Main_Camera.Scrolling_Box.bottom=(float)atof(F);
										}
							case IDCANCEL:
										EndDialog(Camera_hwndDlg,nResult);
										break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK Camera_ViewPort_DlgProc(HWND Camera_ViewPort_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
							CenterInWindow(GlobalHwnd,Camera_ViewPort_hwndDlg);
							return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDC_BUTTON_MAIN_CAMERASETTINGS:
								nResult=DialogBox(GlobalInstance,MAKEINTRESOURCE(IDD_DIALOG_CAMSETTINGS),Camera_ViewPort_hwndDlg,(DLGPROC)Main_Camera_DlgProc);
								break;

							case IDC_BUTTON_MAIN_VIEWPORTSETTINGS:
								nResult=DialogBox(GlobalInstance,MAKEINTRESOURCE(IDD_DIALOG_VIEWSETTINGS),Camera_ViewPort_hwndDlg,(DLGPROC)Main_ViewPort_DlgProc);
								break;

							case IDC_BUTTON_RADAR_CAMERASETTINGS:
								nResult=DialogBox(GlobalInstance,MAKEINTRESOURCE(IDD_DIALOG_CAMSETTINGS),Camera_ViewPort_hwndDlg,(DLGPROC)Radar_Camera_DlgProc);
								break;

							case IDC_BUTTON_RADAR_VIEWPORTSETTINGS:
								nResult=DialogBox(GlobalInstance,MAKEINTRESOURCE(IDD_DIALOG_VIEWSETTINGS),Camera_ViewPort_hwndDlg,(DLGPROC)Radar_ViewPort_DlgProc);
								break;

							case IDOK:
							case IDCANCEL:
										EndDialog(Camera_ViewPort_hwndDlg,nResult);
										break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK GameDlgProc(HWND Game_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
							/*Center in main window*/
							CenterInWindow(GlobalHwnd,Game_hwndDlg);
							/*Load Saved Settings*/
							SetDlgItemInt(Game_hwndDlg,IDD_WINDOW_WIDTH,GameSettings.Width,0);
							SetDlgItemInt(Game_hwndDlg,IDD_WINDOW_HEIGHT,GameSettings.Height,0);
							SetDlgItemInt(Game_hwndDlg,IDD_FRAMERATE,GameSettings.FrameRate,0);
							/*Set Current Window Style*/
							SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_BORDER,BM_SETCHECK,Window_Styles[0],0);
							SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_CAPTION,BM_SETCHECK,Window_Styles[1],0);
							SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_MAXIMIZE,BM_SETCHECK,Window_Styles[2],0);
							SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_MINIMIZE,BM_SETCHECK,Window_Styles[3],0);
							SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_OVERLAPPED,BM_SETCHECK,Window_Styles[4],0);
							SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_POPUP,BM_SETCHECK,Window_Styles[5],0);
							SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_SYSMENU,BM_SETCHECK,Window_Styles[6],0);
							return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDC_STYLE_POPUP:
												SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_OVERLAPPED,BM_SETCHECK,0,0);
												break;
							case IDC_STYLE_OVERLAPPED:
												SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_POPUP,BM_SETCHECK,0,0);
												break;
							case IDOK:
										/*Remember Game Settings*/
										GameSettings.Width=GetDlgItemInt(Game_hwndDlg,IDD_WINDOW_WIDTH,NULL,0);
										GameSettings.Height=GetDlgItemInt(Game_hwndDlg,IDD_WINDOW_HEIGHT,NULL,0);
										GameSettings.FrameRate=GetDlgItemInt(Game_hwndDlg,IDD_FRAMERATE,NULL,0);
										/*Get Window Style*/
										{
											char Style[150];
											*Style=0;
											if(SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_BORDER,BM_GETCHECK,0,0))
											{
												sprintf(Style,"%s|WS_BORDER",Style);
												Window_Styles[0]=1;
											}
											else
												Window_Styles[0]=0;
											if(SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_CAPTION,BM_GETCHECK,0,0))
											{
												sprintf(Style,"%s|WS_CAPTION",Style);
												Window_Styles[1]=1;
											}
											else
												Window_Styles[1]=0;
											if(SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_MAXIMIZE,BM_GETCHECK,0,0))
											{
												sprintf(Style,"%s|WS_MAXIMIZEBOX",Style);
												Window_Styles[2]=1;
											}
											else
												Window_Styles[2]=0;
											if(SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_MINIMIZE,BM_GETCHECK,0,0))
											{
												sprintf(Style,"%s|WS_MINIMIZEBOX",Style);
												Window_Styles[3]=1;
											}
											else
												Window_Styles[3]=0;
											if(SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_OVERLAPPED,BM_GETCHECK,0,0))
											{
												sprintf(Style,"%s|WS_OVERLAPPED",Style);
												Window_Styles[4]=1;
											}
											else
												Window_Styles[4]=0;
											if(SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_POPUP,BM_GETCHECK,0,0))
											{
												sprintf(Style,"%s|WS_POPUP",Style);
												Window_Styles[5]=1;
											}
											else
												Window_Styles[5]=0;
											if(SendDlgItemMessage(Game_hwndDlg,IDC_STYLE_SYSMENU,BM_GETCHECK,0,0))
											{
												sprintf(Style,"%s|WS_SYSMENU",Style);
												Window_Styles[6]=1;
											}
											else
												Window_Styles[6]=0;
											/*Set Built Window Style*/
											char *p=Style+1;//Ignore leading |
											strcpy(GameSettings.Style,p);
										}
							case IDCANCEL:
										EndDialog(Game_hwndDlg,nResult);
										break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK DialogProcSDRect(HWND SDRect_hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	SourceRect=Current_Character->GetSourceRect();
	DestinationRect=Current_Character->GetDestinationRect();
	switch(message)
	{
		case WM_INITDIALOG:
			/* Centering the dialog box in the screen */
			CenterInWindow(GlobalHwnd, SDRect_hwndDlg);
			/* Setting the initial values of the rectangles */
			SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, SourceRect.right-SourceRect.left, 0);
			SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, SourceRect.bottom-SourceRect.top, 0);
			SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, DestinationRect.right-DestinationRect.left, 0);
			SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, DestinationRect.bottom-DestinationRect.top, 0);
			/* Setting the drawing variable to true */
			SendMessage(GetDlgItem(SDRect_hwndDlg, IDC_CHECK_CHAR_SDRect_SHOW), BM_SETCHECK, ShowRects, 0);
			return TRUE;
		
		case WM_COMMAND:
				switch(LOWORD(wParam))
				{
					case IDC_CHECK_CHAR_SDRect_SHOW:
						/* Checking if we draw the rects */
						if(SendMessage(GetDlgItem(SDRect_hwndDlg, IDC_CHECK_CHAR_SDRect_SHOW), BM_GETCHECK, 1, 0))
						{
							ShowRects=1;
							Reset_Surface();
							/* Drawing the character */
							Current_Character->Draw();
							/* Checking if we draw the collision */
							if(!Draw_Preview)
								Current_Character->DrawCollision();
							Graphics_Refresh();
						}
						else
						{
							ShowRects=0;
							/* Clean the surface */
							Reset_Surface();
							/* Drawing the character */
							Current_Character->Draw();
							/* Checking if we draw the collision */
							if(!Draw_Preview)
								Current_Character->DrawCollision();
							/* Refreshing the screen */
							Graphics_Refresh();
						}
						break;

					case IDOK:
						/* checking if greater than the viewport */
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, 0, 0)>=640)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, SourceRect.right-SourceRect.left, 0);
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, 0, 0)>=480)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, SourceRect.bottom-SourceRect.top, 0);
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, 0, 0)>=640)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, DestinationRect.right-DestinationRect.left, 0);
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, 0, 0)>=480)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, DestinationRect.bottom-DestinationRect.top, 0);
						/* Updating the source rect */
						SourceRect.left=320-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, 0, 0)>>1);
						SourceRect.right=320+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, 0, 0)>>1);
						SourceRect.top=240-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, 0, 0)>>1);
						SourceRect.bottom=240+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, 0, 0)>>1);
						/* Updating the destination rect */
						DestinationRect.left=320-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, 0, 0)>>1);
						DestinationRect.right=320+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, 0, 0)>>1);
						DestinationRect.top=240-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, 0, 0)>>1);
						DestinationRect.bottom=240+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, 0, 0)>>1);
						/* Clean */
						Reset_Surface();
						/* Drawing the character */
						Current_Character->Draw();
						/* Checking if we draw the collision */
						if(!Draw_Preview)
							Current_Character->DrawCollision();
						/* Refresh */
						Graphics_Refresh();
						/* Updating the rects in the current character */
						Current_Character->SetSourceRect(SourceRect);
						Current_Character->SetDestinationRect(DestinationRect);
						EndDialog(SDRect_hwndDlg, 0);
						break;

					case IDCANCEL:
						EndDialog(SDRect_hwndDlg, 0);
						break;

				}
				switch(HIWORD(wParam))
				{
					case EN_KILLFOCUS:
						/* checking if greater than the viewport */
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, 0, 0)>=640)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, SourceRect.right-SourceRect.left, 0);
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, 0, 0)>=480)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, SourceRect.bottom-SourceRect.top, 0);
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, 0, 0)>=640)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, DestinationRect.right-DestinationRect.left, 0);
						if(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, 0, 0)>=480)
							SetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, DestinationRect.bottom-DestinationRect.top, 0);
						/* Updating the source rect */
						SourceRect.left=320-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, 0, 0)>>1);
						SourceRect.right=320+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SWIDTH, 0, 0)>>1);
						SourceRect.top=240-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, 0, 0)>>1);
						SourceRect.bottom=240+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_SHEIGHT, 0, 0)>>1);
						/* Updating the destination rect */
						DestinationRect.left=320-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, 0, 0)>>1);
						DestinationRect.right=320+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DWIDTH, 0, 0)>>1);
						DestinationRect.top=240-(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, 0, 0)>>1);
						DestinationRect.bottom=240+(GetDlgItemInt(SDRect_hwndDlg, IDC_EDIT_DHEIGHT, 0, 0)>>1);
						/* Clean and Refresh */
						Reset_Surface();
						/* Drawing the character */
						Current_Character->Draw();
						/* Checking if we draw the collision */
						if(!Draw_Preview)
							Current_Character->DrawCollision();
						/* Updating the rects in the current character */
						Current_Character->SetSourceRect(SourceRect);
						Current_Character->SetDestinationRect(DestinationRect);
						Graphics_Refresh();
						break;
				}
				return TRUE;

	}
	return FALSE;	
}

LRESULT CALLBACK CharacterPropertiesDlgProc(HWND Character_Properties_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
						{
							/* Getting the index of the current selection */
							int index = (int)SendMessage(GetDlgItem(hwndDlg, IDC_LIST_CHAR), LB_GETCURSEL, 0, 0);
							if(index!=-1)
								/* Getting the settings of the selected character */
								Current_Character_Settings = ((Character *)SendMessage(GetDlgItem(hwndDlg, IDC_LIST_CHAR), LB_GETITEMDATA, index, 0))->GetSettings();

							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_POSX, (UINT)Current_Character_Settings.Position.x, 1);
							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_POSY, (UINT)Current_Character_Settings.Position.y, 1);
							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_SCALE, (UINT)Current_Character_Settings.Scale, 0);
							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_ANGLE, (UINT)Current_Character_Settings.Angle, 1);
							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_ANGULARSPEED, (UINT)Current_Character_Settings.Angular_Speed, 1);
							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_SPEED, (UINT)Current_Character_Settings.Speed, 1);
							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_DIRECTIONX, (UINT)Current_Character_Settings.Vector_Direction.x, 1);
							SetDlgItemInt(Character_Properties_hwndDlg, IDD_CHARACTER_DIRECTIONY, (UINT)Current_Character_Settings.Vector_Direction.y, 1);
							CenterInWindow(GlobalHwnd,Character_Properties_hwndDlg);
						}
						return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDOK:
								{
									char F[50];
									/*Get Character World Position*/
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_POSX,F,50);
									Current_Character_Settings.Position.x=(float)atof(F);
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_POSY,F,50);
									Current_Character_Settings.Position.y=(float)atof(F);
									/*Character Scale*/
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_SCALE,F,50);
									Current_Character_Settings.Scale=(float)atof(F);
									/*Character Angle*/
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_ANGLE,F,50);
									Current_Character_Settings.Angle=(float)atof(F);
									/*Character Speed*/
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_SPEED,F,50);
									Current_Character_Settings.Speed=(float)atof(F);									
									/*Character Angular Speed*/
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_ANGULARSPEED,F,50);
									Current_Character_Settings.Angular_Speed=(float)atof(F);
									/*Character Vector Direction*/
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_DIRECTIONX,F,50);
									Current_Character_Settings.Vector_Direction.x=(float)atof(F);
									GetDlgItemText(Character_Properties_hwndDlg, IDD_CHARACTER_DIRECTIONY,F,50);
									Current_Character_Settings.Vector_Direction.y=(float)atof(F);
								}
								/* Filling the Settings in the current_character */
								Current_Character->GetSettings() = Current_Character_Settings;

								EndDialog(Character_Properties_hwndDlg,nResult);
								break;

							case IDCANCEL:
									EndDialog(Character_Properties_hwndDlg,nResult);
									break;
						}
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK CharacterDlgProc(HWND Character_hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT p;				//Declaring a paint structure
	switch(message)
	{
		case WM_INITDIALOG:
						{
							char tmp[55];
							/* Hiding the TwoWay check box */
							ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_TWOWAYCHECK), SW_HIDE);
							/* Showing the PickColor button */
							ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_PICKCOLOR), SW_SHOW);
							/* Hiding the FlipNormal check box */
							ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_FLIPNORMAL), SW_HIDE);
							/* Initializing the radio button to the line and to the collision line */
							SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_LINE), BM_SETCHECK, 1, 0);
							SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_LINE), BM_SETCHECK, 1, 0);
							/* Hiding the draw button */
							ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_DRAW), SW_HIDE);
							/* Filling the edit boxes for the RGB colors from the primitive struct */
							SetDlgItemInt(Character_hwndDlg, IDC_EDIT_CHAR_RED, Primitive_Color.r, 0);
							SetDlgItemInt(Character_hwndDlg, IDC_EDIT_CHAR_GREEN, Primitive_Color.g, 0);
							SetDlgItemInt(Character_hwndDlg, IDC_EDIT_CHAR_BLUE, Primitive_Color.b, 0);
							/* Hiding the collision line and circle radio button */
							ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_LINE), SW_HIDE);
							ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_CIRCLE), SW_HIDE);
							/* Disable the savechanges button */
							EnableWindow(GetDlgItem(Character_hwndDlg,IDC_BUTTON_CHAR_SAVECHANGES),FALSE);
							/* Making the dialog to center */
							CenterInWindow(GlobalHwnd,Character_hwndDlg);
							/* Copying the character and the counter to the tmp array */
							sprintf(tmp,"Character %d", Character_Count+1);
							/* Updating the character_name edit box with the new character */
							SetWindowText(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_NAME), tmp);
							/*Allocate Memory for Character*/
							Current_Character=new Character;
							/* Refreshing the screen */
							Graphics_Refresh();
						}
						return TRUE;
		case WM_PAINT:
						BeginPaint(Character_hwndDlg,&p);
						/*Preview Current Color*/
						if(Draw_Preview)
						{
							RECT r;
							/* Getting the rect of the green color edit box */
							GetWindowRect(GetDlgItem(Character_hwndDlg,IDC_EDIT_CHAR_GREEN),&r);
							/* Modifying the preview color rectangle */
							r.left+=68;
							r.right+=68;
							r.top-=2;
							r.bottom-=2;
							/* Filling the rectangle with the selected color */
							Preview_Color(r,RGB(Primitive_Color.r,Primitive_Color.g,Primitive_Color.b));
						}
						EndPaint(Character_hwndDlg,&p);
						return TRUE;
		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case IDCANCEL:
									/* Hiding the dialog box */
									ShowWindow(Character_hwndDlg, SW_HIDE);
									/* Updating the position of the window */
									AdjustWindowBack(Character_hwndDlg);
									/* Making the selected algorithm to None */
									Algorithm_Selection = None;
									/* Reset the clicks count */
									num=0;
									/*Reset Character Mode*/
									Character_Mode=Mode_Draw;
									/* Clean the surface */
									Reset_Surface();
									/*Refresh Surface*/
									Graphics_Refresh();
									/* Making the Show menu item enabled */
									EnableMenuItem(GetMenu(GlobalHwnd), IDM_CHAR_SHOWDIALOG, MF_ENABLED);
									/* Enabling the Character_Toolbar_Button */
									Character_Toolbar_Button=0;
									break;

							case IDC_BUTTON_CHAR_PROPERTIES:
								nResult = DialogBox(GlobalInstance, (LPCTSTR)IDD_DIALOG_CHAR_PROPERTIES, GlobalHwnd, (DLGPROC)CharacterPropertiesDlgProc);
								break;

							case IDC_BUTTON_CHAR_COLLISION:
								/*Don't Draw Preview*/
								Draw_Preview=0;
								/* Clean the surface */
								Reset_Surface();
								/*Draw eveything from the structures*/
								Current_Character->Draw();
								Current_Character->DrawCollision();

								Graphics_Refresh();
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_TWOWAYCHECK), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_FLIPNORMAL), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_DRAW), SW_SHOW);
								
								/* Hiding the PickColor button */
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_PICKCOLOR), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_COLLISION), SW_HIDE);								
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_TRIANGLE), SW_HIDE);								
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_RED), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_GREEN), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_BLUE), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_RED), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_GREEN), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_BLUE), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_COLORSELECTION), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_COLORPREVIEW), SW_HIDE);
								
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_1), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_2), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_3), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_4), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_5), SW_HIDE);

								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_LINE), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_CIRCLE), SW_SHOW);

								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_LINE), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_CIRCLE), SW_HIDE);

								if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_LINE), BM_GETCHECK, 0, 0))
									Algorithm_Selection = Algorithm_Collision_Line;
								else if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_CIRCLE), BM_GETCHECK, 0, 0))
									Algorithm_Selection = Algorithm_Collision_Circle;
								num=0;
								break;

							case IDC_BUTTON_CHAR_DRAW:
								/*Draw Preview*/
								Draw_Preview=1;
								/* Clean the surface */
								Reset_Surface();
								/*Draw eveything from the structures*/
								Current_Character->Draw();

								Graphics_Refresh();

								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_TWOWAYCHECK), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_COLLISION_FLIPNORMAL), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_DRAW), SW_HIDE);

								/* Showing the PickColor button */
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_PICKCOLOR), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_COLLISION), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_TRIANGLE), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_RED), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_GREEN), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_BLUE), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_RED), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_GREEN), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_BLUE), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_COLORSELECTION), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_CHAR_COLORPREVIEW), SW_SHOW);

								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_1), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_2), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_3), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_4), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_STATIC_COMBINE_5), SW_SHOW);

								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_LINE), SW_HIDE);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_CHAR_COLLISION_CIRCLE), SW_HIDE);

								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_LINE), SW_SHOW);
								ShowWindow(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_CIRCLE), SW_SHOW);
								
								if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_LINE), BM_GETCHECK, 0, 0))
									Algorithm_Selection = Algorithm_Line;
								else if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_RADIO_DRAW_CIRCLE), BM_GETCHECK, 0, 0))
								{
									Algorithm_Selection = Algorithm_Circle;
									if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), BM_GETCHECK, 0, 0))
										Algorithm_Selection = Algorithm_Filled_Circle;
								}
								else
								{
									Algorithm_Selection = Algorithm_Triangle;
									if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), BM_GETCHECK, 0, 0))
										Algorithm_Selection = Algorithm_Filled_Triangle;
								}
								num=0;
								break;

							case IDC_BUTTON_CHAR_ADD:
								{
									/* used to store the character name */
									char tmp[50];
									Reset_Character_Dialog(Character_hwndDlg);
									
									/*Reset Click Count to Zero*/
									num=0;

									/*Disable Save Changes Button*/
									EnableWindow(GetDlgItem(Character_hwndDlg,IDC_BUTTON_CHAR_SAVECHANGES),FALSE);

									/* Storing the name in the private name member in the class */
									GetWindowText(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_NAME), tmp, 50);
									/* Getting the index of the current selection */
									int index=(int)SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_GETCURSEL, 1, 0);
									/* Checking if there exist any character in the list and if the copy character has the same name as the selected one */
									if(index!=-1&&!strcmp(((Character *)SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_GETITEMDATA, index, 0))->GetName(), tmp))
										sprintf(tmp,"Character %d", Character_Count+1);
									/* Setting the name to the new name */
									Current_Character->SetName(tmp);

									SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_INSERTSTRING, -1, (LPARAM)Current_Character->GetName());

									/*Insert a copy of the current character*/
									Character *Instance=new Character;
									Instance->Instance(*Current_Character);

									SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_SETITEMDATA, Character_Count, (LPARAM)Instance);
									Character_Count++;

									sprintf(tmp,"Character %d", Character_Count+1);
									SetWindowText(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_NAME), tmp);

									/*Clean Surface and Refresh*/
									Reset_Surface();
									Graphics_Refresh();

									/*Free Memory Allocated for Current Character and Reset it*/
									delete Current_Character;
									Current_Character=new Character;

									/*Reset Index and Reset Focus to main window*/
									SendMessage(GetDlgItem(Character_hwndDlg,IDC_LIST_CHAR),LB_SETCURSEL,-1,0);
									SetFocus(GlobalHwnd);
									Draw_Preview=1;
									Disable_Enable_All(Character_hwndDlg, FALSE);
									/* updating the global rects */
									SourceRect=Current_Character->GetSourceRect();
									DestinationRect=Current_Character->GetDestinationRect();
								}
								break;

								/* if the selected character has been modified */
							case IDC_BUTTON_CHAR_SAVECHANGES:
								{
									/*Get Currently Selected Character*/
									int index=(int)SendMessage(GetDlgItem(Character_hwndDlg,IDC_LIST_CHAR),LB_GETCURSEL,0,0);
									if(index!=-1)
									{
										/*Free Index Character Pointer*/
										delete (Character*)SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_GETITEMDATA, index, 0);
										/*Set a copy of the current character*/
										Character *Instance=new Character;
										GetDlgItemText(Character_hwndDlg,IDC_EDIT_CHAR_NAME,Current_Character->GetName(),50);
										Instance->Instance(*Current_Character);
										/*Refresh Character String*/
										SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_DELETESTRING, index, 0);
										SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_INSERTSTRING, index, (LPARAM)Current_Character->GetName());
										SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_SETITEMDATA, index, (LPARAM)Instance);
										SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_SETCURSEL, index, 0);
									}
								}
								break;

							case IDC_BUTTON_CHAR_NEWCHAR:
								/* Reset the character dialogbox */
								Reset_Character_Dialog(Character_hwndDlg);
								/* Clean the surface */
								Reset_Surface();
								/* Redraw the window */
								Graphics_Refresh();
								/* Deleting the currently character to be able to set a new character */
								delete Current_Character;
								/* Creating a new character */
								Current_Character = new Character;
								/* Getting the initialized settings from the new character */
								Current_Character_Settings = Current_Character->GetSettings();
								/*Reset Index and Reset Focus to main window*/
								SendMessage(GetDlgItem(Character_hwndDlg,IDC_LIST_CHAR),LB_SETCURSEL,-1,0);
								/*Set Character Name*/
								{
									char tmp[50];
									sprintf(tmp,"Character %d", Character_Count+1);
									SetWindowText(GetDlgItem(Character_hwndDlg, IDC_EDIT_CHAR_NAME), tmp);
								}
								/*Disable Save Changes Button*/
								EnableWindow(GetDlgItem(Character_hwndDlg,IDC_BUTTON_CHAR_SAVECHANGES),FALSE);
								SetFocus(GlobalHwnd);
								Disable_Enable_All(Character_hwndDlg, TRUE);
								/* updating the global rects */
								SourceRect=Current_Character->GetSourceRect();
								DestinationRect=Current_Character->GetDestinationRect();
								EnableWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_SDRECT), TRUE);
								break;

							case IDC_BUTTON_CHAR_REMOVECHAR:
								{
									int index = (int)SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_GETCURSEL, 0, 0);
									if(index!=-1)
									{
										/*Removing Currently Selected Character*/
										delete (Character *)SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_GETITEMDATA, index, 0);
										SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_DELETESTRING, index, 0);
										
										/* if there is more than one character in the list */
										if(index-1>=0)
											SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_SETCURSEL, index-1, 0);
										/* else there is only one character */
										else
											SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_SETCURSEL, 0, 0);

										/* if there is one or more characters in the list --> Update Character Selection */
										if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_LIST_CHAR), LB_GETCOUNT, 0, 0))
											SendMessage(Character_hwndDlg,WM_COMMAND,LBN_SELCHANGE<<16,0);									
										else
										{
											/*No More Characters above it in List ===> Clear Surface and Reset Current Character Pointer*/
											delete Current_Character;
											Current_Character=new Character;

											/* Clean and refresh the surface */
											Reset_Surface();
											Graphics_Refresh();
										}
										Character_Count--;
									}
								}
								break;

								/* updating the selected algorithm to the line algo */
							case IDC_RADIO_DRAW_LINE:
								Algorithm_Selection = Algorithm_Line;
								
								/* Clean the surface */
								Reset_Surface();
								/*Draw eveything from the structures*/
								Current_Character->Draw();
								Graphics_Refresh();
								break;

								/* updating the selected algorithm to the circle algo */
							case IDC_RADIO_DRAW_CIRCLE:
								if(!SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), BM_GETCHECK, 0, 0))
									Algorithm_Selection = Algorithm_Circle;
								else
									Algorithm_Selection = Algorithm_Filled_Circle;
								num=0;
								
								/* Clean the surface */
								Reset_Surface();
								/*Draw eveything from the structures*/
								Current_Character->Draw();
								Graphics_Refresh();
								break;

								/* updating the selected algorithm to the filled circle or triangle algo */
							case IDC_CHECK_DRAW_PRIMITIVE_FILLED:
								if(Algorithm_Selection!=Algorithm_Line)
									if(SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), BM_GETCHECK, 0, 0))
										if(Algorithm_Selection==Algorithm_Circle)
											Algorithm_Selection = Algorithm_Filled_Circle;
										else
											Algorithm_Selection = Algorithm_Filled_Triangle;
									else
										if(Algorithm_Selection==Algorithm_Filled_Circle)
											Algorithm_Selection = Algorithm_Circle;
										else
											Algorithm_Selection = Algorithm_Triangle;
								break;

								/* updating the selected algorithm to the triangle algo */
							case IDC_RADIO_DRAW_TRIANGLE:
								if(!SendMessage(GetDlgItem(Character_hwndDlg, IDC_CHECK_DRAW_PRIMITIVE_FILLED), BM_GETCHECK, 0, 0))
									Algorithm_Selection = Algorithm_Triangle;
								else
									Algorithm_Selection = Algorithm_Filled_Triangle;
								num=0;

								/* Clean the surface */
								Reset_Surface();
								/*Draw eveything from the structures*/
								Current_Character->Draw();
								Graphics_Refresh();
								break;

								/* updating the selected algorithm to the collision line */
							case IDC_RADIO_CHAR_COLLISION_LINE:
								Algorithm_Selection = Algorithm_Collision_Line;
								num=0;

								/* Clean the surface */
								Reset_Surface();
								/*Draw eveything from the structures*/
								Current_Character->Draw();
								Current_Character->DrawCollision();
								Graphics_Refresh();
								break;

								/* updating the selected algorithm to the collision circle */
							case IDC_RADIO_CHAR_COLLISION_CIRCLE:
								Algorithm_Selection = Algorithm_Collision_Circle;
								num=0;

								/* Clean the surface */
								Reset_Surface();
								/*Draw eveything from the structures*/
								Current_Character->Draw();
								Current_Character->DrawCollision();
								Graphics_Refresh();
								break;

							case IDC_BUTTON_CHAR_PICKCOLOR:
								/* Storing the current algorithm in the pickcolor variable */
								Pick_Color=1;

								/* Reset the click count */
								num=0;
								/* Pushing the button */
								SendMessage(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_PICKCOLOR), BM_SETCHECK, 1, 0);
								break;
							case IDC_BUTTON_CHAR_MODE:
														if(Character_Mode==Mode_Draw)
														{
															Character_Mode=Mode_Select;
															SetDlgItemText(Character_hwndDlg,IDC_BUTTON_CHAR_MODE,"Drawing");
															/*Prepare Selection Surface*/
															Reset_Sel_Buffer();
															Sel_Draw(Current_Character->GetPrimitives(),Current_Character->GetCollisions());
														}
														else
														{
															Character_Mode=Mode_Draw;
															SetDlgItemText(Character_hwndDlg,IDC_BUTTON_CHAR_MODE,"Selection");
														}
														break;
							case IDC_BUTTON_CHAR_REMOVE:
														Current_Character->Remove(Current_Selection.p,Current_Selection.c);
														/* Clean the surface */
														Reset_Surface();
														/*Draw eveything from the structures*/
														Current_Character->Draw();
														if(!Draw_Preview)Current_Character->DrawCollision();
														Graphics_Refresh();
														/*Prepare Selection Surface*/
														Reset_Sel_Buffer();
														Sel_Draw(Current_Character->GetPrimitives(),Current_Character->GetCollisions());
														break;
							case IDC_BUTTON_CHAR_SDRECT:
								DialogBox(GlobalInstance, (LPCTSTR)IDD_DIALOG_CHAR_SDRECT, Character_hwndDlg, (DLGPROC)DialogProcSDRect);
								break;
						}
						switch(HIWORD(wParam))
						{
							case LBN_SELCHANGE:
												{
													int index=(int)SendMessage(GetDlgItem(Character_hwndDlg,IDC_LIST_CHAR),LB_GETCURSEL,0,0);
													Reset_Surface();
													/*Free Current Character Memory*/
													delete Current_Character;
													/*Create Selected Character Instance*/
													Current_Character=new Character;
													Current_Character->Instance(*((Character*)SendMessage(GetDlgItem(Character_hwndDlg,IDC_LIST_CHAR),LB_GETITEMDATA,index,0)));
													/*Set Character Name*/
													SetDlgItemText(Character_hwndDlg,IDC_EDIT_CHAR_NAME,Current_Character->GetName());
													Current_Character->Draw();
													if(!Draw_Preview)
														Current_Character->DrawCollision();
													Disable_Enable_All(Character_hwndDlg, true);													
													/* updating the global rects */
													SourceRect=Current_Character->GetSourceRect();
													DestinationRect=Current_Character->GetDestinationRect();
													/*Enable Save Changes Button*/
													EnableWindow(GetDlgItem(Character_hwndDlg,IDC_BUTTON_CHAR_SAVECHANGES),TRUE);
													EnableWindow(GetDlgItem(Character_hwndDlg, IDC_BUTTON_CHAR_SDRECT), FALSE);
													Graphics_Refresh();
												}
												break;
							case EN_UPDATE:
												switch(LOWORD(wParam))
												{
													case IDC_EDIT_CHAR_RED:
													case IDC_EDIT_CHAR_GREEN:
													case IDC_EDIT_CHAR_BLUE:
																			if(GetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_RED,NULL,0)>255)
																			{
																				Primitive_Color.r=255;
																				SetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_RED,Primitive_Color.r,0);
																			}
																			else
																				Primitive_Color.r=(unsigned char)GetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_RED,NULL,0);
																			if(GetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_GREEN,NULL,0)>255)
																			{
																				Primitive_Color.g=255;
																				SetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_GREEN,Primitive_Color.g,0);
																			}
																			else
																				Primitive_Color.g=(unsigned char)GetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_GREEN,NULL,0);
																			if(GetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_BLUE,NULL,0)>255)
																			{
																				Primitive_Color.b=255;
																				SetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_BLUE,Primitive_Color.b,0);
																			}
																			else
																				Primitive_Color.b=(unsigned char)GetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_BLUE,NULL,0);
																			//SetDlgItemInt(Character_hwndDlg,IDC_EDIT_CHAR_RED,Primitive_Color.r,0);
																			/*Preview Current Color*/
																			RECT r;
																			GetWindowRect(GetDlgItem(Character_hwndDlg,IDC_EDIT_CHAR_GREEN),&r);
																			r.left+=68;
																			r.right+=68;
																			r.top-=2;
																			r.bottom-=2;
																			Preview_Color(r,RGB(Primitive_Color.r,Primitive_Color.g,Primitive_Color.b));
																			break;
												}
												break;
						}
						return TRUE;
		
		case WM_MOVE:
						/*Update Window Position*/
						DlgMove=1;
						if(!WinMove)
							AlignWindowToDialog(GlobalHwnd,Character_hwndDlg);
						DlgMove=0;
						return TRUE;
		case WM_DESTROY:
						DestroyCharacterList(Character_hwndDlg);
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK DlgProc(HWND hwndDlg,UINT message, WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_MOVE:
						/*Update Window Position*/
						DlgMove=1;
						if(!WinMove)
							AlignWindowToDialog(GlobalHwnd,hwndDlg);
						DlgMove=0;
						return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam)	//Declaring the Window Procedure function to handle various Window Messages
{
	PAINTSTRUCT p;				//Declaring a paint structure
	static int HoldCtrl=0;
	switch(iMsg)				//Switch satement of the iMsg variable
	{
		case WM_CREATE:
						{
							RECT WinRect;	
							HMENU hMenu=GetMenu(hwnd);
							/*Create DDRAW Object*/
							Graphics_CreateWindowedDisplay(hwnd,EDITOR_WIDTH,EDITOR_HEIGHT);
							/*Get New Window Rectangle*/
							GetWindowRect(hwnd,&WinRect);
							/*Initialize Selection Buffer*/
							Create_Sel_Buffer();
							/*Old Rectangle*/
							RECT OldRect;
							OldRect.left=WinRect.left+((WinRect.right-WinRect.left)>>1);
							OldRect.right=OldRect.left;
							OldRect.top=WinRect.top+((WinRect.bottom-WinRect.top)>>1);
							OldRect.bottom=OldRect.top;
							/*Remove Menu Before Effect*/
							SetMenu(hwnd,NULL);
							/*Smooth Transition Effect*/
							SmoothTransition(hwnd,OldRect,WinRect,2);
							/*Reset Menu*/
							SetMenu(hwnd,hMenu);
							/*Draw Grid*/
							Grid(20,20,5,128,128,128,100,100,100);
							/* Draw the rects */
							if(ShowRects)
							{
								/* Drawing the rectangles */
								DrawRect(SourceRect, 255, 0, 0);
								DrawRect(DestinationRect, 0, 0, 255);
							}
						}
						return 0;
		case WM_MOVE:
						/*Update Dialog Box Position*/
						WinMove=1;
						if(!DlgMove)
							AlignDialogToWindow(hwnd,hwndDlg);
						WinMove=0;
						return 0;
		case WM_PAINT:
						BeginPaint(hwnd,&p);		//Call to function BeginPaint that starts displaying on the screen
						Graphics_Draw(NULL);
						Graphics_Flip();
						EndPaint(hwnd,&p);			//End Screen Display
						return 0;					//return 0 ==>no errors

		case WM_COMMAND:
						switch(LOWORD(wParam))
						{
							case ID_TOOLGAMESETTINGS:
							case IDM_GAMESETTINGS:
												nResult=DialogBox(GlobalInstance,MAKEINTRESOURCE(IDD_GAMESETTINGS),hwnd,(DLGPROC)GameDlgProc);
												break;
							case ID_TOOLVIEWCAM:
							case IDM_CAM_VIEW_SETTINGS:
												nResult=DialogBox(GlobalInstance,MAKEINTRESOURCE(IDD_CAM_AND_VIEW_SETTINGS),hwnd,(DLGPROC)Camera_ViewPort_DlgProc);
												break;
							case ID_TOOLCHARACTER:
							case IDM_CHAR_SHOWDIALOG:
												if(!Character_Toolbar_Button)
												{
													/* Disabling the character toolbar button */
													Character_Toolbar_Button=1;
													RECT WinRect,DlgRect;							
													/*Create Dialog Box*/
													WinMove=1;
													if(!hwndDlg)
														hwndDlg=CreateDialog(GlobalInstance,(LPCTSTR)IDD_DIALOG_CHAR_DESIGN,hwnd,(DLGPROC)CharacterDlgProc);
													WinMove=0;
													AlignDialogToWindow(hwnd,hwndDlg);
													/*Move Window to middle of screen*/
													GetWindowRect(hwnd,&WinRect);
													GetWindowRect(hwndDlg,&DlgRect);
													RECT newR;
													WinMove=1;
													newR.left = WinRect.left-((DlgRect.right-DlgRect.left)>>1);
													newR.right = WinRect.right - ((DlgRect.right-DlgRect.left)>>1);
													newR.top = WinRect.top;
													newR.bottom = WinRect.bottom;
													SmoothTransition(hwnd, WinRect, newR, 3);
													ShowWindow(hwndDlg,SW_SHOWNORMAL);
													/*Default Algorithm*/
													Algorithm_Selection=Algorithm_Line;
													/* Making the Show menu item grayed */
													EnableMenuItem(GetMenu(GlobalHwnd), IDM_CHAR_SHOWDIALOG, MF_GRAYED);
													/*Draw Current Character (if any)*/
													if(Current_Character)
													{
														Current_Character->Draw();
														if(!Draw_Preview)
															Current_Character->DrawCollision();
													}
												}
												break;
							case IDM_MOVEMENT:
												nResult=DialogBox(GlobalInstance,MAKEINTRESOURCE(IDD_MOVEMENT),hwnd,(DLGPROC)MovementDlgProc);
												break;
							case ID_TOOLBUILD:
							case IDM_GENERATE:
												Generate_Main(GameSettings);
												Generate_GameSettings(GameSettings);
												Generate_Graphics();
												GenerateCode_FrameRateController();
												GenerateCode_Keyboard();
												GenerateCode_Mouse();
												Generate_View(Main_Camera,Radar_Camera,Main_ViewPort,Radar_ViewPort);
												GenerateCode_Clipping();
												GenerateCode_StandardTypes();
												GenerateCode_Collision();
												GenerateCode_CharacterDesign();
												break;
							case IDM_CLEANCODE:
												{
												char TDir[250];
												sprintf(TDir,"%s\\Generated Code",Dir);
												if(SetCurrentDirectory(TDir))
												{
													/*Remove All Generated Files*/
													remove("Character Initialization.cpp");
													remove("Character.cpp");
													remove("Clipping.cpp");
													remove("Clipping.h");
													remove("Collision.cpp");
													remove("Collision.h");
													remove("Design.cpp");
													remove("Design.h");
													remove("FrameRateController.cpp");
													remove("FrameRateController.h");
													remove("Graphics.cpp");
													remove("Graphics.h");
													remove("Keyboard.cpp");
													remove("Keyboard.h");
													remove("main.cpp");
													remove("Mouse.cpp");
													remove("Mouse.h");
													remove("Settings.h");
													remove("Standard Header.h");
													remove("View.cpp");
													remove("View.h");
													/*Reset Current Working Directory*/
													SetCurrentDirectory(Dir);
													/*Remove Directory*/
													_rmdir(TDir);
												}
												break;
												}
							case IDM_ABOUT:
											MessageBox(hwnd,"	          \"The Code Mages\"\nSalem Haykal (Wave) & Karl Boghossian (Dr.E|NSTE|N)\n                     2nd Year DigiPen Students\n\n                 We Hope You'll Enjoy Our Editor!\n\n         Copyright DigiPen Institude of Technology.\n	           All rights reserved.","About Game Editor",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
											break;
							case IDM_EXIT:
											if(hwndDlg)
												DestroyWindow(hwndDlg);
											{
												RECT WinRect;
												/*Get Current Window Rectangle*/
												GetWindowRect(hwnd,&WinRect);
												/*New Rectangle*/
												RECT NewRect;
												NewRect.left=(WinRect.left+WinRect.right)>>1;
												NewRect.right=NewRect.left;
												NewRect.top=(WinRect.top+WinRect.bottom)>>1;
												NewRect.bottom=NewRect.top;
												/*Destroy Menu*/
												SetMenu(hwnd,NULL);
												/*Smooth Transition*/
												SmoothTransition(hwnd,WinRect,NewRect,2);
												DestroyWindow(hwnd);
											}
											break;
						}
						return 0;

		case WM_LBUTTONDOWN:
							{
								EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON_CHAR_SDRECT), FALSE);
								POINT Point;
								GetCursorPos(&Point);	//Get Cursor Position and save it in Point structure
								/* Decrementing the y value because of the toolbar */
								Point.y-=TOOLBAR_HEIGHT;
								ScreenToClient(hwnd,&Point);//Update Point coordinates to client area of window
								Point.x=(Point.x>0)?Point.x:0;
								Point.y=(Point.y>0)?Point.y:0;
								/* Checking if the click is inside the source rectangle */
								if((Point.x>SourceRect.left && Point.x<SourceRect.right) && (Point.y>SourceRect.top && Point.y<SourceRect.bottom))
								{
									/* If the pick color button is pressed */
									if(Pick_Color)
									{
										/* Take the primitive color */
										Color_RGB tmp = GetPixelColor(Point.x, Point.y);
										/* Filling the RGB edit boxes with the picked color */
										SetDlgItemInt(hwndDlg, IDC_EDIT_CHAR_RED, tmp.r, 0);
										SetDlgItemInt(hwndDlg, IDC_EDIT_CHAR_GREEN, tmp.g, 0);
										SetDlgItemInt(hwndDlg, IDC_EDIT_CHAR_BLUE, tmp.b, 0);
										/*Update Primitive Color*/
										Primitive_Color = tmp;
										/* reset the pick color variable to 0 */
										Pick_Color=0;
										SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_CHAR_PICKCOLOR), BM_SETCHECK, 0, 0);
									}
									else
									{
										if(Character_Mode==Mode_Draw)	//Drawing Mode
											Draw(hwnd,Point,Primitive_Color);			//Call function Draw with arguments: handle to window and Point structure
										else
										{
											Current_Selection=Resolve_Selection(Current_Character->GetPrimitives(),Current_Character->GetCollisions(),(bool)!Draw_Preview,Point.x,Point.y);
											/*Clean Surface*/
											Reset_Surface();
											/*Draw Current Primitives and Collisions*/
											Current_Character->Draw();
											if(!Draw_Preview)
												Current_Character->DrawCollision();
											/*Draw Selection Bounding Box (if any)*/
											Draw_BoundingBox(Current_Selection.p,Current_Selection.c);
											/*Refresh Surface*/
											Graphics_Refresh();
										}
									}
								}
							}
							return 0;

		case WM_RBUTTONDOWN:
			if((Algorithm_Selection==Algorithm_Line || Algorithm_Selection==Algorithm_Collision_Line)&&num)
			{
				Vertex Vertices[2];
				/*Build Line Vertices*/
				Vertices[0].x=(float)ppt[0].x;Vertices[0].y=(float)ppt[0].y;
				Vertices[1].x=(float)ppt[1].x;Vertices[1].y=(float)ppt[1].y;
				/*Add Line to Character Primitives*/
				if(Algorithm_Selection==Algorithm_Line)
					Current_Character->AddPrimitive(BuildPrimitive(Vertices,Primitive_Color,Type_Line));
				else
				{
					Collision_Data *tmp;
					Current_Character->AddCollision(tmp=BuildCollision(Vertices[0],Vertices[1],Type_Collision_Line));
					DrawCollisionNormal(tmp->V0,tmp->V1,tmp->Normal);
				}
				Graphics_Refresh();
				num=0;												//Cut the rope of the collision
			}
			return 0;

		case WM_MOUSEMOVE :
			{
				/*Update Cursor if necessary*/
				if(Pick_Color)
					SetCursor(LoadCursor(GlobalInstance, (LPCSTR)IDC_DROPPER));
				if(num==1 && (Algorithm_Selection==Algorithm_Line || Algorithm_Selection==Algorithm_Collision_Line))
				{
					Reset_Surface();
					/*Draw eveything from the structures*/
					Current_Character->Draw();
					if(!Draw_Preview)
						Current_Character->DrawCollision();
					POINT mps;

					/* Getting the position of the mouse */
					GetCursorPos(&mps);
					/* Decrementing the y value because of the toolbar */
					mps.y-=TOOLBAR_HEIGHT;
					/* converting the values to the screen to the client area */
					ScreenToClient(GlobalHwnd,&mps);

					/*Check if in Area Boundary*/
					mps.x=(mps.x>0)?mps.x:0;
					mps.y=(mps.y>0)?mps.y:0;
					/* Checking if the mouse is inside the source rectangle */
					if((mps.x>SourceRect.left && mps.x<SourceRect.right) && (mps.y>SourceRect.top && mps.y<SourceRect.bottom))
						/* Filling the values in the ppt */
						ppt[num] = mps;
					/* Checking which algorithm is chosen to know which one to call */
					if(Algorithm_Selection==Algorithm_Line)
						LINE((float)ppt[0].x,(float)ppt[0].y,(float)ppt[1].x,(float)ppt[1].y,Primitive_Color.r,Primitive_Color.g,Primitive_Color.b);
					else
					{
						FILL_CIRCLE((float)(ppt[0].x),(float)(ppt[0].y),2,0,0,255);	//drawing the connections of the beginning of the edge
						COLLISION_LINE((float)ppt[0].x,(float)ppt[0].y,(float)ppt[1].x,(float)ppt[1].y,COLLISION_COLOR_R,COLLISION_COLOR_G,COLLISION_COLOR_B);
						FILL_CIRCLE((float)(ppt[1].x),(float)(ppt[1].y),2,0,0,255);	//drawing the connections of the beginning of the edge
					}

					/* updating the primary surface */
					Graphics_Draw(NULL);
					/* displaying the result */
					Graphics_Flip();			
				}
			}
			return 0;

			case WM_CLOSE:
				if(hwndDlg)
					DestroyWindow(hwndDlg);
				{
					RECT WinRect;
					/*Get Current Window Rectangle*/
					GetWindowRect(hwnd,&WinRect);
					/*New Rectangle*/
					RECT NewRect;
					NewRect.left=(WinRect.left+WinRect.right)>>1;
					NewRect.right=NewRect.left;
					NewRect.top=(WinRect.top+WinRect.bottom)>>1;
					NewRect.bottom=NewRect.top;
					/*Destroy Menu*/
					SetMenu(hwnd,NULL);
					/*Smooth Transition*/
					SmoothTransition(hwnd,WinRect,NewRect,2);
					DestroyWindow(hwnd);
				}
				return 0;

			case WM_KEYDOWN:
				switch (wParam)
				{
					case VK_DELETE:
						SendMessage(hwndDlg,WM_COMMAND, IDC_BUTTON_CHAR_REMOVE,0);
						break;
					case 'M':
						SendMessage(hwndDlg,WM_COMMAND, IDC_BUTTON_CHAR_MODE,0);
						break;
					case VK_CONTROL:
									HoldCtrl=1;
									break;
					case VK_F5:
						if(HoldCtrl)
							SendMessage(hwnd,WM_COMMAND, IDM_GENERATE,0);
						break;
					case VK_F1:
						SendMessage(hwnd,WM_COMMAND, IDM_ABOUT,0);
						break;
				}
				return 0;
		case WM_KEYUP:
						HoldCtrl=0;
						return 0;
		case WM_DESTROY:				//If Destroy message received
							PostQuitMessage(0);			//End Process after quitting (end message Loop)
							return 0;					//return 0 ==>no errors
						
	}
	return DefWindowProc(hwnd,iMsg,wParam,lParam);	//Call to default Window Procedure function to handle all other Window Messages
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)	//WinMain() function declaration
{
	ATOM cID;				//Declare an ATOM variable to store the  registered class ID
	WNDCLASSEX	wndclass;	//Declare a window class variable
	HWND hwnd;				//Declare a handle window variable
	MSG msg;				//Declare a MSG variablle for messages

	/*Initialize the wndclass structure*/
	wndclass.cbSize=sizeof(WNDCLASSEX);				//set size of structure
	wndclass.style=CS_HREDRAW|CS_VREDRAW;			//set class style
	wndclass.lpfnWndProc=WndProc;					//set Window Procedure function
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=hInstance;					//set handle Instance of window
	wndclass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON));	//set default Icon
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);	//set default Cursor
	wndclass.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);	//set background color
	wndclass.lpszMenuName=NULL;						//no menu
	wndclass.lpszClassName="EditorClass";			//set class name
	wndclass.hIconSm=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON)); //same as Icon

	cID=RegisterClassEx(&wndclass);					//Register the class and set the return value to cID ATOM 

	GlobalInstance=hInstance;



	GlobalHwnd=hwnd=CreateWindow(wndclass.lpszClassName, "- Code Mages Editor -", WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,0,0,EDITOR_WIDTH,EDITOR_HEIGHT,NULL,LoadMenu(hInstance,MAKEINTRESOURCE(IDM_MENU)),hInstance,NULL);
	//Call create window functionto create the program's overlapped window entitled :My First Application 
	ShowWindow(hwnd,iCmdShow);						//Show the window after it has been created
	UpdateWindow(hwnd);								//Update the window

	for(int i=0; i<4; i++)
	{
		ToolArray[i].dwData = 0;
		ToolArray[i].fsState = TBSTATE_ENABLED;
		ToolArray[i].fsStyle = TBSTYLE_BUTTON;
		ToolArray[i].iBitmap = i;
		ToolArray[i].idCommand = 40001+i;
		ToolArray[i].iString = 0;
	}
	CreateToolbarEx(hwnd, (WS_CHILD | WS_VISIBLE | WS_BORDER | TBSTYLE_FLAT), IDR_TOOLBAR1, 4, hInstance, IDR_TOOLBAR1, ToolArray, 4, 16, 16, 10, 10, sizeof(struct _TBBUTTON));

	while(GetMessage(&msg,NULL,0,0))				//Get all messages from user to program
	{
		if(!hwndDlg || !IsDialogMessage(hwndDlg,&msg))
		{
			TranslateMessage(&msg);						//Function to translate virtual input to character input
			DispatchMessage(&msg);						//Dispatch message to let Window Procedure handle it
		}
	}
	return (int)msg.wParam;							//return the wParam of msg structure to make sure no errors occured
}