#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#define N 3
int tile[6][N][N];
int n = N - 1;
int cside = 0, crot = 1;
int bscol[6];
attr_t attr[8];
int scrw, scrh;
int step;
struct pos
{
	int x;
	int y;
};

int cx()
{
	int tx, ty;
	getyx(stdscr, ty, tx);
	return tx;
}

int cy()
{
	int tx, ty;
	getyx(stdscr, ty, tx);
	return ty;
}

void LBLSolution();
int GetNSideO(int cs, int d)
{
	// 1 2
	// +
	// 4 3
	switch (d)
	{
	case 1:
		switch (cs)
		{
		case 0:
		case 1:
		case 4:
		case 5:
			return 1;
		case 2:
			return 2;
		case 3:
			return 4;
		}
	case 2:
		switch (cs)
		{
		case 1:
		case 3:
			return 1;
		case 0:
			return 2;
		case 2:
		case 4:
			return 3;
		case 5:
			return 4;
		}
	case 3:
		switch (cs)
		{
		case 0:
		case 1:
		case 4:
		case 5:
			return 1;
		case 2:
			return 4;
		case 3:
			return 2;
		}
	case 4:
		switch (cs)
		{
		case 1:
		case 2:
			return 1;
		case 5:
			return 2;
		case 3:
		case 4:
			return 3;
		case 0:
			return 4;
		}
	case 5:					// back
		switch (cs)
		{
		case 2:
		case 3:
			return 3;
		case 0:
		case 4:
		case 1:
		case 5:
			return 1;
		}
	}
	return -1;
}

pos GetNTilePos(int op, pos cp)
{
	pos tp;
	switch (op)
	{
	case 0:
	case 1:
		return cp;
	case 2:
		tp.x = cp.y;
		tp.y = n - cp.x;
		break;
	case 3:
		tp.x = n - cp.x;
		tp.y = n - cp.y;
		break;
	case 4:
		tp.x = n - cp.y;
		tp.y = cp.x;
		break;
	}
	return tp;
}

int GetNSide(int cs, int d)
{
	if (d == 5)
	{
		return 5 - cs;
	}
	else if (d == 0)
	{
		return cs;
	}
	switch (cs)
	{
	case 0:
		switch (d)
		{
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 4;
		case 4:
			return 3;
		}
	case 4:
		switch (d)
		{
		case 1:
			return 0;
		case 2:
			return 2;
		case 3:
			return 5;
		case 4:
			return 3;
		}
	case 5:
		switch (d)
		{
		case 1:
			return 4;
		case 2:
			return 2;
		case 3:
			return 1;
		case 4:
			return 3;
		}
	}
	switch (d)
	{
	case 1:					// up
		return 5;
	case 2:
		switch (cs)
		{
		case 1:
			return 2;
		case 2:
			return 4;
		case 3:
			return 1;
		}
	case 3:
		return 0;
	case 4:
		switch (cs)
		{
		case 1:
			return 3;
		case 2:
			return 1;
		case 3:
			return 4;
		}
	}
	return -1;
}

int GetNTile(int cs, int d, pos cp)
{
	cp = GetNTilePos(GetNSideO(cs, d), cp);
	return tile[GetNSide(cs, d)][cp.x][cp.y];
}

void rotate(int s, int c)
{
	for (int i = 0; i < n; i++)
	{
		int tmp = tile[s][i][0];
		tile[s][i][0] = tile[s][0][n - i];
		tile[s][0][n - i] = tile[s][n - i][n];
		tile[s][n - i][n] = tile[s][n][i];
		tile[s][n][i] = tmp;
	}
	int ns[4], nso[4];
	for (int i = 0; i < 4; i++)
	{
		ns[i] = GetNSide(s, i + 1);
		nso[i] = GetNSideO(s, i + 1);
	}
	for (int i = 0; i < N; i++)
	{
		pos tp[4];
		for (int j = 0; j < 4; j++)
		{
			pos cp;
			switch (j)
			{
			case 0:
				cp.x = i;
				cp.y = n;
				break;
			case 1:
				cp.x = 0;
				cp.y = i;
				break;
			case 2:
				cp.x = n - i;
				cp.y = 0;
				break;
			case 3:
				cp.x = n;
				cp.y = n - i;
				break;
			}
			tp[j] = GetNTilePos(nso[j], cp);
		}
		int &ntile1 = tile[ns[0]][tp[0].x][tp[0].y];
		int &ntile2 = tile[ns[1]][tp[1].x][tp[1].y];
		int &ntile3 = tile[ns[2]][tp[2].x][tp[2].y];
		int &ntile4 = tile[ns[3]][tp[3].x][tp[3].y];
		int tmp = ntile1;
		ntile1 = ntile4;
		ntile4 = ntile3;
		ntile3 = ntile2;
		ntile2 = tmp;
	}
}

void initcol()
{
	int c = 1;
	for (int i = 0; i < 6; i++)
	{
		bscol[i] = c;
		c *= 2;
	}
}

void initattr()
{
	for (int c = 0; c < 8; c++)
	{
		init_pair(c + 1, COLOR_WHITE, c);
	}
	int p = 0;
	for (int j = 2; j < 8; j++)
	{
		attr[p++] = COLOR_PAIR(j) | A_BOLD;
	}
}

void initcube()
{
	for (int i = 0; i < 6; i++)
	{
		for (int x = 0; x < N; x++)
		{
			for (int y = 0; y < N; y++)
			{
				tile[i][x][y] = bscol[i];
			}
		}
	}
}

void prtile(int tilec)
{
	int c = 1;
	for (int i = 0; i < 6; i++)
	{
		if (c == tilec)
		{
			attron(attr[i]);
			addstr("  ");
			attroff(attr[i]);
			return;
		}
		c *= 2;
	}
}

void prtcsd(int s, int o)
{
	int ix, iy, sx, sy;
	int tx = cx(), ty = cy();
	switch (o)
	{
	case 1:
		ix = 0;
		iy = 0;
		sx = 1;
		sy = 1;
		break;
	case 2:
		ix = 0;
		iy = n;
		sx = 1;
		sy = -1;
		break;
	case 3:
		ix = n;
		iy = n;
		sx = -1;
		sy = -1;
		break;
	case 4:
		ix = n;
		iy = 0;
		sx = -1;
		sy = 1;
		break;
	}
	if (o == 1 || o == 3)
	{
		for (int y = iy; y < N && y >= 0; y += sy)
		{
			for (int x = ix; x < N && x >= 0; x += sx)
			{
				prtile(tile[s][x][y]);
			}
			move(++ty, tx);
		}
	}
	else
	{
		for (int x = ix; x < N && x >= 0; x += sx)
		{
			for (int y = iy; y < N && y >= 0; y += sy)
			{
				prtile(tile[s][x][y]);
			}
			move(++ty, tx);
		}
	}
}

void prtcube()
{
	clear();
	int pt = scrh / 4, pl = (scrw - 18) / 2;
	move(pt, pl + N * 2);
	prtcsd(GetNSide(cside, 1), GetNSideO(cside, 1));
	move(pt + N, pl);
	prtcsd(GetNSide(cside, 4), GetNSideO(cside, 4));
	move(pt + N, pl + N * 2);
	prtcsd(cside, 1);
	move(pt + N, pl + N * 4);
	prtcsd(GetNSide(cside, 2), GetNSideO(cside, 2));
	move(pt + N * 2, pl + N * 2);
	prtcsd(GetNSide(cside, 3), GetNSideO(cside, 3));
	move(pt + N * 3, pl + N * 2);
	prtcsd(5 - cside, 1);
	move(cy() + 1, 0);
}

int IsCubeRst()
{
	for (int i = 0; i < 6; i++)
	{
		for (int x = 0; x < N; x++)
		{
			for (int y = 0; y < N; y++)
			{
				if (tile[i][x][y] != bscol[i])
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

void formula(const char fml[])
{
	printw("crot: %d cside: %d formula: %s\n", crot, cside, fml);
	// getch();
	for (int i = 0; i < strlen(fml); i++)
	{
		int rot = 0, dcod = 0;
		switch (fml[i])
		{
		case '(':
			{
				char para[1024];
				int k, n = 0;
				for (k = i + 1; k < strlen(fml); k++, n++)
				{
					if (fml[k] == ')')
					{
						break;
					}
					para[n] = fml[k];
				}
				int lp = fml[k + 1] - 48;
				if (lp < 1 || lp > 10)
				{
					lp = 1;
				}
				for (int j = 0; j < lp; j++)
				{
					formula(para);
				}
				i = k;
				continue;
			}
		case 'R':
			dcod = 2;
			break;
		case 'L':
			dcod = 4;
			break;
		case 'U':
			dcod = 1;
			break;
		case 'D':
			dcod = 3;
			break;
		case 'F':
		case 'B':
			break;
		default:
			continue;
		}
		switch (dcod + crot)
		{
		case 2:
		case 6:
			rot = 1;
			break;
		case 3:
		case 7:
			rot = 2;
			break;
		case 4:
		case 8:
			rot = 3;
			break;
		case 5:
			rot = 4;
			break;
		}
		switch (fml[i])
		{
		case 'F':
			rot = 0;
			break;
		case 'B':
			rot = 5;
			break;
		}
		int lp = 1;
		step++;
		switch (fml[i + 1])
		{
		case '\'':
		case '3':
			lp = 3;
			break;
		case '2':
			lp = 2;
			step++;
			break;
		}
		printw("%d,%d ", rot, lp);
		for (int j = 0; j < lp; j++)
		{
			rotate(GetNSide(cside, rot), 1);
		}
	}
}

char rtof(int r)
{
	switch (r)
	{
	case 0:
		return 'F';
	case 1:
		return 'U';
	case 2:
		return 'R';
	case 3:
		return 'D';
	case 4:
		return 'L';
	case 5:
		return 'B';
	default:
		return 'X';
	}
}

char *InputFormula()
{
	char input[1024];
	addstr("Input formula\n>");
	refresh();
	echo();
	scanw("%s", input);
	noecho();
	return input;
}

int main()
{
	initscr();
	start_color();
	getmaxyx(stdscr, scrh, scrw);
	noecho();
	scrollok(stdscr, TRUE);
	initcol();
	initattr();
	initcube();
	while (1)
	{
		refresh();
		int ch = getch();
		switch (ch)
		{
		case 'a':
			initcube();
			break;
		case 't':
			{
				char *input = InputFormula();
				int k = 0;
				initcube();
				do
				{
					formula(input);
					prtcube();
					k++;
				}
				while (!IsCubeRst());
				printw("%d finished\n", k);
				refresh();
				getch();
				break;
			}
		case ' ':
			{
				formula(InputFormula());
				break;
			}
		case 's':
			step = 0;
			LBLSolution();
			printw("took %d step(s).\n", step);
			refresh();
			getch();
			break;
		case 'm':
			{
				int tst = 0, s=0;
				while (1)
				{
					int a[20];
					for (int i = 0; i < 20; i++)
					{
						a[i] = rand() % 6;
						printw("%c ", rtof(a[i]));
						rotate(a[i], 0);
					}
					printw("\n");
					step = 0;
					LBLSolution();
					if (IsCubeRst())
					{
						s++;
						tst += step;
						printw("pass with %d avg:%d\n", step, tst / s);
						refresh();
						usleep(100000);
						clear();
					}
					else
					{
						break;
					}
					initcube();
				}
				break;
			}
		case 'q':
			return 0;
		case '1':
			formula("RLU2FU'DF2R2B2LU2F'B'UR2DF2UR2U");
			break;
		case 'f':
			rotate(cside, 1);
			break;
		case 'b':
			rotate(5 - cside, 1);
			break;
		case 'r':
			rotate(GetNSide(cside, 2), 1);
			break;
		case 'l':
			rotate(GetNSide(cside, 4), 1);
			break;
		case 'u':
			rotate(GetNSide(cside, 1), 1);
			break;
		case 'd':
			rotate(GetNSide(cside, 3), 1);
			break;
		}
		prtcube();
	}
	endwin();
	return 0;
}


int GetDir(int cd, int chg)
{
	cd += chg;
	cd = cd % 4;
	if (!cd)
	{
		cd = 4;
	}
	else if (cd < 0)
	{
		cd += 4;
	}
	return cd;
}

pos DeptoPos(int s, int dep, int i)
{
	pos cp;
	switch (s)
	{
	case 1:
		cp.x = i;
		cp.y = n - dep;
		break;
	case 2:
		cp.x = dep;
		cp.y = i;
		break;
	case 3:
		cp.x = n - i;
		cp.y = dep;
		break;
	case 4:
		cp.x = n - dep;
		cp.y = n - i;
		break;
	}
	return cp;
}

int IsSideSolv(int m)
{
	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			if (tile[m][x][y] != bscol[m])
			{
				return 0;
			}
		}
	}
	return 1;
}

int IsLineSolv(int m, int dep)
{
	for (int i = 0; i < N; i++)
	{
		for (int s = 1; s < 4; s++)
		{
			if (GetNTile(m, s, DeptoPos(s, dep, i)) != bscol[GetNSide(m, s)])
			{
				return 0;
			}
		}
	}
	return 1;
}

int IsEdgeSolv(int m)
{
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int ct = tile[m][cp.x][cp.y];
		if (ct != bscol[m])
		{
			return 0;
		}
	}
	return 1;
}

int IsCrossSolv(int m)
{
	int fsb = 0, ofs = 0;
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int ct = tile[m][cp.x][cp.y];
		if (ct != bscol[m])
		{
			return 0;
		}
		fsb = i;
		int tc = GetNTile(m, i, DeptoPos(i, 0, 1));
		for (int j = 1; j < 5; j++)
		{
			if (tc == bscol[GetNSide(m, j)])
			{
				ofs = j - i;
				break;
			}
		}
		break;
	}
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int ct = tile[m][cp.x][cp.y];
		if (ct != bscol[m]
			|| GetNTile(m, i, DeptoPos(i, 0, 1)) != bscol[GetNSide(m, GetDir(i, ofs))])
		{
			return 0;
		}
	}
	return 1;
}

void Trotate(int cd, int td, int rs)
{
	int t = cside;
	cside = rs;
	if (GetDir(cd, 1) == td)
	{
		formula("F");
	}
	else if (GetDir(cd, -1) == td)
	{
		formula("F'");
	}
	else if (cd != td)
	{
		formula("F2");
	}
	cside = t;
}

void SolveTE(int cs, int d, int ofs)
{
	int rd = GetDir(d, ofs);
	int scol = bscol[cs] + bscol[GetNSide(cs, rd)];
	printw("searching for %d dir %d\n", scol, d);
	crot = d;
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int sc = tile[cs][cp.x][cp.y];
		int ct = sc + GetNTile(cs, i, DeptoPos(i, 0, 1));
		if (ct == scol)
		{
			printw("found in top %d\n", i);
			printw("top pos %d,%d sc %d ct %d\n", cp.x, cp.y, sc, ct);
			crot = i;
			if (sc != bscol[cs])
			{
				formula("U'RBR'");
			}
			else
			{
				formula("U2");
			}
			Trotate(d, i, 5 - cside);
			crot = d;
			formula("U2");
			return;
		}
	}
	for (int i = 1; i < 5; i++)
	{
		int sc = GetNTile(cs, i, DeptoPos(i, 1, 0));
		int ct = sc + GetNTile(cs, GetDir(i, -1), DeptoPos(GetDir(i, -1), 1, n));
		if (ct == scol)
		{
			printw("found in middle %d\n", i);
			crot = d;
			if (sc != bscol[cs])	// side match
			{
				if (i == d)
				{
					formula("U'");
					return;
				}
				crot = i;
				formula("U");
				Trotate(d, i, 5 - cside);
				formula("U'");
			}
			else
			{
				if (GetDir(i, -1) == d)
				{
					formula("U");
					return;
				}
				crot = i;
				formula("L'");
				Trotate(d, GetDir(i, -1), 5 - cside);
				formula("L");
			}
			crot = d;
			formula("U2");
			return;
		}
	}
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		if (i == 1 || i == 3)
		{
			cp.y = n - cp.y;
		}
		int sc = GetNTile(cs, 5, cp);
		int ct = sc + GetNTile(cs, i, DeptoPos(i, n, 1));
		printw("bottom pos %d,%d sc %d ct %d\n", cp.x, cp.y, sc, ct);
		if (ct == scol)
		{
			printw("found in bottom %d\n", i);
			crot = d;
			if (sc == bscol[cs])
			{
				Trotate(d, i, 5 - cside);
				formula("U2");
			}
			else
			{
				Trotate(GetDir(d, 1), i, 5 - cside);
				formula("R'UR");
			}
			return;
		}
	}
}

void SolveTC(int cs, int d)
{
	int scol = bscol[cs] + bscol[GetNSide(cs, d)] + bscol[GetNSide(cs, GetDir(d, -1))];
	crot = d;
	printw("searching for %d dir %d\n", scol, d);
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 0);
		int sc = tile[cs][cp.x][cp.y], fc = GetNTile(cs, i, DeptoPos(i, 0, 0));
		int ct = sc + fc + GetNTile(cs, GetDir(i, -1),
									DeptoPos(GetDir(i, -1), 0, n));
		if (ct == scol)
		{
			printw("found in top %d\n", i);
			crot = i;
			if (sc == bscol[cs])
			{
				if (GetDir(d, 1) == i)
				{
					formula("U");
					Trotate(d, i, 5 - cside);
					formula("U'");
					crot = d;
					formula("L'B'L");
				}
				else if (GetDir(i, 1) == d)
				{
					formula("L'");
					Trotate(d, i, 5 - cside);
					formula("L");
					crot = d;
					formula("UBU'");
				}
				else
				{
					formula("U");
					crot = d;
					formula("U");
					formula("B2");
					formula("U'");
					crot = i;
					formula("U'");
				}
			}
			else if (fc == bscol[cs])
			{
				formula("L'BL");
				Trotate(d, i, 5 - cside);
				crot = d;
				formula("UBU'");
			}
			else
			{
				formula("UB'U'");
				Trotate(d, i, 5 - cside);
				crot = d;
				formula("L'B'L");
			}
			crot = d;
			return;
		}
	}
	for (int i = 1; i < 5; i++)
	{
		int ti = i;
		if (i == 1 || i == 3)
		{
			ti = GetDir(i, 2);
		}
		pos cp = DeptoPos(ti, n, n);
		int sc = GetNTile(cs, 5, cp), fc = GetNTile(cs, i, DeptoPos(i, n, 0));
		int ct = sc + fc + GetNTile(cs, GetDir(i, -1),
									DeptoPos(GetDir(i, -1), n, n));
		if (ct == scol)
		{
			printw("found in bottom %d\n", i);
			crot = d;
			if (sc == bscol[cs])
			{
				crot = i;
				Trotate(GetDir(d, 2), i, 5 - cside);
				crot = d;
				formula("UBU'L'B'L");
			}
			else if (fc == bscol[cs])
			{
				Trotate(d, i, 5 - cside);
				formula("UBU'");
			}
			else
			{
				Trotate(d, i, 5 - cside);
				formula("L'B'L");
			}
			return;
		}
	}
}

void SolveMC(int cs, int d, int p)
{
	int scol = bscol[GetNSide(cs, d)] + bscol[GetNSide(cs, GetDir(d, -1))];
	crot = d;
	printw("searching for %d dir %d\n", scol, d);
	if (p)
	{
		for (int i = 1; i < 5; i++)
		{
			int ct = GetNTile(cs, i, DeptoPos(i, 1, 0)) + GetNTile(cs, GetDir(i, -1),
																   DeptoPos(GetDir(i, -1), 1, n));
			if (ct == scol)
			{
				printw("found in middle %d\n", i);
				crot = i;
				formula("L'BLBUB'U'");
				break;
			}
		}
	}
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		if (i == 1 || i == 3)
		{
			cp.y = n - cp.y;
		}
		int fc = GetNTile(cs, i, DeptoPos(i, n, 1));
		int ct = fc + GetNTile(cs, 5, cp);
		if (ct == scol)
		{
			crot = d;
			printw("found in bottom %d\n", i);
			if (fc == bscol[GetNSide(cs, d)])
			{
				Trotate(GetDir(d, 1), i, 5 - cside);
				formula("L'BLBUB'U'");
			}
			else
			{
				Trotate(GetDir(d, 2), i, 5 - cside);
				formula("UB'U'B'L'BL");
			}
			return;
		}
	}
}

void SolveLE(int cs)
{
	cside = cs;
	crot = 1;
	int ec = 0, fe = 0, se = 0;
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		if (tile[cs][cp.x][cp.y] == bscol[cs])
		{
			printw("edge found in %d\n", i);
			if (ec == 1)
			{
				se = i;
			}
			else if (!ec)
			{
				fe = i;
			}
			ec++;
		}
	}
	switch (ec)
	{
	case 0:
		printw("first edge not found\n");
		formula("DRFR'F'D'");
		break;
	case 2:
		switch (se - fe)
		{
		case 1:
			crot = GetDir(se, 2);
			break;
		case 2:
			crot = fe + 1;
			break;
		case 3:
			crot = 1;
			break;
		default:
			printw("error occured\n");
		}
		formula("DRFR'F'D'");
		if (se - fe == 2)
		{
			return;
		}
		break;
	case 4:
		return;
	}
	SolveLE(cs);
}

void SolveLC(int cs)
{
	cside = cs;
	crot = 1;
	int cc = 0, fc = 0, sc = 0;
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 0);
		if (tile[cs][cp.x][cp.y] == bscol[cs])
		{
			printw("corner found in %d\n", i);
			if (cc == 1)
			{
				sc = i;
			}
			else if (!cc)
			{
				fc = i;
			}
			cc++;
		}
	}
	int k = GetDir(fc, 1);
	int l = GetDir(sc, 1);
	printw("%d %d %d\n", cc, fc, sc);
	switch (cc)
	{
	case 0:
		printw("no corner found\n");
		formula("LF2L'F'LF'L'");
		break;
	case 1:
		if (GetNTile(cs, k, DeptoPos(k, 0, 0)) == bscol[cs])
		{
			crot = GetDir(fc, 2);
			formula("R'F2RFR'FR");
		}
		else
		{
			crot = k;
			formula("LF2L'F'LF'L'");
		}
		break;
	case 2:
		switch (sc - fc)
		{
		case 1:
			if (GetNTile(cs, l, DeptoPos(l, 0, 0)) == bscol[cs])
			{
				crot = sc;
			}
			else
			{
				crot = fc;
			}
			formula("R'F2RFR'FR");
			break;
		case 3:
			if (GetNTile(cs, k, DeptoPos(k, 0, 0)) == bscol[cs])
			{
				crot = fc;
			}
			else
			{
				crot = sc;
			}
			formula("R'F2RFR'FR");
			break;
		case 2:
			if (GetNTile(cs, k, DeptoPos(k, 0, 0)) == bscol[cs])
			{
				crot = GetDir(fc, 2);
			}
			else
			{
				crot = fc;
			}
			formula("R'D'L'DRD'LD");
			return;
		default:
			printw("error occured\n");
			return;
		}
		break;
	case 4:
		return;
	}
	SolveLC(cs);
}

void SolveLP(int cs)
{
	cside = cs;
	crot = 1;
	int sl = 0, ulc = 0, lul = 0, sc = 0;
	for (int i = 1; i < 5; i++)
	{
		int bc = GetNTile(cs, i, DeptoPos(i, 0, 0));
		if (GetNTile(cs, i, DeptoPos(i, 0, 2)) == bc)
		{
			if (GetNTile(cs, i, DeptoPos(i, 0, 1)) == bc)
			{
				sc++;
				sl = i;
			}
			else
			{
				lul = i;
				ulc++;
			}
		}
	}
	printw("%d %d  %d %d\n", ulc, sc, lul, sl);
	if (sc == 4)
	{
		return;
	}
	if (ulc == 3)
	{
		if (GetNTile(cs, GetDir(sl, 1), DeptoPos(GetDir(sl, 1), 0, 1)) ==
			GetNTile(cs, GetDir(sl, -1), DeptoPos(GetDir(sl, -1), 0, 0)))
		{
			crot = GetDir(sl, -1);
			formula("L2F'D'UL2DU'F'L2");
		}
		else
		{
			crot = GetDir(sl, 1);
			formula("R2FDU'R2D'UFR2");
		}
		return;
	}
	else if (ulc == 4)
	{
		formula("R'F2RFR'FR");
		crot = GetDir(crot, 1);
		formula("LF2L'F'LF'L'");
	}
	else
	{
		if (sc)
		{
			crot = GetDir(sl, -1);
		}
		else
		{
			crot = GetDir(lul, -1);
		}
		formula("R2D2R'U'RD2R'UR'");
	}
	SolveLP(cs);
}

void LBLSolution()
{
	int fsd = 0;
	int cpl = 0, mcpl = 0;
	for (int i = 0; i < 6; i++)
	{
		cpl =
			IsSideSolv(i) + IsLineSolv(i, 0) + IsLineSolv(i,
														  1) + IsSideSolv(5 -
																		  i) + IsLineSolv(i, 2);
		if (cpl > mcpl)
		{
			mcpl = cpl;
			// cside = fsd = i;
		}
	}
	int fsb = 1, ofs = 0;
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int ct = tile[fsd][cp.x][cp.y];
		if (ct != bscol[fsd])
		{
			continue;
		}
		fsb = i;
		int tc = GetNTile(fsd, i, DeptoPos(i, 0, 1));
		for (int j = 1; j < 5; j++)
		{
			if (tc == bscol[GetNSide(fsd, j)])
			{
				ofs = j - i;
				break;
			}
		}
		break;
	}
	printw("first side orientation %d offset %d\n", fsb, ofs);
	printw("solving the first layer cside:%d\n", cside);
	printw("bottom cross\n");
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int ct = tile[fsd][cp.x][cp.y];
		printw("edge %d pos %d,%d ct %d\n", i, cp.x, cp.y, ct);
		if (ct != bscol[fsd]
			|| GetNTile(fsd, i, DeptoPos(i, 0, 1)) != bscol[GetNSide(fsd, GetDir(i, ofs))])
		{
			printw("solving %d\n", i);
			SolveTE(fsd, i, ofs);
			crot = 1;
		}
	}
	Trotate(fsb, GetDir(fsb, ofs), fsd);	// layer alignment
	if (!IsCrossSolv(fsd))
	{
		printw("Cross not solved, sub terminated\n");
		return;
	}
	else
	{
		printw("Cross solved, proceed\n");
	}
	printw("fill corners\n");
	for (int i = 1; i < 5; i++)
	{
		printw("corner %d\n", i);
		pos cp;
		cp = DeptoPos(i, n, 0);
		int ct = tile[fsd][cp.x][cp.y];
		if (ct != bscol[fsd] || GetNTile(fsd, i, DeptoPos(i, 0, 0)) != bscol[GetNSide(fsd, i)])
		{
			printw("solving %d\n", i);
			SolveTC(fsd, i);
			crot = 1;
		}
	}
	if (!IsSideSolv(fsd) || !IsLineSolv(fsd, 0))
	{
		printw("First layer not solved, sub terminated\n");
		return;
	}
	printw("solving the second(mid) layer\n");
	for (int p = 0; p < 2; p++)
	{
		printw("priority %d\n", p);
		for (int i = 1; i < 5; i++)
		{
			if (GetNTile(fsd, i, DeptoPos(i, 1, 0)) != bscol[GetNSide(fsd, i)]
				|| GetNTile(fsd, GetDir(i, -1),
							DeptoPos(GetDir(i, -1), 1, n)) != bscol[GetNSide(fsd, GetDir(i, -1))])
			{
				printw("solving %d\n", i);
				SolveMC(fsd, i, p);
				crot = 1;
			}
		}
	}
	if (!IsLineSolv(fsd, 1))
	{
		printw("Mid layer not solved, sub terminated\n");
		return;
	}
	printw("Solving the last layer edge\n");
	SolveLE(5 - fsd);
	if (!IsEdgeSolv(5 - fsd))
	{
		printw("Last layer edge not solved, sub terminated\n");
		return;
	}
	printw("Solving the last layer corner\n");
	SolveLC(5 - fsd);
	printw("Permutating the last layer\n");
	SolveLP(5 - fsd);
	printw("2-3 Alignment\n");
	if (GetNTile(cside, 1, DeptoPos(1, 0, 0)) == bscol[GetNSide(cside, 2)])
	{
		formula("F");
	}
	if (GetNTile(cside, 1, DeptoPos(1, 0, 0)) == bscol[GetNSide(cside, 4)])
	{
		formula("F'");
	}
	if (GetNTile(cside, 1, DeptoPos(1, 0, 0)) == bscol[GetNSide(cside, 3)])
	{
		formula("F2");
	}
	cside = fsd;
	crot = 1;
}