#include <bits/stdc++.h>
#include <windows.h>
///ABDO TRY MAKING PIECES UNABLE TO TAKE THE KING IN CAPTURES FUNCTION
//why is this good or bad
//its bad nvm
using namespace std;
string real_start_sqr,real_end_sqr;
int time_i_have = 0;
int start_time = 0;
bool there_is_error= 0;
char board[12][12];
int val_of_piece[300];
int nodes=0,cap_nodes=0;
#define non_cap_val 100000
string start_sqr, end_sqr;
bool can_castle_black_q=1,
     can_castle_black_k=1,
     can_castle_white_k=1,
     can_castle_white_q=1;
int max_depth=6;
vector<pair<int,int>>best_moves;
unsigned long long White_mask,black_mask;
int pawn_val_black[8][8] =
{
{0,  0,  0,  0,  0,  0,  0,  0},
{50, 50, 50, 50, 50, 50, 50, 50},
{10, 10, 20, 30, 30, 20, 10, 10},
{5,  5, 10, 25, 25, 10,  5,  5},
{0,  0,  0, 20, 20,  0,  0,  0},
{5, -5,-10,  0,  0,-10, -5,  5},
{5, 10, 10,-20,-20, 10, 10,  5},
{0,  0,  0,  0,  0,  0,  0,  0}
};
int pawn_val_white[8][8] =
{
{0,  0,  0,  0,  0,  0,  0,  0},
{5, 10, 10,-20,-20, 10, 10,  5},
{5, -5,-10,  0,  0,-10, -5,  5},
{0,  0,  0, 20, 20,  0,  0,  0},
{5,  5, 10, 25, 25, 10,  5,  5},
{10, 10, 20, 30, 30, 20, 10, 10},
{50, 50, 50, 50, 50, 50, 50, 50},
{0,  0,  0,  0,  0,  0,  0,  0}
};
int knight_value[8][8] = {
        {-50, -40, -30, -30, -30, -30, -40, -50},
        {-40, -20, 0, 0, 0, 0, -20, -40},
        {-30, 0, 10, 15, 15, 10, 0, -30},
        {-30, 5, 15, 20, 20, 15, 5, -30},
        {-30, 0, 15, 20, 20, 15, 0, -30},
        {-30, 5, 10, 15, 15, 10, 5, -30},
        {-40, -20, 0, 5, 5, 0, -20, -40},
        {-50, -40, -30, -30, -30, -30, -40, -50}
    };
int bishop_value[8][8] = {
        {-20, -10, -10, -10, -10, -10, -10, -20},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-10, 0, 5, 10, 10, 5, 0, -10},
        {-10, 5, 5, 10, 10, 5, 5, -10},
        {-10, 0, 10, 10, 10, 10, 0, -10},
        {-10, 10, 10, 10, 10, 10, 10, -10},
        {-10, 5, 0, 0, 0, 0, 5, -10},
        {-20, -10, -10, -10, -10, -10, -10, -20}
    };
int queen_value[8][8] = {
        {-20, -10, -10, -5, -5, -10, -10, -20},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-10, 0, 5, 5, 5, 5, 0, -10},
        {-5, 0, 5, 5, 5, 5, 0, -5},
        {-5, 0, 5, 5, 5, 5, 0, -5},
        {-10, 0, 5, 5, 5, 5, 0, -10},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-20, -10, -10, -5, -5, -10, -10, -20}
    };


//value_of_the_move    end_sqr   start_sqr
//        j   i      j   i
//000000000000000000   000 000    000 000
//the code is 0 based
void int_to_ind(int x,int &i,int &j)
{
     i = x>>3;
     j= (x&((1ull<<3)-1ull));
}
string ind_to_notation(array<int,2>current)
{
    string tomove = "";
    tomove+=current[1]+'a'-1;
    tomove += char(current[0]+'0');
    return tomove;
}
array<int,2> notation_to_ind(string mv)
{
    array<int,2> a = {mv[1] - '0'-1, mv[0]-'a'};
    return a;
}
void init_board(string fen)
{
    White_mask = 0;
    black_mask = 0;

    int row=7,col=0;
    for(int i=0; i<8; i++)for(int j=0; j<8; j++)board[i][j]='.';
    for(int i=0; i<(int)fen.size(); i++)
    {
        //cout<<fen[i]<<" ";
        if(fen[i]=='/')
        {
            row--;
            col=0;
        }
        else if(fen[i]>='1' && fen[i]<='8')
        {
            col+=fen[i]-'0';
        }
        else
        {
            if(fen[i]>='A' && fen[i]<='Z')White_mask|=(1ull<<((row*8)+col));
            if(fen[i]>='a' && fen[i]<='z')black_mask|=(1ull<<((row*8)+col));

            board[row][col++]=fen[i];

        }
    }
    //  cout<<endl;
}
void bin(unsigned long long n)
{
    if (n > 1)
        bin(n >> 1);
    printf("%d", n & 1);
}
void encode_move(int start_i,int start_j,int end_i,int end_j,int move_val,int &x)
{
    x=0;
    x|=(start_i);
    x|=(start_j<<3);
    x|=(end_i<<6);
    x|=(end_j<<9);
    x|=(move_val<<12);
}
void decode_move(int &start_i,int &start_j,int &end_i,int &end_j,int &val, int x)
{

    start_i = (x&((1<<3)-1));
    x=(x>>3);
    start_j = (x&((1<<3)-1));
    x=(x>>3);
    end_i = (x&((1<<3)-1));
    x=(x>>3);
    end_j = (x&((1<<3)-1));
    x=(x>>3);
    val=x;
}
bool comp(int &a,int &b)
{
    int t,vala,valb;
    decode_move(t,t,t,t,vala,a);
    decode_move(t,t,t,t,valb,b);
    return vala<valb;
}
bool comp_caps(int &a,int &b)
{
    int end_i1,end_j1,end_i2,end_j2,t,vala,valb;
    decode_move(t,t,end_i1,end_j1,vala,a);
    decode_move(t,t,end_i2,end_j2,valb,b);
    if(board[end_i1][end_j1] == 'P' || board[end_i1][end_j1] == 'p')
        return 0;
    if(board[end_i2][end_j2] == 'P' || board[end_i2][end_j2] == 'p')
        return 1;
    return vala<valb;
}
bool comp_eval(pair<int,int>&a,pair<int,int>&b)
{
    return a.second<b.second;
}

void get_rook(vector<int>&moves,int sqr_row,int sqr_col,bool turn)
{
    for(int i=sqr_row+1; i<8; i++)
    {
        int j=sqr_col;
        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
    for(int i=sqr_row-1; i>=0; i--)
    {
        int j=sqr_col;
        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
    for(int j=sqr_col+1; j<8; j++)
    {
        int i=sqr_row;
        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
    for(int j=sqr_col-1; j>=0; j--)
    {
        int i=sqr_row;
        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
}
void get_bishop(vector<int>&moves,int sqr_row,int sqr_col,bool turn)
{
    for(int i=sqr_row+1,j=sqr_col+1; i<8 && j<8; i++,j++)
    {

        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
    for(int i=sqr_row-1,j=sqr_col+1; i>=0 && j<8; i--,j++)
    {

        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
    for(int i=sqr_row+1,j=sqr_col-1; i<8 && j>=0; i++,j--)
    {

        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
    for(int i=sqr_row-1,j=sqr_col-1; i>=0 && j>=0; i--,j--)
    {

        if(board[i][j] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
            moves.push_back(x);
        }
        else
        {
            if(turn == 1)
            {
                if(board[i][j] >= 'a' && board[i][j] <= 'z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }
            }
            if(turn == 0)
            {
                if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                    moves.push_back(x);
                }

            }
            break;
        }
    }
}
void get_queen(vector<int>&moves,int sqr_row,int sqr_col,bool turn)
{
    get_rook(moves,sqr_row,sqr_col,turn);
    get_bishop(moves,sqr_row,sqr_col,turn);
}
void get_king(vector<int>&moves,int sqr_row,int sqr_col,bool turn)
{
    for(int i=sqr_row-1; i<=sqr_row+1; i++)
    {
        for(int j=sqr_col-1; j<=sqr_col+1; j++)
        {
            if(i>=0 && j>=0 && i<8 && j<8)
            {
                if(board[i][j] == '.')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,non_cap_val,x);
                    moves.push_back(x);
                }
                else
                {
                    if(turn == 1)
                    {
                        if(board[i][j] >= 'a' && board[i][j] <= 'z')
                        {
                            int x;
                            encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                            moves.push_back(x);
                        }
                    }
                    if(turn == 0)
                    {
                        if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                        {
                            int x;
                            encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                            moves.push_back(x);
                        }

                    }
                }
            }
        }
    }


}

void get_knight(vector<int>&moves,int sqr_row,int sqr_col,bool turn)
{
    int dx[]={1,2,2,1,-1,-2,-2,-1};
    int dy[]={2,1,-1,-2,2,1,-1,-2};
    for(int x=0;x<8;x++)
    {
        int i=sqr_row+dx[x];
        int j=sqr_col+dy[x];
        if(i>=0 && i<8 && j>=0 && j<8)
        {
            if(turn == 1 && board[i][j]>='A' && board[i][j]<='Z')continue;
            if(turn == 0 && board[i][j]>='a' && board[i][j]<='z')continue;
             if(board[i][j] == '.')
                {
                    int x;
                    encode_move(sqr_row,sqr_col,i,j,non_cap_val-400,x);
                    moves.push_back(x);
                }
                else
                {
                    if(turn == 1)
                    {
                        if(board[i][j] >= 'a' && board[i][j] <= 'z')
                        {
                            int x;
                            encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                            moves.push_back(x);
                        }
                    }
                    if(turn == 0)
                    {
                        if(board[i][j] >= 'A' && board[i][j] <= 'Z')
                        {
                            int x;
                            encode_move(sqr_row,sqr_col,i,j,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[i][j]],x);
                            moves.push_back(x);
                        }

                    }
                }
        }

    }
}
void get_pawn(vector<int>&moves,int sqr_row,int sqr_col)
{
    int r = sqr_row,c = sqr_col;
    if(board[r][c] == 'P')
    {
        if(r == 1)
        {
            if(r+2<=7 && board[r+2][c] == '.' && r+1<8 && board[r+1][c] == '.')
            {
                int x;
                encode_move(sqr_row,sqr_col,r+2,c,non_cap_val-500,x);
                moves.push_back(x);

            }
        }
        if(r+1<8 && board[r+1][c] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,r+1,c,non_cap_val,x);
            moves.push_back(x);
        }
        if(r+1<8 && c+1<8 && board[r+1][c+1]>= 'a' && board[r+1][c+1] <='z')
        {
            int x;
            encode_move(sqr_row,sqr_col,r+1,c+1,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[r+1][c+1]],x);
            moves.push_back(x);
        }
        if(r+1<8 && c-1>=0 && board[r+1][c-1]>= 'a' && board[r+1][c-1] <='z')
        {
            int x;
            encode_move(sqr_row,sqr_col,r+1,c-1,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[r+1][c-1]],x);
            moves.push_back(x);
        }
    }
    else
    {
        if(r == 7)
        {
            if(r-2>=0 && board[r-2][c] == '.' && r-1>=0 && board[r-1][c] == '.')
            {
                int x;
                encode_move(sqr_row,sqr_col,r-2,c,non_cap_val,x);
                moves.push_back(x);
            }
        }
        if(r-1>=0 && board[r-1][c] == '.')
        {
            int x;
            encode_move(sqr_row,sqr_col,r-1,c,non_cap_val,x);
            moves.push_back(x);
        }
        if(r-1>=0 && c+1<8 && board[r-1][c+1]>= 'A' && board[r-1][c+1] <='Z')
        {
            int x;
            encode_move(sqr_row,sqr_col,r-1,c+1,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[r-1][c+1]],x);
            moves.push_back(x);
        }
        if(r-1>=0 && c-1>=0 && board[r-1][c-1]>= 'A' && board[r-1][c-1] <='Z')
        {
            int x;
            encode_move(sqr_row,sqr_col,r-1,c-1,val_of_piece[(int)board[sqr_row][sqr_col]]-val_of_piece[(int)board[r-1][c-1]],x);
            moves.push_back(x);
        }
    }

}
//can reach depth 8 in 13 secs in starting board
int get_static_eval(int whitemat,int blackmat,int last_move_size_white, int last_move_size_black,int bonuses)
{
//    cout<<bonuses<<" ";
    return whitemat - blackmat + bonuses;
}
int get_bonus(int bonuses,char in_old_place,char in_new_place,int start_i,int start_j,int end_i,int end_j,bool turn)
{
    int new_bonus = bonuses;
    if(turn)
    {
        if(in_old_place == 'P')
        {
            new_bonus+= pawn_val_white[end_i][end_j];
            new_bonus-= pawn_val_white[start_i][start_j];
        }
        if(in_old_place == 'N')
        {
            new_bonus+= knight_value[end_i][end_j];
            new_bonus-= knight_value[start_i][start_j];
        }
        if(in_old_place == 'B')
        {
            new_bonus+= bishop_value[end_i][end_j];
            new_bonus-= bishop_value[start_i][start_j];
        }
        if(in_new_place == 'p')
        {
            new_bonus-= pawn_val_black[end_i][end_j]*-1;
        }
        if(in_new_place == 'n')
        {
            new_bonus-= knight_value[end_i][end_j]*-1;
        }
        if(in_new_place == 'b')
        {
            new_bonus-= bishop_value[end_i][end_j]*-1;
        }
    }
    else
    {
        if(in_old_place == 'p')
        {
            new_bonus+= pawn_val_black[end_i][end_j]*-1;
            new_bonus-= pawn_val_black[start_i][start_j]*-1;
        }
        if(in_old_place == 'n')
        {
            new_bonus+= knight_value[end_i][end_j]*-1;
            new_bonus-= knight_value[start_i][start_j]*-1;
        }
        if(in_old_place == 'b')
        {
            new_bonus+= bishop_value[end_i][end_j]*-1;
            new_bonus-= bishop_value[start_i][start_j]*-1;
        }

        if(in_new_place == 'P')
        {
            new_bonus-= pawn_val_white[end_i][end_j];
        }
        if(in_new_place == 'N')
        {
            new_bonus-= knight_value[end_i][end_j];
        }
        if(in_new_place == 'B')
        {
            new_bonus-= bishop_value[end_i][end_j];
        }


    }
    return new_bonus;
}
int get_main_bonus(int i,int j,int bonus)
{
    int new_bonus = bonus;
    if(board[i][j] == 'P')new_bonus+=pawn_val_white[i][j];
    if(board[i][j] == 'N')new_bonus+=knight_value[i][j];
    if(board[i][j] == 'B')new_bonus+=bishop_value[i][j];

    if(board[i][j] == 'p')new_bonus-=pawn_val_black[i][j];
    if(board[i][j] == 'n')new_bonus-=knight_value[i][j];
    if(board[i][j] == 'b')new_bonus-=bishop_value[i][j];

    return new_bonus;
}
int simulate_caps(bool turn,int whitemat,int blackmat,int alpha,int beta,int depth,int last_move_size_white,int last_move_size_black,int bonuses)
{
    if((int)GetTickCount()-start_time > (time_i_have - 1000)/40)
    {
        there_is_error = 1;
        return 0;
    }
    cap_nodes++;

    if(depth == 0 || abs(whitemat - blackmat) >= 90000)
    {
        return get_static_eval(whitemat,blackmat,last_move_size_white,last_move_size_black,bonuses);
    }
    vector<int>moves;
    if(turn == 1)
    {
        ///replace this loop with bit manipulation
        unsigned long long temp_mask = White_mask,cc=0;
        while(temp_mask)
        {
            int bit_index = __builtin_ctzll(temp_mask);
            int i,j;
            temp_mask = (temp_mask>>(bit_index+1));
            if(bit_index == 63)temp_mask=0;
            int_to_ind(bit_index+cc,i,j);
            cc+=bit_index+1;
            if(board[i][j] == 'R')get_rook(moves,i,j,turn);
            if(board[i][j] == 'B')get_bishop(moves,i,j,turn);
            if(board[i][j] == 'K')get_king(moves,i,j,turn);
            if(board[i][j] == 'Q')get_queen(moves,i,j,turn);
            if(board[i][j] == 'N')get_knight(moves,i,j,turn);
            if(board[i][j] == 'P')get_pawn(moves,i,j);
        }
        int ans=-1e8;
        sort(moves.begin(),moves.end(),comp);
        //cout<<moves.size()<<endl;
        for(int i=0; i<(int)moves.size(); i++)
        {
            int start_i,start_j,end_i,end_j,val;
            decode_move(start_i,start_j,end_i,end_j,val,moves[i]);
            ///Try to break instead of continuing
            if(board[end_i][end_j]=='.' || board[end_i][end_j]=='p')continue;
           // cout<<start_i<<" "<<start_j<<" "<<end_i<<" "<<end_j;
           //// bin(White_mask);
            //cout<<endl;
            //bin(black_mask);
           // cout<<endl<<endl;
            char in_old_place = board[start_i][start_j];
            char in_new_place = board[end_i][end_j];

            board[end_i][end_j] = board[start_i][start_j];
            board[start_i][start_j] = '.';
            blackmat -= val_of_piece[(int)in_new_place];
            White_mask^=(1ull<<(start_i*8+start_j));
            White_mask^=(1ull<<(end_i*8+end_j));
            unsigned long long old_bit = (black_mask&(1ull<<(end_i*8 + end_j)));
            black_mask|=(1ull<<(end_i*8 + end_j));
            black_mask^=(1ull<<(end_i*8 + end_j));
            int new_bonus = get_bonus(bonuses,in_old_place,in_new_place,start_i,start_j,end_i,end_j,turn);
            int next_eval = simulate_caps(!turn,whitemat,blackmat,alpha,beta,depth-1,moves.size(),last_move_size_black,new_bonus);

            board[start_i][start_j] = in_old_place;
            board[end_i][end_j] = in_new_place;
            blackmat += val_of_piece[(int)in_new_place];
            White_mask^=(1ull<<(start_i*8+start_j));
            White_mask^=(1ull<<(end_i*8+end_j));
            black_mask|=old_bit;

            if(next_eval > ans)
            {
                ans=next_eval;
            }
            if(ans>=beta)break;
            alpha=max(alpha,ans);

        }
        if(ans == -1e8)return get_static_eval(whitemat,blackmat,last_move_size_white,last_move_size_black,bonuses);
        return ans;
    }
    else
    {
        unsigned long long temp_mask = black_mask,cc=0;
        while(temp_mask)
        {
            int bit_index = __builtin_ctzll(temp_mask);
            int i,j;
            temp_mask = (temp_mask>>(bit_index+1));
            if(bit_index == 63)temp_mask=0;
            int_to_ind(bit_index+cc,i,j);
            cc+=bit_index+1;
            if(board[i][j] == 'r')get_rook(moves,i,j,turn);
            if(board[i][j] == 'b')get_bishop(moves,i,j,turn);
            if(board[i][j] == 'k')get_king(moves,i,j,turn);
            if(board[i][j] == 'q')get_queen(moves,i,j,turn);
            if(board[i][j] == 'n')get_knight(moves,i,j,turn);
            if(board[i][j] == 'p')get_pawn(moves,i,j);
        }
        sort(moves.begin(),moves.end(),comp);
        int ans=1e8;
        for(int i=0; i<(int)moves.size(); i++)
        {
            int start_i,start_j,end_i,end_j,val;
            decode_move(start_i,start_j,end_i,end_j,val,moves[i]);
            if(board[end_i][end_j]=='.'  || board[end_i][end_j] == 'P')continue;
            char in_old_place = board[start_i][start_j];
            char in_new_place = board[end_i][end_j];

            board[end_i][end_j] = board[start_i][start_j];
            board[start_i][start_j] = '.';
            whitemat -= val_of_piece[(int)in_new_place];
            black_mask^=(1ull<<(start_i*8+start_j));
            black_mask^=(1ull<<(end_i*8+end_j));
            unsigned long long old_bit = (White_mask&(1ull<<(end_i*8 + end_j)));
            White_mask|=(1ull<<(end_i*8 + end_j));
            White_mask^=(1ull<<(end_i*8 + end_j));

            int new_bonus = get_bonus(bonuses,in_old_place,in_new_place,start_i,start_j,end_i,end_j,turn);
            int next_eval = simulate_caps(!turn,whitemat,blackmat,alpha,beta,depth-1,last_move_size_white,moves.size(),new_bonus);

            board[start_i][start_j] = in_old_place;
            board[end_i][end_j] = in_new_place;
            whitemat += val_of_piece[(int)in_new_place];
            black_mask^=(1ull<<(start_i*8+start_j));
            black_mask^=(1ull<<(end_i*8+end_j));
            White_mask|=old_bit;

            if(next_eval < ans)
            {
                ans=next_eval;
            }
            if(ans<=alpha)break;
            beta=min(beta,ans);

        }
        if(ans == 1e8)return get_static_eval(whitemat,blackmat,last_move_size_white,last_move_size_black,bonuses);
        return ans;
    }



}
int eval(bool turn,int whitemat,int blackmat,int alpha,int beta,int depth,int last_move_size_white,int last_move_size_black,int bonuses)
{
    if((int)GetTickCount()-start_time > (time_i_have - 1000)/40)
    {
        there_is_error = 1;
        return 0;
    }
    nodes++;
   last_move_size_black = 0 ;
   last_move_size_white = 0;
    if(depth == 0 || abs(whitemat - blackmat)>90000)
    {
        /*bin(White_mask);cout<<endl;bin(black_mask);cout<<endl;
        for(int i=7;i>=0;i--)
        {
            for(int j=0;j<8;j++)
            {
                cout<<board[i][j];
            }
            cout<<endl;
        }*/

        return simulate_caps(turn,whitemat,blackmat,alpha,beta,14,last_move_size_white,last_move_size_black,bonuses);
    }
    vector<int>moves;
    string temp_start_sqr,temp_end_sqr;

    if(turn == 1)
    {
       /* if(depth == max_depth && start_sqr.size() == 2)
        {
            array<int,2>mv_ind_start = notation_to_ind(start_sqr);
            array<int,2>mv_ind_end = notation_to_ind(end_sqr);
            int x=0;
            encode_move(mv_ind_start[0],mv_ind_start[1],mv_ind_end[0],mv_ind_end[1],-5000,x);
            moves.push_back(x);
           // int a,val;
           // decode_move(a,a,a,a,val,x);
            ////cout<<val<<endl;
        }*/
        bool Get_all_moves = 1;
        if(depth == max_depth)
        {
            if(best_moves.size()>0)
            {
                Get_all_moves=0;
                for(int i=0;i<(int)best_moves.size();i++)
                {
                    moves.push_back(best_moves[i].first);
                }
                best_moves.clear();
            }
        }
        ///replace this loop with bit manipulation
        if(Get_all_moves)
        {
            unsigned long long temp_mask = White_mask,cc=0;
            while(temp_mask)
            {
                int bit_index = __builtin_ctzll(temp_mask);
                int i,j;

                temp_mask = (temp_mask>>(bit_index+1));
                int_to_ind(bit_index+cc,i,j);
                if(bit_index == 63)temp_mask=0;
                /*if(!(i>=0 && i<8 && j>=0 && j<8))
                {
                    cout<<i<< " "<<j<<endl;
                    bin(White_mask);
                    cout<<endl;
                    bin(black_mask);
                    cout<<endl;
                }*/
                cc+=bit_index+1;
                if(board[i][j] == 'R')get_rook(moves,i,j,turn);
                if(board[i][j] == 'B')get_bishop(moves,i,j,turn);
                if(board[i][j] == 'K')get_king(moves,i,j,turn);
                if(board[i][j] == 'Q')get_queen(moves,i,j,turn);
                if(board[i][j] == 'N')get_knight(moves,i,j,turn);
                if(board[i][j] == 'P')get_pawn(moves,i,j);
            }
            sort(moves.begin(),moves.end(),comp);
        }
        int ans=-1e8;

        //cout<<moves.size()<<endl;
        for(int i=0; i<(int)moves.size(); i++)
        {
            int start_i,start_j,end_i,end_j,val;
            decode_move(start_i,start_j,end_i,end_j,val,moves[i]);
           // cout<<start_i<<" "<<start_j<<" "<<end_i<<" "<<end_j;
           //// bin(White_mask);
            //cout<<endl;
            //bin(black_mask);
           // cout<<endl<<endl;
            char in_old_place = board[start_i][start_j];
            char in_new_place = board[end_i][end_j];

            board[end_i][end_j] = board[start_i][start_j];
            board[start_i][start_j] = '.';
            blackmat -= val_of_piece[(int)in_new_place];
            White_mask^=(1ull<<(start_i*8+start_j));
            White_mask^=(1ull<<(end_i*8+end_j));
            unsigned long long old_bit = (black_mask&(1ull<<(end_i*8 + end_j)));
            black_mask|=(1ull<<(end_i*8 + end_j));
            black_mask^=(1ull<<(end_i*8 + end_j));
            if(in_old_place == 'P' && end_i == 7)
            {
                board[end_i][end_j] = 'Q';
                whitemat += val_of_piece[(int)'Q'];
                whitemat -= val_of_piece[(int)'P'];
            }
            int new_bonus = get_bonus(bonuses,in_old_place,in_new_place,start_i,start_j,end_i,end_j,turn);
            int next_eval = eval(!turn,whitemat,blackmat,alpha,beta,depth-1,moves.size(),last_move_size_black,new_bonus);
            if(depth == max_depth)
            best_moves.push_back({moves[i],next_eval});
            if(in_old_place == 'P' && end_i == 7)
            {
                //grid[end_i][end_j] = 'Q';
                whitemat -= val_of_piece[(int)'Q'];
                whitemat += val_of_piece[(int)'P'];
            }
            board[start_i][start_j] = in_old_place;
            board[end_i][end_j] = in_new_place;
            blackmat += val_of_piece[(int)in_new_place];
            White_mask^=(1ull<<(start_i*8+start_j));
            White_mask^=(1ull<<(end_i*8+end_j));
            black_mask|=old_bit;

            if(next_eval > ans)
            {
                ans=next_eval;
                temp_start_sqr = ind_to_notation({start_i + 1, start_j + 1});
                temp_end_sqr = ind_to_notation({end_i + 1, end_j + 1});
                if(board[start_i][start_j] == 'P' && end_i == 7)temp_end_sqr+='Q';
            }
            if(ans>=beta)break;
            alpha=max(alpha,ans);

        }
        if(depth == max_depth)
        {
            /*bin(White_mask);
            cout<<endl;
            bin(black_mask);
            cout<<endl;
            for(int i=0;i<moves.size();i++)
            {
                int start_i,start_j,end_i,end_j,val;
                decode_move(start_i,start_j,end_i,end_j,val,moves[i]);
                cout<<start_i<<" "<<start_j<<" "<<end_i<<" "<<end_j<<" "<<val<<endl;
            }*/
            start_sqr = temp_start_sqr;
            end_sqr = temp_end_sqr;
        }
        return ans;
    }
    else
    {
        bool Get_all_moves = 1;
        if(depth == max_depth)
        {
            if(best_moves.size()>0)
            {
                Get_all_moves=0;
                for(int i=0;i<(int)best_moves.size();i++)
                {
                    moves.push_back(best_moves[i].first);
                }
                best_moves.clear();
            }
        }
        unsigned long long temp_mask = black_mask,cc=0;
        if(Get_all_moves)
        {
            while(temp_mask)
            {
                unsigned long long bit_index = __builtin_ctzll(temp_mask);
                int i,j;
                temp_mask = (temp_mask>>(bit_index+1));
                if(bit_index == 63)temp_mask=0;
                int_to_ind(bit_index+cc,i,j);
                /*
                if(!(i>=0 && i<8 && j>=0 && j<8))
                {
                    cout<<bit_index<<" "<<cc<<endl;
                    cout<<i<< " "<<j<<endl;
                    bin(White_mask);
                    cout<<endl;
                    bin(black_mask);
                    cout<<endl;
                    bin(temp_mask>>(bit_index));
                }*/
                //assert(i>=0 && i<8 && j>=0 && j<8);
                cc+=bit_index+1;
                if(board[i][j] == 'r')get_rook(moves,i,j,turn);
                if(board[i][j] == 'b')get_bishop(moves,i,j,turn);
                if(board[i][j] == 'k')get_king(moves,i,j,turn);
                if(board[i][j] == 'q')get_queen(moves,i,j,turn);
                if(board[i][j] == 'n')get_knight(moves,i,j,turn);
                if(board[i][j] == 'p')get_pawn(moves,i,j);
            }
            sort(moves.begin(),moves.end(),comp);
        }

        int ans=1e8;
        for(int i=0; i<(int)moves.size(); i++)
        {
            int start_i,start_j,end_i,end_j,val;
            decode_move(start_i,start_j,end_i,end_j,val,moves[i]);

            char in_old_place = board[start_i][start_j];
            char in_new_place = board[end_i][end_j];

            board[end_i][end_j] = board[start_i][start_j];
            board[start_i][start_j] = '.';
            whitemat -= val_of_piece[(int)in_new_place];
            black_mask^=(1ull<<(start_i*8+start_j));
            black_mask^=(1ull<<(end_i*8+end_j));
            unsigned long long old_bit = (White_mask&(1ull<<(end_i*8 + end_j)));
            White_mask|=(1ull<<(end_i*8 + end_j));
            White_mask^=(1ull<<(end_i*8 + end_j));
            if(in_old_place == 'p' && end_i == 0)
            {
                board[end_i][end_j] = 'q';
                blackmat += val_of_piece[(int)'q'];
                blackmat -= val_of_piece[(int)'p'];
            }
            int new_bonus = get_bonus(bonuses,in_old_place,in_new_place,start_i,start_j,end_i,end_j,turn);
            int next_eval = eval(!turn,whitemat,blackmat,alpha,beta,depth-1,last_move_size_white,moves.size(),new_bonus);
            if(depth == max_depth)
            best_moves.push_back({moves[i],next_eval});
            if(in_old_place == 'p' && end_i == 0)
            {
                //grid[end_i][end_j] = 'Q';
                blackmat -= val_of_piece[(int)'q'];
                blackmat += val_of_piece[(int)'p'];
            }
            board[start_i][start_j] = in_old_place;
            board[end_i][end_j] = in_new_place;
            whitemat += val_of_piece[(int)in_new_place];
            black_mask^=(1ull<<(start_i*8+start_j));
            black_mask^=(1ull<<(end_i*8+end_j));
            White_mask|=old_bit;

            if(next_eval < ans)
            {
                ans=next_eval;
                temp_start_sqr = ind_to_notation({start_i + 1, start_j + 1});
                temp_end_sqr = ind_to_notation({end_i + 1, end_j + 1});
                if(board[start_i][start_j] == 'p' && end_i == 0)temp_end_sqr+='q';
            }
            if(ans<=alpha)break;
            beta=min(beta,ans);

        }
        if(depth == max_depth)
        {
            start_sqr = temp_start_sqr;
            end_sqr = temp_end_sqr;
        }
        return ans;
    }



}
int main()
{
    for(int i=0; i<=9; i++)
        for(int j=0; j<=9; j++)board[i][j]='.';

    board[0][2]='R';
    board[0][1]='R';
    board[7][0]='k';
    board[7][7]='K';

    val_of_piece[(int)'R'] = 500;
    val_of_piece[(int)'Q'] = 900;
    val_of_piece[(int)'B'] = 330;
    val_of_piece[(int)'N'] = 320;
    val_of_piece[(int)'P'] = 100;
    val_of_piece[(int)'K'] = 100000;

    val_of_piece[(int)'r'] = 500;
    val_of_piece[(int)'q'] = 900;
    val_of_piece[(int)'b'] = 330;
    val_of_piece[(int)'n'] = 320;
    val_of_piece[(int)'p'] = 100;
    val_of_piece[(int)'k'] = 100000;

    init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    string s="position startpos moves";

//    auto start = high_resolution_clock::now();
    while(true)
    {
        int movessofar=0;
        int bonuses = 0;

//        tempset.clear();
xx:
        ;
        //string x;
        //cin>>x;
        // s+=' ';
        //s+=x;
        getline(cin,s);
        if(s == "uci")
        {
            cout<<"uciok"<<endl;
            goto xx;
        }
        if(s == "ucinewgame")
        {
            goto xx;
        }
        if(s == "isready")
        {
            cout<<"readyok"<<endl;
            goto xx;
        }

        if(s[0] == 'p' && s[1] == 'o' && s[2] == 's')
        {
            bonuses = 0;
            movessofar=0;
            init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
            can_castle_black_q=1;
            can_castle_black_k=1;
            can_castle_white_k=1;
            can_castle_white_q=1;
            if(s == "position startpos")
            {
                goto xx;
            }
            else
            {
                string tomove="";
                s+=' ';
                for(int i=24; i<(int)s.size(); i++)
                {
                    if(s[i]==' ')
                    {
                        movessofar++;
                        string strt = "";
                        strt+=tomove[0];
                        strt+=tomove[1];
                        string nxt = "";
                        nxt+=tomove[2];
                        nxt+=tomove[3];

                        array<int,2>to_ind = notation_to_ind(nxt);
                        array<int,2>current = notation_to_ind(strt);
                        if(board[current[0]][current[1]] == 'K' && (nxt == "g1" || nxt == "c1") && (strt == "e1"))
                        {
                            if(nxt == "g1")
                            {

                                board[0][4]='.';
                                board[0][5]='R';
                                board[0][6]='K';
                                board[0][7]='.';
                            }
                            if(nxt == "c1")
                            {

                                board[0][2]='K';
                                board[0][3]='R';
                                board[0][0]='.';
                                board[0][4]='.';
                            }
                            tomove="";
                        }
                        else if(board[current[0]][current[1]] == 'k' && (nxt == "g8" || nxt == "c8") && strt == "e8")
                        {
                            if(nxt == "g8")
                            {
                                board[7][6]='k';
                                board[7][5]='r';
                                board[7][7]='.';
                                board[7][4]='.';
                            }
                            if(nxt == "c8")
                            {
                                board[7][2]='k';
                                board[7][3]='r';
                                board[7][0]='.';
                                board[7][4]='.';
                            }
                            tomove = "";
                        }
                        else
                        {
                            int i=current[0],j=current[1];
                            char x = board[to_ind[0]][to_ind[1]];
                            char y = board[i][j];
                            if(tomove.size()==5)
                            {
                                y=tomove[4];
                                if(movessofar%2 == 1)y-=' ';
                            }
                            board[i][j]='.';
                            board[to_ind[0]][to_ind[1]] = y;
                            //bonuses = get_bonus(bonuses,y,x,i,j,to_ind[0],to_ind[1],(movessofar%2));
                            tomove="";
                        }
                    }
                    else
                    {
                        tomove+=s[i];
                    }
                }
                //goto xx;

            }
            goto xx;
        }
        if(s == "quit")return 0;
        stringstream proctime(s);
        string tempp;
        int wtime = 0,btime = 0;
        proctime>>tempp>>tempp>>wtime>>tempp>>btime;

        //cout<<wtime<<endl<<btime<<endl;
        bool turn = (movessofar%2 == 0);
       // cout<<turn <<" "<<movessofar<<endl;
        // sleep_for(nanoseconds(300000000));
        int whitemat=0,blackmat=0;
        White_mask=0;
        black_mask=0;

        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(board[i][j]!='.')
                {
                    bonuses = get_main_bonus(i,j,bonuses);
                }
                if(board[i][j]>='A' && board[i][j]<='Z')White_mask|=(1ull<<((i*8)+j));
                if(board[i][j]>='a' && board[i][j]<='z')black_mask|=(1ull<<((i*8)+j));
                if(board[i][j]>='a' && board[i][j]<='z')
                {
                    blackmat+=val_of_piece[(int)board[i][j]];
                }
                if(board[i][j]>='A' && board[i][j]<='Z')
                {
                    whitemat+=val_of_piece[(int)board[i][j]];
                }

            }
        }

        /*unsigned long long temp_mask = black_mask;
        vector<int>moves;
        cout<<temp_mask<<endl;
        bin(temp_mask);
        int cc=0;
        while(temp_mask)
        {
            bin(temp_mask);
            cout<<endl;
            int bit_index = __builtin_ctzll(temp_mask);
            cout<<bit_index<<endl;
            int i,j;
            temp_mask = (temp_mask>>(bit_index+1));
            int_to_ind(bit_index+cc,i,j);
            cc+=bit_index+1;
            cout<<i<<" "<<j<<endl;
            if(board[i][j] == 'R')get_rook(moves,i,j,turn);
            if(board[i][j] == 'B')get_bishop(moves,i,j,turn);
            if(board[i][j] == 'K')get_king(moves,i,j,turn);
            if(board[i][j] == 'Q')get_queen(moves,i,j,turn);
            if(board[i][j] == 'N')get_knight(moves,i,j,turn);
            if(board[i][j] == 'P')get_pawn(moves,i,j);
        }
        int ans=-1e8;*/
        //sort(moves.begin(),moves.end(),comp);
        /*vector<int>moves;
        get_knight(moves,6,2,1);
        for(int i=0;i<moves.size();i++)
        {
            int start_i,start_j,end_i,end_j,val;
            decode_move(start_i,start_j,end_i,end_j,val,moves[i]);
            cout<<start_i<<" "<<start_j<<" "<<end_i<<" "<<end_j<<" "<<val<<endl;
        }*/
        //return 0;
        //cout<<whitemat<<" "<<blackmat<<endl;

      auto startTime = std::chrono::high_resolution_clock::now();

      if(turn)time_i_have = wtime;
      else time_i_have = btime;
    start_time = GetTickCount();
      for(int cur_depth = 2; cur_depth <= 12; cur_depth+=2) { // (iterative deepening)
      //      bin(White_mask);
      //cout<<endl;
      //bin(black_mask);
      //cout<<endl;
        there_is_error = 0;
        nodes=0;
        cap_nodes=0;
        max_depth = cur_depth;
        if(turn)
        {
            sort(best_moves.begin(),best_moves.end(),comp_eval);
            reverse(best_moves.begin(),best_moves.end());
        }
        else
        {
            sort(best_moves.begin(),best_moves.end(),comp_eval);
        }
        int x = eval(turn,whitemat,blackmat,-1e8,1e8,cur_depth,0,0,bonuses);
        if(there_is_error == 0)
        {
            real_start_sqr = start_sqr;
            real_end_sqr = end_sqr;
        }
        if(there_is_error)break;
        cout <<cur_depth<<" "<<bonuses << " nodes "<<nodes<<" "<<cap_nodes<<" score cp "<<x<<'\n';
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "Time Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms\n";
        cout<<start_sqr<<" "<<end_sqr<<endl;
        //int y = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

      }
        //system("CLS");
        /*

        for(int i=7; i>=0; i--)
        {
            for(int j=0; j<8; j++)
            {
                cout<<board[i][j];
            }
            cout<<endl;
        }*/
        //max_depth = 6;
        //if(movessofar>50)max_depth++;
        //cout<<eval(turn,whitemat,blackmat,-1e8,1e8,max_depth,0,0,0)<<endl;
        //cout<<end_sqr.size()<<endl;
        best_moves.clear();
        cout<<"bestmove "<<real_start_sqr<<real_end_sqr<<endl;
       // s+= start_sqr;
       // s+= end_sqr;
        //return 0;
    }

    //int x=0;
    //encode_move(1,5,6,7,19,x);
    // bin(x);

    /*vector<int>moves;
    get_king(moves,7,7,1);
    for(int i=0;i<moves.size();i++)
    {
        int start_i,start_j,end_i,end_j,val;

        decode_move(start_i,start_j,end_i,end_j,val,moves[i]);
        cout<<start_i<<" "<<start_j<<" "<<end_i<<" "<<end_j<<" "<<val<<endl;
    }

    cout<<endl;
    */

    /*while(1)
    {
        eval(turn,whitemat,blackmat);
        cout<<start_sqr<<next_sqr;
        turn = !turn;

    }*/
}
///position startpos moves d2d4 g8f6 g1f3 e7e6 b1c3 d7d5 e2e3 c8d7 f1d3 c7c5 e1g1 c5c4 d3e2 f8b4 a2a3 b8c6 a3b4 d8b6 b4b5 a7a6 b5c6 b7c6 c3a4 g7g6 a4b6 d7c8 b6a8 h7h6 a8c7 h8h7
///position startpos moves e2e4 e7e5 g1f3 b8c6 b1c3 g8f6 f1c4 f6e4 c3e4 d7d5 c4d5 d8d5 e4c3 d5a5 e1g1 c8g4 h2h3 g4h5 f1e2
///position startpos moves d2d4 d7d5 c1f4 e7e6 e2e3 c7c5 b1c3 a7a6 a2a3 b7b5 d1d2 c5d4 e3d4 g8e7 a3a4 b5a4 g1f3 e7g6 f4g3 f8b4 a1a4 b4c3 b2c3 g6f8 d2f4 f8d7 f3g5 f7f6 g5e6 d8b6 f4g4 d7e5 d4e5 c8e6 g4g7 b6b1 e1d2 h8f8 a4b4 b1a2 b4b6 d5d4 e5f6 b8c6 f1e2
