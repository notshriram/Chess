#include"SDL.h"
#include"SDL_ttf.h"
#include<cstdio>

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
				}
				else {
					if (DesRow == SrcRow - 1) {
						return true;
					}
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
	TTF_Font* font = TTF_OpenFont("CHEQ_TT.ttf", 128);;
	
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

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
			{
				if ((i + j) & 1) SDL_SetRenderDrawColor(renderer, 10, 50, 100, 255);
				else SDL_SetRenderDrawColor(renderer, 240, 240, 220, 255);
				SDL_Rect r = { 64 * j,64 * i,64,64 };
				SDL_RenderFillRect(renderer, &r);
				if (BoardMat[i][j]!=nullptr) 
				{
					SDL_Surface* surface = nullptr;;
					SDL_Texture* texture =nullptr;
					switch (BoardMat[i][j]->GetPiece())
					{
					case 'P': {surface = TTF_RenderGlyph_Blended(font,(BoardMat[i][j]->GetColor()=='W'?'p':'o'), black); break; }
					case 'B': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'b' : 'n'), black); break; }
					case 'N': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'h' : 'j'), black); break; }
					case 'Q': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'q' : 'w'), black); break; }
					case 'K': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'k' : 'l'), black); break; }
					case 'R': {surface = TTF_RenderGlyph_Blended(font, (BoardMat[i][j]->GetColor() == 'W' ? 'r' : 't'), black); break; }
					}
					int texW = 64;
					int texH = 64;
					texture = SDL_CreateTextureFromSurface(renderer, surface);
					SDL_Rect dstrect = { 64*j, 64*i, texW, texH };
					SDL_RenderCopy(renderer, texture, NULL, &dstrect);
					SDL_FreeSurface(surface);
					SDL_DestroyTexture(texture);
				}
			}
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
	Piece* BoardMat[8][8];
};
int main(int argc, char** argv) 
{	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_Window* window = SDL_CreateWindow("chess",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,512,512,SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,2);
	//SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_Event e;
	Board board;
	
	//SDL_SetRenderDrawColor(renderer,0,0,0,255);
	bool isRunning = true;
	while (isRunning) {
	/*	pT = cT;
		cT = SDL_GetTicks();
		dT = cT - pT;*/
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)isRunning = false;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:isRunning = false; break;
				}

			}
		}
		SDL_RenderClear(renderer);
		board.draw(renderer);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}