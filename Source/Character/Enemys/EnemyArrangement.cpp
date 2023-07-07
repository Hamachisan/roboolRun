#include "EnemyArrangement.h"
#include <string>
#include <fstream>
//#include <time.h>

EnemyArrangement::EnemyArrangement(int stageNum)
{

	//csvファイル読み込み

    switch (stageNum)
    {

    case StageLevel::Level1_Stage1:
        CsvLoad(stage_, "Data/Maps/Level1_stage1.csv");
            break;
    case StageLevel::Level1_Stage2:
        CsvLoad(stage_, "Data/Maps/Level1_stage2.csv");
        break;
    case StageLevel::Level1_Stage3:
        CsvLoad(stage_, "Data/Maps/Level1_stage3.csv");
        break;
    case StageLevel::Level1_Stage4:
        CsvLoad(stage_, "Data/Maps/Level1_stage4.csv");
        break;
    case StageLevel::Level1_Stage5:
        CsvLoad(stage_, "Data/Maps/Level1_stage5.csv");
        break;
    case StageLevel::Level1_Stage6:
        CsvLoad(stage_, "Data/Maps/Level1_stage6.csv");
        break;
    case StageLevel::Level1_Stage7:
        CsvLoad(stage_, "Data/Maps/Level1_stage7.csv");
        break;
    case StageLevel::Level1_Stage8:
        CsvLoad(stage_, "Data/Maps/Level1_stage8.csv");
        break;

    case StageLevel::Level2_Stage1:
        CsvLoad(stage_, "Data/Maps/Level2_stage1.csv");
        break;
    case StageLevel::Level2_Stage2:
        CsvLoad(stage_, "Data/Maps/Level2_stage2.csv");
        break;
    case StageLevel::Level2_Stage3:
        CsvLoad(stage_, "Data/Maps/Level2_stage3.csv");
        break;
    case StageLevel::Level2_Stage4:
        CsvLoad(stage_, "Data/Maps/Level2_stage4.csv");
        break;
    case StageLevel::Level2_Stage5:
        CsvLoad(stage_, "Data/Maps/Level2_stage5.csv");
        break;
    case StageLevel::Level2_Stage6:
        CsvLoad(stage_, "Data/Maps/Level2_stage6.csv");
        break;
    case StageLevel::Level2_Stage7:
        CsvLoad(stage_, "Data/Maps/Level2_stage7.csv");
        break;
    case StageLevel::Level2_Stage8:
        CsvLoad(stage_, "Data/Maps/Level2_stage8.csv");
        break;
 
    }
}

EnemyArrangement::~EnemyArrangement()
{

}

void CsvLoad(EnemyBlock data[][ENEMY_COLUMN], const char* filename)
{

    ifstream stream(filename);
    string line;
    const string delim = ",";

    int row = 0;
    int col;
    while (getline(stream, line))
    {
        col = 0;

        //delimを区切り文字として切り分け、intに変換してdata[][]に格納する
        for (string::size_type spos, epos = 0; (spos = line.find_first_not_of(delim, epos)) != string::npos;)
        {
            string token = line.substr(spos, (epos = line.find_first_of(delim, spos)) - spos);
            data[row][col++].num = stoi(token);
        }

        ++row;
    }
}

