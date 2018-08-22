#include "Viewing.h"
#include "Standard Header.h"
#include <vector>
#include <list>
#include "FuzzyMiniMax.h"

#define INFINITY 100000
#define ROWS 8
#define COLUMNS 8
//#define DEPTH 4
#define CELL_WIDTH 60
#define CELL_HEIGHT 60

using namespace std;

extern HWND GlobalHwnd;
extern ViewPort *ViewPort_List;
extern Character *WhiteS;
extern Character *BlackS;
extern Character *Map;

extern MiniMax_Fuzzy fuzzy_minimax;

enum GameState { Win_State, Draw_State, Lose_State, None_State };
enum Turn { Player_Turn, Computer_Turn };
enum SoldierState { Black , White , Dead };

/*  Check for each r or c for the availability of the board (-1 or 8)
	see if needed RC or indices
*/

bool ObligatoryEat=false;
extern bool TwoPlayers;
extern bool FuzzyDifficulty;
bool MustSwitch=false;
int DEPTH=0;

struct SoldierPosition
{
	int r, c;
};

struct SaveBestMoves
{
	int Idx;
	SoldierPosition BestDestination;
};

struct Soldier
{
	SoldierState soldier_State;
	SoldierPosition Position;
	bool IsDama;
	bool IsSelected;
	bool IsEating;

	vector<SoldierPosition> Victims;
};

/* The minimax class */
class MiniMax
{
	public:
			/* To hold the game state */
			GameState GState;
			/* To hold who's turn it is */
			Turn TState;
			/* The Checkers board */
			SoldierState Board [ROWS][COLUMNS];
			/* Array of 16 Black Soldiers */
			Soldier Black_Soldiers[16];
			/* Array of 16 White Soldiers */
			Soldier White_Soldiers[16];
			int White_Soldier_Count , Black_Soldier_Count;

			/*Global Saved Soldier*/
			int SelectedSoldierIndex;

			vector<SaveBestMoves> AllBestMoves;

			/* Default constructor */
			MiniMax ()
			{
				ResetGame();

				//MiniMaxPrinciple(Board,White_Soldiers,Black_Soldiers);
				//SwitchTurn();
			}

			void ResetGame (void)
			{
				/* Game state is none */
				GState=None_State;
				/* Turn is (...) as default */
				TState=Player_Turn;
				//TState=Computer_Turn;

				SelectedSoldierIndex=-1;
				ObligatoryEat=false;

				/* WhiteSoldiersNbr & BlackSoldiersNbr = 16 */
				White_Soldier_Count=Black_Soldier_Count=16;
				
				/* Loop through the board */
				for(int r=0; r<ROWS; r++)
				{
					for(int c=0; c<COLUMNS; c++)
					{
						if(r>0 && r<3)
							Board[r][c] = Black;
						else if(r>4 && r<7)
							Board[r][c] = White;
						else
							Board[r][c] = Dead;
					}
				}

				for(int s=0; s<16; s++)
				{
					Black_Soldiers[s].soldier_State = Black;
					White_Soldiers[s].soldier_State = White;
					Black_Soldiers[s].IsSelected = White_Soldiers[s].IsSelected = false;
					Black_Soldiers[s].IsDama = White_Soldiers[s].IsDama = false;
					Black_Soldiers[s].IsDama = White_Soldiers[s].IsEating = false;
					Black_Soldiers[s].Victims.clear();	White_Soldiers[s].Victims.clear();
					

					if(s<8)
					{
						Black_Soldiers[s].Position.r = 2;
						White_Soldiers[s].Position.r = 5;
						Black_Soldiers[s].Position.c = White_Soldiers[s].Position.c = s;
					}
					else
					{
						Black_Soldiers[s].Position.r = 1;
						White_Soldiers[s].Position.r = 6;
						Black_Soldiers[s].Position.c = White_Soldiers[s].Position.c = s-8;
					}
				}

				//KillSoldier(Board,Black_Soldiers,1,Black_Soldier_Count);
				//KillSoldier(Board,Black_Soldiers,2,Black_Soldier_Count);
				/*KillSoldier(Board,Black_Soldiers,3,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,4,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,5,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,6,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,7,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,8,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,9,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,10,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,11,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,12,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,13,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,14,Black_Soldier_Count);
				KillSoldier(Board,Black_Soldiers,15,Black_Soldier_Count);*/

				//KillSoldier(Board,White_Soldiers,2,White_Soldier_Count);
				//KillSoldier(Board,White_Soldiers,3,White_Soldier_Count);
				/*KillSoldier(Board,White_Soldiers,4,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,5,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,6,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,7,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,8,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,9,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,10,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,11,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,12,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,13,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,14,White_Soldier_Count);
				KillSoldier(Board,White_Soldiers,15,White_Soldier_Count);*/
			}

			/* Function to switch the current turn */
			void SwitchTurn (void)
			{
				UpdateGameState();
				if(GState==Draw_State)
				{
					DynamicDraw();

					MessageBox(GlobalHwnd, "DRAW", "Game Over", (MB_OK));
					ResetGame();
				}
				else if(GState==Win_State)
				{
					DynamicDraw();

					if(!TwoPlayers)
						MessageBox(GlobalHwnd, "YOU LOSE!", "Game Over", (MB_OK));
					else
						MessageBox(GlobalHwnd, "Player 2 WINS!", "Game Over", (MB_OK));

					ResetGame();
				}
				else if(GState==Lose_State)
				{
					DynamicDraw();

					if(!TwoPlayers)
						MessageBox(GlobalHwnd, "YOU WIN", "Game Over", (MB_OK));
					else
						MessageBox(GlobalHwnd, "Player 1 WINS!", "Game Over", (MB_OK));

					ResetGame();
				}
				else
				{
					/* if it is the player turn */
					if(TState==Player_Turn)
					{
						/* Turn to computer */
						TState=Computer_Turn;
						/* Deselect the player's soldier */
						SelectSoldier(White_Soldiers,-1);
						/* Check for obligatory eat for the computer */
						SelectedSoldierIndex=-1;
						ObligatoryEat=CheckAvailableEats(Board,Black_Soldiers,Black,-1);

						if(!TwoPlayers)
						{
							DynamicDraw();
							if(FuzzyDifficulty)
								DEPTH=fuzzy_minimax.Get_Depth(4,White_Soldier_Count,Black_Soldier_Count,5);
							/* Do the computer move using minimax */
							MiniMaxPrinciple(Board,White_Soldiers,Black_Soldiers,White_Soldier_Count,Black_Soldier_Count);
							SwitchTurn();
						}
					}
					else
					{
						/*Player Turn*/
						TState=Player_Turn;
						/* Deselect the computer's soldier */
						SelectSoldier(Black_Soldiers,-1);
						SelectedSoldierIndex=-1;
						ObligatoryEat=CheckAvailableEats(Board,White_Soldiers,White,-1);
					}
				}
			}
			
			//==================================================================================================================================
			/* Function to move a soldier from a place to a destination */
			//==================================================================================================================================
			void MoveSoldier(SoldierState board[][COLUMNS], Soldier *SoldierList, SoldierState Type, int SoldierIndex, int Destination_r, int Destination_c)
			{
				/* Setting the Board current state to Dead (Dead slot) */
				board[SoldierList[SoldierIndex].Position.r][SoldierList[SoldierIndex].Position.c] = Dead;

				/* Changing the position of the soldier to the destination position (r & c) */
				SoldierList[SoldierIndex].Position.r = Destination_r;
				SoldierList[SoldierIndex].Position.c = Destination_c;

				if(Type==White && Destination_r==0 && !SoldierList[SoldierIndex].IsDama)
					SoldierList[SoldierIndex].IsDama=true;
				else if(Type==Black && Destination_r==7 && !SoldierList[SoldierIndex].IsDama)
					SoldierList[SoldierIndex].IsDama=true;

				/* Setting the Board Destination state to the soldiers state */
				board[Destination_r][Destination_c] = Type;
			}

			//==================================================================================================================================
			/* Function to kill a soldier */
			//==================================================================================================================================
			void KillSoldier(SoldierState board[][COLUMNS], Soldier *SoldierList, int SoldierIndex, int &NumberOfSoldiers )
			{
				/* Setting the soldiers slot to Dead(empty)*/
				board[SoldierList[SoldierIndex].Position.r][SoldierList[SoldierIndex].Position.c] = Dead;
				/* Setting the soldiers state to Dead*/
				SoldierList[SoldierIndex].soldier_State = Dead;
				SoldierList[SoldierIndex].Position.r=SoldierList[SoldierIndex].Position.c=-1;
				/* Decrement the number of soldiers left by one */
				NumberOfSoldiers--;
			}
			//==================================================================================================================================
			//==================================================================================================================================
			int GetSoldierIndex(Soldier *SoldierList, int r, int c)
			{
				/* Looping through the array of soldiers in order to search for the correct soldier */
				for(int s=0; s<16; s++)
					/* If the soldier was found we return its index */
					if(SoldierList[s].Position.r == r && SoldierList[s].Position.c == c)
						return s;
				return -1;
			}

			/* To check if the soldier is an eater */
			bool SoldierIsEater (Soldier *Soldier_List, int SoldierIndex)
			{
				return (Soldier_List[SoldierIndex].Victims.size()!=0);
			}

			//==================================================================================================================================
			//==================================================================================================================================
			void SelectSoldier(Soldier *SoldierList, int SoldierIndex)
			{
				/* Looping through the array in order to search for the selected soldier and deselected */
				for(int s=0; s<16; s++)
					if(SoldierList[s].IsSelected)
						SoldierList[s].IsSelected = false;
				/* Select a certain soldier according to the SoldierIndex parameter */
				if(SoldierIndex != -1 )
					SoldierList[SoldierIndex].IsSelected = true;
			}

			//==================================================================================================================================
			//==================================================================================================================================

			//================================================================================================
			/*Checking for available moves of the current soldier knowing it from the index.
			The Type variable is used to determine if the current soldier is White or Black,
			so that it can move Upwards or Downwards!*/
			//================================================================================================
			vector<SoldierPosition> CheckAvailableMoves(SoldierState board[][COLUMNS], Soldier* Soldier_List, int SoldierIndex, SoldierState Type)
			{
				int r,c;
				/*Saving the current row and column of the sent soldier index*/
				r= Soldier_List[SoldierIndex].Position.r;
				c= Soldier_List[SoldierIndex].Position.c;

				/*Declaring a temporary vector that holds the movements of the current soldier*/
				vector<SoldierPosition> moves;
				/*Declaring a temporary position that holds the position of the current soldier*/
				SoldierPosition pos;
				
				/*Checking...*/
				/*If the soldier is going Left and the cell that the soldier is going to is inside 
				the board and not Dead*/
				if(c-1>-1 && board[r][c-1]==Dead)
				{
					pos.r= r;
					pos.c= c-1;
					moves.push_back(pos);
				}
				/*If the soldier is going Right and the cell that the soldier is going to is inside 
				the board and not Dead*/
				if(c+1<COLUMNS && board[r][c+1]==Dead)
				{
					pos.r= r;
					pos.c= c+1;
					moves.push_back(pos);
				}
				/*If the soldier type is White or it was a IsDama*/
				/*If the current soldier position was inside the board and is going Up and 
				 the cell that the soldier is going to is inside the board and not Dead*/
				if(Type==White || Soldier_List[SoldierIndex].IsDama)
					if(r-1>-1 && Board[r-1][c]==Dead)
					{
						pos.r= r-1;
						pos.c= c;
						moves.push_back(pos);
					}
				/*If the soldier type is Black or it was a IsDama*/
				/*If the current soldier position was inside the board and is going Down and 
				 the cell that the soldier is going to is inside the board and not Dead*/
				if(Type==Black || Soldier_List[SoldierIndex].IsDama)
					if(r+1<ROWS && Board[r+1][c]==Dead)
					{
						pos.r= r+1;
						pos.c= c;
						moves.push_back(pos);
					}

				return moves;
			}

			/* Function to check all the eats for the soldier or the list of soldiers */
			bool CheckAvailableEats (SoldierState board[][COLUMNS], Soldier *Soldier_List, SoldierState Type, int SoldierIndex)
			{
				/* The obligatory eat */
				bool MustEat=false;
				SoldierState OppositeType;

				if(Type==White)
					OppositeType=Black;
				else
					OppositeType=White;

				/* Check eats for one soldier */
				if(SoldierIndex!=-1)
				{
					if(Soldier_List[SoldierIndex].soldier_State!=Dead)
					{
						Soldier_List[SoldierIndex].Victims.clear();

						int r=Soldier_List[SoldierIndex].Position.r, c=Soldier_List[SoldierIndex].Position.c;
						SoldierPosition VictPos;

						/* Left Eat */
						if(c-2>-1 && board[r][c-2]==Dead && board[r][c-1]==OppositeType)
						{
							MustEat=true;
							VictPos.r=r;	VictPos.c=c-1;
							Soldier_List[SoldierIndex].Victims.push_back(VictPos);
							Soldier_List[SoldierIndex].IsSelected=true;
						}
						/* Right Eat */
						if(c+2<8 && board[r][c+2]==Dead && board[r][c+1]==OppositeType)
						{
							MustEat=true;
							VictPos.r=r;	VictPos.c=c+1;
							Soldier_List[SoldierIndex].Victims.push_back(VictPos);
							Soldier_List[SoldierIndex].IsSelected=true;
						}
						/* White soldier or black IsDama --> check Top */
						if(Type==White  || Soldier_List[SoldierIndex].IsDama)
						{
							if(r-2>-1 && board[r-2][c]==Dead && board[r-1][c]==OppositeType)
							{
								MustEat=true;
								VictPos.r=r-1;	VictPos.c=c;
								Soldier_List[SoldierIndex].Victims.push_back(VictPos);
								Soldier_List[SoldierIndex].IsSelected=true;
							}
						}
						/* Black soldier or white IsDama --> check bottom */
						if(Type==Black || Soldier_List[SoldierIndex].IsDama)
						{
							if(r+2<8 && board[r+2][c]==Dead && board[r+1][c]==OppositeType)
							{
								MustEat=true;
								VictPos.r=r+1;	VictPos.c=c;
								Soldier_List[SoldierIndex].Victims.push_back(VictPos);
								Soldier_List[SoldierIndex].IsSelected=true;
							}
						}
					}
				}
				/* Check eats for all the soldiers in the list */
				else
				{
					/* Loop through all the soldiers in the list */
					for(int s=0; s<16; s++)
						/* Check the eats for the current soldiers, if one of the soldiers has an obligatory eat --> the must eat is true */
						if(CheckAvailableEats(board,Soldier_List,Type,s))
							MustEat=true;
				}				
				/* if we must eat or not */
				return MustEat;
			}

			/* Function to check the validity of the move along with checking
			   the rules and eating if possible while moving */
			void CheckAndExecuteMove (SoldierState board[][COLUMNS], Soldier *soldiers_list, Soldier *opp_soldiers_list, SoldierState Type, int &OppNumberOfSoldiers, int r, int c)
			{
				bool MustSwitch=false;
				/* if the current cell is empty, we have 2 choices: */
				if(board[r][c]==Dead)
				{
					int curr_r, curr_c;
					/* - 1st choice is that we are just selecting an empty cell whithout 
						selecting which soldier to move..
						- 2nd choice is that we already selected a soldier so the r & c are
						not the initial values */
					if(SelectedSoldierIndex!=-1)
					{
						curr_r= soldiers_list[SelectedSoldierIndex].Position.r;
						curr_c= soldiers_list[SelectedSoldierIndex].Position.c;
						
						if(CheckAvailableEats(board,soldiers_list,Type,SelectedSoldierIndex))
						{
							for(unsigned int v=0; v<soldiers_list[SelectedSoldierIndex].Victims.size(); v++)
							{
								/* Left Eat */
								if(r==soldiers_list[SelectedSoldierIndex].Victims[v].r && c==soldiers_list[SelectedSoldierIndex].Victims[v].c-1 && r==curr_r)
								{
									KillSoldier(board,opp_soldiers_list,GetSoldierIndex(opp_soldiers_list,soldiers_list[SelectedSoldierIndex].Victims[v].r,soldiers_list[SelectedSoldierIndex].Victims[v].c),OppNumberOfSoldiers);
									MoveSoldier(board,soldiers_list,Type,SelectedSoldierIndex,r,c);
									SwitchTurn();
								}
								/* Right Eat */
								else if(r==soldiers_list[SelectedSoldierIndex].Victims[v].r && c==soldiers_list[SelectedSoldierIndex].Victims[v].c+1 && r==curr_r)
								{
									KillSoldier(board,opp_soldiers_list,GetSoldierIndex(opp_soldiers_list,soldiers_list[SelectedSoldierIndex].Victims[v].r,soldiers_list[SelectedSoldierIndex].Victims[v].c),OppNumberOfSoldiers);
									MoveSoldier(board,soldiers_list,Type,SelectedSoldierIndex,r,c);
									SwitchTurn();
								}
								/* Top Eat */
								else if((Type==White || soldiers_list[SelectedSoldierIndex].IsDama) && r==soldiers_list[SelectedSoldierIndex].Victims[v].r-1 && c==soldiers_list[SelectedSoldierIndex].Victims[v].c && c==curr_c)
								{
									KillSoldier(board,opp_soldiers_list,GetSoldierIndex(opp_soldiers_list,soldiers_list[SelectedSoldierIndex].Victims[v].r,soldiers_list[SelectedSoldierIndex].Victims[v].c),OppNumberOfSoldiers);
									MoveSoldier(board,soldiers_list,Type,SelectedSoldierIndex,r,c);
									SwitchTurn();
								}
								/* Bottom Eat */
								else if((Type==Black || soldiers_list[SelectedSoldierIndex].IsDama) && r==soldiers_list[SelectedSoldierIndex].Victims[v].r+1 && c==soldiers_list[SelectedSoldierIndex].Victims[v].c && c==curr_c)
								{
									KillSoldier(board,opp_soldiers_list,GetSoldierIndex(opp_soldiers_list,soldiers_list[SelectedSoldierIndex].Victims[v].r,soldiers_list[SelectedSoldierIndex].Victims[v].c),OppNumberOfSoldiers);
									MoveSoldier(board,soldiers_list,Type,SelectedSoldierIndex,r,c);
									SwitchTurn();
								}
								break;
							}
						}
						else
						{
							vector<SoldierPosition> Moves = CheckAvailableMoves(board,soldiers_list,SelectedSoldierIndex,Type);
							for(unsigned int m=0; m<Moves.size(); m++)
							{
								if(r==Moves[m].r && c==Moves[m].c)
								{
									MoveSoldier(board,soldiers_list,Type,SelectedSoldierIndex,r,c);
									SwitchTurn();
								}
							}
						}
					}
				}
				/* We either have a white or a black soldier */
				/* if the current cell contains the same color as the turn */
				else if(board[r][c]==Type)
				{
					/* Take the id of the current r & c soldier */
					int tmp_idx=GetSoldierIndex(soldiers_list,r,c);
					/* Check if we have an obligatory eat */
					if(ObligatoryEat)
					{
						/* if we must eat --> select only one of the eaters */
						if(SoldierIsEater(soldiers_list,tmp_idx))
						{
							SelectSoldier(soldiers_list,tmp_idx);
							SelectedSoldierIndex=tmp_idx;
						}
					}
					/* we don't have an obligatory eat --> select */
					else
					{
						SelectSoldier(soldiers_list,tmp_idx);
						SelectedSoldierIndex=tmp_idx;
					}
				}
			}

			/* To check if the game has ended */
			void UpdateGameState (void)
			{
				if( White_Soldier_Count == 0 )
					GState=Win_State;
				else if( Black_Soldier_Count == 0 )
					GState=Lose_State;
				else if( White_Soldier_Count == 1 &&  Black_Soldier_Count == 1 )
						GState=Draw_State;
				else
					GState=None_State;
			}

			/* Function to check if there is a win or lose to know to return 1000 or -1000 */
			bool CheckWinLose (int &result, int WhiteSoldierNbr, int BlackSoldierNbr)
			{
				/* Computer wins */
				if( WhiteSoldierNbr == 0 )
				{
					result=1000;
					return true;
				}
				/* Player wins */
				else if( BlackSoldierNbr == 0 )
				{
					result=-1000;
					return true;
				}
				return false;
			}

			/* Function to return the evaluation of the current board state */
			int Evaluate (int current_white_nbr, int current_black_nbr, Soldier* white_soldiers, Soldier* black_soldiers)
			{				
				/* vars to count the white and black damas */
				int blackDamas=0, whiteDamas=0;
				int oldBlackDamas=0, oldWhiteDamas=0;
				
				/* Loop through the white & black soldiers */
				for(int i=0; i<16; i++)
				{
					/* If the current white soldier is alive */
					if(white_soldiers[i].soldier_State!=Dead)
					{
						/* if it is a dama update the current dama counter */
						if(white_soldiers[i].IsDama)
							whiteDamas++;
						/* if the old dama is white --> update the old dama counter */
						if(White_Soldiers[i].IsDama)
							oldWhiteDamas++;
					}
					/* If the current black soldier is alive */
					if(black_soldiers[i].soldier_State!=Dead)
					{
						/* if it is a dama update the current dama counter */
						if(black_soldiers[i].IsDama)
							blackDamas++;
						/* if the old dama is white --> update the old dama counter */
						if(Black_Soldiers[i].IsDama)
							oldBlackDamas++;				
					}
				}
				/* here we take the current nbr of blacks and subtract from old nbr of blacks because
				   if we had for ex 10 at the beginning of the turn and we ended here with 8 --> it is
				   a loss: 8-10=-2.
				   We multiply by 2 because if the loss of black is -2 and the win for the white is 2
				   --> -2+2=0 means nothing happened, but in this case the AI saw that this value (0) 
				   is like a normal move so he can pick it but in fact it has more losing value than
				   a normal move that's why I multiplied by 2 --> 8-10=-2*2=-4 & 12-10=2 --> -4+2=-2 <-- losing a little bit*/
				current_black_nbr=(current_black_nbr-Black_Soldier_Count)*2;
				/* here we take the old number of whites - the current nbr of whites so if in this turn
				   we have 12 whites and at the end we have 10 --> 12-10=2 which favours the computer */
				current_white_nbr=White_Soldier_Count-current_white_nbr;
				
				/* same for the nbr of damas */
				blackDamas= blackDamas-oldBlackDamas;
				whiteDamas= oldWhiteDamas-whiteDamas;

				/* return the value of the current blacks + whites * 5 + blackdamas + white damas*6 (which meas that a dama is more
				   important than a normal eat or a normal loss */
				return ((current_black_nbr+current_white_nbr)*5)+((blackDamas+whiteDamas)*6);
			}

			/* The Player's function */
			int Player (SoldierState board[][COLUMNS], Soldier *white_soldiers, Soldier *black_soldiers, int WhiteSoldierNbr, int BlackSoldierNbr, int depth, int alpha, int beta)
			{
				/* Set beta and value to infinity */
				int value = INFINITY;
				beta=INFINITY;
				int tmp_value = 0;

				/* Creating temporary copy to the board */
				SoldierState TmpBoard[ROWS][COLUMNS];
				/* Creating temporary array to store a copy of the soldiers array */
				Soldier TmpWhiteSoldiers[16], TmpBlackSoldiers[16];
				int TmpWhiteSoldierNbr, TmpBlackSoldierNbr;

				/* Check the available eats for the player */
				bool obligatoryeat=CheckAvailableEats(board,white_soldiers,White,-1);

				/* SAVE THE REAL CURRENT GAME STATE */
				SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);

				int winlose_result=0;
				/* Base case reached when there is a win or lose */
				if(CheckWinLose(winlose_result,WhiteSoldierNbr,BlackSoldierNbr))
					return winlose_result;
				/* if there is neither a win nor a lose, check if we must stop the recursion but the depth limit */
				if(depth<=0)
					return Evaluate(WhiteSoldierNbr,BlackSoldierNbr,white_soldiers,black_soldiers);
				
				/* If we have an obligatory eat */
				if(obligatoryeat)
				{
					/* Here we don't need to check whether we can eat or move because we already checked if we must eat */
					for(int s=0; s<16; s++)
					{
						/* Take this soldier and check if he is an eater */
						if(TmpWhiteSoldiers[s].soldier_State!=Dead && SoldierIsEater(TmpWhiteSoldiers,s))
						{
							int r=TmpWhiteSoldiers[s].Position.r, c=TmpWhiteSoldiers[s].Position.c;
							
							/* Loop through its victims */
							for(unsigned int v=0; v<TmpWhiteSoldiers[s].Victims.size(); v++)
							{
								SoldierPosition CurDes;
								/* Left Eat */
								if(r==TmpWhiteSoldiers[s].Victims[v].r && c==TmpWhiteSoldiers[s].Victims[v].c+1)
								{
									CurDes.r=r;	CurDes.c=c-2;
								}
								/* Right Eat */
								else if(r==TmpWhiteSoldiers[s].Victims[v].r && c==TmpWhiteSoldiers[s].Victims[v].c-1)
								{
									CurDes.r=r;	CurDes.c=c+2;
								}
								/* Top Eat */
								else if(r==TmpWhiteSoldiers[s].Victims[v].r+1 && c==TmpWhiteSoldiers[s].Victims[v].c)
								{
									CurDes.r=r-2; CurDes.c=c;
								}
								/* Bottom Eat (DAMA) */
								else if(TmpWhiteSoldiers[s].IsDama && r==TmpWhiteSoldiers[s].Victims[v].r-1 && c==TmpWhiteSoldiers[s].Victims[v].c)
								{
									CurDes.r=r+2; CurDes.c=c;
								}

								/* Kill the black soldier */
								KillSoldier(TmpBoard,TmpBlackSoldiers,GetSoldierIndex(TmpBlackSoldiers,TmpWhiteSoldiers[s].Victims[v].r,TmpWhiteSoldiers[s].Victims[v].c),TmpBlackSoldierNbr);
								/* Move the white soldier */
								MoveSoldier(TmpBoard,TmpWhiteSoldiers,White,s,CurDes.r,CurDes.c);
								/* switch the turn */
								tmp_value=Computer(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,depth-1,alpha,beta);

								/* After doing the move, we can still have a move --> so we call recursively */
								if(tmp_value<value)
									/* Update value with tmp_value */
									value=tmp_value;

								/* Pruning the uneeded branches */
								beta=value;
								/* if the new value is less than the current value --> ignore it */
								if(beta<=alpha)
									return alpha;

								/* Unplace */
								SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);
							}
						}
					}
				}
				else
				{
					/* Here we don't need to check whether we can eat or move because we already checked if we must eat */
					for(int s=0; s<16; s++)
					{
						/* if the current cell is valid */
						if(TmpWhiteSoldiers[s].soldier_State!=Dead)
						{
							vector<SoldierPosition> moves = CheckAvailableMoves(TmpBoard,TmpWhiteSoldiers,s,White);
							for(unsigned int m=0; m<moves.size(); m++)
							{
								MoveSoldier(TmpBoard,TmpWhiteSoldiers,White,s,moves[m].r,moves[m].c);
								tmp_value=Computer(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,depth-1,alpha,beta);

								/* if the result is greater than value --> update value */
								if(tmp_value<value)
									/* Update value with tmp_value */
									value=tmp_value;

								/* Pruning the uneeded branches */
								beta=value;
								/* if the new value is less than the current value --> ignore it */
								if(beta<=alpha)
									return alpha;
								
								/* Unplace */
								SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);
							}
						}
					}
				}
				return beta;
			}

			/* The Computer's function */
			int Computer(SoldierState board[][COLUMNS], Soldier *white_soldiers, Soldier *black_soldiers, int WhiteSoldierNbr, int BlackSoldierNbr, int depth, int alpha, int beta)
			{
				int value = -INFINITY;
				alpha=-INFINITY;
				int tmp_value = 0;

				/* Creating temporary copy to the board */
				SoldierState TmpBoard[ROWS][COLUMNS];
				/* Creating temporary array to store a copy of the soldiers array */
				Soldier TmpWhiteSoldiers[16], TmpBlackSoldiers[16];
				int TmpWhiteSoldierNbr, TmpBlackSoldierNbr;

				/* Check the available eats for the computer */
				bool obligatoryeat=CheckAvailableEats(board,black_soldiers,Black,-1);

				/* SAVE THE REAL CURRENT GAME STATE */
				SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);

				
				int winlose_result=0;
				/* Base case reached when there is a win or lose */
				if(CheckWinLose(winlose_result,WhiteSoldierNbr,BlackSoldierNbr))
					return winlose_result;
				/* if there is neither a win nor a lose, check if we must stop the recursion but the depth limit */
				if(depth<=0)
					return Evaluate(WhiteSoldierNbr,BlackSoldierNbr,white_soldiers,black_soldiers);
		
				/* If we have an obligatory eat */
				if(obligatoryeat)
				{
					/* Here we don't need to check whether we can eat or move because we already checked if we must eat */
					for(int s=0; s<16; s++)
					{
						/* Take this soldier and check if he is an eater */
						if(TmpBlackSoldiers[s].soldier_State!=Dead && SoldierIsEater(TmpBlackSoldiers,s))
						{
							int r=TmpBlackSoldiers[s].Position.r, c=TmpBlackSoldiers[s].Position.c;

							/* Loop through its victims */
							for(unsigned int v=0; v<TmpBlackSoldiers[s].Victims.size(); v++)
							{
								SoldierPosition CurDes;

								/* Left Eat */
								if(r==TmpBlackSoldiers[s].Victims[v].r && c==TmpBlackSoldiers[s].Victims[v].c+1)
								{
									CurDes.r=r;	CurDes.c=c-2;
								}
								/* Right Eat */
								else if(r==TmpBlackSoldiers[s].Victims[v].r && c==TmpBlackSoldiers[s].Victims[v].c-1)
								{
									CurDes.r=r;	CurDes.c=c+2;
								}
								/* Bottom Eat */
								else if(r==TmpBlackSoldiers[s].Victims[v].r-1 && c==TmpBlackSoldiers[s].Victims[v].c)
								{
									CurDes.r=r+2; CurDes.c=c;
								}
								/* Top Eat (DAMA) */
								else if(TmpBlackSoldiers[s].IsDama && r==TmpBlackSoldiers[s].Victims[v].r+1 && c==TmpBlackSoldiers[s].Victims[v].c)
								{
									CurDes.r=r-2; CurDes.c=c;
								}

								/* Kill the white soldier */
								KillSoldier(TmpBoard,TmpWhiteSoldiers,GetSoldierIndex(TmpWhiteSoldiers,TmpBlackSoldiers[s].Victims[v].r,TmpBlackSoldiers[s].Victims[v].c),TmpWhiteSoldierNbr);
								/* Move the black soldier */
								MoveSoldier(TmpBoard,TmpBlackSoldiers,Black,s,CurDes.r,CurDes.c);
								/* Switch turn */
								tmp_value=Player(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,depth-1,alpha,beta);

								/* After doing the move, we can still have a move --> so we call recursively */
								if(tmp_value>value)
									/* Update value with tmp_value */
									value=tmp_value;

								/* Pruning the uneeded branches */
								alpha=value;
								/* if the current value is greater than beta --> cut it */
								if(alpha>=beta)
									return beta;

								/* Unplace */
								SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);
							}
						}
					}
				}
				else
				{
					/* Here we don't need to check whether we can eat or move because we already checked if we must eat */
					for(int s=0; s<16; s++)
					{
						/* if the current cell is valid */
						if(TmpBlackSoldiers[s].soldier_State!=Dead)
						{
							vector<SoldierPosition> moves = CheckAvailableMoves(TmpBoard,TmpBlackSoldiers,s,Black);
							for(unsigned int m=0; m<moves.size(); m++)
							{
								MoveSoldier(TmpBoard,TmpBlackSoldiers,Black,s,moves[m].r,moves[m].c);
								tmp_value=Player(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,depth-1,alpha,beta);

								/* if the result is greater than value --> update value */
								if(tmp_value>value)
									/* Update value with tmp_value */
									value=tmp_value;

								/* Pruning the uneeded branches */
								alpha=value;
								/* if the current value is greater than beta --> cut it */
								if(alpha>=beta)
									return beta;
								
								/* Unplace */
								SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);
							}
						}
					}
				}
				return alpha;
			}
	
			/* The MiniMax Function */
			void MiniMaxPrinciple (SoldierState board[8][COLUMNS], Soldier *white_soldiers, Soldier *black_soldiers, int &WhiteSoldierNbr, int &BlackSoldierNbr)
			{
				/* Init value to -inf because we want the maximum */
				int Value = -INFINITY;
				int tmp_value = 0;

				/* Creating temporary copy to the board */
				SoldierState TmpBoard[ROWS][COLUMNS];
				/* Creating temporary array to store a copy of the soldiers array */
				Soldier TmpWhiteSoldiers[16], TmpBlackSoldiers[16];
				int TmpWhiteSoldierNbr, TmpBlackSoldierNbr;

				/* SAVE THE REAL CURRENT GAME STATE */
				SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
								board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);
				
				/* here we are after checking if we have an obligatory eat in the SwitchTurn function */
				/* If we must eat */
				if(ObligatoryEat)
				{
					/* Loop through the soldiers */
					for(int s=0; s<16; s++)
					{
						/* and check if the current soldier is not dead and he is an eater */
						if(TmpBlackSoldiers[s].soldier_State!=Dead && SoldierIsEater(TmpBlackSoldiers,s))
						{
							/* Take its position */
							int r=TmpBlackSoldiers[s].Position.r, c=TmpBlackSoldiers[s].Position.c;

							/* Loop through his victims */
							for(unsigned int v=0; v<TmpBlackSoldiers[s].Victims.size(); v++)
							{
								SoldierPosition CurDes;

								/* Left Eat */
								if(r==TmpBlackSoldiers[s].Victims[v].r && c==TmpBlackSoldiers[s].Victims[v].c+1)
								{
									CurDes.r=r;	CurDes.c=c-2;
								}
								/* Right Eat */
								else if(r==TmpBlackSoldiers[s].Victims[v].r && c==TmpBlackSoldiers[s].Victims[v].c-1)
								{
									CurDes.r=r;	CurDes.c=c+2;
								}
								/* Bottom Eat */
								else if(r==TmpBlackSoldiers[s].Victims[v].r-1 && c==TmpBlackSoldiers[s].Victims[v].c)
								{
									CurDes.r=r+2; CurDes.c=c;
								}
								/* Top Eat */
								else if(TmpBlackSoldiers[s].IsDama && r==TmpBlackSoldiers[s].Victims[v].r+1 && c==TmpBlackSoldiers[s].Victims[v].c)
								{
									CurDes.r=r-2; CurDes.c=c;
								}

								/* Kill the victim */
								KillSoldier(TmpBoard,TmpWhiteSoldiers,GetSoldierIndex(TmpWhiteSoldiers,TmpBlackSoldiers[s].Victims[v].r,TmpBlackSoldiers[s].Victims[v].c),TmpWhiteSoldierNbr);
								/* Move the eater to the destination */
								MoveSoldier(TmpBoard,TmpBlackSoldiers,Black,s,CurDes.r,CurDes.c);

								/* switch the turn */
								tmp_value=Player(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,DEPTH,-INFINITY,INFINITY);

								/* if the value is better than the value */
								if(tmp_value>Value)
								{
									/* Update value with tmp_value */
									Value=tmp_value;
									/* save the best move */
									SaveBestMoves sbm;
									sbm.Idx = s;
									sbm.BestDestination = CurDes;
									/* Clear the vector of equal moves that later on we will use for random */
									AllBestMoves.clear();
									AllBestMoves.push_back(sbm);
								}
								/* if we have the same value */
								else if(tmp_value==Value)
								{
									/* save it in the vector of equal moves to make a random in it */
									SaveBestMoves sbm;
									sbm.Idx = s;
									sbm.BestDestination = CurDes;
									AllBestMoves.push_back(sbm);
								}

								/* Unplace */
								SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);
							}
						}
					}
				}
				else
				{
					/* Here we don't need to check whether we can eat or move because we already checked if we must eat */
					for(int s=0; s<16; s++)
					{
						/* if the current cell is valid */
						if(TmpBlackSoldiers[s].soldier_State!=Dead)
						{
							/* Take the available moves for the current soldier */
							vector<SoldierPosition> moves = CheckAvailableMoves(TmpBoard,TmpBlackSoldiers,s,Black);

							/* Loop through them */
							for(unsigned int m=0; m<moves.size(); m++)
							{
								/* Move the soldier to the current cell */
								MoveSoldier(TmpBoard,TmpBlackSoldiers,Black,s,moves[m].r,moves[m].c);
								/* Switch the turn */
								tmp_value=Player(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,DEPTH,-INFINITY,INFINITY);

								/* if the result is greater than value --> update value */
								if(tmp_value>Value)
								{
									/* Update value with tmp_value */
									Value=tmp_value;
									/* save the best move */
									SaveBestMoves sbm;
									sbm.Idx = s;
									sbm.BestDestination = moves[m];
									/* Clear the vector of equal moves that later on we will use for random */
									AllBestMoves.clear();
									AllBestMoves.push_back(sbm);
								}
								/* if we have the same value */
								else if(tmp_value==Value)
								{
									/* save it in the vector of equal moves to make a random in it */
									SaveBestMoves sbm;
									sbm.Idx = s;
									sbm.BestDestination = moves[m];
									AllBestMoves.push_back(sbm);
								}
								
								/* Unplace */
								SaveGameState(TmpBoard,TmpWhiteSoldiers,TmpBlackSoldiers,TmpWhiteSoldierNbr,TmpBlackSoldierNbr,
											board,white_soldiers,black_soldiers,WhiteSoldierNbr,BlackSoldierNbr);
							}
						}
					}
				}
				/* Executing the best chosen move for the computer */
				ExecuteComputerBestMove(board,white_soldiers,black_soldiers,WhiteSoldierNbr);
			}

			/* Function to execute the best move */
			void ExecuteComputerBestMove (SoldierState board[8][COLUMNS], Soldier *white_soldiers, Soldier *black_soldiers, int &WhiteSoldierNbr)
			{
				bool TmpObligatoryEat=true;
				int BestSoldier_Idx;
				SoldierPosition BestDestination;

				/* if only we have one move --> the best move is the only move in the vector */
				if(AllBestMoves.size() == 1)
				{
					BestSoldier_Idx = AllBestMoves[0].Idx;
					BestDestination = AllBestMoves[0].BestDestination;
				}
				/* take any best move of the vector */
				else
				{
					int random = rand()%(int)AllBestMoves.size();
					BestSoldier_Idx = AllBestMoves[random].Idx;
					BestDestination = AllBestMoves[random].BestDestination;
				}

				/* 1st case --> normal move if the difference between the soldier position and the destination is 1 */
				if(abs(black_soldiers[BestSoldier_Idx].Position.r-BestDestination.r)==1 || abs(black_soldiers[BestSoldier_Idx].Position.c-BestDestination.c)==1)
					MoveSoldier(board,black_soldiers,Black,BestSoldier_Idx,BestDestination.r,BestDestination.c);
				else
				{
					/* Draw dynamically */
					DynamicDraw();
					SoldierPosition CurDes;
					SoldierPosition VictPos;

					/* Save the position of the soldier */
					int r=black_soldiers[BestSoldier_Idx].Position.r, c=black_soldiers[BestSoldier_Idx].Position.c, s=BestSoldier_Idx;

					/* Left Eat */
					if(r==BestDestination.r && c==BestDestination.c+2)
					{
						VictPos.r=BestDestination.r;	VictPos.c=BestDestination.c+1;
						CurDes.r=r;	CurDes.c=c-2;
					}
					/* Right Eat */
					else if(r==BestDestination.r && c==BestDestination.c-2)
					{
						VictPos.r=BestDestination.r;	VictPos.c=BestDestination.c-1;
						CurDes.r=r;	CurDes.c=c+2;
					}
					/* Bottom Eat */
					else if(r==BestDestination.r-2 && c==BestDestination.c)
					{
						VictPos.r=BestDestination.r-1;	VictPos.c=BestDestination.c;
						CurDes.r=r+2;	CurDes.c=c;
					}
					/* Top Eat */
					else if(r==BestDestination.r+2 && c==BestDestination.c)
					{
						VictPos.r=BestDestination.r+1;	VictPos.c=BestDestination.c;
						CurDes.r=r-2;	CurDes.c=c;
					}

					/* Kill the victim */
					KillSoldier(board,white_soldiers,GetSoldierIndex(white_soldiers,VictPos.r,VictPos.c),WhiteSoldierNbr);
					/* Move the eater to the destination */
					MoveSoldier(board,black_soldiers,Black,s,CurDes.r,CurDes.c);
				}
			}

			/* Function to save the board in temp arrays */
			void SaveGameState (SoldierState tmp_board[8][COLUMNS], Soldier *tmp_white_soldiers, Soldier *tmp_black_soldiers, int &tmp_white_nbr, int &tmp_black_nbr,
								SoldierState board[8][COLUMNS], Soldier *white_soldiers, Soldier *black_soldiers, int white_nbr, int black_nbr)
			{
				/* Copy the board */
				memcpy(tmp_board,board,COLUMNS*ROWS*(sizeof(SoldierState)));
				/* Copy the soldiers arrays */
				for(int i=0; i<16; i++)
				{
					tmp_white_soldiers[i]=white_soldiers[i];
					tmp_black_soldiers[i]=black_soldiers[i];
				}
				/* Copy the nbr of whites & blacks */
				tmp_white_nbr=white_nbr;
				tmp_black_nbr=black_nbr;
			}

			void Draw (void)
			{
				Matrix<3> Object_Matrix, Mapping_Matrix, Final;
				/* Building the viewing matrix */
				Viewing_Transformation(ViewPort_List, Mapping_Matrix);
				VERTEX InitialPos;	InitialPos.x=-210;	InitialPos.y=210;
				
				/* Transforming the objects from their model coodinates to the world coord */
				Map->BuildCharacterMatrix(Object_Matrix.M);
				/* Concatinating the final matrix */
				Final=Mapping_Matrix*Object_Matrix;
				/* Drawing the object */
				Map->Draw(Final.M);

				/* Looping through all the objects */
				for(int i=0; i<16; i++)
				{
					if(Black_Soldiers[i].soldier_State!=Dead)
					{
						BlackS->GetSettings().Position.x = InitialPos.x + Black_Soldiers[i].Position.c*60;
						BlackS->GetSettings().Position.y = InitialPos.y - Black_Soldiers[i].Position.r*60;
						/* Transforming the objects from their model coodinates to the world coord */
						BlackS->BuildCharacterMatrix(Object_Matrix.M);
							
						/* Concatinating the final matrix */
						Final=Mapping_Matrix*Object_Matrix;
						
						/* If the current soldier is selected */
						if(Black_Soldiers[i].IsSelected)
						{
							/* change its color */
							BlackS->GetPrimitives()[0].Color.r=100;
						}
						else
							BlackS->GetPrimitives()[0].Color.r=255;

						/* Drawing the object */
						BlackS->Draw(Final.M);

						if(Black_Soldiers[i].IsDama)
						{
							BlackS->GetPrimitives()[0].Color.r=0;
							BlackS->GetPrimitives()[0].V1.x=10;
							/* Drawing the object */
							BlackS->Draw(Final.M);
							BlackS->GetPrimitives()[0].V1.x=20;
						}
					}
					if(White_Soldiers[i].soldier_State!=Dead)
					{
						WhiteS->GetSettings().Position.x = InitialPos.x + White_Soldiers[i].Position.c*60;
						WhiteS->GetSettings().Position.y = InitialPos.y - White_Soldiers[i].Position.r*60;

						/* Transforming the objects from their model coodinates to the world coord */
						WhiteS->BuildCharacterMatrix(Object_Matrix.M);
						/* Concatinating the final matrix */
						Final=Mapping_Matrix*Object_Matrix;

						/* If the current soldier is selected */
						if(White_Soldiers[i].IsSelected)
						{
							/* change its color */
							WhiteS->GetPrimitives()[0].Color.r=WhiteS->GetPrimitives()[0].Color.g=WhiteS->GetPrimitives()[0].Color.b=100;
						}
						else
							WhiteS->GetPrimitives()[0].Color.r=WhiteS->GetPrimitives()[0].Color.g=WhiteS->GetPrimitives()[0].Color.b=255;

						/* Drawing the object */
						WhiteS->Draw(Final.M);

						if(White_Soldiers[i].IsDama)
						{
							WhiteS->GetPrimitives()[0].Color.r=WhiteS->GetPrimitives()[0].Color.g=WhiteS->GetPrimitives()[0].Color.b=0;
							WhiteS->GetPrimitives()[0].V1.x=10;
							/* Drawing the object */
							WhiteS->Draw(Final.M);
							WhiteS->GetPrimitives()[0].V1.x=20;
						}
					}
				}

				BlackS->GetSettings().Position.x = -280;
				BlackS->GetSettings().Position.y = 120;
				/* Transforming the objects from their model coodinates to the world coord */
				BlackS->BuildCharacterMatrix(Object_Matrix.M);
				/* Concatinating the final matrix */
				Final=Mapping_Matrix*Object_Matrix;
				BlackS->GetPrimitives()[0].Color.r=255;
				/* Drawing the object */
				BlackS->Draw(Final.M);

				WhiteS->GetSettings().Position.x = -280;
				WhiteS->GetSettings().Position.y = -120;
				/* Transforming the objects from their model coodinates to the world coord */
				WhiteS->BuildCharacterMatrix(Object_Matrix.M);
				/* Concatinating the final matrix */
				Final=Mapping_Matrix*Object_Matrix;
				WhiteS->GetPrimitives()[0].Color.r=WhiteS->GetPrimitives()[0].Color.g=WhiteS->GetPrimitives()[0].Color.b=255;
				/* Drawing the object */
				WhiteS->Draw(Final.M);
			}

			void DynamicDraw (void)
			{
				Init_Buffer(100);
				/* Drawing the viewports */
				Init_View(0);
				Draw();
				/*Copy buffer to Video RAM*/
				Graphics_Draw(NULL);

				char arr[10];
				sprintf(arr,"%i",Black_Soldier_Count);
				Graphics_TextOut(33,145,arr, 255, 0, 0);
				sprintf(arr,"%i",White_Soldier_Count);
				Graphics_TextOut(33,385,arr, 255, 255, 255);
				if(TState==Player_Turn)
					Graphics_TextOut(20,240,"Player", 255,255,255);
				else
					Graphics_TextOut(33,240,"AI", 255,0,0);
				sprintf(arr,"Depth = %i",DEPTH);
				Graphics_TextOut(0,0,arr, 255,0,0);

				Graphics_Flip();
				Sleep(300);
			}
};

