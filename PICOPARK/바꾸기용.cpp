// �̰͵� �Ⱦ��� �ڵ�

#include "stdafx.h"

void CGameloop::Init(HWND hWnd)
{
	LoadImages();
	InitializeMaps();

	for (int i = 0; i < NUM_PLAYERS; i++)
		player[i].Init();

	//�÷��̾� �ʱⰪ - ���� ������ ���߱� ����
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		if (player[i].type == PICO)
		{
			player[i].x = 200;
			player[i].y = 675;
		}
		else
		{
			player[i].x = 600;
			player[i].y = 675;
		}
	}

	//�������
	PlaySound(L"�������.wav", NULL, SND_ASYNC | SND_LOOP);

	//Ÿ�̸� ����
	SetTimer(hWnd, 1, 100, NULL);
	SetTimer(hWnd, 2, 10, NULL);
	SetTimer(hWnd, 3, 10, NULL);
	SetTimer(hWnd, 4, 1000, NULL);
}

void CGameloop::ResetStage(Cstage st) {
	if (maps.find(st) == maps.end()) {
		return; // Invalid stage number
	}

	player[0].x = (st == Cstage::Stage2) ? 200 : (st == Cstage::Stage3) ? 75 : 150;
	player[0].y = (st == Cstage::Stage2) ? 675 : (st == Cstage::Stage3) ? 675 : 575;
	player[1].x = (st == Cstage::Stage2) ? 600 : (st == Cstage::Stage3) ? 150 : 250;
	player[1].y = (st == Cstage::Stage2) ? 675 : (st == Cstage::Stage3) ? 675 : 575;
	mapSelection = (st == Cstage::Stage2) ? 0 : (st == Cstage::Stage3) ? 2 : 5;

	// Reset player properties
	for (int i = 0; i < NUM_PLAYERS; i++)
		player[i].Init();

	// Reset map blocks from the loaded maps
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			maps[mapSelection][i][j].x = i * 100;
			maps[mapSelection][i][j].y = j * 100;
			maps[mapSelection][i][j].type = maps[st][i][j].type;
			maps[mapSelection][i][j].select = maps[st][i][j].select;
		}
	}
}

void CGameloop::LoadImages()
{
	// Load player images
	P1Image[0].Load(L"����1.jpg");
	P1Image[1].Load(L"�̵�1.jpg");
	P1Image[2].Load(L"����1.jpg");
	P1Image[3].Load(L"����1.png");

	P2Image[0].Load(L"����2.jpg");
	P2Image[1].Load(L"�̵�2.jpg");
	P2Image[2].Load(L"����2.jpg");
	P2Image[3].Load(L"����2.png");

	// Load block images
	block[0].Load(L"��1.png");
	block[1].Load(L"��2.png");
	block[2].Load(L"��3.png");
	block[3].Load(L"��4.png");
	block[4].Load(L"������.png");
	block[5].Load(L"������.png");
	block[6].Load(L"��������ġO.png");
	block[7].Load(L"��������ġX.png");
	block[8].Load(L"�ʷϽ���ġO.png");
	block[9].Load(L"�ʷϽ���ġX.png");
	block[10].Load(L"�Ķ�����ġO.png");
	block[11].Load(L"�Ķ�����ġX.png");
	block[12].Load(L"����.png");
	block[13].Load(L"����.png");
	block[14].Load(L"�Ѿ�2.png");
	block[15].Load(L"�Ѿ�1.png");

	// Load stage images
	stage[0].Load(L"STAGE0.png");
	stage[1].Load(L"STAGE1.png");
	stage[2].Load(L"STAGE2.png");
	stage[3].Load(L"STAGE3.png");
	stage[4].Load(L"STAGE4.png");

	// Load number images
	number[0].Load(L"0.png");
	number[1].Load(L"1.png");
	number[2].Load(L"2.png");
	number[3].Load(L"3.png");
	number[4].Load(L"4.png");
	number[5].Load(L"5.png");
	number[6].Load(L"6.png");
	number[7].Load(L"7.png");
	number[8].Load(L"8.png");
	number[9].Load(L"9.png");
	semicolon.Load(L"�����ݷ�.png");

	// Load stage time images
	stageTime[0].Load(L"�ð�1.png");
	stageTime[1].Load(L"�ð�2.png");
	stageTime[2].Load(L"�ð�3.png");
}

void CGameloop::InitializeMaps()
{
	LoadAllMapsFromFile("maps.txt", maps);
}

void CGameloop::LoadAllMapsFromFile(const string& filePath, map<int, vector<vector<BLOCK>>>& maps) {
	ifstream file(filePath);
	if (file.is_open()) {
		string line;
		int currentMap = -1;

		while (getline(file, line)) {
			// ������(# mapXX)�� ã�Ƽ� ���� ���� ����
			if (line.find("# map") == 0) {
				currentMap = stoi(line.substr(5));
				maps[currentMap] = vector<vector<BLOCK>>(8, vector<BLOCK>(8));
				continue;
			}

			if (currentMap == -1) {
				continue; // ��ȿ�� ���� �������� �ʾҴٸ� ��ŵ
			}

			// ���� �ʿ� ������ �߰�
			int x, y, type;
			std::string selectStr;
			bool select;

			istringstream iss(line);
			if (iss >> x >> y >> type >> selectStr) {
				select = (selectStr == "true");
				int i = x / 100;
				int j = y / 100;
				maps[currentMap][i][j].x = x;
				maps[currentMap][i][j].y = y;
				maps[currentMap][i][j].type = type;
				maps[currentMap][i][j].select = select;
			}
		}
		file.close();
	}
}