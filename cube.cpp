#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#define N 3
int tile[6][N][N];
int n = N - 1;
int cside = 0, crot = 1;
int bscol[6];
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
			printf("\033[0;%dm  ", i + 41);
			printf("\033[0;0m");
			return;
		}
		c *= 2;
	}
}

void prtcsd(int s, int o)
{
	int ix, iy, sx, sy;
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
			// move(cy()+1,cx());
			putchar('\n');
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
			// move(cy()+1,cx());
			putchar('\n');
		}
	}
	printf("\n");
}

void prtcube()
{
	clear();
	system("clear");
	// printf("t\n");
	// move(0, N);
	prtcsd(GetNSide(cside, 1), GetNSideO(cside, 1));
	// printf("l\n");
	// move(N, 0);
	prtcsd(GetNSide(cside, 4), GetNSideO(cside, 4));
	// printf("f\n");
	// move(N, N);
	prtcsd(cside, 1);
	// printf("r\n");
	// move(0, N * 2 + 1);
	prtcsd(GetNSide(cside, 2), GetNSideO(cside, 2));
	// printf("d\n");
	// move(N * 2 + 1, N);
	prtcsd(GetNSide(cside, 3), GetNSideO(cside, 3));
	// printf("b\n");
	// move(N * 3 + 2, N);
	prtcsd(5 - cside, 1);
	// move(cy() + 1, 0);
}

void prtest()
{
	for (int i = 0; i < 6; i++)
	{
		printf("%d: ", i);
		for (int d = 1; d < 5; d++)
		{
			printf("%d,%d ", GetNSide(i, d), GetNSideO(i, d));
		}
		printf("\n");
	}
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
	printf("crot: %d cside: %d formula: %s\n", crot, cside, fml);
	// getchar();
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
		switch (fml[i + 1])
		{
		case '\'':
		case '3':
			lp = 3;
			break;
		case '2':
			lp = 2;
			break;
		}
		// printf("%d,%d ",rot,lp);
		for (int j = 0; j < lp; j++)
		{
			rotate(GetNSide(cside, rot), 1);
		}
	}
}

int bkup[6][N][N];
int Recursolv(int cd, int md)	// bug introduced
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			for (int m = 0; m < j; m++)
			{
				rotate(i, 0);
			}
			if (cd + 1 == md)
			{
				if (IsCubeRst())
				{
					return 1;
				}
				for (int k = 0; k < 6; k++)
				{
					for (int x = 0; x < N; x++)
					{
						for (int y = 0; y < N; y++)
						{
							tile[k][x][y] = bkup[k][x][y];
						}
					}
				}
				continue;
			}
			if (Recursolv(cd + 1, md))
			{
				return 1;
			}
		}
	}
	return 0;
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

int IsCrossSolv(int m);
int IsLineSolv(int m, int dep);
pos DeptoPos(int s, int dep, int i);
int GetDir(int cd, int chg);
int main()
{
	initcol();
	initcube();
	// prtest();
	while (1)
	{
		int ch = getchar();
		char exp[5];
		gets(exp);
		switch (ch)
		{
		case 'a':
			initcube();
			break;
		case 't':
			{
				char input[1024];
				scanf("%s", input);
				int k = 0;
				do
				{
					formula(input);
					prtcube();
					k++;
				}
				while (!IsCubeRst());
				printf("%d finished\n", k);
				break;
			}
		case 'i':
			{
				char input[1024];
				scanf("%s", input);
				formula(input);
				break;
			}
		case 's':
			LBLSolution();
			getchar();
			break;
		case 'm':
			{
				int s = 0, t = 0;
				while (++t)
				{
					int a[10];
					for (int i = 0; i < 10; i++)
					{
						a[i] = rand() % 6;
						printf("%c ", rtof(a[i]));
						rotate(a[i], 0);
					}
					putchar('\n');
					LBLSolution();
					if (IsCrossSolv(cside) && IsLineSolv(cside, 0) && IsLineSolv(cside, 1))
					{
						s++;
						printf("pass\n");
						system("clear");
					}
					else
					{
						printf("=。= %d/%d\n", s, t);
						getchar();
						break;
					}
					initcube();
				}
				break;
			}
		case 'n':
			if (IsCrossSolv(cside))
			{
				printf("pass\n");
			}
			else
			{
				printf("=。=\n");
			}
			getchar();
			break;
		case 'c':
			for (int i = 0; i < 6; i++)
			{
				for (int x = 0; x < N; x++)
				{
					for (int y = 0; y < N; y++)
					{
						bkup[i][x][y] = tile[i][x][y];
					}
				}
			}
			for (int i = 1; i < 15; i++)
			{
				printf("Trying depth %d... ", i);
				if (Recursolv(0, i))
				{
					printf("Cracked\n");
					getchar();
					break;
				}
				printf("Not matched\n");
			}
			break;
		case 'q':
			return 0;
		case '1':
			formula("RLU2FU'DF2R2B2LU2F'B'UR2DF2UR2U");
			break;
		case '2':
			formula("FRU2RF");
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
			pos cp = DeptoPos(s, dep, i);
			if (GetNTile(m, s, cp) != bscol[GetNSide(m, s)])
			{
				return 0;
			}
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
	printf("searching for %d dir %d\n", scol, d);
	crot = d;
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int sc = tile[cs][cp.x][cp.y];
		int ct = sc + GetNTile(cs, i, DeptoPos(i, 0, 1));
		if (ct == scol)
		{
			printf("found in top %d\n", i);
			printf("top pos %d,%d sc %d ct %d\n", cp.x, cp.y, sc, ct);
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
			printf("found in middle %d\n", i);
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
		printf("bottom pos %d,%d sc %d ct %d\n", cp.x, cp.y, sc, ct);
		if (ct == scol)
		{
			printf("found in bottom %d\n", i);
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
	printf("searching for %d dir %d\n", scol, d);
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 0);
		int sc = tile[cs][cp.x][cp.y], fc = GetNTile(cs, i, DeptoPos(i, 0, 0));
		int ct = sc + fc + GetNTile(cs, GetDir(i, -1),
									DeptoPos(GetDir(i, -1), 0, n));
		if (ct == scol)
		{
			printf("found in top %d\n", i);
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
			printf("found in bottom %d\n", i);
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
	printf("searching for %d dir %d\n", scol, d);
	if (p)
	{
		for (int i = 1; i < 5; i++)
		{
			int ct = GetNTile(cs, i, DeptoPos(i, 1, 0)) + GetNTile(cs, GetDir(i, -1),
																   DeptoPos(GetDir(i, -1), 1, n));
			if (ct == scol)
			{
				printf("found in middle %d\n", i);
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
			printf("found in bottom %d\n", i);
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
	printf("first side orientation %d offset %d\n", fsb, ofs);
	printf("solving the first layer cside:%d\n", cside);
	printf("bottom cross\n");
	for (int i = 1; i < 5; i++)
	{
		pos cp = DeptoPos(i, n, 1);
		int ct = tile[fsd][cp.x][cp.y];
		printf("edge %d pos %d,%d ct %d\n", i, cp.x, cp.y, ct);
		if (ct != bscol[fsd]
			|| GetNTile(fsd, i, DeptoPos(i, 0, 1)) != bscol[GetNSide(fsd, GetDir(i, ofs))])
		{
			printf("solving %d\n", i);
			SolveTE(fsd, i, ofs);
			crot = 1;
		}
	}
	Trotate(fsb, GetDir(fsb, ofs), fsd);	// layer alignment
	if (!IsCrossSolv(fsd))
	{
		printf("Cross not solved, sub terminated\n");
		return;
	}
	else
	{
		printf("Cross solved, proceed\n");
	}
	printf("fill corners\n");
	for (int i = 1; i < 5; i++)
	{
		printf("corner %d\n", i);
		pos cp;
		cp = DeptoPos(i, n, 0);
		int ct = tile[fsd][cp.x][cp.y];
		if (ct != bscol[fsd] || GetNTile(fsd, i, DeptoPos(i, 0, 0)) != bscol[GetNSide(fsd, i)])
		{
			printf("solving %d\n", i);
			SolveTC(fsd, i);
			crot = 1;
		}
	}
	if (!IsSideSolv(fsd) || !IsLineSolv(fsd, 0))
	{
		printf("First layer not solved, sub terminated\n");
		return;
	}
	printf("solving the second(mid) layer\n");
	for (int p = 0; p < 2; p++)
	{
		printf("prioriry %d\n", p);
		for (int i = 1; i < 5; i++)
		{
			if (GetNTile(fsd, i, DeptoPos(i, 1, 0)) != bscol[GetNSide(fsd, i)]
				|| GetNTile(fsd, GetDir(i, -1),
							DeptoPos(GetDir(i, -1), 1, n)) != bscol[GetNSide(fsd, GetDir(i, -1))])
			{
				printf("solving %d\n", i);
				SolveMC(fsd, i, p);
				crot = 1;
			}
		}
	}
	if (!IsLineSolv(fsd, 1))
	{
		printf("Mid layer not solved, sub terminated\n");
		return;
	}
	printf("Solving the last layer\n");
}