#include"SDL.h"
#include"SDL_ttf.h"
#include<iostream>
static int mouseX, mouseY, srcrow, srccol;
static bool isRunning = true;
SDL_Color white = { 255, 255, 255 ,255 };
SDL_Color black = {0,0,0,255};
class Piece {
public:
	Piece(char Color) : Color(Color) {}
	~Piece() {}
	virtual char GetPiece() = 0;
	char GetColor() {
		return Color;
	}
	bool isLegal(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) {
		Piece* qpDest = PieceBoardMat[DesRow][DesCol];
		if ((qpDest == 0) || (Color != qpDest->GetColor())) {
			return LegalSquares(SrcRow, SrcCol, DesRow, DesCol, PieceBoardMat);
		}
		return false;
	}
private:
	virtual bool LegalSquares(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) = 0;
	char Color;
};
static Piece* currentpiece=nullptr;
class Pawn : public Piece {
public:
	Pawn(char Color) : Piece(Color) {}
	~Pawn() {}
private:
	virtual char GetPiece() {
		return 'P';
	}
	bool LegalSquares(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) {
		Piece* qpDest = PieceBoardMat[DesRow][DesCol];
		if (qpDest == 0) {
			// Destination square is unoccupied
			if (SrcCol == DesCol) {
				if (GetColor() == 'W') {
					if (DesRow == SrcRow + 1) {
						return true;
					}
					if ((SrcRow==1)&&(DesRow == SrcRow + 2)) {
						return true;
					}
				}
				else {
					if (DesRow == SrcRow - 1) {
						return true;
					}
					if ((SrcRow == 6) && (DesRow == SrcRow - 2))return true;
				}
			}
		}
		else {
			// Dest holds piece of opposite color
			if ((SrcCol == DesCol + 1) || (SrcCol == DesCol - 1)) {
				if (GetColor() == 'W') {
					if (DesRow == SrcRow + 1) {
						return true;
					}
				}
				else {
					if (DesRow == SrcRow - 1) {
						return true;
					}
				}
			}
		}
		return false;
	}
};
class Knight : public Piece
{
public:
	Knight(char Color) : Piece(Color) {}
	~Knight() {}
private:
	virtual char GetPiece() {
		return 'N';
	}
	bool LegalSquares(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) {
		// Destination square is unoccupied or occupied by opposite color
		if ((SrcCol == DesCol + 1) || (SrcCol == DesCol - 1)) {
			if ((SrcRow == DesRow + 2) || (SrcRow == DesRow - 2)) {
				return true;
			}
		}
		if ((SrcCol == DesCol + 2) || (SrcCol == DesCol - 2)) {
			if ((SrcRow == DesRow + 1) || (SrcRow == DesRow - 1)) {
				return true;
			}
		}
		return false;
	}
};
class Bishop : public Piece
{
public:
	Bishop(char Color) : Piece(Color) {}
	~Bishop() {}
private:
	virtual char GetPiece() {
		return 'B';
	}
	bool LegalSquares(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) {
		if ((DesCol - SrcCol == DesRow - SrcRow) || (DesCol - SrcCol == SrcRow - DesRow)) {
			// Make sure that all invervening squares are empty
			int iRowOffset = (DesRow - SrcRow > 0) ? 1 : -1;
			int iColOffset = (DesCol - SrcCol > 0) ? 1 : -1;
			int iCheckRow;
			int iCheckCol;
			for (iCheckRow = SrcRow + iRowOffset, iCheckCol = SrcCol + iColOffset;
				iCheckRow != DesRow;
				iCheckRow = iCheckRow + iRowOffset, iCheckCol = iCheckCol + iColOffset)
			{
				if (PieceBoardMat[iCheckRow][iCheckCol] != 0) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
};
class Rook : public Piece
{
public:
	Rook(char Color) : Piece(Color) {}
	~Rook() {}
private:
	virtual char GetPiece() {
		return 'R';
	}
	bool LegalSquares(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) {
		if (SrcRow == DesRow) {
			// Make sure that all invervening squares are empty
			int iColOffset = (DesCol - SrcCol > 0) ? 1 : -1;
			for (int iCheckCol = SrcCol + iColOffset; iCheckCol != DesCol; iCheckCol = iCheckCol + iColOffset) {
				if (PieceBoardMat[SrcRow][iCheckCol] != 0) {
					return false;
				}
			}
			return true;
		}
		else if (DesCol == SrcCol) {
			// Make sure that all invervening squares are empty
			int iRowOffset = (DesRow - SrcRow > 0) ? 1 : -1;
			for (int iCheckRow = SrcRow + iRowOffset; iCheckRow != DesRow; iCheckRow = iCheckRow + iRowOffset) {
				if (PieceBoardMat[iCheckRow][SrcCol] != 0) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
};

class Queen : public Piece
{
public:
	Queen(char Color) : Piece(Color) {}
	~Queen() {}
private:
	virtual char GetPiece() {
		return 'Q';
	}
	bool LegalSquares(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) {
		if (SrcRow == DesRow) {
			// Make sure that all invervening squares are empty
			int iColOffset = (DesCol - SrcCol > 0) ? 1 : -1;
			for (int iCheckCol = SrcCol + iColOffset; iCheckCol != DesCol; iCheckCol = iCheckCol + iColOffset) {
				if (PieceBoardMat[SrcRow][iCheckCol] != 0) {
					return false;
				}
			}
			return true;
		}
		else if (DesCol == SrcCol) {
			// Make sure that all invervening squares are empty
			int iRowOffset = (DesRow - SrcRow > 0) ? 1 : -1;
			for (int iCheckRow = SrcRow + iRowOffset; iCheckRow != DesRow; iCheckRow = iCheckRow + iRowOffset) {
				if (PieceBoardMat[iCheckRow][SrcCol] != 0) {
					return false;
				}
			}
			return true;
		}
		else if ((DesCol - SrcCol == DesRow - SrcRow) || (DesCol - SrcCol == SrcRow - DesRow)) {
			// Make sure that all invervening squares are empty
			int iRowOffset = (DesRow - SrcRow > 0) ? 1 : -1;
			int iColOffset = (DesCol - SrcCol > 0) ? 1 : -1;
			int iCheckRow;
			int iCheckCol;
			for (iCheckRow = SrcRow + iRowOffset, iCheckCol = SrcCol + iColOffset;
				iCheckRow != DesRow;
				iCheckRow = iCheckRow + iRowOffset, iCheckCol = iCheckCol + iColOffset)
			{
				if (PieceBoardMat[iCheckRow][iCheckCol] != 0) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
};

class King : public Piece
{
public:
	King(char Color) : Piece(Color) {}
	~King() {}
private:
	virtual char GetPiece() {
		return 'K';
	}
	bool LegalSquares(int SrcRow, int SrcCol, int DesRow, int DesCol, Piece* PieceBoardMat[8][8]) {
		int iRowDelta = DesRow - SrcRow;
		int iColDelta = DesCol - SrcCol;
		if (((iRowDelta >= -1) && (iRowDelta <= 1)) &&
			((iColDelta >= -1) && (iColDelta <= 1)))
		{
			return true;
		}
		return false;
	}
};
class Board {
public:
	TTF_Font* font = TTF_OpenFont("Alpha.ttf", 128);;
	
	Board() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				BoardMat[i][j] = nullptr;
			}

		for (int j = 0; j < 8; ++j) {
			BoardMat[6][j] = new Pawn('B');
		}
		for (int j = 0; j < 8; ++j) {
			BoardMat[1][j] = new Pawn('W');
		}
		BoardMat[0][0] = new Rook('W');
		BoardMat[0][1] = new Knight('W');
		BoardMat[0][2] = new Bishop('W');
		BoardMat[0][3] = new King('W');
		BoardMat[0][4] = new Queen('W');
		BoardMat[0][5] = new Bishop('W');
		BoardMat[0][6] = new Knight('W');
		BoardMat[0][7] = new Rook('W');
		BoardMat[7][0] = new Rook('B');
		BoardMat[7][1] = new Knight('B');
		BoardMat[7][2] = new Bishop('B');
		BoardMat[7][3] = new King('B');
		BoardMat[7][4] = new Queen('B');
		BoardMat[7][5] = new Bishop('B');
		BoardMat[7][6] = new Knight('B');
		BoardMat[7][7] = new Rook('B');
		
	}
	void draw(SDL_Renderer* renderer) {

		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++)
			{
				if ((i + j) & 1) SDL_SetRenderDrawColor(renderer, 10, 50, 100, 255);
				else SDL_SetRenderDrawColor(renderer, 240, 240, 220, 255);
				SDL_Rect r = { 64 * (7 - j),64 * (7 - i),64,64 };
				if (currentpiece != nullptr) {
					if (isKingCheck(currentpiece->GetColor())) {
						char cColor = currentpiece->GetColor();
						if (currentpiece->isLegal(7-srcrow,7-srccol,i,j, BoardMat))
						{
							Piece* qpTemp = BoardMat[i][j];
							BoardMat[i][ j] = BoardMat[7-srcrow][7-srccol];
							BoardMat[7-srcrow][7-srccol] = nullptr;
							bool bCanMove = !isKingCheck(cColor);
							BoardMat[7-srcrow][7-srccol] = BoardMat[i][j];
							BoardMat[i][j] = qpTemp;
							if (bCanMove) {
								if (BoardMat[i][j] != nullptr)SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
								else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
							}
						}
					}
					else if (currentpiece->isLegal(7 - srcrow, 7 - srccol, i, j, BoardMat))
					{	
						Piece* temp = BoardMat[i][j];

						BoardMat[i][j] = BoardMat[7 - srcrow][7 - srccol];
						BoardMat[7 - srcrow][7 - srccol] = nullptr;
						bool flag = isKingCheck(currentpiece->GetColor());
						BoardMat[7 -srcrow][7 - srccol] = BoardMat[ i][ j];
						BoardMat[ i][j] = temp;
						if (flag)
						{
							if ((i + j) & 1) SDL_SetRenderDrawColor(renderer, 10, 50, 100, 255);
							else SDL_SetRenderDrawColor(renderer, 240, 240, 220, 255);
						}
						else if(BoardMat[i][j]!=nullptr)SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
						else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);

					}
				}
				SDL_RenderFillRect(renderer, &r);;
				if (BoardMat[i][j] != nullptr)
				{
					SDL_Surface* surface = nullptr;;
					SDL_Texture* texture = nullptr;
					switch (BoardMat[i][j]->GetPiece())
					{
					case 'P': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'p' : 'o'), black); break; }
					case 'B': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'b' : 'n'), black); break; }
					case 'N': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'h' : 'j'), black); break; }
					case 'Q': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'q' : 'w'), black); break; }
					case 'K': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'k' : 'l'), black); break; }
					case 'R': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'r' : 't'), black); break; }
					}
					texture = SDL_CreateTextureFromSurface(renderer, surface);
					SDL_Rect dstrect = { 64 * (7 - j), 64 * (7 - i), 64,64 };
					SDL_RenderCopy(renderer, texture, NULL, &dstrect);
					SDL_FreeSurface(surface);
					SDL_DestroyTexture(texture);
				}
			}
			}
		SDL_RenderPresent(renderer);
	}
	~Board() {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				delete BoardMat[i][j];
				BoardMat[i][j] = 0;
			}
		}
		//TTF_CloseFont(font);
	}
	

	bool CanMove(Piece* p,int srcrow,int srccol) {
		int cColor = p->GetColor();
					//see if piece has a legal move
						for (int iMoveRow = 0; iMoveRow < 8; ++iMoveRow) {
							for (int iMoveCol = 0; iMoveCol < 8; ++iMoveCol) {
								if (p->isLegal(srcrow, srccol, iMoveRow, iMoveCol, BoardMat)) {
									// Make move and check whether king is in check
									Piece* qpTemp = BoardMat[iMoveRow][iMoveCol];
									BoardMat[iMoveRow][iMoveCol] = BoardMat[srcrow][srccol];
									BoardMat[srcrow][srccol] = 0;
									bool bCanMove = !isKingCheck(cColor);
									// Undo the move
									BoardMat[srcrow][srccol] = BoardMat[iMoveRow][iMoveCol];
									BoardMat[iMoveRow][iMoveCol] = qpTemp;
									if (bCanMove) {
										return true;
									}
								}
							}
						}
		return false;
	}
	bool isKingCheck(char col) {
		int kingrow, kingcol;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
			{
				if (BoardMat[i][j] != nullptr)
				{
					if ((BoardMat[i][j]->GetPiece() == 'K') && (BoardMat[i][j]->GetColor() == col))
					{
						kingrow=i;
						kingcol=j;
					}
				
				}
			}
		}
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
			{
				if (BoardMat[i][j] != nullptr) 
				{
					if (BoardMat[i][j]->isLegal(i, j, kingrow, kingcol, BoardMat))
						return true;
				}

			}
		}
		return false;
	}

	Piece* BoardMat[8][8];
};
class Chess {
public:
	Chess() :turn('W') {}
	~Chess() {}
	void Init(SDL_Renderer* renderer)
	{
		board.draw(renderer);
	}
	void switchturn() {
		turn = (turn == 'W' ? 'B':'W');
	}
	bool dest = false;
	int fromrow, fromcol, torow, tocol;
	void update(SDL_Renderer* renderer,bool moving) {
		if (moving) {
			if (dest) {
				torow = int(mouseY / 64);
				tocol = int(mouseX / 64);
				if (currentpiece->isLegal(7-srcrow,7-srccol,7-torow,7-tocol,board.BoardMat)) 
				{
					Piece* temp = board.BoardMat[7 - torow][7 - tocol];
						board.BoardMat[7 - torow][7 - tocol] = board.BoardMat[7 - fromrow][7 - fromcol];
						board.BoardMat[7 - fromrow][7 - fromcol] = nullptr;
						if (board.isKingCheck(turn))
						{
							board.BoardMat[7 - fromrow][7 - fromcol] = board.BoardMat[7 - torow][7 - tocol];
							board.BoardMat[7 - torow][7 - tocol] = temp;
							std::cout << "Illegal, king exposed to check\n";
						}
						else
						switchturn();
					
				}
				else {
					std::cout << "illegal\n";
				}
				bool flag = true;
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						if (board.BoardMat[i][j] != nullptr) {
							if (board.BoardMat[i][j]->GetColor() == turn)
								if (board.CanMove(board.BoardMat[i][j], i, j))
								{
									flag = false;
								}
						}
					}
				}

				if (flag) {
					std::cout << "Checkmate. "<<(turn=='W'?"Black":"White")<<" wins\n";
					isRunning = false;
				}
				currentpiece = nullptr;
				dest = false;
			}
			else
			{
				
				fromrow = int(mouseY / 64);
				fromcol = int(mouseX / 64);
				if (board.BoardMat[7 - fromrow][7 - fromcol] == nullptr)dest = false;
				else if (board.BoardMat[7 - fromrow][7 - fromcol]->GetColor() != turn)dest = false;
				else {
					if (board.isKingCheck(turn))
					{
						if (board.CanMove(board.BoardMat[7 - fromrow][7 - fromcol],7-fromrow,7-fromcol))
						{
							currentpiece = (board.BoardMat[7 - fromrow][7 - fromcol]);
							srcrow = fromrow;
							srccol = fromcol;
							dest = true;
						}
						else {
							std::cout << "Illegal\n";
								dest = false;
						}
						
					
					}
					else {
						currentpiece = (board.BoardMat[7 - fromrow][7 - fromcol]);
						srcrow = fromrow;
						srccol = fromcol;
						dest = true;
					}

				}
				board.draw(renderer);
			}
		}
		else board.draw(renderer);
		
	}
private:
	char turn;
	Board board;
};
int main(int argc, char** argv) 
{	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_Window* window = SDL_CreateWindow("chess",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,512,512,SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,2);
	SDL_Event e;
	Chess chess;
	bool moving=false;
	SDL_RenderClear(renderer);
	chess.Init(renderer);
	SDL_RenderPresent(renderer);
	
	while (isRunning) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)isRunning = false;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:isRunning = false; break;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				switch (e.button.button) {
				case SDL_BUTTON_LEFT:SDL_GetMouseState(&mouseX, &mouseY);moving=true; break;
				}
			}
		}
		SDL_RenderClear(renderer);
		chess.update(renderer,moving);
		moving = false;
		//SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}