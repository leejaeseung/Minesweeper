#include "MineSweeper.h"
#include <time.h>
//#include <stdlib.h>
//#include <iostream>
//using namespace std;

#define NO_ACTION 0
#define NEW_GAME 1
#define CONTINUE_GAME 2
#define LOAD_GAME 3

#define CLICK_MAP 1
#define CLICK_BACK_TO_MENU 2
#define CLICK_SAVE 3

#define OPEN_CELL 1
#define GAME_WIN 2
#define GAME_FAIL 3

MineSweeper::MineSweeper()
    :started(false), saved(false), end(false), load(false), saveIdx(0), loadIdx(0)
{
    srand(time(NULL));
    saveMap.resize(SAVE_MAX);
}

void MineSweeper::play()
{
    while (1)
    {
        auto clk_pos = clk.getPos();

        if (started) {
            switch (UM.gameProcess(clk_pos.first, clk_pos.second, end, &saveIdx)) {
            case NO_ACTION:
                //��ȿ���� ���� �� Ŭ�� �� or ������ ����ǰ� �� Ŭ�� ��
                continue;
            case CLICK_MAP:
                //�� Ŭ��
                UM.tightXY(&clk_pos.first, &clk_pos.second);
                //Ŀ���� ��ǥ�� �������� ������.

                switch (map.click(clk_pos.first, clk_pos.second)) {
                    //Ŭ��
                case NO_ACTION:
                    //�̹� ������ ���� Ŭ������ ��
                    continue;
                case OPEN_CELL:
                    UM.printMap(map);
                    replayMap.push_back(map);
                    continue;       //������ ��� ����
                case GAME_WIN:
                    //���� Ŭ���� : Ŭ���� �޼��� ���
                   
                    UM.printMap(map);
                    UM.printEndMsg(true);

                    break;
                case GAME_FAIL:
                    //���� �й� : �й� �޼��� ���
                    
                    UM.printMap(map);
                    UM.printEndMsg(false);
                    
                    break;
                }
                //������ ����, ���÷���
                end = true;
                replayMap.push_back(map);
                replay();
                continue;
            case CLICK_BACK_TO_MENU:
                //�޴��� ���ư�.
                started = false;
                UM.reset();
                UM.printMenu(saved);
                continue;
            case CLICK_SAVE:
                save(map);
                break;
            default:
                break;
           }
        }
        else {
            switch (UM.menuProcess(clk_pos.first, clk_pos.second, saved, load, &loadIdx, saveMap)) {
            case NO_ACTION:
                continue;
            case NEW_GAME:
                map = Map(UM.getMode(), UM.getWidth(), UM.getHeight());
                //���� ���� ����.
                break;
            case CONTINUE_GAME:
                //Continue ��ư�� ���� -> �ҷ��� �ʵ��� ���
                load = true;
                UM.printLoad(saveMap);
                continue;
            case LOAD_GAME:
                map = saveMap[loadIdx];
                break;
            default:
                break;
            }
            gameStart();
            //���� ����
        }
    }
}

void MineSweeper::replay()
{
    for (auto nowMap : replayMap) {
        Sleep(1000);
        UM.printMap(nowMap);
    }
}

void MineSweeper::gameStart()
{
    started = true;
    end = false;
    load = false;
    UM.printMap(map);
    replayMap.clear();
    replayMap.push_back(map);
}

void MineSweeper::save(const Map& nowMap)
{
    saved = true;
    saveMap[saveIdx] = nowMap;
}